; RUN: opt -load %bindir/ReachableIntegerValues/LLVMReachableIntegerValues${MOD_EXT} -load %bindir/DuplicateBB/LLVMDuplicateBB${MOD_EXT} -duplicate-bb %s -S -o %t0.ll
; RUN: opt -load %bindir/ReachableIntegerValues/LLVMReachableIntegerValues${MOD_EXT} -load %bindir/DuplicateBB/LLVMDuplicateBB${MOD_EXT} -duplicate-bb %s -S | FileCheck %s

; CHECK-LABEL: while.cond
; check that we have more than one phi in the output
; CHECK: phi
; CHECK: phi
; CHECK: phi
; CHECK: phi
define i32 @foo(i32 %i, i32 %j) {
entry:
  br label %while.cond

while.cond:
  %iaddr = phi i32 [ %i, %entry ], [ %add, %while.cond ]
  %0 = xor i32 %iaddr, %j
  %1 = and i32 %0, 255
  %cmp = icmp eq i32 %1, 0
  %add = add i32 %iaddr, %j
  br i1 %cmp, label %while.end, label %while.cond

while.end:
  %iaddr.lcssa = phi i32 [ %iaddr, %while.cond ]
  ret i32 %iaddr.lcssa
}

