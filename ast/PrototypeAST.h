#ifndef __PROTOTYPE_AST_H__
#define __PROTOTYPE_AST_H__

#include <string>
#include <vector>
#include <llvm/IR/IRBuilder.h>

// Represents the "prototype" for a function, Name and Arguments Names
// No type, as all values by default are double
// Foo(a, b, c);
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &name, std::vector<std::string> Args) : Name(name), Args(std::move(Args)) {}
  llvm::Function *codegen();

  const std::string& getName() const {
    return Name;
  } 
};

#endif
