#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

enum Token {
    tok_eof = -1,

    //commands
    tok_int = -2,
    tok_extern = -3,
    tok_return = -4,

    // primary
    tok_identifier = -5,
    tok_num = -6,
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
    return ThisChar;
}

class ExprAST {
    public:
        virtual ~ExprAST()=default;
};

class NumExprAST : public ExprAST {
    int Value;

    public:
        NumExprAST(int Value) : Value(Value) {}
};

class VarExprASt : public ExprAST {
    std::string Name;

    public:
        VarExprASt(const std::string &Name) : Name(Name) {}
};

class BinExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) 
            : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args) 
            : Callee(Callee), Args(std::move(Args)) {}
};

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

    public:
        PrototypeAST(const std::string &Name, std::vector<std::string> Args) 
            : Name(Name), Args(std::move(Args)) {}

        const std::string &getName() const { return Name; }
};

class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

int main() {
    int tok = 0;
    while (tok != tok_eof) {
        tok = gettok();
        std::cout << tok << std::endl; 
    }
}
