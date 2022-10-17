//
// Created by W-Mai on 2022/10/2.
//

#ifndef EXPRESSIONCALC_EXPRESSION_CALC_H
#define EXPRESSIONCALC_EXPRESSION_CALC_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

#define LAMBDA_EXPR(expr) [](const double* params, const int num) -> double { return expr; }

namespace XCLZ {

enum class ErrorType {
    Well,
    FunctionNotFound,
    EvalError,
    SyntaxError,
    BracketNotMatched
};

typedef struct Error {
    ErrorType   type = ErrorType::Well;
    std::string msg;
} Error_t;

typedef std::map<char, int>              TokenLevel_t;
typedef std::vector<std::string>         Notation_t;
typedef std::string                      Expression_t;
typedef std::string                      String_t;
typedef std::string::const_iterator      ExpressionIt_t;
typedef std::map<ErrorType, std::string> ErrorType2Name_t;

typedef double (*MyFunc_t)(const double* params, const int num);
typedef std::map<std::string, std::pair<int, MyFunc_t>> NoteTable_t;

// Class Declaration
class eXpressionCalc {

private:
    ErrorType2Name_t ErrorType2Name;
    TokenLevel_t     TokenLevel;
    NoteTable_t      NoteTable;
    Error_t          Error;

    Expression_t   Expr;
    ExpressionIt_t ExprIt;

    void resetError();

    bool     eatWhitespace();
    bool     checkNumber();
    String_t readNumber();
    bool     checkFunc();
    String_t readFunc();
    bool     isNumber(const Expression_t& expr);

public:
    eXpressionCalc();
    Notation_t reversePolishNotation();
    double     evalNotation(const Notation_t& notation);

    void           setExpression(Expression_t expression);
    const Error_t& getError();
    String_t       errorToString();

    void addFunc(const String_t& note, int pNum, MyFunc_t func);
    void addToken(char token, int level);
};
}

#endif // EXPRESSIONCALC_EXPRESSION_CALC_H
