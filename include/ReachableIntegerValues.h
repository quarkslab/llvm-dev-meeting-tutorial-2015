#include "llvm/Pass.h"
#include "llvm/IR/ValueMap.h"

#include <set>

class ReachableIntegerValuesPass : public llvm::FunctionPass {

public:
  using ReachableIntegerValuesMapTy =
      llvm::ValueMap<llvm::BasicBlock const *, std::set<llvm::Value *>>;
  static char ID;
  ReachableIntegerValuesPass();

  void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;
  bool runOnFunction(llvm::Function &) override;
  ReachableIntegerValuesMapTy const &getReachableIntegerValuesMap() const;

private:
  ReachableIntegerValuesMapTy ReachableIntegerValuesMap;
};
