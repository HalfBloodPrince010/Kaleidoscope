#ifndef _FOR_EXPR_AST_H
#define _FOR_EXPR_AST_H

#include <memory>
#include <string>
#include "ExprAST.h"

class ForExprAST: public ExprAST {
    std::string VarName;  
    std::unique_ptr<ExprAST> Start, End, Step, Body;

public:
    ForExprAST(const std::string &VarName, std::unique_ptr<ExprAST> Start, 
        std::unique_ptr<ExprAST> End, std::unique_ptr<ExprAST> Step, std::unique_ptr<ExprAST> Body): VarName(VarName), Start(std::move(Start)), End(std::move(End)), Step(std::move(Step)), Body(std::move(Body)) {}

    llvm::Value *codegen() override;
};


#endif