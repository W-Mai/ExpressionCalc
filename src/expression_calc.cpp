#include "expression_calc.h"

std::map<ErrorType, std::string> ErrorType2Name = {
    {ErrorType::BracketNotMatched, "Bracket Not Matched"},
    { ErrorType::FunctionNotFound, "Function Not Found" },
    { ErrorType::EvalError,        "Evaluate Error"     },
    { ErrorType::SyntaxError,      "Syntax Error"       }
};

TokenLevel_t __attribute__((weak)) TokenLevel {
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

NoteTable_t __attribute__((weak)) NoteTable {
    {"+",      { 2, LAMBDA_EXPR(params[0] + params[1]) }                                      },
    { "-",     { 2, LAMBDA_EXPR(params[0] - params[1]) }                                      },
    { "*",     { 2, LAMBDA_EXPR(params[0] * params[1]) }                                      },
    { "/",     { 2, LAMBDA_EXPR(params[0] / params[1]) }                                      },
    { "^",     { 2, LAMBDA_EXPR(powf(params[0], params[1])) }                                 },
    { "%",     { 2, LAMBDA_EXPR(fmod(params[0], params[1])) }                                 },
    { "\\",    { 2, LAMBDA_EXPR((int)((int)params[0] / (int)params[1])) }                     },
    { "add",   { 2, LAMBDA_EXPR(params[0] + params[1]) }                                      },
    { "sub",   { 2, LAMBDA_EXPR(params[0] - params[1]) }                                      },
    { "mul",   { 2, LAMBDA_EXPR(params[0] * params[1]) }                                      },
    { "div",   { 2, LAMBDA_EXPR(params[0] / params[1]) }                                      },
    { "pow",   { 2, LAMBDA_EXPR(powf(params[0], params[1])) }                                 },
    { "mod",   { 2, LAMBDA_EXPR(fmod(params[0], params[1])) }                                 },
    { "divi",  { 2, LAMBDA_EXPR((int)((int)params[0] / (int)params[1])) }                     },
    { "sqrt",  { 1, LAMBDA_EXPR(sqrt(params[0])) }                                            },
    { "abs",   { 1, LAMBDA_EXPR(fabs(params[0])) }                                            },
    { "sin",   { 1, LAMBDA_EXPR(sin(params[0])) }                                             },
    { "cos",   { 1, LAMBDA_EXPR(cos(params[0])) }                                             },
    { "tan",   { 1, LAMBDA_EXPR(tan(params[0])) }                                             },
    { "asin",  { 1, LAMBDA_EXPR(asin(params[0])) }                                            },
    { "acos",  { 1, LAMBDA_EXPR(acos((params[0]))) }                                          },
    { "atan",  { 1, LAMBDA_EXPR(atan(params[0])) }                                            },
    { "ln",    { 1, LAMBDA_EXPR(log(params[0])) }                                             },
    { "log",   { 1, LAMBDA_EXPR(log10(params[0])) }                                           },
    { "log2",  { 1, LAMBDA_EXPR(log2(params[0])) }                                            },
    { "floor", { 1, LAMBDA_EXPR(floor(params[0])) }                                           },
    { "ceil",  { 1, LAMBDA_EXPR(ceil(params[0])) }                                            },
    { "sign",  { 1, LAMBDA_EXPR(abs(params[0]) < 1e-10 ? 0 : params[0] > 0 ? 1
                                                                          : -1) }},
    { "PI",    { 0, LAMBDA_EXPR(M_PI) }                                                       },
    { "E",     { 0, LAMBDA_EXPR(M_E) }                                                        },
};

inline bool checkNumber(std::string::const_iterator& it, const std::string& expression) {
    return isdigit(*it)
        || ((it == expression.begin() || *(it - 1) == '(')
            && ((*it == '-' || *it == '+') && (it + 1) != expression.end()))
        || *it == '.' && (it + 1) != expression.end() && isdigit(*(it + 1));
}

std::string readNumber(std::string::const_iterator& it, const std::string& expression) {
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

inline bool checkFunc(std::string::const_iterator& it, const std::string& expression) {
    return isalnum(*it);
}

std::string readFunc(std::string::const_iterator& it, const std::string& expression) {
    std::string rtn;
    while (it != expression.end() && checkFunc(it, expression)) rtn += *it++;
    return rtn;
}

bool eatWhitespace(std::string::const_iterator& it, const std::string& expression) {
    while (it != expression.end() && (isspace(*it) || *it == ',')) it++;
    return it == expression.end();
}

Notation_t reversePolishNotation(const std::string& expression, const TokenLevel_t& tokenLevel, Error& e) {
    e.type = ErrorType::Well;
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
                    ERROR(e, ErrorType::BracketNotMatched, std::string({ '(' }), result);
                } else {
                    buffer.pop_back();
                }

            } else {
                auto search_res_ch   = tokenLevel.find(ch);
                auto search_res_back = tokenLevel.find(*buffer.back());
                if (search_res_ch == tokenLevel.end()) {
                    ERROR(e, ErrorType::SyntaxError, std::string({ ch }), result);
                }
                if (search_res_back == tokenLevel.end()) {
                    ERROR(e, ErrorType::SyntaxError, std::string({ *buffer.back() }), result);
                }
                if (tokenLevel.at(ch) <= tokenLevel.at(*buffer.back())) {
                    while (!buffer.empty() && tokenLevel.at(ch) <= tokenLevel.at(*buffer.back())) {
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
    return result;
}

double evalNotation(const Notation_t& notation, Error& e) {
    e.type = ErrorType::Well;
    if (notation.empty())
        return INFINITY;

    std::stack<double> resultStack;

    for (auto& note : notation) {
        auto it = note.begin();
        if (checkNumber(it, note)) {
            resultStack.push(strtod(note.c_str(), nullptr));
        } else {
            if (note == "(" || note == ")") {
                ERROR(e, ErrorType::BracketNotMatched, note, INFINITY);
            }

            auto nt_it = NoteTable.find(note);
            if (nt_it == NoteTable.end()) {
                ERROR(e, ErrorType::FunctionNotFound, note, INFINITY);
            }

            const auto pCount = nt_it->second.first;
            const auto param  = new double[pCount];
            for (auto i = pCount - 1; i >= 0; i--) {
                if (resultStack.empty()) {
                    delete[] param;
                    ERROR(e, ErrorType::SyntaxError, note, INFINITY);
                }
                param[i] = resultStack.top();
                resultStack.pop();
            }

            resultStack.push(nt_it->second.second(param, pCount));

            delete[] param;
        }
    }

    if (resultStack.size() > 1) {
        ERROR(e, ErrorType::EvalError, "Can't Evaluate Correctly", resultStack.top());
    }

    return resultStack.top();
}
