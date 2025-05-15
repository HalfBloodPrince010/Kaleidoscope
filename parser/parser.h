#ifndef __PARSER_H__
#define __PARSER_H__

#include "ExprAST.h"
#include "NumberExprAST.h"
#include "VariableExprAST.h"
#include "CallExprAST.h"
#include "BinaryExprAST.h"
#include "PrototypeAST.h"
#include "FunctionAST.h"
#include "IfExprAST.h"
#include "lexer.h"
#include "token.h"
#include "logger.h"

#include <_ctype.h>
#include <algorithm>
#include <map>
#include <memory>
#include <vector>

extern std::map<char, int> BinopPrecedence;
std::unique_ptr<ExprAST> ParseNumberExpr();
std::unique_ptr<ExprAST> ParseParenExpr();
std::unique_ptr<ExprAST> ParseIdentifierExpr();
std::unique_ptr<ExprAST> ParseExpression();
std::unique_ptr<ExprAST> ParsePrimary();
std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
std::unique_ptr<ExprAST> ParseIfExpr();
std::unique_ptr<PrototypeAST> ParsePrototype();
std::unique_ptr<PrototypeAST> ParseExtern();
std::unique_ptr<FunctionAST> ParseDefinition();
std::unique_ptr<FunctionAST> ParseTopLevelExpr();


#endif