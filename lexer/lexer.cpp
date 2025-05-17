#include "lexer.h"
#include "token.h"
#include <cctype>
#include <cstdio>
#include <string>
#include<iostream>

int CurTok;
std::string IdentifierStr;
double NumVal;


int getToken() {
    // Skip all the whitespace
    static int LastChar = ' ';
    
    while(isspace(LastChar)) {
        LastChar = getchar();
    }

    if(isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while(isalnum(LastChar = getchar())) {
            IdentifierStr += LastChar;
        }

        if(IdentifierStr == "def") {
            // Function Definition
            return TOKEN_DEF;
        }

        if(IdentifierStr == "extern") {
            return TOKEN_EXTERN;
        }

        if(IdentifierStr == "if") {
            return TOKEN_IF;
        }

        if(IdentifierStr == "then") {
            return TOKEN_THEN;
        }

        if(IdentifierStr == "else") {
            return TOKEN_ELSE;
        }

        if(IdentifierStr == "for") {
            return TOKEN_FOR;
        }

        if(IdentifierStr == "in") {
            return TOKEN_IN;
        }

        return TOKEN_IDENTIFIER;
    }


    if(isdigit(LastChar) || LastChar == '.') {
        // Floating point digit 9.098 or .0909
        std::string numberLexeme;
        
        do {
            numberLexeme += LastChar;
            LastChar = getchar();
        } while(isdigit(LastChar) || LastChar == '.');

        NumVal = std::stod(numberLexeme);
        return TOKEN_NUMBER;
    }

    if(LastChar == '#') {
        // Comment
        do {
            LastChar = getchar();
        } while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) {
            // We start from beginning, this return ensures we always return a meaningful TOKEN
            return getToken();
        }
    }

    if (LastChar == EOF) {
        return TOKEN_EOF;
    }

    // For other tokens like +, -, we can parse it to TOKEN_PLUS, TOKEN_STAR etc.
    // Instead we just return the ascii.
    int currChar = LastChar;
    // static doesn't update the LastChar, hence we need to progress it for next Token Cycle.
    LastChar = getchar();
    return currChar;
};

int getNextToken() {
    return CurTok = getToken();
};