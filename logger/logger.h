#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "ExprAST.h"
#include "PrototypeAST.h"
#include<memory>

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
llvm::Value *LogErrorV(const char *Str);

#endif