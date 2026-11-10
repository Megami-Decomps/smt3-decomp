# Matching notes: effect/effMagatuhi

## effDebugDrawRectLine (effect/effMagatuhi, 0x154)

72 of 85 words match, at the exact 340-byte size and with the same instruction
multiset and the same six callee-saved registers (`$16` radius, `$17` `x1`,
`$18` `y1`, `$19` the flag, `$20` `y0`, `$21` `x0`).

The thirteen differing words are the argument setup for the first
`dds3PrimLine`, reordered. Retail emits `lui $7` for `0x60008080`, then the
`sll`/`addiu` pairs that finish `x1` and `y1`, then `ori $7`, then the two
`lui`s for `0xFF0000`, and sets `$4` and `$5` from `$21`/`$20` last; the draft
sets `$4` first and lets `ori $7` slip between the two `sll`s and the two
`addiu`s. Nothing is added or dropped.

The shape that got it this far is **separate `x1`/`y1` locals**. Reusing `left`
and `top` for the second coordinate pair (the natural reading of retail, which
updates `$17`/`$18` in place) costs 14 more words, because the flag then takes
`$17` and `left`/`top` shift up to `$18`/`$19`: giving the second pair their own
locals shortens `left`/`top`'s live ranges enough to reorder the allocation back.
Declaration order of the locals has no effect at all here, in line with the
`frSetFontString` row in [matching levers](matching-levers.md).

Ruled out for the remaining thirteen, all still at 13 (or worse): `y1` before
`x1`; `top +=` before `left +=`; the add and the scale interleaved per axis;
`x1 = (left + radius * 2) * 16 + 0x7000` as one expression; `radius * 2` inline
at both adds; the `* 16` and the `+ 0x7000` as separate statements; the pair
computed inside the `if`; `0x60008080`, `0xFF0000` and the mode `0` as named
locals; `if (drawCross == 0) return;` and bare `if (drawCross)`. A packet temp
per call (43) and a `tag` local for `effDebugDrawTag` (42) are much worse, and
drawing the two lines in the other order is 18.

This looks like the ruled-out "pure instruction scheduling" row: the registers
and the instructions agree and only their order in one block does not.


## effDebugMoveCamera (effect/effMagatuhi, 0x628)

377 of 394 words match at the native 1,576-byte size and 0x40 frame.
The pad-driven camera controller remains guarded, using existing VU0 macros
and one localized operand on its genuinely initialized sample pointer.
The earlier body emitted 1,564 bytes with a 0x30 frame; the measurements below
separate that baseline from the retained pointer-lifetime improvement.

The first right-stick channel translates eye and target along the normalized
look direction. The other channels construct normalized cross-product axes,
load the actual `SetRotateMatrixAxis` transform, then rotate target about eye.
The two digital controls translate along the derived up axis, with button 10
winning over button 8. The unsigned-byte dead zone includes both 64 and 192.
Each stick pair is captured before its first operation; the second pair is
loaded after the first pair's optional rotation call. The saved final channel
must not be reread after later rotations.

The scale scratch writes XYZ only. Its volatile stores are the existing
bare-address VU0 hand-off idiom; the full quadword load still observes the
uninitialized W lane, as native does. No invented initialization or padding is
added. The three angular rates have the same binary32 value, represented by
`0.06f`; linear movement uses 22.5 and the original multiplication order.

A qualified baseline no-dumps CSE/allocation measurement on this owner verifies hooked,
unhooked and ordinary compiler output parity. CSE rewrites input loads from a
real local pointer into direct references to the small-data buffer: UID13
becomes buffer+3, UID191 buffer+1 and UID194 buffer+0. The shared horizontal
sample (pseudo86, six references, live187, one crossed call) receives `$16`.
The native additionally retains the input address in `$17`, while this source
folds that pointer away, losing its save/restore and changing the frame.
A later bounded actual-pointer control uses one nonvolatile
`FAKEMATCH_ASM_LIVE(stick)` immediately after the initialized buffer+2 pointer,
before the real loads. C89-required declaration movement has an ordinary
control with identical output. Qualified unhooked jump/CSE snapshots and
plain/dump assembly, allocated-section, symbol and relocation parity show
that the operand retains the genuine pointer-relative loads. It restores the
native 1,576-byte size and 0x40 frame, but only 377 of 394 words match:
the pointer receives `$16` and horizontal sample `$17`, opposite native,
with entry scheduling, equivalent byte arithmetic and branch-form residuals.
This is not a match or recovered original spelling. All 79 current exact
peers and owned data remain exact. No binding, volatile pointer/buffer,
additional consumer, alternate placement or buffer-view change was tried.
The qualified snapshots are not hooked allocator-decision evidence. The
empty operand is not a memory barrier, and the raw native W-lane contract
remains. The real buffer has eight bytes; buffer+2 and offsets 1, 0, -1, -2
stay within that object. Existing calls justify the pointer lifetime, and
emitted output preserves sample/callback order without an asynchronous-input
guarantee. XYZ-only scratch initialization does not establish deterministic
W, portable uninitialized-lane semantics or global flag unobservability.

Separate channel locals and chained volatile XYZ assignments were examined.
The latter introduce scalar reloads absent from native; separate XYZ stores
preserve the hand-off without those reloads. An incomplete buffer declaration
also loses native GP addressing, so the existing eight-byte provider view is
retained. The historical baseline preserved 78 exact peers; the current
owner preserves 79, without crediting that separate addition here.

Final integrated production owner 79/0 and active target 377/394 with all
peers/data preserved pass. Canonical three-mode strict and plain/dump output
parity pass with inherited unrelated pointer warnings. Fresh stable
140-owner/480-draft qualification, 459 tests, 817 layout assertions,
inline/shell checks, seven IRX modules, retail SHA1/byte comparison/no-op
and reconciled reports pass. Coverage stays 8594 functions / 1485744 bytes;
no exact matching credit is claimed.

## effDebugDrawMenu (effect/effMagatuhi, 0x764)

473 of 473 words match at 1892 bytes in ordinary C. Separate color cases
9/10/11/12 represent actual attributes 2/4/5/6 and byte offsets 0/1/2/3;
only case 9 draws the packed-color rectangle first. Each snapshots/restores
attribute at the native point, reloads the label after SetAttr, then reloads
the value pointer/byte after the first print. Item type is not rechecked
across callbacks. Separate numeric cases 4/5 retain genuine native entries.
Direct format calls for 13/14 recognize only 0x44/0x48/0x42/6; unknown values
skip formatting and print the existing text, without an invented default.

Type 15's local u32 view of the actual s32 value recognizes exactly 0/1/2/3.
Negative representations cannot become those small values, so no domain
restriction or false field/provider type is introduced. The signed control
is 1908 bytes; changing only this local selector yields native equality-only
dispatch and 1892 bytes. Paired expansion shows signed versus unsigned range
splits, not a claimed hidden compiler decision. All 24 outer cases/defaults
and native distinct body placements remain. No constraint, binding, volatile,
extra consumer or new type recovery is needed.

The update callback and parameter dereference remain before the null-menu
test; original invalid-input behavior is not repaired. Callback-sensitive
menu/count/selection and item observations remain fresh. Control and final
complete-owner plain/dump builds independently pass normalized assembly,
allocated-section contracts, relocations and function-inventory parity.
Bounded formatting/color decision models cover selected signed32 domains and
all byte-type routes, not full callback execution, arbitrary memory or all
32-bit inputs. Exact calls/accesses provide separate native evidence.

Independent owner checking gives 79 match, 0 differ, preserving all 78 peers
and owned literals/tables. Combined retail checksum, 427 tests across 26 tooling
files, 147 guarded owners / 547 drafts, seven IRX owners and report gates passed
for source `b362bd871f355c758be545f97a43237e7f324ead`. Exact-source CI
[41078870717](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41078870717)
completed successfully, with every actual step passing and none skipped.
