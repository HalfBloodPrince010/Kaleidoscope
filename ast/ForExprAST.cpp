#include "ForExprAST.h"
#include "llvmSupport.h"
#include "logger.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"


llvm::Value *ForExprAST::codegen() {
    // Emit the start code
    llvm::Value *StartVal = Start->codegen();
    if(!StartVal) {
        return nullptr;
    }

    // Preheader (using current block as preheader)
    // Also Make the new basic block for the loop header, inserting after current block.
    llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();
    llvm::BasicBlock *PreheaderBB = Builder->GetInsertBlock();
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(*TheContext, "loop", TheFunction);

    Builder->CreateBr(LoopBB);

    // Start inserting at the Loop BB
    Builder->SetInsertPoint(LoopBB);
    llvm::PHINode *Variable = Builder->CreatePHI(llvm::Type::getDoubleTy(*TheContext), 2, VarName);
    Variable->addIncoming(StartVal, PreheaderBB);

    /*
    x = 5
    for x in 1..10:  # Shadows outer x
        print(x)
    print(x)  # Should still be 5
    */
    llvm::Value *OldVal = NamedValues[VarName];
    NamedValues[VarName] = Variable;

    // Emit the body of the loop.
    if (!Body->codegen()) {
        return nullptr;
    }

    llvm::Value *StepVal = nullptr;
    if(Step) {
        StepVal = Step->codegen();
        if(!StepVal) {
            return nullptr;
        }
    } else {
        // If not specified, use 1.0.
        StepVal = llvm::ConstantFP::get(*TheContext, llvm::APFloat(1.0));
    }
    llvm::Value *NextVar = Builder->CreateFAdd(Variable, StepVal, "nextvar");

    // Compute End Condition
    llvm::Value *EndCond = End->codegen();
    if(!EndCond) {
        return nullptr;
    }

    // Convert condition to a bool by comparing non-equal to 0.0.
    EndCond = Builder->CreateFCmpONE(EndCond, llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0)), "loopcond");

    // Create the "after loop" block and insert it.
    llvm::BasicBlock *LoopEndBB = Builder->GetInsertBlock();
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(*TheContext, "afterloop", TheFunction);

    Builder->CreateCondBr(EndCond, LoopBB, AfterBB);
    Builder->SetInsertPoint(AfterBB);

    Variable->addIncoming(NextVar, LoopEndBB);

    // Restore the unshadowed variable.
    if (OldVal)
        NamedValues[VarName] = OldVal;
    else
        NamedValues.erase(VarName);

    // for expr always returns 0.0.
    return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(*TheContext));
}