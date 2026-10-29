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

### SetAction_READY: live delay/index scalar and a named display value

The 3540-byte action-ready routine matches with one real `s32 i` used for the
HP-cost delay and then reset independently for each formation scan. The delay
is consumed in `nbAddHpMpKoukaPacket(a->timer + i, ...)`; no value from that
phase reaches a scan. The later loops need their index across callbacks. This
is ordinary disjoint-phase scalar reuse, not a dummy use or an added dependency.

The final skill display also names its value: `s32 skillName =
a->work->nowindex`, then tests that value against `0xF7` and passes it to
`nbDispSkillName`. The special-name tests preceding it still read the native
unit fields. There is no callback between this local's read, test and use.

Three complete-owner sources were qualified with the canonical compiler
SHA-256 `d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`:
the matched source, a control with a separate `delay` local, and a control
using the final field directly in the test and call. For each, stock `-da`
assembly and all non-debug sections, including relocation and symbol tables,
agreed with ordinary compilation. No debugger or optimization-flag change was
used. Reproduce a control by applying only that stated source change:

```sh
SMT3_AS_UNIT=src/smt3/newbattle/nbActionProcess.c \
  SMT3_KEEP_S=/tmp/ordinary.s tools/cc.sh /tmp/ready.c -o /tmp/ordinary.o
SMT3_AS_UNIT=src/smt3/newbattle/nbActionProcess.c \
  SMT3_KEEP_S=/tmp/stock.s tools/cc.sh -da -dumpbase /tmp/ready-rtl \
  /tmp/ready.c -o /tmp/stock.o
```

In the matched `00.rtl`, delay definitions 1884/1896/1901 and scan
initializations 2701/2880/2963 all write pseudo92. `20.greg` assigns it `$17`.
The separate-delay control instead writes pseudo98 for the delay and assigns
it `$8`, leaving three instruction-register differences. Both values are
real signed integers and both controls preserve the native callback reloads.

For the named display value, `00.rtl` instruction3611 produces SI pseudo730.
In `19.lreg` it stays live through the guard into the call's argument copy;
`20.greg` assigns it `$4`. The direct-field control's test uses pseudo732 in
`$3`, dies at the guard, and its call loads the field separately. That lets
`29.dbr` move the test load into the preceding comparison's delay slot and
remove another load, producing a body eight bytes short. Naming the consumed
value retains retail's two branch-likely loads of `$4` and final `beq`.

The complete action owner checks **79 match, 0 differ**, including the native
strings and eight switch tables. The visual pointer is a deliberate snapshot
across preparation callbacks; formation, action and unit fields are freshly
read where retail reloads them. File requests use the provider's pointer type,
and the first movement packet explicitly narrows its selection to `u16`
while the reverse-target shift retains the provider's full-width mask.
