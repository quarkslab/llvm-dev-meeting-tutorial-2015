; RUN: grep -v ' mul ' %s
; RUN: opt -load %bindir/MBA/LLVMMBA${MOD_EXT} -mba -mba-ratio=1 %s -S -o %t0
; RUN: grep ' mul ' %t0

define i32 @foo(i32 %i, i32 %j) {
entry:
  br label %while.cond

while.cond:
  %i.addr.0 = phi i32 [ %i, %entry ], [ %add, %while.cond ]
  %0 = xor i32 %i.addr.0, %j
  %1 = and i32 %0, 255
  %cmp = icmp eq i32 %1, 0
  %add = add i32 %i.addr.0, %j
  br i1 %cmp, label %while.end, label %while.cond

while.end:
  %i.addr.0.lcssa = phi i32 [ %i.addr.0, %while.cond ]
  ret i32 %i.addr.0.lcssa
}

