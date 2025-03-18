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


/*
The KaleidoscopeJIT class is a simple JIT built specifically for these tutorials, available inside the 
LLVM source code at llvm-src/examples/Kaleidoscope/include/KaleidoscopeJIT.h.
*/
std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;


/// Analyis pass Manager
/*
Stores and manages analyses specific to loops within functions. Provides
details about Dominator trees.
*/
std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;

/*
Stores analysis results for individual functions
*/
std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;

/*
Manages analyses for Call Graph Strongly Connected Components. 
This enables interprocedural analysis across function boundaries.
Can do analysis across multiple function like Context Senstive analysis
*/
std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;

/*
Handles module-level analyses that span multiple functions, such as global 
variable analysis.
*/
std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;


/// Transform pass Managers
/*
Manages "transform" passes that operate on individual functions.
This is a tranformation passes, this changes the code.
*/
std::unique_ptr<llvm::FunctionPassManager> TheFPM;


/// Instrumentation
/*
Provides hooks for events during pass execution such as pass start/end.
*/
std::unique_ptr<llvm::PassInstrumentationCallbacks> ThePIC;

/*
Implements common instrumentation like timing the passes and 
printing IR before/after passes.
*/
std::unique_ptr<llvm::StandardInstrumentations> TheSI;
