//
// Created by W-Mai on 2022/10/2.
//

#ifndef EXPRESSIONCALC_EXPRESSION_CALC_H
#define EXPRESSIONCALC_EXPRESSION_CALC_H

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
    typedef const std::string&               Expression_t;
    typedef std::string::const_iterator&     ExpressionIt_t;
    typedef std::map<ErrorType, std::string> ErrorType2Name_t;

    typedef double (*MyFunc_t)(const double* params, const int num);
    typedef std::map<std::string, std::pair<int, MyFunc_t>> NoteTable_t;

#define ERROR(type_, msg_, tag_) \
    Error.type = (type_);        \
    Error.msg  = (msg_);         \
    goto tag_

    class eXpressionCalc {

    private:
        ErrorType2Name_t ErrorType2Name;
        TokenLevel_t     TokenLevel;
        NoteTable_t      NoteTable;
        Error_t          Error;

        void resetError();

        inline bool        checkNumber(ExpressionIt_t it, Expression_t expression);
        std::string        readNumber(ExpressionIt_t it, Expression_t expression);
        static inline bool checkFunc(ExpressionIt_t it, Expression_t expression);
        std::string        readFunc(ExpressionIt_t it, Expression_t expression);
        bool               eatWhitespace(ExpressionIt_t it, Expression_t expression);

    public:
        eXpressionCalc();
        Notation_t reversePolishNotation(Expression_t expression);
        double     evalNotation(const Notation_t& notation);
    };
}

#endif // EXPRESSIONCALC_EXPRESSION_CALC_H
