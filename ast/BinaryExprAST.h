#ifndef __BINARY_EXPR_AST_H__
#define __BINARY_EXPR_AST_H__

#include <memory>
#include "ExprAST.h"


class BinaryExprAST : public ExprAST {
  char Opcode;
  std::unique_ptr<ExprAST> LHS, RHS;

public:
  BinaryExprAST(char Opcode, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS): Opcode(Opcode), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

#endif
