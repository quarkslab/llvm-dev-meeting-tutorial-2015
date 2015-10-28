/**
 * Obfuscation through Mixed Boolean Arithmetic (MBA)
 *
 * This just performs an instruction substitution, replacing a simple operation
 * (an add between two integers) by an equivalent instruction that is more
 * complex.
 *
 * This showcases the development of a simple Basic Block pass.
 */

/* for debug support
 * see
 * http://llvm.org/docs/ProgrammersManual.html#the-debug-macro-and-debug-option
 */
#define DEBUG_TYPE "mba"
#include "llvm/Support/Debug.h"

/* for stat support
 * see http://llvm.org/docs/WritingAnLLVMPass.html#pass-statistics
 */
#include "llvm/ADT/Statistic.h"
STATISTIC(MBACount, "The # of substituted instructions");

#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include "Utils.h"

/*****************************************
 * Pass implementation
 *****************************************/

// Pass Option declaration, stored as a static variable
// It uses the Ratio class from Utils.h
// see http://llvm.org/docs/CommandLine.html#quick-start-guide
static llvm::cl::opt<Ratio> MBARatio{
    "mba-ratio",
    llvm::cl::desc("Only apply the mba pass on <ratio> of the candidates"),
    llvm::cl::value_desc("ratio"), llvm::cl::init(1.), llvm::cl::Optional};

using namespace llvm;

// anonymous namespace -> avoid exporting unneeded symbols
namespace {

// A pass that perform a simple instruction substitution
// see http://llvm.org/docs/WritingAnLLVMPass.html#the-basicblockpass-class
class MBA : public BasicBlockPass {

public:
  // The address of this static is used to uniquely identify this pass in the
  // pass registry. The PassManager relies on this address to find instance of
  // analyses passes and build dependencies on demand.
  // The value does not matter.
  static char ID;
  compat::RandomNumberGenerator RNG;

  MBA() : BasicBlockPass(ID)
          , RNG(nullptr)
  {}

  // Called once for each module, before the calls on the basic blocks.
  // We could use doFinalization to clear RNG, but that's not needed.
  bool doInitialization(Module &M) override {
    RNG = M.createRNG(this);
    return false;
  }

  // Called for each basic block of the module
  // Rely on the equality: a + b == (a ^ b) + 2 * (a & b)
  bool runOnBasicBlock(BasicBlock &BB) override {
    bool modified = false;
    std::uniform_real_distribution<double> Dist(0., 1.);

    auto &BIL = BB.getInstList();
    for (auto IIT = BIL.begin(), IE = BIL.end(); IIT != IE; ++IIT) {
      Instruction &Inst = *IIT;
      // not regular C++ a dynamic_cast!
      // see http://llvm.org/docs/ProgrammersManual.html#the-isa-cast-and-dyn-cast-templates
      auto *BinOp = dyn_cast<BinaryOperator>(&Inst);
      if (!BinOp)
        continue;

      if (Dist(RNG) > MBARatio.getRatio())
        continue;

          unsigned Opcode = BinOp->getOpcode();
          if (Opcode == Instruction::Add and BinOp->getType()->isIntegerTy()) {
            // The IRBuilder helps you inserting instructions in a clean and
            // fast way
            // see
            // http://llvm.org/docs/ProgrammersManual.html#creating-and-inserting-new-instructions
            IRBuilder<> Builder(BinOp);

            Value *NewValue = Builder.CreateAdd(
              Builder.CreateXor(BinOp->getOperand(0),
                                BinOp->getOperand(1)),
              Builder.CreateMul(
                ConstantInt::get(BinOp->getType(), 2),
                Builder.CreateAnd(
                  BinOp->getOperand(0),
                  BinOp->getOperand(1)))
            );

            DEBUG(dbgs() << *BinOp << " -> " << *NewValue << "\n");

            // ReplaceInstWithValue basically does this (`IIT' is passed by reference):
            // IIT->replaceAllUsesWith(NewValue);
            // IIT = BB.getInstList.erase(IIT);
            //
            // `erase' returns a valid iterator of the instruction before the
            // one that has been erased. This keeps iterators valid.
            //
            // see also
            // http://llvm.org/docs/ProgrammersManual.html#replacing-an-instruction-with-another-value
            ReplaceInstWithValue(BB.getInstList(),
                                 IIT, NewValue);
            modified = true;

            // update statistics!
            // They are printed out with -analyze on the opt command line
            ++MBACount;
      }
    }
    return modified;
  }
};
}

// pass registration is done through the constructor of static objects...

/* opt pass registration
 */
char MBA::ID = 0;
static RegisterPass<MBA>
    X("mba",  // the option name -> -mba
      "Mixed Boolean Arithmetic Substitution", // option description
      true, // true as we don't modify the CFG
      false // true if we're writing an analysis
      );

/* clang pass registration (optional)
 */
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

static void registerClangPass(const PassManagerBuilder &,
                              legacy::PassManagerBase &PM)
{ PM.add(new MBA()); }

// Note the registration point, clang offers several insertion point where you
// can insert your pass
static RegisterStandardPasses RegisterClangPass
    (PassManagerBuilder::EP_EarlyAsPossible, registerClangPass);
