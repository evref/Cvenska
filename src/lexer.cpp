#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>

enum Token {
    tok_eof = -1,

    //commands
    tok_int = -2,
    tok_extern = -3,
    tok_return = -4,

    // symbols
    tok_lpar = -5,
    tok_rpar = -6,
    tok_lcurl = -7,
    tok_rcurl = -8,
    tok_semicolon = -9,
    tok_plus = -10,
    tok_equal = -11,

    // primary
    tok_identifier = -12,
    tok_num = -13,
};

static std::string IdentifierStr;
static int IntVal;

static int gettok() {
    static int LastChar = ' ';

    while (isspace(LastChar)) LastChar = getchar();

    if (isalpha(LastChar)) { // identifier [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar()))) IdentifierStr += LastChar;

        if (IdentifierStr == "hel") return tok_int;
        if (IdentifierStr == "extern") return tok_extern;
        if (IdentifierStr == "returnera") return tok_return;

        return tok_identifier;
    }

    if (isdigit(LastChar)) { 
        std::string IntStr;

        do {
            IntStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar));

        IntVal = atoi(IntStr.c_str());
        return tok_num;
    }

    if (LastChar == '/' && (LastChar = getchar()) == '/') {
        do {
            LastChar = getchar();
        } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) return gettok();
    }

    if (LastChar == EOF) return tok_eof;

    int ThisChar = LastChar;
    LastChar = getchar();

    if (ThisChar == 40) return tok_lpar;
    if (ThisChar == 41) return tok_rpar;
    if (ThisChar == 123) return tok_lcurl;
    if (ThisChar == 125) return tok_rcurl;
    if (ThisChar == 59) return tok_semicolon;
    if (ThisChar == 43) return tok_plus;
    if (ThisChar == 61) return tok_equal;

    return ThisChar;
}

int main() {
    int tok = 0;
    while (tok != tok_eof) {
        tok = gettok();
        std::cout << tok << std::endl; 
    }
}
