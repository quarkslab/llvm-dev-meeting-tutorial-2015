// RUN: clang %s -O2 -o %tref
// RUN: clang %s -O2 -S -emit-llvm -o %t.ll
// RUN: opt -load %bindir/MBA/LLVMMBA${MOD_EXT} -mba %t.ll -S -o %t0.ll
// RUN: grep ' and ' %t0.ll
// RUN: clang %t0.ll -o %t0
// RUN: %tref -42 42
// RUN: %t0 -42 42
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
  if(argc != 3)
    return 1;
  int a = atoi(argv[1]),
      b = atoi(argv[2]);

  return a + b;
}
