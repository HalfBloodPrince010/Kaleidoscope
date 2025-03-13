#ifndef __LLVM_SUPPORT_H__
#define __LLVM_SUPPORT_H__

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include <memory>
#include <string>
#include <map>


/*
The LLVMContext class serves as the environment for all LLVM operations and owns critical 
data structures including type tables, constant pools, and metadata repositories.
*/
extern llvm::LLVMContext TheContext;


/*
The IRBuilder class is your primary tool for generating LLVM instructions. It maintains an 
insertion point (where new instructions will be placed) and provides convenient methods for 
creating all instruction types.
*/
extern llvm::IRBuilder<> Builder;


/*
A Module is the top-level container for all LLVM IR code, representing a compilation unit or 
translation unit in traditional compiler terminology. It holds functions, global variables, 
and other global symbols that comprise your program.
*/
extern std::unique_ptr<llvm::Module> TheModule;


/*
The NamedValues map isn't a built-in LLVM class but a pattern used to associate variable names 
from your source language with their LLVM representations
*/
extern std::map<std::string, llvm::Value *> NamedValues;

#endif