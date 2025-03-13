#include "llvmSupport.h"

/*
The LLVMContext objects owns core LLVM Data Structures like LLVM Types
*/
llvm::LLVMContext TheContext;


/*
The IRBuilder object helps generate IR Instructions
*/
llvm::IRBuilder<> Builder(TheContext);;


/*
A Module represents a compilation unit or translation unit in traditional 
compiler terminology. It holds functions, global variables,  and other 
global symbols that comprise your program.
*/
std::unique_ptr<llvm::Module> TheModule;


/*
The NamedValues map is used to associate variable names 
from your source language with their LLVM Value/Alloca representations.
*/
std::map<std::string, llvm::Value *> NamedValues;
