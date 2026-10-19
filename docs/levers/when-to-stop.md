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

Label the draft with one line saying how close it is and what differs, which
`revert_differing.py` writes from check_unit's measurement:

    /* NON_MATCHING: 682 of 694 words, exact size; three reload spill slots rotated. */

Then write the draft up in [Non-matching drafts](nonmatching.md): what the
remaining difference actually is, and which source shapes you ruled out. The
one-line label tells the next person whether to look; that page tells them
where to start and what not to retry. `python3 tools/check_nonmatching.py`
compiles every draft and refuses one with no entry, so this is not optional.
`--discard` deletes a draft not worth keeping.

A draft is never evidence of anything. It is not compiled by the build, so
`tools/check_nonmatching.py` is what stops it rotting when a struct or a
signature changes under it.

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
- A set of reload spill slots that appear in a rotated order. The slot offsets
  follow pseudo-register numbers, so the fix would have to change which pass
  creates a value rather than its source shape. `fldSkyDrawFilter` sits at 682
  of 694 words on a three-slot rotation that twenty-odd source shapes, nine
  declaration orders and every pointer form tried leave untouched.
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
- `xori reg, reg, 0` in front of a `movn`. `frGetXShiftLev` in
  `interface/frFont` has one, on `(fontIndex < 2)` feeding
  `movn $18, $6, $3`, so that function is capped by this row and not worth a
  long attack. No `!= 0` shape (plain `if`,
  ternary, `> 0`, `>= 1`, explicit `^ 0`, a `continue`, a local of either
  width) adds it; every one of them emits the `movn` alone.
  `sdfModelLinkDrawPacket` in `sdf/sdfModel` is a second instance, on
  `if (aMode != 0) { aOtIndex = packed >> 16; }`, and twenty-three spellings
  were compiled and dumped in isolation against it: both ternary polarities,
  an `if`/`else`, `== 0` with the arms swapped, `!!`, `-0`, `> 0` unsigned, a
  one-case `switch`, a 0/1 condition local, three shapes of a zero local that
  constant-folds, and `u8`/`u16`/`s8`/`s16`/`s64`/`u64`/pointer condition
  types. The only thing that produces an `xori` at all is comparing against a
  **non-zero** constant, so the zero case is not reachable. The missing
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
