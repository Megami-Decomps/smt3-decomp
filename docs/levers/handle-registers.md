# `lw $4` against `lw $2` plus `move $4, $2`


`if (p->h != NULL) { f(p->h); p->h = NULL; }` compiles two ways in this tree.
Retail's shape loads the handle straight into the argument register and tests
it there, so the copy does not exist and the delay slot goes to the next
block's load as a `beql`. ee-gcc only does that when `$2` is unavailable
across the guarded block: `pipeFreeEffects` in `pipe/pipeEffect` gets
`lw $4, 12($16)` because `$2` there holds the `u8` counter's `i + 255`, and
the identical shape in `code_00116AD0` gets `lw $2` plus `move $4, $2`
because nothing else wants `$2`.

So this is not a spelling of the guard. Ruled out on it: a reused local and a
fresh local per arm (cse propagates both), `while` for the `if`, `s32` and
unprototyped callee declarations, an `s32` parameter in place of `void *`, a
cast at the call site, and every loop spelling of the enclosing walk. Two
functions in `code_00116AD0` sit on it; see
[Non-matching drafts](nonmatching.md).

So when a caller sits within a couple of branch-annul words of exact and
calls something still in assembly, decompile the callee first rather than
permuting the caller. This is the same mechanism as the CONTEXT row below,
seen from the other side: what the compiler can see of the rest of the unit
changes the code it emits for one function.
