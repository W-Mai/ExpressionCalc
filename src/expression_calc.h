//
// Created by W-Mai on 2022/10/2.
//

#ifndef EXPRESSIONCALC_EXPRESSION_CALC_H
#define EXPRESSIONCALC_EXPRESSION_CALC_H

#include <cmath>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

typedef std::map<char, int>      TokenLevel_t;
typedef std::vector<std::string> Notation_t;

enum class ErrorType {
    Well,
    FunctionNotFound,
    EvalError,
    SyntaxError,
    BracketNotMatched
};

struct Error {
    ErrorType   type = ErrorType::Well;
    std::string msg;
};

typedef double (*MyFunc_t)(const double* params, const int num);
typedef std::map<std::string, std::pair<int, MyFunc_t>> NoteTable_t;

#define ERROR(err, type_, msg_, res_) \
    (err).type = (type_);             \
    (err).msg  = (msg_);              \
    return res_

#define LAMBDA_EXPR(expr) [](const double* params, const int num) -> double { return expr; }

extern std::map<ErrorType, std::string> ErrorType2Name;
extern TokenLevel_t                     TokenLevel;
extern NoteTable_t                      NoteTable;

Notation_t reversePolishNotation(const std::string& expression, const TokenLevel_t& tokenLevel, Error& e);

inline bool checkNumber(std::string::const_iterator& it, const std::string& expression);
std::string readNumber(std::string::const_iterator& it, const std::string& expression);
inline bool checkFunc(std::string::const_iterator& it, const std::string& expression);
std::string readFunc(std::string::const_iterator& it, const std::string& expression);

double evalNotation(const Notation_t& notation, Error& e);

#endif // EXPRESSIONCALC_EXPRESSION_CALC_H
