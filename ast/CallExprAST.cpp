#include "CallExprAST.h"
#include "FunctionAST.h"
#include "llvmSupport.h"
#include "logger.h"


llvm::Value *CallExprAST::codegen() {
    // Function should already be defined in order to call
    // The Module has all the function and global variables defined.
    llvm::Function *CalleeF = getFunction(Callee);

    if(!CalleeF) {
        return LogErrorV("Unknown Function Referenced\n");
    }

    if(CalleeF->arg_size() != Args.size()) {
        return LogErrorV("Incorrect # of Arguments Passed");
    }

    // Convert the Args into LLVM Values
    std::vector<llvm::Value *> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; i++) {
        ArgsV.push_back(Args[i]->codegen());

        if (!ArgsV.back()) {
            return nullptr;
        }
    }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}