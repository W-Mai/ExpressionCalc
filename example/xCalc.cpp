/*
 * xCalc.cpp
 * Created by W-Mai on 2022/10/3.
 *
 * MIT License
 * Copyright (c) 2022 XCLZ STUDIO@W-Mai
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
