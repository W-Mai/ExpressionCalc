# ExpressionCalc
表达式计算器

- 支持+ - * / \ % ^等双目运算
- 支持()
- 支持sqrt abs sin cos tan asin acos atan ln log log2 floor ceil sign等函数
- 支持常量 PI E

易于拓展，方便扩展自己的自定义函数

支持语法报错等

## 例子
```
1+1+1+1+1+1+1

Result: 7.000000
```

```
sin(PI()/2)*6-tan(atan(6))

Result: -0.000000
```

```
((1+2)*7-6
[Bracket Not Matched]: (.
```

```
fuck(6)+7
[Function Not Found]: fuck.
```

## 已知bug
- 输出不够人性化
- 错误信息有时候不准确23333

欢迎提交PR
