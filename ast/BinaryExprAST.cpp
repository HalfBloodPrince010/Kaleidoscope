#include "BinaryExprAST.h"
#include "llvmSupport.h"
#include "logger.h"


llvm::Value *BinaryExprAST::codegen() {
    llvm::Value *L = LHS->codegen();
    llvm::Value *R = RHS->codegen();

    if (!L || !R) {
        return nullptr;
    }

    switch(Opcode) {
        case '+':
          return Builder->CreateFAdd(L, R, "addtmp");
        case '-':
          return Builder->CreateFSub(L, R, "subtmp");
        case '*':
          return Builder->CreateFMul(L, R, "multmp");
        case '<':
           L = Builder->CreateFCmpULT(L, R, "cmptmp");  /// Converts to 0, 1, we need to convert to floating values
           return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
        default:
          return LogErrorV("Invalid Binary Operation");
    }

}