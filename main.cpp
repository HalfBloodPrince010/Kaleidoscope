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

// Parser headers
#include "parser/parser.h"

// Logger headers
#include "logger/logger.h"

#include <cstdio>
#include <iostream>

static void HandleDefinition() {
  if(auto FnAST = ParseDefinition()) {
    fprintf(stderr, "Parsed a Function Definition.\n");
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
    fprintf(stderr, "Parsed a Extern Definition.\n");
  } else {
    /// Skip for error recovery, same as above [synchronization]
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (auto AnonFn = ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr\n");
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
  MainLoop();

  return 0;
}