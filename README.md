# ExpressionCalc

表达式计算器

- 支持+ - * / \ % ^等双目运算
- 支持()
- 支持sqrt abs sin cos tan asin acos atan ln log log2 floor ceil sign等函数
- 支持常量 PI E

易于拓展，方便扩展自己的自定义函数

支持语法报错等

## 构建

```shell
build_dir="build"
mkdir -p $build_dir && cd $build_dir
cmake .. && make
```

## 测试

```shell
./test.sh
```

## 例子

```shell
1+1+1+1+1+1+1

Result: 7.000000
```

```shell
sin(PI()/2)*6-tan(atan(6))

Result: -0.000000
```

```shell
(( 1 + 2) * 7 - 6
[Bracket Not Matched]: (.
```

```shell
fuck(6)+7
[Function Not Found]: fuck.
```

### 在`example/xCalc.cpp`中

使用`addFunc`接口添加`if`、`fib`、`hello`等指令。

可以方便的写出如下代码

```shell
if(sin(20)
  (cos(20) + PI())
  (sin(PI()) - 20)
) + 30;
hello()

# 输出:
# Result: 33.549675
# Hello World
```

## 已知bug

- 输出不够人性化
- 错误信息有时候不准确23333

欢迎提交PR
