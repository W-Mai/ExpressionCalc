//
// Created by W-Mai on 2022/10/3.
//

#include "expression_calc.h"

#include <iomanip>
#include <iostream>

using namespace std;

int main()
{
    Error e;
    string inputExpression;
    while (cin >> inputExpression) {
        // inputExpression = "1+2+3+(5/6-3)";
        const auto res = reversePolishNotation(inputExpression, TokenLevel, e);
        if (e.type != ErrorType::Well) {
            cout << "[" + ErrorType2Name[e.type] + "]: " + e.msg << endl;
            continue;
        }
        const auto val = evalNotation(res, e);

        if (e.type != ErrorType::Well) {
            cout << "[" + ErrorType2Name[e.type] + "]: " + e.msg + "." << endl;
        } else
            cout << "Result: " << fixed << setprecision(6) << val << endl;
    }
}
