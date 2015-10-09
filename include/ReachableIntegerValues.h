#include "llvm/Pass.h"
#include "llvm/IR/ValueMap.h"

#include <set>

class ReachableIntegerValuesPass : public llvm::FunctionPass {

  public:
    using ReachableIntegerValuesMapTy = llvm::ValueMap< llvm::BasicBlock const*, std::set<llvm::Value *>>;
    static char ID;
    ReachableIntegerValuesPass();
    bool runOnFunction(llvm::Function&) override;
    void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;
    ReachableIntegerValuesMapTy const& getReachableIntegerValuesMap() const;

  private:

    ReachableIntegerValuesMapTy ReachableIntegerValuesMap;
};
