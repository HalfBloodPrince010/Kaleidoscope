#include "parser.h"


std::map<char, int> BinopPrecedence;

static int GetTokenPrecedence() {
    if(!isascii(CurTok)) {
        return -1;
    }
    
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0) {
        return -1;
    }

    return TokPrec;
}



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
           return LogError("Unknown Token when Parsing the Expression\n");
        case TOKEN_NUMBER:
            return ParseNumberExpr();
        case TOKEN_IDENTIFIER:
            return ParseIdentifierExpr();
        case '(':
            return ParseParenExpr();
    }
}


/*
binoprhs
   ::= ('+' primary)*

Binary Op using the Operator Precedence Parsing

LBP --> Left Binding Power
RBP --> Right Binding Power

Case 1:
------------------------------------------------------------------
a + b * c;
^ 
ParsePrimary, consumes "a" [LHS], now we need to check if its Binary Op?
expr(0)

expr(RBP) /// Initial Value Passed is 0 
    get Token Precedence for "+" /// RBP =10 for +.
    (LBP < RBP)
    Hence, a is RHS of something. We continue to Parsing, onto "b"
    call expr(10)


a + b * c;
    ^
    currently processing "b"

expr(RBP) /// 10 for "+"
    consume LHS -- say "b"
    next Token -- "*"   /// 40 is the binding power for "*"
    Check if [b is LHS of b * c] or [RHS of a + b]

    ("*", b, c) is RHS of "a + .."
    ("+", a, ("*", b, c))


Case 2:
------------------------------------------------------------------
a;
^
ParsePrimary, consumes "a" [LHS], now we need to check if its Binary Op?
expr(0)

expr(RBP)
    get Token Precedence for ";"  /// -1
    (LBP < RBP)?  False
    return LHS

*/
std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
    while(true) {
        int TokPrec = GetTokenPrecedence();

        if(TokPrec < ExprPrec) {
            return LHS;
        }

        // Then its a Binary Operation, consume the token, and parse RHS
        int Binop = CurTok;
        getNextToken();  /// Consume the Binop

        auto RHS = ParsePrimary();
        if(!RHS) {
            /// If there is a Binop, we expect RHS
            return nullptr;
        }

        int NextPrec = GetTokenPrecedence();
        if(TokPrec < NextPrec) {
            /// RBP < LBP
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if(!RHS) {
                return nullptr;
            }
        }

        LHS = std::make_unique<BinaryExprAST>(Binop, std::move(LHS), std::move(RHS));
    }

}


/// expression
///   ::= primary binoprhs
///
std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if(!LHS) {
        return nullptr;
    }

    return ParseBinOpRHS(0, std::move(LHS));
}


/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<PrototypeAST> ParsePrototype() {
    if(CurTok != TOKEN_IDENTIFIER) {
        return LogErrorP("Expected Function Name Identifier in the Prototype");
    }
    
    std::string FnName = IdentifierStr;
    getNextToken();  /// Consume the id
    if(CurTok != '(') {
        return LogErrorP("Expected '(' after the Function Name Identifier");
    }

    /// atan2(arg1 arg2);
    std::vector<std::string> ArgNames;
    while(getNextToken() == TOKEN_IDENTIFIER) {
        ArgNames.push_back(IdentifierStr);
    }

    if(CurTok != ')') {
        return LogErrorP("Expected ')' after the ArgNames");
    }

    getNextToken(); /// consume )

    return std::make_unique<PrototypeAST>(FnName, ArgNames);
}

/// toplevelexpr ::= expression
/// To allow the evaluation of expressions that are not associated with any named function
std::unique_ptr<FunctionAST> ParseTopLevelExpr() {
    if(auto E = ParseExpression()) {
        // Make a Anonymous Prototype
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }

    return nullptr;
}

/// definition ::= 'def' prototype expression
std::unique_ptr<FunctionAST> ParseDefinition() {
    getNextToken(); /// consume def
    auto Proto = ParsePrototype();
    if (!Proto) {
        return nullptr;
    }

    if(auto Body = ParseExpression()) {
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(Body));
    }

    return nullptr;
}


/// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken(); /// consume extern
    return ParsePrototype();
}