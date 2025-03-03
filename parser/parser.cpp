#include "parser.h"
#include "CallExprAST.h"
#include "ExprAST.h"
#include "lexer.h"
#include "logger.h"
#include "token.h"


/// numberexpr ::= number
std::unique_ptr<ExprAST> ParseNumberExpr() {
    // NumVal holds the number
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken(); // consume the number
    return Result;
}


/// This routine parses expressions between "(" and ")" characters
/// parenexpr ::= '(' expression ')'
std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken(); // Consume '('

    auto V = ParseExpression();
    if(!V) {
        return nullptr;
    }

    if(CurTok != ')') {
        return LogError("Expected ')'");
    }

    getNextToken(); // consume ')'
    return V;
}


/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')' --> CALL EXPR
std::unique_ptr<ExprAST> ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    getNextToken(); // consume the Identifier

    if(CurTok != '(') {
        // Not a CallExpr, just regular identifier
        return std::make_unique<VariableExprAST>(IdName);
    }

    // CallExpr
    getNextToken(); // consume '('
    std::vector<std::unique_ptr<ExprAST>> Args;
    // Check if Function has no Args: Foo()
    if(CurTok != ')') {
        while(true) {
            if (auto Arg = ParseExpression()) {
                Args.push_back(std::move(Arg));
            } else {
                return nullptr;
            }

            if(CurTok == ')') {
                break;
            }

            if(CurTok != ',') {
                return LogError("Expected ')' or ',' in the Argument List");
            }

            getNextToken(); // consume ','
        }
    }

    getNextToken(); // consume ')'

    return std::make_unique<CallExprAST> (IdName, std::move(Args));
}


/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::unique_ptr<ExprAST> ParsePrimary() {
    switch(CurTok) {
        default:
           return LogError("Unknown Token when Parsing the Expression");
        case TOKEN_NUMBER:
            return ParseNumberExpr();
        case TOKEN_IDENTIFIER:
            return ParseIdentifierExpr();
        case '(':
            return ParseParenExpr();
    }
}

/// expression
///   ::= primary
///
std::unique_ptr<ExprAST> ParseExpression() {
    auto V = ParsePrimary();
    if(!V) {
        return nullptr;
    }

    return V;
}