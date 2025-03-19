#ifndef __LLVM_SUPPORT_H__
#define __LLVM_SUPPORT_H__

#include "KaleidoscopeJIT.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include <memory>
#include <string>
#include <map>


/*
The LLVMContext class serves as the environment for all LLVM operations and owns critical 
data structures including type tables, constant pools, and metadata repositories.
*/
extern std::unique_ptr<llvm::LLVMContext> TheContext;



/*
The IRBuilder class is your primary tool for generating LLVM instructions. It maintains an 
insertion point (where new instructions will be placed) and provides convenient methods for 
creating all instruction types.
*/
extern std::unique_ptr<llvm::IRBuilder<>> Builder;


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


/*
The KaleidoscopeJIT class is a simple JIT built specifically for these tutorials, available inside the 
LLVM source code at llvm-src/examples/Kaleidoscope/include/KaleidoscopeJIT.h.
*/
extern std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;


/// Analyis pass Manager
/*
Stores and manages analyses specific to loops within functions. Provides
details about Dominator trees.
*/
extern std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;

/*
Stores analysis results for individual functions
*/
extern std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;

/*
Manages analyses for Call Graph Strongly Connected Components. 
This enables interprocedural analysis across function boundaries.
Can do analysis across multiple function like Context Senstive analysis
*/
extern std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;

/*
Handles module-level analyses that span multiple functions, such as global 
variable analysis.
*/
extern std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;



/// Transform pass Managers
/*
Manages "transform" passes that operate on individual functions.
This is a tranformation passes, this changes the code.
*/
extern std::unique_ptr<llvm::FunctionPassManager> TheFPM;


/*
Provides hooks for events during pass execution such as pass start/end.
*/
extern std::unique_ptr<llvm::PassInstrumentationCallbacks> ThePIC;
/*
Implements common instrumentation like timing the passes and 
printing IR before/after passes.
*/
extern std::unique_ptr<llvm::StandardInstrumentations> TheSI;

extern llvm::ExitOnError ExitOnErr;

#endif