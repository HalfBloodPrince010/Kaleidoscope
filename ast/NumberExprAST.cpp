#include "NumberExprAST.h"
#include "llvmSupport.h"
#include "llvm/ADT/APFloat.h"


llvm::Value *NumberExprAST::codegen() {
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Val));
}