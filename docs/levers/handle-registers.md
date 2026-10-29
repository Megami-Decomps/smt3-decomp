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

### SetAction_NEGORESULT: fresh work views and a separate scan cursor

The reconstruction matches all 1172 retail instruction bytes. The earlier guarded
source had the native size, control flow and both dispatch tables, but 21 words
differed in register choices. The live battle-work view and final party cursor
explain those choices with ordinary C.

`data` names the same battle-work record wherever it is read. Each return-packet
callback is followed by a fresh `data = a->data` before checking the current party
flags and taking its form index. The result filters similarly load their current
work view. The final scan takes a fresh view once, then retains it while counting
parties and conditionally setting `endtype`; there are no calls inside that scan.
A separate `scanParty` cursor keeps that scan independent of `party`, which holds
a provider result across recruitment callbacks in an earlier result case.

Two qualified `--alloc --no-dumps` probes used compiler SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
Both reported `verify: OK`, hooked/unhooked assembly parity and parity with plain
compilation. The baseline is the guarded body from commit `9caf8aed`, enabled by
itself; the candidate is the current definition. To reproduce either observation:

```sh
tools/cc1probe.sh /tmp/nego-result.c \
    --as-unit src/smt3/newbattle/nbActionProcess.c \
    --func SetAction_NEGORESULT --alloc --no-dumps --out /tmp/nego-alloc
```

In baseline local-allocation block 12, the fresh main and party-base quantities
(reg175 and reg176) each have priority 40000. Quantity order gives them `$3` and
`$4`. In the matched source the shared live work view is reg98: global allocation
records 21 references, lifetime 39, no crossed calls, and copy preferences for
`$4`; it receives `$4`. The remaining party-base quantity (reg174) receives `$3`.
The same live view closes the result-filter register differences. Enabling only
this change leaves seven words differing, all in the final scan.

Separating the scan cursor supplies reg97 with seven references, lifetime 12 and
no crossed calls. Global allocation gives it `$5`; the count (reg92, eight
references and lifetime 32) then receives `$6`. Previously the reused `party`
pseudo received `$16`, and the count received `$7` while avoiding other pseudos'
preferred argument registers. This is a real cursor over the ten party records,
not an extra value or dependency. No register is pinned and no callback read is
cached across a call.

### Preserve the full texture index before reducing the parameter

`stfLoadUITexture` matches all 140 bytes by preserving `fullIndex = index`
and then reducing the parameter with `index %= 4`. The first two texture arrays
have four entries and use the remainder; the third has fifteen entries and
uses the original full index. Native saves the original argument in `$17`,
computes the signed remainder in `$16`, and independently scales the full index
for the third array. A fresh remainder local instead left 12 of 35 words
differing at the same size. The parameter and snapshot lifetimes explain the
native allocation without spelling out division's machine lowering.

Keep the signed `% 4`: native's negative-input bias preserves C's signed
remainder semantics. Each texture call is followed by a fresh `oStaff_UI` load
before storing its result, so the source retains separate global accesses.
The provider's parameter remains the actual `TMX_HEADER_t *` contract. The
complete normal owner checks 9 match / 0 differ.
