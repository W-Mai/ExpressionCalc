//
// Created by W-Mai on 2022/10/3.
//

#include "expression_calc.h"

using namespace std;

int main()
{
    while (true) {
        string inputExpression;
        cin >> inputExpression;
        // inputExpression = "1+2+3+(5/6-3)";
        Error e;

        // while (true) {
        const auto res = reversePolishNotation(inputExpression, tokenLevel, e);
        if (e.type != ErrorType::Well) {
            cout << "[" + ErrorType2Name[e.type] + "]: " + e.msg << endl; // 报错信息或许去掉.比较好，考虑到因.引发的错误？
            continue;
        }
        const auto val = evalNotation(res, e);
        //}

        if (e.type != ErrorType::Well) {
            cout << "[" + ErrorType2Name[e.type] + "]: " + e.msg + "." << endl;
        } else
            cout << endl
                 << "Result: " << fixed << setprecision(6) << val << endl;
    }
}
