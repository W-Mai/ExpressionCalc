#include "expression_calc.h"

std::map<ErrorType, std::string> ErrorType2Name = {
    {ErrorType::BracketNotMatched, "Bracket Not Matched"},
    {ErrorType::FunctionNotFound, "Function Not Found"},
    {ErrorType::EvalError,"Evaluate Error"},
    {ErrorType::SyntaxError,"Syntax Error"}
};

const TokenLevel_t TokenLevel {
    {'+', 1}, {'-', 1},
    {'*', 2}, {'/', 2}, {'^', 2}, {'%', 2}, {'\\', 2},
    {'(', 0}, {')', 0}
};

const std::map<std::string, int> NoteTable {
    {"+",2}, {"-",2},
    {"*",2}, {"/",2}, {"^", 2}, {"%", 2}, {"\\", 2},
    {"sqrt",1}, {"abs",1},
    {"sin",1}, {"cos", 1}, {"tan", 1},
    {"asin",1}, {"acos", 1}, {"atan", 1},
    {"ln",1}, {"log", 1}, {"log2", 1},
    {"floor", 1}, {"ceil", 1},
    {"sign", 1},
    {"PI", 0},
    {"E", 0},
};

inline bool checkNumber(std::string::const_iterator& it, const std::string& expression)
{
    return isdigit(*it)
        || ((it == expression.begin() || *(it - 1) == '(')
            && ((*it == '-' || *it == '+') && (it + 1) != expression.end()))
        || *it == '.' && (it + 1) != expression.end() && isdigit(*(it + 1));
}

std::string readNumber(std::string::const_iterator& it, const std::string& expression)
{
    int dot_count = 0;
    std::string rtn;
    while (it != expression.end() && checkNumber(it, expression)) {
        if (*it == '.')
            dot_count ++;
        if (dot_count > 1)
            break ;

        if (*it != '+')
            rtn += *it++;
        else
            ++it;
    }

    return rtn;
}

inline bool checkFunc(std::string::const_iterator& it, const std::string& expression)
{
    return isalnum(*it);
}

std::string readFunc(std::string::const_iterator& it, const std::string& expression)
{
    std::string rtn;
    while (it != expression.end() && checkFunc(it, expression))
        rtn += *it++;
    return rtn;
}

Notation_t reversePolishNotation(const std::string& expression, const TokenLevel_t& tokenLevel, Error& e)
{
    e = {ErrorType::Well};
    Notation_t result;
    std::vector<std::string::const_iterator> buffer;
    std::stack<std::pair<std::string, std::string::const_iterator>> tmpFunc;
    for (auto it = expression.begin(); it != expression.end();) {
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
                    e = { ErrorType::BracketNotMatched, std::string({ '(' }) };
                    return result;
                } else {
                    buffer.pop_back();
                }

            } else {
                auto search_res_ch = tokenLevel.find(ch);
                auto search_res_back = tokenLevel.find(*buffer.back());
                if (search_res_ch == tokenLevel.end()) {
                    e = { ErrorType::SyntaxError, std::string({ ch }) };
                    return result;
                }
                if (search_res_back == tokenLevel.end()) {
                    e = { ErrorType::SyntaxError, std::string({ *buffer.back() }) };
                    return result;
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

double evalNotation(const Notation_t& notation, Error& e)
{
    e = {ErrorType::Well};
    if (notation.empty())
        return INFINITY;
    std::stack<double> resultStack;
    for (const auto& note : notation) {
        // cout << note << " ";
        auto it = note.begin();
        if (checkNumber(it, note)) {
            resultStack.push(strtod(note.c_str(), nullptr));
        } else {
            if (note == "(" || note == ")") {
                e = { ErrorType::BracketNotMatched, note };
                return INFINITY;
            }

            auto nt_it = NoteTable.find(note);
            if (nt_it == NoteTable.end()) {
                e = { ErrorType::FunctionNotFound, note };
                return INFINITY;
            }
            const auto pCount = nt_it->second;

            const auto param = new double[pCount];
            for (auto i = pCount - 1; i >= 0; i--) {
                if (resultStack.empty()) {
                    e = { ErrorType::SyntaxError, note };
                    return INFINITY;
                }
                param[i] = resultStack.top();
                resultStack.pop();
            }

            if (note == "hello")
                resultStack.push(0);
            else if (note == "PI")
                resultStack.push(M_PI);
            else if (note == "E")
                resultStack.push(M_E);
            else if (note == "abs")
                resultStack.push(abs(param[0]));
            else if (note == "sqrt")
                resultStack.push(sqrt(param[0]));
            else if (note == "sin")
                resultStack.push(sin(param[0]));
            else if (note == "cos")
                resultStack.push(cos(param[0]));
            else if (note == "tan")
                resultStack.push(tan(param[0]));
            else if (note == "asin")
                resultStack.push(asin(param[0]));
            else if (note == "acos")
                resultStack.push(acos(param[0]));
            else if (note == "atan")
                resultStack.push(atan(param[0]));
            else if (note == "ln")
                resultStack.push(log(param[0]));
            else if (note == "log")
                resultStack.push(log10(param[0]));
            else if (note == "log2")
                resultStack.push(log2(param[0]));
            else if (note == "floor")
                resultStack.push(floor(param[0]));
            else if (note == "ceil")
                resultStack.push(ceil(param[0]));
            else if (note == "sign")
                resultStack.push(abs(param[0]) < 1e-10 ? 0 : param[0] > 0 ? 1
                                                                          : -1);
            else if (note == "+")
                resultStack.push(param[0] + param[1]);
            else if (note == "-")
                resultStack.push(param[0] - param[1]);
            else if (note == "*")
                resultStack.push(param[0] * param[1]);
            else if (note == "/")
                resultStack.push(param[0] / param[1]);
            else if (note == "^")
                resultStack.push(pow(param[0], param[1]));
            else if (note == "%")
                resultStack.push(fmod(param[0], param[1]));

            delete[] param;
        }
    }

    if (resultStack.size() > 1) {
        e = { ErrorType::EvalError, "Can't Evaluate Correctly" };
    }
    return resultStack.top();
}
