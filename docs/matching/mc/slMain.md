# Matching notes: mc/slMain

## slSaveData_writeFile (mc/slMain, 0x238)

106 of 142 words match, at the exact 568-byte size.

The 36 remaining words are one register renaming carried through the whole
record-copy block. The instruction sequence is identical; retail holds the
byte offset `52 * slot` in `$5` and `%lo(D_00569740)` in `$2`, and the draft
holds the offset in `$2` and the base in `$7`, so every address temp after it
shifts by one number. Nothing in the block differs otherwise.

What the shape needed, and is worth keeping:

- The save header lives at offset 0 of the save work, so the stores go through
  `((slFileHead_t *)DDS3_GBWK)` written out per statement; that reloads
  `DDS3_GBWK` for every field, which retail does too.
- Two types for the same 52 bytes. The header in the save work is 4-aligned
  (its 20-byte block at `+0x10` copies as `ldl`/`ldr` plus a plain `lw` for
  the trailing word), while the on-card record array is byte-aligned (the same
  block copies as `ldl`/`ldr` plus `lwl`/`lwr`). One struct for both gives the
  wrong half either way.
- The record side is a `u8 (*)[52]` view. Five of the thirteen field stores
  (`+0x1`, `+0x2`, `+0x8`, `+0x10`, `+0x24`) go through the subscript and the
  other eight through a cached `u8 *rec`: that mix is what produces retail's
  two `addu` operand orders and its three copy temps. All thirteen through the
  pointer is six words short, all thirteen through the subscript is twelve
  words over.
- Source order ends `playTime`, `endcnt`, `newfile`, `mode`, `name`,
  `mapOpen`; putting `mode` back in field order after `level` costs nine
  words.

Ruled out: the declaration and assignment order of the two pointer locals;
`rec` as `u8 *` and as `u8 (*)[52]`; a separate `base` local so the symbol
address is created first; `slot` as `u32`; writing the `+0x8` store
base-first and the `+0x1`/`+0x2`/`+0x24` stores index-first with explicit
casts; and five orderings of the last six stores.

## slFileSelCalc (mc/slMain, 0x340)

208 of 208 words match at 832 bytes in ordinary C. The selected index owns
its actual decrement, increment and normalization results. An ordinary
branch-local old-index snapshot feeds the subtraction; normalization updates
`selected += 50` and then `selected %= selectionRange`, where the real divisor
is five. All diagnostic register bindings and empty identity operands were
removed together and the complete owner remained exact. None is retained or
claimed necessary by the final reconstruction.

The initial paging condition reads the actual count. A later count snapshot
is placed at each genuine branch exit: after the increment in its arm, before
the selected-index reload in the other paging arm, and in the low-count arm.
Each path still makes one final observation, with no callback between the
moved operations. Selected-index reloads remain after the relevant pad/sound
callbacks. The final C writes the save mode before the query message, while emitted
instructions store the query message before the save mode, matching native.
These are adjacent ordinary stores to distinct globals with no intervening
callback; the source order produces the desired compiler scheduling rather
than directly specifying the native order. No artificial memory access or
dependency is added.
The original signed arithmetic and remainder contracts remain unchanged;
this is not a claim of safety for arbitrarily corrupted index values.

The early zero-info exit and the genuine default no-action path share the
existing final zero return. Qualified CSE evidence for this control retains
the shared zero definition, where the earlier separate-exit form eliminated
it. The observed delay-slot pass then places that definition in the annulled
branch delay slot, producing the native branch-likely/zero-result sequence.
No action work, callback or cleanup is bypassed by the shared label.

The final ordinary source has its own qualified scheduler evidence. Before
sched1, the divisor definition UID214 precedes the genuine in-place add
UID217, and the add has no REG_DEAD annotation. Sched1 reports equal priority
39 and cost one, with three dependents for the divisor and two for the add,
and actually issues the divisor before the add. Divmod then consumes both
real values and the global store consumes the normalized result. An earlier
diagnostic's REG_WEIGHT explanation was an inference, not a directly observed
hooked tie-break event; it is not asserted as the final source's definitive
cause. No original-source spelling is inferred.

Independent semantic review and canonical complete-owner checking give
100 match, 0 differ, preserving all
99 peers and owned data. Final ordinary plain versus stock/verbose dump
assembly agrees after excluding only the first `.file` scratch-path directive,
and whole-owner text bytes agree. This qualifies the retained compiler
observations without calling raw assembly byte-identical.

Final local gates pass the retail checksum, 26 tooling test files / 427 cases,
all 146 guarded owners / 523 drafts with stable source/compiler inputs,
seven IRX owners, and reconciled production reports.

Complete-object comparison also confirms all five allocated sections, 766
symbol contracts including 104 function records, and 1,759 ordered relocation
and addend contracts. The final commented production object agrees with the
qualified ordinary object on those contracts and the ELF header; differing
scratch filenames occur only in non-emitting debug metadata.

Source commit `7d166188accf3d9459e84105b002c7134f3c4696` passed exact-source
hosted CI [41176090359](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41176090359),
attempt 1, including build and deploy. The run completed successfully at
06:21:55 UTC on 2026-10-31; the deployment reported success at 06:21:52 UTC.

## slCopyNonZero (mc/slMain, 0x40)

16 of 16 words match at the native 64-byte size. The complete owner checks
101 exact C functions and owned data. The two-byte input-pair loop preserves
the conditional nonzero lead-byte copy, unconditional tail-byte copy and final
NUL store. Signed nonpositive counts still skip the input loop and write only
the terminator. Loads and stores retain their native byte widths and order,
including when source and destination ranges overlap.

One localized empty operand constrains the initialized, live source cursor at
the trailing-store boundary. It emits no instruction and has no memory clobber;
it is not a general memory barrier. The cursor still feeds its genuine increment
and subsequent input loads. The independent source/destination increments then
occur in source-before-destination order. No fixed register, extra memory
access, fabricated bounds, dummy consumer or production compiler flag is added.

Qualified final compiler dumps show the specific dependency: in the unmodified
14/16 draft, the second scheduler queues the tail store for two modeled cycles
after its load, issuing the source increment in cycle zero and the countdown
in cycle one before the store. With the localized volatile operand, the store
precedes the operand, and the countdown and cursor increments follow it. The
source increment issues before the destination increment; the delay-slot pass
places the latter in the loop branch's delay slot. These are measured compiler
snapshots, not a claim about original source spelling or a universal ordering
guarantee for empty operands.

Removing the operand while retaining the increment order returns to 14/16.
Making only the operand nonvolatile gives 13/16. Keeping the volatile operand
but the old destination-before-source increment order gives 14/16: the tail
store is correct but the two increments occupy opposite sides of the branch.
An ordinary explicit countdown loop also gives 13/16 and changes the branch
from nonzero to greater-than-zero. All these controls remain 64 bytes and
preserve the 100 previous C matches.

Plain, diagnostic (`-da -fsched-verbose=5`) and uninitialized-warning builds of
the final source agree on all five allocated sections, 766 non-file symbols,
1759 ordered relocations and normalized assembly. The warning log is empty.
The diagnostic options are not production settings.
