# What iop-gcc 2.8.1 wants

Shapes that reproduce the IRX modules' instructions, learned one function at a
time while decompiling them. The compiler is `iop-gcc 2.8.1` at `-O2 -G 0`; the
build is in [Building the IRX modules](irx-modules.md).

Each of these cost somebody several iterations. Read the list before grinding a
function by hand: the remaining difference is usually one of them.

## Which branch, and where a return lands

- **Branch polarity names the fall-through.** gcc emits the test for the `else`
  arm, so retail `bltz X, L` where `L` loads field A means the source is
  `if (x >= 0) B; else A;`.
- **The constant in the first branch's delay slot says which return is the
  fall-through.** `modhsyn_e19` needed `if (val >= 0x31) return -1;` first, not
  the positive form.
- **Where `return 0` lands is often the whole difference.** Nested ifs with one
  trailing return give the merged form; an `||` chain gives "set the value in
  each branch delay slot and jump to the epilogue"; a flat run of early returns
  gives separate tails. Try all three.
- **An early `return 0` duplicated at the end blocks cross-jumping.** Wrapping
  the body in `if (state == 1) { ... } return 0;` is what `hsynEffectDepthSet`
  needed.
- **A statement between `i--` and the `if` fills the branch delay slot.**
  `hsynPitchBendApply` wanted `period--; e = d; if (period <= 0)`.

## Types and arithmetic

- **Unsigned shifts and divides need an unsigned operand, not a cast on the
  result.** A `u32` local for the product, `60000000U` for the literal.
- `x / 128` on a signed value gives `bgez / addiu 0x7F / sra 7`;
  `(s16)(y >> 1)` gives `sll 15 / sra 16`.
- **`0x80 - f(x)` returned as `u8` needs the call result in a `u32` local**, or
  gcc materialises `-0x80` instead of `0x80`. Both are the same low byte, which
  is why the mask hides it.
- **A `u8` local's zero-extend sinks to the return** unless the masked value is
  a distinct `int`: `int r = 1; ... r = v & 0xFF; return r;`.
- **A `u8` accumulator keeps a comparison result in its own register**, which is
  how `modhsyn_e17` gets its extra `addu`.
- **`u16` for a truncating copy keeps two registers alive**, giving
  `negu $6, $8` rather than `negu $6, $6`.
- **`(x & bit) != 0` split into two statements gives `andi` then `sltu`; inline
  it gives `srl` then `andi`.** `hsynEffectAttrRead` and `Write` both need
  `t = sel & 0x10; core = t != 0;`.
- **`v = x; v = v * v;` is not `v = x * x;`** The extra statement frees the
  argument register. `(q * q) / 16384` as one expression is the opposite case:
  splitting it forces a register copy.
- **Two-step index arithmetic must stay two statements.** `idx = sel >> 4;
  idx--;` or gcc folds `(sel >> 4) - 1 != -1` into `sel >> 4 != 0`.
- **Write a long arithmetic chain step by step** when retail does not interleave
  two of them: a division's rounding branch is a block boundary the scheduler
  cannot cross.
- **Hoist a constant above a division to get it into a register.** `r = 1`
  before the divide is what makes gcc reuse the `1` as the `srlv` shift amount.
- **Force reassociation with a local.** `msg = (vol << 16) | 0x7B0; ... ch | msg`
  holds where the same expression in parentheses does not.

## Loops

- **Index-based loops (`e[i].buf`) create the induction-variable copy retail
  has; pointer-increment loops (`e++`) do not.**
- **`heads[i]` against an explicit `head` pointer decides where a hoisted loop
  invariant lands.** When the body also needs a constant such as
  `1 << v->index`, the indexed form puts the strength-reduced pointer after the
  invariant in the preheader, which `hsynVoiceStop` needs and
  `hsynVoiceSetPitch` does not.
- **Let gcc strength-reduce `out[n]`** rather than keeping your own cursor: the
  reduced induction variable is born in the inner preheader, after the guard.
- **Drop a redundant `if (n > 0)` around a `for (j = 0; j < n; j++)`**, or gcc
  emits the guard twice.
- **`ch = 0;` before the loop with an empty `for` init** reorders two preheader
  initialisations, which is what `modhsyn_e23` wanted.
- `for (i = 7; i != -1; i--)` gives `bne $reg, -1`; `i >= 0` gives `bgez`. Both
  appear in retail.
- **The three-list broadcast loop**, used by five MODHSYN setters, matches as
  `heads[3]` filled with `chan-8`, `chan`, `chan+8`, then
  `for (i = 0, head = heads; i < 3; i++, head++)` around
  `for (v = end->chNext; v != end; v = v->chNext)`. The comma init is what gives
  `i` the higher callee-saved register.

## Reaching data

- **The `{count, pointer}` segment.** Several functions reach a table through
  *base+8* then *+4*, never one folded `+0xC`. Matching needs the pair treated
  as one object: `seg = &work->tbl->nBufWord; seg[1]`, or
  `ofs = (slotNo << 4) | 8; seg = (u32 *)(ofs + (u32)tbl->seq)`. The `|` rather
  than `+` is what yields `ori`, and the `ofs + (u32)ptr` operand order picks the
  destination register. It does not always hold: `hsynServiceTick` is two short
  because gcc bases the entry loads on `entry+0xC` anyway.
- **An index table counts 8-byte units, not entries**, and the unit
  subexpression has to be shared with the bounds check or gcc emits
  `sll 4 / lw 0xC`.
- **Hoist a global load above pointer stores** when retail loads it first: gcc
  cannot move it past the stores, because it cannot prove they do not alias, so
  leaving it below costs a nop.
- **A tag comparator wants its two byte pointers as named locals.**
  `p0 = (char *)&tag0; ... p0[i]` matches where `*((char *)&tag0 + i)` gives the
  operands the other way round. Loop counter `u32`, for `sltiu`.

## What is not fixable from the C

**A store to a global immediately before a test.** gas refuses to move the
second half of a `sw reg, sym` macro into the following branch's delay slot, so
the result is one extra nop however the C is written. `modhsyn_e16` is the
recorded case. Guard it under `NON_MATCHING` and move on.

## Before chasing a difference at all

A `+4` in `.text` moves `.bss` by 0x10, through the output section's alignment,
so one wrong-sized chunk makes every later `%hi`/`%lo` data reference differ.
Run `irx_diff.py <module> --sizes` and fix sizes first.

`irx_diff.py` reads the image that was packed last. It warns when that image is
older than the source, but check the build output for `FAILED` before trusting
any diff: a compile that failed leaves the previous image in place and every
function still diffs clean.
