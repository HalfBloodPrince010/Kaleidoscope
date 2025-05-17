#ifndef __TOKEN_H__
#define __TOKEN_H__

enum TOKEN {
    // End of File
    TOKEN_EOF = -1,

    TOKEN_DEF = -2,
    TOKEN_EXTERN = -3,

    // Primary
    TOKEN_IDENTIFIER = -4,
    TOKEN_NUMBER = -5,

    // Control Flow
    TOKEN_IF = -6,
    TOKEN_THEN = -7,
    TOKEN_ELSE = -8,
    TOKEN_FOR = -9,
    TOKEN_IN = -10
};


#endif