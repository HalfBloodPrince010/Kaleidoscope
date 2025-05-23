#ifndef __FUNCTION_AST_H__
#define __FUNCTION_AST_H__

#include "ExprAST.h"
#include "PrototypeAST.h"
#include <memory>

extern llvm::Function *getFunction(std::string Name);

// FunctionAST - This class represents a function definition itself.
// Prototye + Body of the Function
/*
Foo(a, b, c) a + b + c;
*/
class FunctionAST {
  std::unique_ptr<PrototypeAST> Prototype;
  std::unique_ptr<ExprAST> Body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Prototype, std::unique_ptr<ExprAST> Body): Prototype(std::move(Prototype)), Body(std::move(Body)) {}
  llvm::Function *codegen();
};

#endif
