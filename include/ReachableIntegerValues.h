#include "llvm/Pass.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/ADT/SmallPtrSet.h"


class ReachableIntegerValuesPass : public llvm::FunctionPass {

public:
  using ReachableIntegerValuesMapTy =
      llvm::ValueMap<llvm::BasicBlock const *, llvm::SmallPtrSet<llvm::Value *,8>>;
  static char ID;
  ReachableIntegerValuesPass();

  void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;
  bool runOnFunction(llvm::Function &) override;
  ReachableIntegerValuesMapTy const &getReachableIntegerValuesMap() const;
  void print(llvm::raw_ostream &O, llvm::Module const *) const override;

private:
  ReachableIntegerValuesMapTy ReachableIntegerValuesMap;
};
