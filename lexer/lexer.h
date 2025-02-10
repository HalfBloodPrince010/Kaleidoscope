#ifndef __LEXER_H__
#define __LEXER_H__

#include<cstdlib>
#include<string>

// Buffer to hold the current token, that is being Parsed
extern int CurTok;

int getToken();

// Gets the next token by calling getToken and stores it in CurTok
int getNextToken();

// If the Token parsed is an Identifier, the
// IdentifierStr holds the string "literal"
extern std::string IdentifierStr;

// If the Token parsed is a number, the
// NumVal holds the number "literal"
extern double NumVal;


#endif