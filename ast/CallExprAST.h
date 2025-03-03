#ifndef __CALL_EXPR_AST__
#define __CALL_EXPR_AST__

#include "ExprAST.h"
#include <memory>
#include <string>
#include <vector>


// Expression class for function calls
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExprAST> > Args;

public:
  CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST> > Args) : Callee(Callee), Args(std::move(Args)) {}
};

#endif