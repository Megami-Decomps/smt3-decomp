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
  the body in `if (state == 1) { ... } return 0;` is what `sceHsynEffectDepthSet`
  needed.
- **A statement between `i--` and the `if` fills the branch delay slot.**
  `sceHsynPitchBendApply` wanted `period--; e = d; if (period <= 0)`.

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
  it gives `srl` then `andi`.** `sceHsynEffectAttrRead` and `Write` both need
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
- **Reusing one variable for a destructive mask kills the CSE of the load.**
  `t = tone->velHigh; t &= 0x80; if (t) { t = tone->velHigh & 0x7F; ... }` gives
  retail's two `lbu`s of the same field, because overwriting the pseudo that
  held the load invalidates the available expression. `t = x & 0x80` keeps the
  load in a temp and gcc then CSEs it and sinks the `& 0x7F` into the branch
  delay slot. `sceHsynSdSetParamBend` needed all four of its arms written that way.
- **Write a divisor back into the same variable to stop the scheduler.**
  `num = t - v->vel; t = t - tone->unk03;` leaves the second `subu` dependent on
  `t`'s register, so it cannot be hoisted into the preceding load's delay slot;
  a fresh `den` local can be, and costs a `nop`.
- **A repeated literal rematerialises; a named local gets its own register.**
  `v->tag = 0xFF; ... return 0xFF;` lets gcc hoist the constant into a branch
  delay slot, lose it to the body, and build it again, which is what retail does
  twice in `sceHsynVoiceFree`. An `int r = 0xFF` gets a callee-saved register
  and costs a copy at the return instead.
- **An `s8` stack parameter passed on without a prototype is loaded before the
  guard.** `sceHsynCmdSetParamRamp` declares its ninth argument `s8` and hands it to an
  `INCLUDE_ASM` callee; the `lw`/`sll`/`sra` then sit above the function's first
  test. Declaring it `int` and casting at the call site leaves the load in the
  loop preheader instead.
- **A narrow parameter passed to a parameter of the same narrow type is
  re-extended.** gcc does not track that an incoming `s8` already arrives
  sign-extended, so `s8` to `s8` costs `sll`/`sra` while `s8` to `int` is a
  plain move. `sceHsynVoiceUpdatePass` forwards its status byte with a `move`,
  which is what made `sceHsynChannelNoteOn`'s third parameter `int`.
- **`(x << 8) / 254` and `(x << 7) / 127` are not the same code.** An even
  divisor sends gcc through its pre-shift path, which folds the shift and picks
  the single-word magic retail has; the odd form picks a 33-bit magic and three
  extra instructions. `sceHsynChannelSetExpressionDirect` scales a 0..127 controller and needs the
  `<< 8` over 254 spelling.
- **Force reassociation with a local.** `msg = (vol << 16) | 0x7B0; ... ch | msg`
  holds where the same expression in parentheses does not.
- **A `u8` destination folds `x & 0xFF` into a move; an `int` destination keeps
  the `andi`.** `sceHsynVoiceDirectPass` passes a masked copy of a byte to a `u8`
  formal and to the test that guards it; only `int vel = val & 0xFF;` leaves the
  `andi` in place for both.
- **A variable reused for a second load of the same width loses gcc's
  knowledge of its bits**, so a later narrowing conversion is emitted rather
  than elided. `sceHsynVoiceDirectPass` reads two bytes into one local for that
  reason, and the register sharing follows.
- **A pointer cast with the base added first keeps retail's operand order.**
  `(u8 *)slot + idx * sizeof(sceHSynChannel_t) + 0x18` gives
  `addu base, idx*56` then `addiu 0x18`, where `&slot->chan[idx]` folds the
  `0x18` into the index arithmetic and adds the base last.

## Loops

- **Index-based loops (`e[i].buf`) create the induction-variable copy retail
  has; pointer-increment loops (`e++`) do not.**
- **`heads[i]` against an explicit `head` pointer decides where a hoisted loop
  invariant lands.** When the body also needs a constant such as
  `1 << v->index`, the indexed form puts the strength-reduced pointer after the
  invariant in the preheader, which `sceHsynChannelAllSoundOff` needs and
  `sceHsynChannelSetPitchBend` does not.
- **Let gcc strength-reduce `out[n]`** rather than keeping your own cursor: the
  reduced induction variable is born in the inner preheader, after the guard.
- **Drop a redundant `if (n > 0)` around a `for (j = 0; j < n; j++)`**, or gcc
  emits the guard twice.
- **`ch = 0;` before the loop with an empty `for` init** reorders two preheader
  initialisations, which is what `modhsyn_e23` wanted.
- **Two walks over the same head array want two cursors, or none.** In
  `sceHsynChannelLimitByPitch` the indexed form `end = heads[i]` gives each loop its own
  strength-reduced pointer *and* puts it after the hoisted loop invariant in the
  preheader, which is the order retail has; a shared `head` cursor merges the
  two and reverses that order.
- `for (i = 7; i != -1; i--)` gives `bne $reg, -1`; `i >= 0` gives `bgez`. Both
  appear in retail.
- **A derived bound keeps the guard's constant in a register.**
  `for (slotNo = 0; slotNo * 2 + 1 < n; slotNo++)` makes gcc strength-reduce
  `slotNo*2+1` into its own induction variable and build the folded initial
  value 1 into a register for the loop guard (`addiu $2,$0,1` then `slt`).
  The equivalent `for (unit = 1; unit < n; unit += 2)` folds the guard to
  `slti n, 2` and shares the 1 with the induction variable, which also changes
  every callee-saved assignment. `modmidi_sceMidi_Init` needed the derived form.
- **`do { ... } while (i++ < n)` is one instruction longer than the `for`.**
  Guarded by `if (n > 0)` with `i = 1`, it reproduces retail's
  `addu $2,$16,$0 / slt / bne / addiu $16,$16,1` latch, where
  `for (i = 0; i < n; i++)` compares the post-increment value directly and
  saves the copy.
- **Two early-exit sites that each increment the counter give it the lower
  callee-saved register, and cross-jumping merges them back.** `sceHsynServiceVoices`
  has one `n++` block in retail; writing it once leaves the counter one register
  too high, and writing `goto skip`/`goto skip2` with an `n++` behind each label
  doubles the reference count, wins the register, and still comes out as one
  block.
- **A counter shared with a later loop gets the callee-saved register.**
  Reusing one variable as both the slot index and the following loop's counter
  is what gives it `$16` and the induction-variable copy; two separate
  variables move every other pointer one register along.
- **A per-iteration pointer to an array element stops strength reduction.**
  `cnt = &sceHsynFreeCount[i];` then `(*cnt)++` gives retail's
  `addu $19, $21, $3` inside the loop, where `sceHsynFreeCount[i]++` lets gcc
  hoist the base and reduce the pointer, costing an extra `addiu`.
- **The three-list broadcast loop**, used by five MODHSYN setters, matches as
  `heads[3]` filled with `chan-8`, `chan`, `chan+8`, then
  `for (i = 0, head = heads; i < 3; i++, head++)` around
  `for (v = end->chNext; v != end; v = v->chNext)`. The comma init is what gives
  `i` the higher callee-saved register.

## Reaching data

- **Stepping to the next `{count, pointer}` segment needs a self-advancing
  pointer.** `seg += 2` on a `u32 *` gives retail's `addiu $17, $17, 8` and
  reads through the new base; any separately named pointer (`vt =
  (sceMidiVoiceTbl_t *)&tbl->nVoiceWord`, even through an integer cast) lets
  gcc fold the first load back onto the old base and keep both registers
  alive. `modmidi_sceMidi_Init` walks the work table that way.

- **The `{count, pointer}` segment.** Several functions reach a table through
  *base+8* then *+4*, never one folded `+0xC`. Matching needs the pair treated
  as one object: `seg = &work->tbl->nBufWord; seg[1]`, or
  `ofs = (slotNo << 4) | 8; seg = (u32 *)(ofs + (u32)tbl->seq)`. The `|` rather
  than `+` is what yields `ori`, and the `ofs + (u32)ptr` operand order picks the
  destination register. It does not always hold: `sceHsynServiceTick` is two short
  because gcc bases the entry loads on `entry+0xC` anyway.
- **An array written back to front wants a pointer local.** Two or more stores
  to `sym[n]`, `sym[n-1]`, ... keep the symbol itself as the `%lo` base only if
  the address comes from a local pointer; writing `sym[1] = 0; sym[0] = 0;`
  directly makes gcc base them on `sym+4`. `sceHsynInitCores` clears six arrays
  that way and `modhsyn_sceHSyn_Init` one.
- **Indexing a cached base beats a self-incrementing pointer when the uses are
  at two offsets.** `entry[slotNo].seq` and `entry[slotNo].slot` reduce to one
  pointer based at the entry; `entry++` with the same two uses biases the
  pointer by the larger offset and reads the other one negatively.
- **An index table counts 8-byte units, not entries**, and the unit
  subexpression has to be shared with the bounds check or gcc emits
  `sll 4 / lw 0xC`.
- **Hoist a global load above pointer stores** when retail loads it first: gcc
  cannot move it past the stores, because it cannot prove they do not alias, so
  leaving it below costs a nop.
- **A tag comparator wants its two byte pointers as named locals.**
  `p0 = (char *)&tag0; ... p0[i]` matches where `*((char *)&tag0 + i)` gives the
  operands the other way round. Loop counter `u32`, for `sltiu`.

## Switches

- **Case bodies come out in source order; the compare chain does not.** gcc
  builds the comparison tree from the case *values*, so reordering the cases
  only moves the bodies. `sceMidiParseMetaEvent` needed `case 0x51` written
  before `case 0x2F`, and `sceMidiScanLoops` the two RPN arms first.

- **A two-armed test on a command byte is a `switch`, not an `if`/`else if`.**
  gcc lays a switch out as the compare chain first, then the case bodies, each
  ending in a jump to the common tail; `if (sel == 0) ... else if (sel == 1)`
  inlines the first body and comes out two instructions short.
  `sceHsynCmdNoteOff` and `sceHsynCmdNoteOn` both needed the switch form.
- **An empty case is still a case.** gcc's binary search picks its nodes from
  the case *values*, so a value whose body is empty still shows up as a compare
  that jumps to the epilogue, and leaving it out reshapes the whole chain.
  `sceHsynVoiceUpdatePass` needed `case 0xA0:` and `case 0xD0:` with nothing in
  them to reproduce retail's tree.

## Shared tails

- **A block-scoped local coalesces with the argument register; a
  function-scoped one does not.** `sceMidiApplyPlayState` builds eleven channel
  messages; declaring `u32 msg` once at the top gives it a single pseudo that
  cannot live in `$7`, so every call ends `or $7, $17, $2`. Declaring
  `u32 msg` inside each `if` block lets gcc build the value in `$7` the way
  retail does. Writing the expression inline instead makes gcc reassociate it
  to `(ch | 0xB0) | (v << 16)`.

- **A `goto` is sometimes the only way to get retail's block order.** Where one
  arm sets a value and both arms fall into a common tail, writing the tail twice
  puts the short arm first and duplicates it. `sceHsynVoiceParamRampStep` needs the
  arithmetic arm, then `goto done`, then the short arm falling through into the
  shared tail, which is exactly what retail's layout is.
- **Walk the list with one `v = next` after the if-else chain.** gcc copies that
  single `move` into each arm's branch delay slot, which is what retail's three
  copies in `sceHsynChannelNoteOff` are; writing it in every arm also matches the
  count but changes which pseudo wins a callee-saved register.

## What is not fixable from the C

**A jump table while `.rodata` is assembly.** A switch dense enough for a
table cannot be written at all: the assembly `.rodata` already holds the
function's `L_<address>` case labels, so replacing the body leaves them
undefined and the link fails. Eight functions are in that position, two in
MODMIDI (`sceMidiPlaySong`, `sceMidiPlayEnv`) and six in MODHSYN
(`sceHsynLfoStep`, `sceHsynVoiceEventDispatch`, `sceHsynLfoSetupA`,
`sceHsynLfoSetupB`, `sceHsynCmdDispatch`, `sceHsynVoiceSetParamRamp`). Write the C, guard it
and move on.

**gcc's cross-jumping.** Where two paths end in the same few instructions,
gcc 2.8 merges them and jumps into the middle of the surviving copy, even
when retail keeps both. Source-level attempts to break it up (a named result,
a `goto` to a label placed where retail's block sits, reordering the arms,
spelling the shared statement differently) all leave the merge in place:
`jump.c` runs after the shape of the C stops mattering. `sceMidiParseMetaEvent`,
`modmidi_e8`, `sceMidiSongRepeat` and `sceMidiSearchPlay` are each one or a few
instructions short for this reason.

**A load retail recomputes.** Where retail loads the same field twice with the
two loads separated only by stores to other fields of the same object, gcc's
CSE keeps the first value live and the function comes out one instruction
short. `sceHsynVoiceVolumeCommit` (`envB.level` across a branch) and
`sceHsynVoiceUpdatePathB` (`v->vag` across two halfword stores) are the recorded
cases; a local for the first read, a pointer local for the stores and
reordering the arms all leave the CSE in place.

**A heavily spilled body.** `sceHsynChannelNoteOn`, `sceHsynVoiceStartPathA` and
`sceHsynVoiceStartPathB` each park a dozen values in frame slots and reload them every
time. A C body with the same locals keeps them in registers instead, and the
spill pattern is not reachable from the source.

**A store to a global immediately before a test.** gas refuses to move the
second half of a `sw reg, sym` macro into the following branch's delay slot, so
the result is one extra nop however the C is written. `modhsyn_e16` is the
recorded case. Guard it under `NON_MATCHING` and move on.

## Before chasing a difference at all

A `+4` in `.text` moves `.bss` by 0x10, through the output section's alignment,
so one wrong-sized chunk makes every later `%hi`/`%lo` data reference differ.
Run `irx_diff.py <module> --sizes` and fix sizes first. But `--sizes` only
reports chunks whose *length* changed: a function with retail's instruction
count and the wrong registers reports nothing, and a wrong-sized function
earlier in the file hides every later difference. Always finish with the plain
`irx_diff.py <module>`, whose first line is the list of differing chunks, and
only trust a green `ninja` as proof.

`irx_diff.py` reads the image that was packed last. It warns when that image is
older than the source, but check the build output for `FAILED` before trusting
any diff: a compile that failed leaves the previous image in place and every
function still diffs clean.
