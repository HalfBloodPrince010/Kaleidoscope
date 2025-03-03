#ifndef __PARSER_H__
#define __PARSER_H__

#include "ExprAST.h"
#include "NumberExprAST.h"
#include "VariableExprAST.h"
#include "CallExprAST.h"
#include "lexer.h"
#include "token.h"

#include <memory>
#include <vector>

std::unique_ptr<ExprAST> ParseNumberExpr();
std::unique_ptr<ExprAST> ParseParenExpr();
std::unique_ptr<ExprAST> ParseIdentifierExpr();
std::unique_ptr<ExprAST> ParseExpression();
std::unique_ptr<ExprAST> ParsePrimary();


#endif