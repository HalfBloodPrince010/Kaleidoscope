#include "logger.h"


std::unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "LogError: %s", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}