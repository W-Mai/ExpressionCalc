//
// Created by W-Mai on 2022/10/2.
//

#ifndef EXPRESSIONCALC_EXPRESSION_CALC_H
#define EXPRESSIONCALC_EXPRESSION_CALC_H

#include <iostream>
#include <cmath>
#include <map>
#include <stack>
#include <vector>

typedef std::map<char, int> TokenLevel_t;
typedef std::vector<std::string> Notation_t;

enum class ErrorType {
    Well,
    FunctionNotFound,
    EvalError,
    SyntaxError,
    BracketNotMatched
};

struct Error {
    ErrorType type = ErrorType::Well;
    std::string msg;
};

extern std::map<ErrorType, std::string> ErrorType2Name;
extern const TokenLevel_t TokenLevel;
extern const std::map<std::string, int> NoteTable;

Notation_t reversePolishNotation(const std::string& expression, const TokenLevel_t& tokenLevel, Error& e);

inline bool checkNumber(std::string::const_iterator& it, const std::string& expression);
std::string readNumber(std::string::const_iterator& it, const std::string& expression);
inline bool checkFunc(std::string::const_iterator& it, const std::string& expression);
std::string readFunc(std::string::const_iterator& it, const std::string& expression);

double evalNotation(const Notation_t& notation, Error& e);

#endif // EXPRESSIONCALC_EXPRESSION_CALC_H
