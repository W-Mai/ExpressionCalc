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
