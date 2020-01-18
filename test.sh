#!/bin/bash
try() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}
<<"COMMENT"

try 0 0
try 42 42
try 21 "20-4+5"
try 41 " 12 + 34 - 5 "
try 47 "5+6*7"
try 15 "5*(9-6)"
try 4 "(3+5)/2"
try 1 "+1"
try 0 "-2 + 3 + (- 1)"

try 0 '0==1'
try 1 '42==42'
try 1 '0!=1'
try 0 '42!=42'

try 1 '0<1'
try 0 '1<1'
try 0 '2<1'
try 1 '0<=1'
try 1 '1<=1'
try 0 '2<=1'

try 1 '1>0'
try 0 '1>1'
try 0 '1>2'
try 1 '1>=0'
try 1 '1>=1'
try 0 '1>=2'
try 14  'a = 3; b = 5 * 6 - 8; a + b / 2;'
try 14  'a = 3; ab = 5 * 6 - 8; a + ab / 2;'
try 3 'a = 3; ab = 5 * 6 - 8; a + ab / 2; return a;'
try 22 'a = 3; ab = 5 * 6 - 8; a + ab / 2; return ab;'
try 0 'if (0) return 2;'
try 0 'a = 0; if (a) return 2;'
try 2 'if (1) return 2;'
try 2 'ab = 1; if (ab) return 2;'
try 2 'ab = 0; if (ab) ab = 2; if(ab == 0) ab = ab + 2; return ab;'
try 3 'ab = 0; if (ab) if(ab == 0) ab = 2 else ab = 1 else ab = 3; return ab;'
try 5 'ab = 0; while(ab < 5) ab = ab + 1; return ab;'
COMMENT
try 5 'ab = 0; while(ab < 5) { ab = ab + 1; }return ab;'
try 20 'ab = 0; bc = 10; while(ab < 5) { ab = ab + 1; bc = bc + 2;} return bc;'
try 7 'ab = 0; if(ab == 0){ ab = 2; ab = ab + 5} else  ab = 4; return ab;'
try 5 'ab = 0; if(ab){ ab = 2; ab = ab + 5} else {ab = 4; ab = ab + 1;} return ab;'
echo OK
