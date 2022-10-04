//
// Created by W-Mai on 2022/10/3.
//

#include "expression_calc.h"

#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    Error  e;
    char   expr[1024] { 0 };
    string inputExpression;

    NoteTable["hello"] = {
        0, [](const double*, const int) -> double {
            cout << "Hello World" << endl;
            return 0;
        }
    };

    NoteTable["fib"] = {
        3, [](const double* p, const int) -> double {
            double target = p[2];
            double a = p[0], b = p[1], c = a + b;

            if (target <= 0) return INFINITY;
            if (target == 1) return a;
            if (target == 2) return b;

            while (target-- > 3) {
                a = b;
                b = c;
                c = a + b;
            }
            return c;
        }
    };

    NoteTable["if"] = { 3, LAMBDA_EXPR(params[0] ? params[1] : params[2]) };

    while (cin.getline(expr, sizeof expr, ';')) {
        inputExpression = expr;

        if (inputExpression == "exit") break;

        const auto res = reversePolishNotation(inputExpression, TokenLevel, e);
        if (e.type != ErrorType::Well) {
            cout << "[" + ErrorType2Name[e.type] + "]: " + e.msg << endl;
            continue;
        }
        const auto val = evalNotation(res, e);

        if (e.type != ErrorType::Well)
            cout << "[" + ErrorType2Name[e.type] + "]: " + e.msg + "." << endl;
        else
            cout << "Result: " << fixed << setprecision(6) << val << endl;
    }
}
