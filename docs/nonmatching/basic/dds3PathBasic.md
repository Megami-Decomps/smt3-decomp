# Non-matching drafts: basic/dds3PathBasic

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## dds3AllocPathBasicWorkData (basic/dds3PathBasic, 0x1CC)

109 of 115 words match, at the exact 460-byte size.

Every instruction is retail's. What differs is where the `table->count` reload
for the loop test lands on the position arm. Retail emits it inside that arm,
right after `dds3AllocSplineHandle` returns and before the `pSplineHandle`
store, and jumps past the shared reload; the draft has no reload in the arm and
branches to the shared one instead, which costs the same three words in a
different order.

Pinned down on the way, all of it needed for the rest to match:

- The curve entry is an explicit `dds3PathCurveEntry_t *` advanced by the loop,
  not `table->entries[i]`; indexing makes gcc keep two invariant bases and a
  multiply instead of one induction variable.
- The switch arms are in source order 4, 0, 2, 5 (scalar, position, rotation,
  world transform). gcc reorders the dispatch tree to the front of the switch
  and leaves the arm bodies in source order, so the body addresses fix it.
- `keys->frames[keys->count - 1]` is a named `u32` local. Written inline twice
  it is loaded twice, because the branchy unsigned-to-float expansion splits the
  basic block and CSE does not reach across it. The two `cvt.s.w` sequences are
  retail's: only the load is shared.
- `pSplineHandle` is `s32` while `count` and `flag` are `u32`, so only the flag
  store kills the count load. That is what puts a reload after the flag store
  on the rotation/scalar/light arms and none at all on the default arm.

Ruled out: arm order within the position case (flag before or after the
pointer store changes nothing, the scheduler rotates it), a temporary for the
call result, `continue` in place of `break`, a `do`/`while` loop, switching on
a local copy of `entry->kind`, and swapping the two loop updates.
