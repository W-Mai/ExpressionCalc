//
// Created by W-Mai on 2022/10/3.
//

#include "expression_calc.h"

#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    char   expr[1024] { 0 };
    string inputExpression;
    XCLZ::eXpressionCalc calc;

    while (cin.getline(expr, sizeof expr, ';')) {
        inputExpression = expr;

        if (inputExpression == "exit") break;

        const auto res = calc.reversePolishNotation(inputExpression);
        const auto val = calc.evalNotation(res);

        cout << "Result: " << fixed << setprecision(6) << val << endl;
    }
}
