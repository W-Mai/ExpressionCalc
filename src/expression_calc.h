//
// Created by W-Mai on 2022/10/2.
//

#ifndef EXPRESSIONCALC_EXPRESSION_CALC_H
#define EXPRESSIONCALC_EXPRESSION_CALC_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

typedef std::map<char, int> TokenLevel;
typedef std::vector<std::string> Notation;

enum class ErrorType {
    Well,
    FunctionNotFound,
    EvalError,
    SyntaxError,
    BracketNotMatched
};

struct Error {
    ErrorType type = ErrorType::Well;
    std::string msg = "";
};

extern std::map<ErrorType, std::string> ErrorType2Name;
extern const TokenLevel tokenLevel;
extern const std::map<std::string, int> NoteTable;

Notation reversePolishNotation(const std::string& expression, const TokenLevel& tokenLevel, Error& e);

inline bool checkNumber(std::string::const_iterator& it, const std::string& expression);
std::string readNumber(std::string::const_iterator& it, const std::string& expression);
inline bool checkFunc(std::string::const_iterator& it, const std::string& expression);
std::string readFunc(std::string::const_iterator& it, const std::string& expression);

double evalNotation(const Notation& notation, Error& e);

#endif // EXPRESSIONCALC_EXPRESSION_CALC_H
