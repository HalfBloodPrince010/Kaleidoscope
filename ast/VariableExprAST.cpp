#include "VariableExprAST.h"
#include "llvmSupport.h"
#include "logger.h"

llvm::Value *VariableExprAST::codegen() {
    llvm::Value *V = NamedValues[Name];

    if(!V) {
        LogErrorV("Variable Name Unknown.");
    }

    return V;
}