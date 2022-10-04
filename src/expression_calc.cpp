#include "expression_calc.h"

namespace XCLZ {

    eXpressionCalc::eXpressionCalc() {
        ErrorType2Name = {
            {ErrorType::BracketNotMatched, "Bracket Not Matched"},
            { ErrorType::FunctionNotFound, "Function Not Found" },
            { ErrorType::EvalError,        "Evaluate Error"     },
            { ErrorType::SyntaxError,      "Syntax Error"       }
        };

        TokenLevel = {
            {'+',   1},
            { '-',  1},
            { '*',  2},
            { '/',  2},
            { '^',  2},
            { '%',  2},
            { '\\', 2},
            { '(',  0},
            { ')',  0}
        };

        NoteTable = {
            {"+",      { 2, LAMBDA_EXPR(params[0] + params[1]) }                                              },
            { "-",     { 2, LAMBDA_EXPR(params[0] - params[1]) }                                              },
            { "*",     { 2, LAMBDA_EXPR(params[0] * params[1]) }                                              },
            { "/",     { 2, LAMBDA_EXPR(params[0] / params[1]) }                                              },
            { "^",     { 2, LAMBDA_EXPR(powf(params[0], params[1])) }                                         },
            { "%",     { 2, LAMBDA_EXPR(fmod(params[0], params[1])) }                                         },
            { "\\",    { 2, LAMBDA_EXPR((int)((int)params[0] / (int)params[1])) }                             },
            { "add",   { 2, LAMBDA_EXPR(params[0] + params[1]) }                                              },
            { "sub",   { 2, LAMBDA_EXPR(params[0] - params[1]) }                                              },
            { "mul",   { 2, LAMBDA_EXPR(params[0] * params[1]) }                                              },
            { "div",   { 2, LAMBDA_EXPR(params[0] / params[1]) }                                              },
            { "pow",   { 2, LAMBDA_EXPR(powf(params[0], params[1])) }                                         },
            { "mod",   { 2, LAMBDA_EXPR(fmod(params[0], params[1])) }                                         },
            { "divi",  { 2, LAMBDA_EXPR((int)((int)params[0] / (int)params[1])) }                             },
            { "sqrt",  { 1, LAMBDA_EXPR(sqrt(params[0])) }                                                    },
            { "abs",   { 1, LAMBDA_EXPR(fabs(params[0])) }                                                    },
            { "sin",   { 1, LAMBDA_EXPR(sin(params[0])) }                                                     },
            { "cos",   { 1, LAMBDA_EXPR(cos(params[0])) }                                                     },
            { "tan",   { 1, LAMBDA_EXPR(tan(params[0])) }                                                     },
            { "asin",  { 1, LAMBDA_EXPR(asin(params[0])) }                                                    },
            { "acos",  { 1, LAMBDA_EXPR(acos((params[0]))) }                                                  },
            { "atan",  { 1, LAMBDA_EXPR(atan(params[0])) }                                                    },
            { "ln",    { 1, LAMBDA_EXPR(log(params[0])) }                                                     },
            { "log",   { 1, LAMBDA_EXPR(log10(params[0])) }                                                   },
            { "log2",  { 1, LAMBDA_EXPR(log2(params[0])) }                                                    },
            { "floor", { 1, LAMBDA_EXPR(floor(params[0])) }                                                   },
            { "ceil",  { 1, LAMBDA_EXPR(ceil(params[0])) }                                                    },
            { "sign",  { 1, LAMBDA_EXPR(abs(params[0]) < 1e-10 ? 0 : params[0] > 0 ? 1
                                                                                  : -1) }},
            { "PI",    { 0, LAMBDA_EXPR(M_PI) }                                                               },
            { "E",     { 0, LAMBDA_EXPR(M_E) }                                                                },
        };
    }

    void eXpressionCalc::resetError() {
        Error.type = ErrorType::Well;
        Error.msg  = "";
    }

    inline bool eXpressionCalc::checkNumber(ExpressionIt_t it, Expression_t expression) {
        return isdigit(*it)
            || ((it == expression.begin() || *(it - 1) == '(')
                && ((*it == '-' || *it == '+') && (it + 1) != expression.end()))
            || *it == '.' && (it + 1) != expression.end() && isdigit(*(it + 1));
    }

    std::string eXpressionCalc::readNumber(ExpressionIt_t it, Expression_t expression) {
        int         dot_count = 0;
        std::string rtn;
        while (it != expression.end() && checkNumber(it, expression)) {
            if (*it == '.') dot_count++;
            if (dot_count > 1) break;
            if (*it != '+') rtn += *it++;
            else ++it;
        }
        return rtn;
    }

    inline bool eXpressionCalc::checkFunc(ExpressionIt_t it, Expression_t expression) {
        return isalnum(*it);
    }

    std::string eXpressionCalc::readFunc(ExpressionIt_t it, Expression_t expression) {
        std::string rtn;
        while (it != expression.end() && checkFunc(it, expression)) rtn += *it++;
        return rtn;
    }

    bool eXpressionCalc::eatWhitespace(ExpressionIt_t it, Expression_t expression) {
        while (it != expression.end() && (isspace(*it) || *it == ',')) it++;
        return it == expression.end();
    }

    Notation_t eXpressionCalc::reversePolishNotation(Expression_t expression) {
        resetError();
        Notation_t                                                      result;
        std::vector<std::string::const_iterator>                        buffer;
        std::stack<std::pair<std::string, std::string::const_iterator>> tmpFunc;
        for (auto it = expression.begin(); it != expression.end();) {
            if (eatWhitespace(it, expression))
                break;

            auto ch = *it;
            if (!checkNumber(it, expression)) {
                if (checkFunc(it, expression)) {
                    tmpFunc.push({ readFunc(it, expression), it });
                    continue;
                }
                if (ch == '(' || buffer.empty()) {
                    buffer.push_back(it);
                } else if (ch == ')') {
                    while (!buffer.empty() && *buffer.back() != '(') {
                        result.push_back(std::string { *buffer.back() });
                        buffer.pop_back();
                    }

                    if (!tmpFunc.empty() && buffer.back() == tmpFunc.top().second) {
                        result.push_back(tmpFunc.top().first);
                        tmpFunc.pop();
                    }

                    if (buffer.empty()) {
                        ERROR(ErrorType::BracketNotMatched, std::string({ '(' }), ERROR_BUT_RETURN_RESULT_TAG);
                    } else {
                        buffer.pop_back();
                    }

                } else {
                    auto search_res_ch   = TokenLevel.find(ch);
                    auto search_res_back = TokenLevel.find(*buffer.back());
                    if (search_res_ch == TokenLevel.end()) {
                        ERROR(ErrorType::SyntaxError, std::string({ ch }), ERROR_BUT_RETURN_RESULT_TAG);
                    }
                    if (search_res_back == TokenLevel.end()) {
                        ERROR(ErrorType::SyntaxError, std::string({ *buffer.back() }), ERROR_BUT_RETURN_RESULT_TAG);
                    }
                    if (TokenLevel.at(ch) <= TokenLevel.at(*buffer.back())) {
                        while (!buffer.empty() && TokenLevel.at(ch) <= TokenLevel.at(*buffer.back())) {
                            result.push_back(std::string { *buffer.back() });
                            buffer.pop_back();
                        }
                    }
                    buffer.push_back(it);
                }
                ++it;
            } else {
                std::string num = readNumber(it, expression);
                result.push_back(num);
            }
        }

        while (!buffer.empty()) {
            result.push_back(std::string { *buffer.back() });
            buffer.pop_back();
        }

ERROR_BUT_RETURN_RESULT_TAG:
        return result;
    }

    double eXpressionCalc::evalNotation(const Notation_t& notation) {
        resetError();
        if (notation.empty())
            return INFINITY;

        std::stack<double> resultStack;

        for (auto& note : notation) {
            auto it = note.begin();
            if (checkNumber(it, note)) {
                resultStack.push(strtod(note.c_str(), nullptr));
            } else {
                if (note == "(" || note == ")") {
                    ERROR(ErrorType::BracketNotMatched, note, ERROR_BUT_RETURN_INFINITY_TAG);
                }

                auto nt_it = NoteTable.find(note);
                if (nt_it == NoteTable.end()) {
                    ERROR(ErrorType::FunctionNotFound, note, ERROR_BUT_RETURN_INFINITY_TAG);
                }

                const auto pCount = nt_it->second.first;
                const auto param  = new double[pCount];
                for (auto i = pCount - 1; i >= 0; i--) {
                    if (resultStack.empty()) {
                        delete[] param;
                        ERROR(ErrorType::SyntaxError, note, ERROR_BUT_RETURN_INFINITY_TAG);
                    }
                    param[i] = resultStack.top();
                    resultStack.pop();
                }

                resultStack.push(nt_it->second.second(param, pCount));

                delete[] param;
            }
        }

        if (resultStack.size() > 1) {
            ERROR(ErrorType::EvalError, "Can't Evaluate Correctly", ERROR_BUT_RETURN_RESULT_TAG);
        }

ERROR_BUT_RETURN_RESULT_TAG:
        return resultStack.top();
ERROR_BUT_RETURN_INFINITY_TAG:
        return INFINITY;
    }
}
