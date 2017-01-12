// RUN: clang -S -emit-llvm %s -o %t.ll
// RUN: opt -load %bindir/ReachableIntegerValues/LLVMReachableIntegerValues${MOD_EXT} -load %bindir/DuplicateBB/LLVMDuplicateBB${MOD_EXT} -duplicate-bb %t.ll -S | FileCheck %s

// CHECK: icmp
#include <stdio.h>

int add( int x, int y )
{
        return x + y;
}

int sub( int x, int y )
{
        return x - y;
}

int main(void){
        printf("hello, there. math things: %d\n", add( 10, 20 ) + sub( 50, 10 ) );
        return 0;
}
