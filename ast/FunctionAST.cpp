#include "FunctionAST.h"
#include "llvmSupport.h"
#include "llvm/IR/BasicBlock.h"


llvm::Function *FunctionAST::codegen() {
    /// When Function Decl F was created, Proto added it to the LLVM Module
    llvm::Function *TheFunction = TheModule->getFunction(Prototype->getName());

    if (!TheFunction) {
        TheFunction = Prototype->codegen();
    }
    
    if (!TheFunction) {
        return nullptr;
    }

    // Create a Basic Block for the Function
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", TheFunction);
    Builder.SetInsertPoint(BB);

    NamedValues.clear();
    /// Add the Function Parameter to Value Mapping, Body of the Function when using VariableExpr will fetch from NamedValues
    for(auto &Arg: TheFunction->args()) {
        NamedValues[Arg.getName().data()] = &Arg;  /// { a : *LLVM::Value} 
    }

    if(llvm::Value *retVal = Body->codegen()) {
        Builder.CreateRet(retVal);

        verifyFunction(*TheFunction);
        
        return TheFunction;
    }

    TheFunction->eraseFromParent();
    return nullptr;
}