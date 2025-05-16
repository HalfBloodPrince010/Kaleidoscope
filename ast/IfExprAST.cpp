#include "IfExprAST.h"
#include "llvmSupport.h"
#include "logger.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"

llvm::Value *IfExprAST::codegen() {
    llvm::Value *CondV = Cond->codegen();

    if(!CondV) {
        return nullptr;
    }

    // Convert condition to a bool by comparing non-equal to 0.0.
    CondV = CondV = Builder->CreateFCmpONE(
        CondV, 
        llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0)), 
        "ifcond"
    );
    
    llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();
    
    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // // end of the function.
    // Note only this BB is added to the Function
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(*TheContext, "then", TheFunction);

    llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(*TheContext, "else");
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*TheContext, "ifcont");

    Builder->CreateCondBr(CondV, ThenBB, ElseBB);

    // Emit then value.
    Builder->SetInsertPoint(ThenBB);
    
    llvm::Value *ThenV = Then->codegen();
    if (!ThenV) {
        return nullptr;
    }
    
    Builder->CreateBr(MergeBB);
    /*
    The final line here is quite subtle, but is very important. The basic issue is that when we create the Phi node 
    in the merge block, we need to set up the block/value pairs that indicate how the Phi will work. Importantly, 
    the Phi node expects to have an entry for each predecessor of the block in the CFG.

    The reason for again getting the "basic block" which we were already in is because the the Then codegen could have had
    nested if else case which would have changed the basic block. Hence to an edge to MergeBB we need the right BB.
    */
    ThenBB = Builder->GetInsertBlock();

    // This BB was not added to function before
    TheFunction->insert(TheFunction->end(), ElseBB);
    Builder->SetInsertPoint(ElseBB);
    llvm::Value *ElseV = Else->codegen();
    if(!ElseV) {
        return nullptr;
    }

    Builder->CreateBr(MergeBB);
    ElseBB = Builder->GetInsertBlock();


    // PHI Nodes
    // This BB was not added to function before
    TheFunction->insert(TheFunction->end(), MergeBB);
    Builder->SetInsertPoint(MergeBB);
    llvm::PHINode *PN = Builder->CreatePHI(llvm::Type::getDoubleTy(*TheContext), 2, "iftmp");
    PN->addIncoming(ThenV, ThenBB);
    PN->addIncoming(ElseV, ElseBB);
    return PN;

}