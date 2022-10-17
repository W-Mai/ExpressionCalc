//
// Created by W-Mai on 2022/10/3.
//

#include "expression_calc.h"

#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    char                 expr[1024] { 0 };
    string               inputExpression;
    XCLZ::eXpressionCalc calc;

    calc.addFunc("hello", 0, [](const double*, const int) -> double {
        cout << "Hello World" << endl;
        return 0;
    });

    calc.addFunc("fib", 3, [](const double* p, const int) -> double {
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
    });

    calc.addFunc("if", 3, LAMBDA_EXPR(params[0] ? params[1] : params[2]));

    while (cin.getline(expr, sizeof expr, ';')) {
        inputExpression = expr;

        if (inputExpression == "exit") break;

        calc.setExpression(inputExpression);
        const auto res = calc.reversePolishNotation();
        if (calc.getError().type != XCLZ::ErrorType::Well) {
            cout << "[" + calc.errorToString() + "]: " + calc.getError().msg << endl;
            continue;
        }

        const auto val = calc.evalNotation(res);
        if (calc.getError().type != XCLZ::ErrorType::Well) {
            cout << "[" + calc.errorToString() + "]: " + calc.getError().msg + "." << endl;
        } else {
            cout << "Result: " << fixed << setprecision(6) << val << endl;
        }
    }
}
