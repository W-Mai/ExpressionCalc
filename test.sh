#!/usr/bin/env zsh

build_dir="build"
mkdir -p $build_dir && pushd $build_dir
cmake .. && make
popd
calc_bin=./build/xCalc

echo '
if(sin(20)
  (cos(20) + PI())
  (sin(PI()) - 20)
) + 30;
hello()
' | $calc_bin

for i in {1..100}; do
  echo "fib(1, 1, $i) / fib(1, 1, $i - 1)" | $calc_bin
done
