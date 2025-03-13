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

// Parser headers
#include "parser/parser.h"

// Logger headers
#include "logger/logger.h"
#include "llvm/Support/raw_ostream.h"

#include <cstdio>
#include <iostream>

// static void InitializeModule() {
//   // Open a new context and module.
//   TheContext = std::make_unique<llvm::LLVMContext>();
//   TheModule = std::make_unique<llvm::Module>("my cool jit", *TheContext);

//   // Create a new builder for the module.
//   Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
// }

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
  BinopPrecedence['<'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40;

  fprintf(stderr, "ready> ");

  getNextToken();

  TheModule = std::make_unique<llvm::Module>("My Cool JIT", TheContext);

  MainLoop();

  return 0;
}