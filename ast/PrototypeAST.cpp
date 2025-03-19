#include "PrototypeAST.h"
#include "llvmSupport.h"
#include "llvm/IR/Function.h"


llvm::Function *PrototypeAST::codegen() {
     ///  Create function prototype: returnType name(a, b ...)
     /// Function Parameters are double by default in this lang
     std::vector<llvm::Type *> Doubles(Args.size(), llvm::Type::getDoubleTy(*TheContext));
     /// FunctionType --> <returnType, Params, isVarArg Function>
     llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*TheContext), Doubles, false);
     /// Create the Function using above Type, which get added to the current Module
     llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule.get());

     /// Argument naming: Finally, we set names for the function arguments
     unsigned Idx = 0;
     for(auto &arg : F->args()) {
        arg.setName(Args[Idx]);
        Idx++;
     }

     return F;
}