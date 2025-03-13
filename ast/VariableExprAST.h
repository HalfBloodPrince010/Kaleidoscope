#ifndef __VARIABLE_EXPR_AST_H__
#define __VARIABLE_EXPR_AST_H__

#include <string>
#include "ExprAST.h"


// Expression class for variable literals like "abc"
class VariableExprAST : public ExprAST {
  std::string Name;

public:
  VariableExprAST(std::string &Name): Name(Name) {}
  llvm::Value *codegen() override;
};

#endif
