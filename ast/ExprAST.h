#ifndef __EXPR_AST_H__
#define __EXPR_AST_H__

#include <llvm/IR/IRBuilder.h>

class ExprAST {
public:
  virtual ~ExprAST() {};
  virtual llvm::Value *codegen() = 0;
};

#endif
