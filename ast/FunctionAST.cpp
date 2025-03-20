#include "FunctionAST.h"
#include "PrototypeAST.h"
#include "llvmSupport.h"
#include "llvm/IR/BasicBlock.h"


llvm::Function *getFunction(std::string Name) {
    // Check if the Function is already defined in the Current Module
    if(auto *F = TheModule->getFunction(Name)) {
        return F;
    }

    // If not check whether we can codegen the declaration (not definition) from the existing Prototye
    auto FI = FunctionProtos.find(Name);
    if(FI != FunctionProtos.end()) {
        return FI->second->codegen();
    }

    // If no existing prototype exists, return null.
  return nullptr;
}


llvm::Function *FunctionAST::codegen() {
    // Transfer ownership of the prototype to the FunctionProtos map, but keep a
    // reference to it for use below.
    auto &P = *Prototype;
    FunctionProtos[Prototype->getName()] = std::move(Prototype);
    llvm::Function *TheFunction = getFunction(P.getName());

    if (!TheFunction) {
        TheFunction = Prototype->codegen();
    }
    
    if (!TheFunction) {
        return nullptr;
    }

    // Create a Basic Block for the Function
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    NamedValues.clear();
    /// Add the Function Parameter to Value Mapping, Body of the Function when using VariableExpr will fetch from NamedValues
    for(auto &Arg: TheFunction->args()) {
        NamedValues[Arg.getName().data()] = &Arg;  /// { a : *LLVM::Value} 
    }

    if(llvm::Value *retVal = Body->codegen()) {
        Builder->CreateRet(retVal);

        verifyFunction(*TheFunction);

        // Optimize the function.
        TheFPM->run(*TheFunction, *TheFAM);
        
        return TheFunction;
    }

    TheFunction->eraseFromParent();
    return nullptr;
}