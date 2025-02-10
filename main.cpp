#include"lexer/lexer.h"
#include "lexer/token.h"
#include <cstdio>
#include <iostream>

static void MainLoop() {
  while (true) {
    fprintf(stderr, "ready> ");
    switch (CurTok) {
    case TOKEN_EOF:
      std::cout << "End of File Token: " << TOKEN_EOF << "\n";
      return;
    case ';': // ignore top-level semicolons.
      getNextToken();
      break;
    case TOKEN_DEF:
      std::cout << "Definition Token: " << TOKEN_DEF << "\n";
      getNextToken();
      break;
    case TOKEN_NUMBER:
      std::cout << "Number Token: " << TOKEN_NUMBER << "-->" << NumVal << "\n";
      getNextToken();
      break;
    case TOKEN_EXTERN:
      std::cout << "Extern Token: " << TOKEN_EXTERN << "\n";
      getNextToken();
      break;
    default:
      std::cout << "Default Token: " << IdentifierStr << "\n";
      getNextToken();
      break;
    }
  }
}

int main() {
    fprintf(stderr, "ready> ");

    getNextToken();
    MainLoop();

    return 0;
}