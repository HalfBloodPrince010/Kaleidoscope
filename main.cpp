// Lexer headers
#include "lexer/lexer.h"
#include "lexer/token.h"

// AST headers
#include "ast/BinaryExprAST.h"
#include "ast/CallExprAST.h"
#include "ast/ExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/VariableExprAST.h"

// LLVM IR Support header
#include "llvm-support/llvmSupport.h"
#include "llvm-support/KaleidoscopeJIT.h"

// Parser headers
#include "parser/parser.h"

// Logger headers
#include "logger/logger.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"

#include <cassert>
#include <cstdio>
#include <iostream>

static void InitializeModuleAndManagers(void) {
  TheContext = std::make_unique<llvm::LLVMContext>();
  TheModule = std::make_unique<llvm::Module>("KaleidoscopeJIT", *TheContext);
  TheModule->setDataLayout(TheJIT->getDataLayout());

  // Create a new builder for the module.
  Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

  // Create Analysis Managers
  TheLAM = std::make_unique<llvm::LoopAnalysisManager>();
  TheFAM = std::make_unique<llvm::FunctionAnalysisManager>();
  TheCGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
  TheMAM = std::make_unique<llvm::ModuleAnalysisManager>();

  // Create Pass Managers
  TheFPM = std::make_unique<llvm::FunctionPassManager>();
  ThePIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
  TheSI = std::make_unique<llvm::StandardInstrumentations>(*TheContext, true);
  
  TheSI->registerCallbacks(*ThePIC, TheMAM.get());

  /// Transformation Passes
  /*
  Local Optimizations like Constant Folding, Algebraic Simplifications like
  Identity (a + 0 = a, b * 1 = b) and strength reduction like (a * 2 --> a << 1)
  */ 
  TheFPM->addPass(llvm::InstCombinePass());

  /*
  Reassociate passes help with reordering instructions to help with 
  commutative and associative optimizations
  */
  TheFPM->addPass(llvm::ReassociatePass());

  /*
  GVN - Global Value Numbering, similar to Local Value Numbering, helps
  with Common Sub Expression elimination.
  */
  TheFPM->addPass(llvm::GVNPass());

  // Simplify the control flow graph (deleting unreachable blocks, etc). aka Dead Code Elimination (DCE)
  TheFPM->addPass(llvm::SimplifyCFGPass());

  /// Register analysis passes used in these transform passes.
  /*
    We need these Analysis Passes to Run before the Trannsformation run because,

    - GVNPass requires dominator tree analysis to determine when expressions compute the same value
    - SimplifyCFGPass needs control flow analysis to safely transform the CFG
  */
  llvm::PassBuilder PB;
  PB.registerModuleAnalyses(*TheMAM);
  PB.registerFunctionAnalyses(*TheFAM);
  PB.crossRegisterProxies(*TheLAM, *TheFAM, *TheCGAM, *TheMAM);
}

static void HandleDefinition() {
  if(auto FnAST = ParseDefinition()) {
    if(auto FnIR = FnAST->codegen()) {
      fprintf(stderr, "Parsed a Function Definition.\n");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");
    }
    
  } else {
    /// Skip - aka Synchronization.
    /* We want to report as many errors as possible in the
    parser
    */
    getNextToken();
  }
}

static void HandleExtern() {
  if(auto ProtoAST = ParseExtern()) {
    if(auto FnIR = ProtoAST->codegen()) {
      fprintf(stderr, "Parsed a Extern Definition.\n");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");
    }
    
  } else {
    /// Skip for error recovery, same as above [synchronization]
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (auto AnonFn = ParseTopLevelExpr()) {
    if(auto FnIR = AnonFn->codegen()) {
      fprintf(stderr, "Parsed a top-level expr\n");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");

      auto RT = TheJIT->getMainJITDylib().createResourceTracker();

      /// 1. Move the LLVM IR module to the JIT, making its functions available for execution.
      auto TSM = llvm::orc::ThreadSafeModule(std::move(TheModule), std::move(TheContext));
      ExitOnErr(TheJIT->addModule(std::move(TSM), RT));

      /// 2. Create a new module for the future code
      InitializeModuleAndManagers();

      // Search the JIT for the __anon_expr symbol.
      auto ExprSymbol = ExitOnErr(TheJIT->lookup("__anon_expr"));
      
      /*
      ExprSymbol.getAddress() - Retrieves the raw memory address where the JIT-compiled function resides. 
      This is the actual location in memory where the machine code for the function has been placed.

      Convert this to a function pointer of returnType double which takes no arguments.
      */
      double (*FP)() = ExprSymbol.getAddress().toPtr<double (*)()>();
      fprintf(stderr, "Evaluated to %f\n", FP());

      // Delete the anonymous expression module from the JIT.
      ExitOnErr(RT->remove());
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void MainLoop() {
  while (true) {
    fprintf(stderr, "ready> ");
    switch (CurTok) {
    case TOKEN_EOF:
      return;
    case ';': // ignore top-level semicolons.
      getNextToken();
      break;
    case TOKEN_DEF:
      HandleDefinition();
      break;
    case TOKEN_EXTERN:
      HandleExtern();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}

int main() {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();

  BinopPrecedence['<'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40;

  fprintf(stderr, "ready> ");

  getNextToken();

  auto jit = llvm::orc::KaleidoscopeJIT::Create();
  if (!jit ){
      LogError("Failed to initialize the JIT compiler. Terminating...");
      return 1;
  }
  TheJIT = std::move(*jit);
  InitializeModuleAndManagers();

  MainLoop();

  return 0;
}