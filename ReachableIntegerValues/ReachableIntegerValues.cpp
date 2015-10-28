/**
 * Analysis of registers that can be accessed by a BB
 *
 * This should produce a mapping between basic blocks and set of registers.
 *
 * It showcases the development of an Analysis.
 */

#include "ReachableIntegerValues.h"

#include "llvm/IR/Dominators.h"
#include "llvm/Support/Debug.h"

#include <deque>

#define DEBUG_TYPE "reachable-integer-values"

using namespace llvm;

ReachableIntegerValuesPass::ReachableIntegerValuesPass() : FunctionPass(ID) {}

bool ReachableIntegerValuesPass::runOnFunction(Function &F) {
  // The same instance of the analysis is created and registered, then used
  // repetitively, so we must clear its state each time we enter runOnFunction
  ReachableIntegerValuesMap.clear();

  // First compute sets of integer values defined for each Basic block
  ReachableIntegerValuesMapTy DefinedValuesMap;
  for (BasicBlock &BB : F) {
    auto &Values = DefinedValuesMap[&BB];
    for (Instruction &Inst : BB)
      if (Inst.getType()->isIntegerTy())
        Values.insert(&Inst);
  }

  // Second compute the reachable integer values

  // arguments and globals are always live
  auto &HeadValues = ReachableIntegerValuesMap[&F.getEntryBlock()];
  for (Argument &Arg : F.args())
    HeadValues.insert(&Arg);

  // then use dominance tree to merge relevant sets
  auto *Root =
      getAnalysis<DominatorTreeWrapperPass>().getDomTree().getRootNode();
  std::deque<decltype(Root)> NodesToProcess;
  NodesToProcess.push_back(Root);

  DEBUG(errs() << "In Function:\n" << F);

  while (!NodesToProcess.empty()) {
    auto *NodeToProcess = NodesToProcess.back();
    NodesToProcess.pop_back();
    DEBUG(errs() << "processing BB " << NodeToProcess->getBlock() << "\n");
    auto &ParentDefinedValues = DefinedValuesMap[NodeToProcess->getBlock()];
    auto &ParentReachableValues =
        ReachableIntegerValuesMap[NodeToProcess->getBlock()];
    // Add the ParentDefinedValues and ParentReachableValues to all children's
    // ReachableIntegerValues.
    for (auto *Child : *NodeToProcess) {
      DEBUG(errs() << "updating dominated child " << Child->getBlock() << "\n");
      // Add the child to the work list now that it is processed.
      NodesToProcess.push_back(Child);

      auto ChildReachableValue = ReachableIntegerValuesMap[Child->getBlock()];
      // add defined values to dominated nodes
      ChildReachableValue.insert(ParentDefinedValues.begin(),
                                 ParentDefinedValues.end());
      // add inherited values from dominating node
      ChildReachableValue.insert(ParentReachableValues.begin(),
                                 ParentReachableValues.end());
    }
  }

  // An analysis should not modify its argument
  return false;
}

// This instructs the PassManager of the analyses required and preserved by
// this pass. The Pass Manager will schedule required passes earlier in the
// pipeline and make them available for this pass. Identifying the preserved
// analyses allows to save compile time by avoiding to recompute analysis when
// the results won't change.
// Analyses are read-only, so they generally preserve everything
// see
// http://llvm.org/docs/WritingAnLLVMPass.html#specifying-interactions-between-passes
void ReachableIntegerValuesPass::getAnalysisUsage(AnalysisUsage &Info) const {
  Info.addRequired<DominatorTreeWrapperPass>();
  Info.setPreservesAll();
}

ReachableIntegerValuesPass::ReachableIntegerValuesMapTy const &
ReachableIntegerValuesPass::getReachableIntegerValuesMap() const {
  return ReachableIntegerValuesMap;
}

char ReachableIntegerValuesPass::ID = 0;
static RegisterPass<ReachableIntegerValuesPass>
    X("reachable-integer-values",         // pass option
      "Compute Reachable Integer values", // pass description
      true,                               // does not modify the CFG
      true                                // and it's an analysis
      );
