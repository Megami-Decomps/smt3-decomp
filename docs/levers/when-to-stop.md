# When to stop, and keeping the draft


A differing function must not stay in the build: a near miss earns no credit
and hides that the unit is incomplete. The draft itself is worth keeping, so
`python tools/revert_differing.py <unit.c>` wraps every function the unit
check will not credit as

    #ifdef NON_MATCHING
    <the draft>
    #else
    INCLUDE_ASM(const s32, "unit/path", name);
    #endif

The normal build never defines `NON_MATCHING`, so it takes the asm and stays
byte-identical, and the function still counts as unmatched everywhere. Run
`configure.py --force-split` afterwards, since the set of `INCLUDE_ASM` lines
has changed, and `python3 tools/check_nonmatching.py` to confirm the draft
still compiles.

The guard is the marking. No source annotation or matching note is required.
Every callee must be declared before use; the retained-C compile treats implicit
function declarations as errors. Existing optional findings are under
[matching/](../matching/README.md). `--discard` deletes a draft not worth keeping.

A draft and its `#else` fallback must stay in the same file when a container
is split, and the fallback may carry an `INCLUDE_RODATA` line before its
`INCLUDE_ASM`. `tools/split_unit.py` does not know about either: it has
copied a draft region into every part that inherited any of the container's
text, parked a part's extern declarations inside the draft branch so the
region could not simply be deleted, left an orphan `#else`, and emitted empty
`#ifdef NON_MATCHING` pairs. After a split, run the draft checker before
believing the result.

These have been ruled out repeatedly and are not worth a second attempt:

- Pure instruction scheduling, where the same instructions appear in a
  different order with no semantic difference to steer.
- Register allocation that needs one register fewer than the C uses, shifting
  every later allocation. Change the source shape or stop.
- A plain register-to-register copy retail keeps and the C coalesces away,
  normally of a parameter or of a value read once and used twice. It costs a
  word, so the function comes out short and every later allocation shifts.
  Earlier drafts of `ragBuildList2` and `ragBuildList3` both missed retail's
  `daddu $16, $5, $0` before the row offset. Separate locals, alternate array
  address spellings, casts and moving the loop update all coalesced the same
  way. Both functions later matched by preserving the real output-address
  formation and the separate count lifetimes; see [the list builders](
  ../matching/fclRagInit.md#ragbuildlist2-and-ragbuildlist3-372-bytes-each).
  The remaining cases in `code_001FAE80` are `cmbCountEvoReady`, which
  narrows its count parameter in place and then copies it to `$17`, and
  `cmbGetLevelUpCount`, which copies the level so the compare and increment
  use different registers. Measure the size and change the underlying
  use or lifetime before trying more copy spellings.
- A set of reload spill slots that appear in a rotated order. `alter_reg`
  hands slots out in ascending pseudo number, so permuting declarations or
  pointer spellings often cannot help. The earlier [sky-gradient draft](
  ../matching/field/fldSky.md#fldskydrawfilter-fieldfldsky-0xad8) stopped
  at 682 of 694 words: GCSE PRE created three pseudos while walking its
  expression table, and reload's order followed their creation. A later
  match came from factoring the real channel interpolation into an inline
  helper. Prefer this kind of ordinary structural change over trying to
  manipulate pseudo numbering or frame layout.
- An epilogue duplicated into a branch-likely delay slot is **no longer in
  this list**: the cause is known and it is usually fixable. See the section
  on callee visibility below. Note the narrow scope: ee-gcc 2.96 emits `bnel` from
  ordinary C readily, 42 times in `field/fldDraw` and 126 in
  `newbattle/nbMakePacket`, so a `bnel` in the retail code is not a reason to
  skip a function. Only this one use of it has resisted.
- A frameless conditional sibling call. ee-gcc always saves `$ra` for a
  sibcall.
- Zero-extending a byte with a `sll 24`, `srl 24` pair. ee-gcc emits
  `andi 0xff` for every source shape tried.
- `sltiu rd, rd, 1` negating a comparison, where ee-gcc emits
  `xori rd, rd, 1`. combine knows an `sltiu` result is already 0 or 1 and
  rewrites the "equals zero" test into the cheaper `xori`, so the second
  `sltiu` needs a value whose range it has lost. `cmpChkDoNotUseSkill` in
  `code_0022C578` is capped at one differing word by this; `x >= C`,
  `!(x < C)`, `(x < C) == 0`, `x < C ? 0 : 1`, `(u32)x >= C`,
  `(u32)(x < C) < 1` and a `u32` local holding the comparison all emit the
  `xori`.
- `xori reg, reg, 0` in front of a `movn`. `frGetXShiftLev` in
  `interface/frFont` has one, on `(fontIndex < 2)` feeding
  `movn $18, $6, $3`, so that draft retains this residual. No `!= 0` shape (plain `if`,
  ternary, `> 0`, `>= 1`, explicit `^ 0`, a `continue`, a local of either
  width) adds it; every one of them emits the `movn` alone.
  `sdfModelLinkDrawPacket` in `sdf/sdfModel` is a second instance, on
  `if (aMode != 0) { aOtIndex = packed >> 16; }`, and twenty-three spellings
  were compiled and dumped in isolation against it: both ternary polarities,
  an `if`/`else`, `== 0` with the arms swapped, `!!`, `-0`, `> 0` unsigned, a
  one-case `switch`, a 0/1 condition local, three shapes of a zero local that
  constant-folds, and `u8`/`u16`/`s8`/`s16`/`s64`/`u64`/pointer condition
  types. In those isolated trials, only comparison against a **non-zero**
  constant produced an `xori`; that is not a general impossibility result.
  Qualified matched-SMT3 observations show XOR-zero created by late `ce`
  conversion after CSE; see [the pass-timing evidence](../matching/code_00116AD0.md#fldanahoriselectinit-code_00116ad0-0x7c).
  The failed spellings remain useful negative results for these drafts. The missing
  reference also costs the allocation: retail gives `aMode` a saved register
  ahead of the next parameter, and without the `xori` the whole body's
  registers rotate.
- A `jal` plus a shared epilogue in a `switch` default arm, where `return`
  against `break`, a non-void or struct return type on the callee, and a dead
  assignment of the call's *result* all leave the arm a sibcall. A dead store
  to another variable does work, so try that first: see the tail-call row
  above.

A function whose body is mostly inline assembly is refused by the production
report anyway, at 8 instructions and more than half the body, so writing one
to force a match earns nothing. See [Reading progress](progress.md).
