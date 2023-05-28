#!/usr/bin/env bash

assert() {
  expected="$1"
  input="$2"

  actual=`echo "$input" | ./build/main`
    
  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 9    '9'
assert 10   '4 + 6'
assert -4   '-2^2'
assert 12   '6 / 2 * 4'
assert -2   '-(1 + 1)'
assert 9    '6 / 2 * (1 + 2)'
assert 1    '8 / 2(2 + 2)'
assert 256  '-(2^(4 * 4 / 2)) * -1'
assert 512  '2^3^2'
assert -100 '(-100)'
assert 4    '(4 / 2)(8 / 4)'
assert -8   '-2(4)'
assert 9    'sqrt(81)'
assert 1    'sin(3.14159/2)'

echo OK