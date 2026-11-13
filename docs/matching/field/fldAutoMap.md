# Matching notes: field/fldAutoMap

## fldAutoMapCloseBlock2 (field/fldAutoMap, 0xA8)

42 of 42 words match at 168 bytes. One localized empty read/write operand on
`index = area - 1` restores the native order of two real decrements. Both
writing branches consume index in their actual address calculation. The
operand emits no instruction, pins no register, adds no memory access or
clobber, and does not extend the initialized index's lifetime. No call crosses
that lifetime. The ordinary deletion control remains 40 of 42 words matching;
constraining the block bit instead leaves the same residual, and an input-only
index operand gives 16 matching words. These are paired output observations,
not an internal scheduler ranking or recovered original source spelling.

Native computes the address index at +0x4 before the division, and the block
bit at +0x14. The earlier note incorrectly called +0x14 a `beql` delay slot:
the divide-check branch is at +0xC and its `break` delay instruction is +0x10.
Deleting the operand exchanges only the decrements at +0x4 and +0x14.

Independent review confirms signed remainder by 100, early return for room
values below 10, the split after subtracting 10 at room 30, and unchanged
low/high banks 0xFC58/0x17F10. The high bank retains the full room index.
Each writing path performs one 64-bit load, clears the selected bit and
performs one 64-bit store. The matched OpenBlock2 sibling corroborates the
same banks and index/unsigned-mask roles, with OR instead of AND/complement.

Inherited source limits remain explicit: `area - 1` and `SBlock - 1` must be
representable; reached signed offset arithmetic must be representable; reached
shift counts must be 0..63; storage must be valid and suitably aligned. Area
1..32 and SBlock 1..64 are a sufficient logical domain, not proven global
caller bounds. The script caller's zero substitutions do not establish those
bounds. No new masks, truncation, checks or input restrictions were added.

Complete owner: 36 match, 0 differ. Eighteen development whole-assembly and
entire-ELF normal/stock-dump pairs cover six sources in three modes. Fresh
integrated guarded/final qualification adds six passing pairs with 123 stable
collected inputs, all strict modes clean. All 39/35/39 production/SKIP_ASM/retained peers and owned data are
preserved. Independent resolution of the two GP-relative global loads gives
all 168 native target bytes. No hooked compiler-decision trace is claimed.

Optimized and ASan/UBSan host fixtures each pass 2,911,040 comparisons across
582,208 cases and five source variants. They model the empty operand as an
identity and exercise the stated valid storage/arithmetic domain; they do not
establish arbitrary-index safety or EE runtime behavior. Leak detection is
disabled for the sanitizer fixture in the execution environment.

Integrated retail checksum/byte comparison, generated-source validation,
505 unittest cases without skips plus 818 layout assertions, frozen guarded
EE/IRX checks and reconciled reports pass. Recovery and shared-panel peers
remain exact. No source-import change is needed. Only this 168-byte function
gains credit: game totals become 8,654 functions and 1,503,224 bytes. The
subsequent build has no work.

### Earlier ordinary-source controls (historical)

Earlier declaration-order and inline-use experiments did not close the two
setup differences. Declaring the index first corrected their order but moved
its register and changed the high-bank arm. All six declaration orders and
36 declaration/assignment splits were explored; no ordinary exact form was
found. Inline index/bit expressions and a second offset local were also
rejected. Those historical counts were not rerun for this integration.

### Initialized-index sibling control (historical closed negative)

A bounded recheck on c61f8129 reproduced the retained 40/42-word draft at
168 bytes, with 34 exact peers. The matched OpenBlock2 sibling's ordinary
initializer order gave 24/42; binding only the genuinely initialized index
locally to its native register gave 26/42, still 168 bytes. The binding
restored setup through +0x28 and the low-offset arm, but not the high arm's
branch, mask and pointer allocation. Both controls were rejected. They left the original
source and assembly fallback unchanged at that time; no further binding grid
was justified by that result.

Whole-owner plain/dump assembly, allocated sections, non-file symbols and
ordered relocations agree for each control, with owned data and peers exact.
Qualified RTL confirms the real index definition and annulled branch/shift
behavior, not an internal ranking or eligibility cause; no hooked debugger
trace was available. The inherited contract still requires valid aligned
storage, representable signed address arithmetic and reached shift counts
in 0..63. The script caller does not establish global input bounds. No new
checks, truncation, input restrictions or matching credit were introduced.

## fldAutoMapDrawIconOfsR (field/fldAutoMap, 0x244)

10 of 146 words match, at the exact 584-byte size.

The function builds one textured quad for an icon. What it does is fully
recovered; what is left is register allocation.

`fldAutoMapIconOfsTable` is `s16 [][8]`: `[0]` and `[1]` are the texture
origin, `[2]` and `[3]` the size, `[4]` and `[5]` the anchor offset. The u
coordinates and the x coordinates scale by 16, the v and y coordinates by 8.
`sdfPrimLinkFt4` takes 21 arguments, the last 13 in the outgoing frame at
`0x0` through `0x60` in 8-byte EABI slots:

    sdfPrimLinkFt4(tag, color, mode,
                   x0, y0, u0, v0,  x1, y1, u1, v1,
                   x2, y2, u2, v2,  x3, y3, u3, v3,  mask, next)

`mode` selects the quad's winding: the default lays the four corners out
top-left, top-right, bottom-left, bottom-right; `mode == 8` flips the rows;
`mode == 4` transposes, taking the anchor from `[4]`/`[5]` the other way
round and the extent from `[3]`/`[2]`. The tail is two separate
`fldDrawGetOt` calls, the first supplying the argument and the second the
`regFunc` to call through, so the source reads
`((...)fldDrawGetOt()->regFunc)(ot, tag)` with `ot` from an earlier call.

What is needed to get the three `lh` loads and their shifts right: `s32`
locals for `t[0]`..`t[3]`. Read as `s16` at each use, ee-gcc loads `lhu` for
the `* 16` use and then sign-extends with `sll 16`, `sra 13` for the `* 8`
one, which adds two instructions per arm. `s16 *t = fldAutoMapIconOfsTable
[index];` as an initializer at the declaration is what gives retail's four
saved registers and the 0xB0 frame; assigning it after the three leading
calls spends one fewer and lands at 564 bytes.

The remaining difference: retail copies `x` into `$f21` and `y` into `$f20`,
the draft the other way round, so every `mul.s` and the two `cvt.w.s`/`mfc1`
pairs in each of the three arms name the other register. Both compute `y`
first, in the same statement order, and swapping the statement order does not
move the assignment. Retail also keeps `index * 16` in `$16` and rebuilds the
table address into a temp after the calls, while the draft keeps the whole
pointer in `$16`; recomputing `fldAutoMapIconOfsTable[index]` inside the
default arm does reproduce that, but costs size elsewhere.

Measured and rejected: the row addressed as `fldAutoMapIconOfsTable[index][k]`
at every use (ee-gcc builds a chain of pointer copies, 620 bytes), `x0` before
`y0` in each arm, and the four combinations of those with the pointer either
initialized or assigned.

## fldAutoMapSeqStart (field/fldAutoMap, 0x30C)

148 of 195 words match, at the exact 780-byte size. Everything up to `+0x1A4` is
identical; the rest is the closing block that fills three vectors.

Recovered along the way, and used by the matching `fldInitPacLoader`:
`fldAreaWork_t.pacRes[10]` at `+0xD0` is the pac loader's handle and mapped
pointer per entry, and this function reads the mapped pointers `pacRes[7]`,
`[9]`, `[1]`, `[3]`, `[5]` straight into `sdfTexSet`. It also reads
`amFloor` at `+0x64`.

`D_00532230` is the `{0, 0, -1, 1}` up vector, assigned to a local
`sdfVector4_t` (which gives the `ldl`/`ldr`/`sdl`/`sdr` run), and the three
locals go out through `SDF_VU0_COPYVECTOR` to `D_002F5E00`, `fldAutoMapRect`
and `D_002F5E10`. `D_0054C4C4` and `D_0054C4C8` are this unit's `.lit4`
entries at file offsets `0x44D4C4` and `0x44D4C8`, holding `-24000.0f` and
`20000.0f`; the other four constants in the block (200.0, 255.0, 1000.0,
2048.0) have zero low halves and come out as `lui`/`mtc1`, so they add
nothing to the pool.

Two things were needed to reach the exact size. `fldAreaWork_t`'s light block
has to be addressed through a local pointer to `&dds3LightParam2[0x20]`, which
is what folds the `0x20` into the `%hi`/`%lo` pair instead of leaving it in
the store displacements. And the three `0.0f` stores have to go through one
`f32` local: written as literals ee-gcc emits three `sw $0`, where retail
materialises the zero once with `mtc1 $0` and stores it with `swc1`, which is
one word more. Assigning `target.y` and then copying it into the other two
works equally well (49 words out instead of 50 before the rest was ordered).

What is left: retail holds the two world coordinates in `$f3` and `$f0` and
the zero in `$f1`, the draft in `$f2`, `$f1` and `$f0`, and retail does the
up-vector copy before the float arithmetic rather than after. All 720
orderings of the six statement groups in that block were measured (the best
six tie at 47 words), as were all 24 orderings of the four `D_00551CAC`
through `D_00551CB8` stores. Nothing moves the register choice.

A forward prototype for `fldAllIconDel` is required: the unit defines it below
this function, and the implicit declaration makes ee-gcc treat it as returning
`int`.

## fldAutoMapDrawIconOfsRF (field/fldAutoMap, 0x33C)

207 of 207 words match at the native 828-byte extent. The complete owner is
35 match / 0 differ, including owned data. The rotated icon builds four actual
texture-coordinate endpoints, rotates its four corners through VU0, converts
the screen coordinates and emits one textured quad.

Two real scalar phases must remain distinct: signed integer `width`/`height`
feed the float extents, while `u1`/`vv1` are the later texture endpoints. The
integer arithmetic stays within s32 for every signed-halfword table element.
The ordinary separated form still matches 187/207 words. Local bindings on
`vv0` and `vv1`, to native call-preserved `$17` and `$18`, close the remaining
allocation differences. Both have actual quad-call consumers; no empty asm,
extra access, dummy consumer, invented type or bound is introduced.

Deletion-only controls at the same 828-byte extent:

- Remove both bindings: 187/207 words.
- Remove only `vv0`: 196/207.
- Remove only `vv1`: 180/207.
- Keep the bindings but reuse each extent as its endpoint: 196/207, with
  a float conversion/store register swap and one commuted integer sum.

These are complete-owner emitted-output measurements, not an inferred internal
allocator mechanism or evidence of historical source spelling. Binding all four
texture coordinates regresses and is not retained. Ordinary source remains
preferred everywhere it already matches.

The initialized values cross only `SetRotateMatrixY` before their final quad
argument consumers. Native review follows its eleven transitive function bodies.
Nine do not touch `$17`/`$18`; `__ieee754_rem_pio2f` and
`__kernel_rem_pio2f` save and restore both. The existing VU0 sequence modifies
its vector/accumulator state and does not touch those GPRs. The
XYZ operation does not consume the uninitialized input W component; all native
vector loads, arithmetic and stores are retained in their original order.

Color packing converts the signed sine result to u32 before the left shift,
matching the native low-word shift without negative signed-shift undefined
behavior. This is byte-neutral. The two allocator/draw declarations now match
the actual providers: `sdfGrAllocPacketBuff` returns an s32 EE address, consumed
through `EE_ADDR_AS`, and `sdfPrimLinkFt4` takes signed color and an allocator
function pointer. Its allocator argument remains null. The declaration-only
control preserves all existing code and data in all three compiler modes.

Complete production, SKIP_ASM and retained-draft ordinary/`-da` assembly and
whole-object pairs agree; strict implicit-declaration builds pass. Resolved
peer checks preserve all 39/34/39 other functions and their relocation targets.
The production 96-byte and retained-draft 64-byte literal pools are unchanged.
SKIP_ASM gains only this target's native eight-byte literal prefix; all peer
literal values and the remaining pool bytes are unchanged. No hidden internal
compiler trace is claimed.

Host tests of the actual final body, with modeled VU operations, compare packet
arguments, callback order and mutable globals/table data with an independent
reference. They pass 100,000 finite-input/full-s16-table scenarios in optimized
and ASan/UBSan builds. This is sampled source behavior, not EE FPU emulation,
proof for arbitrary invalid indices or a new contract for out-of-range float
conversions. LeakSanitizer is disabled under the execution environment.

## fldDrawAreaBlockFont (field/fldAutoMap, 0x388)

57 of 226 words match, 908 bytes against retail's 904.

The area-name panel: it steps the name's fade counter by the panel mode, then
draws the place name (or the area name) and its backing panel. Fully recovered
as behaviour; what is left is one saved register and the two fade arms.

What the function reads, since none of it was named before:

- `fldGlobalBase + 0x90` is the panel mode, 0 to 7, which drives the jump
  table. The HD reconstruction calls it `AllPanelMode`. The draft spells it
  `*(s32 *)((u8 *)&fldGlobalBase + 0x90)` rather than naming the field, since
  nothing compiled uses it yet.
- `D_0054EEA4` is the fade counter, 0.0 to 1.0 (`gFldAreaNameCnt` in the HD
  source), and `fldAreaFontWork` is its direction, -1, 0 or 1
  (`fldDrawAreaBlockFont_pm_mode` there).
- Mode 0 and 1 snap the counter to 0.0 and 1.0; 2 and 3 step it by 0.1 toward
  0.0 and 1.0, clamping and zeroing the direction at the end; 4 through 7 step
  it in whichever direction the counter already holds, but only while it is
  strictly between 0.01 and 0.99.
- The panel position is an interpolation whose endpoints are all zero except
  one: `x = (s32)(v * 0.0f + (1.0f - v) * 250.0f)` and
  `y = (s32)(v * 0.0f + (1.0f - v) * 0.0f)`, which is what shares the one
  `mul.s` by zero between the two. The HD source has the same expression with
  a 3.75 and a 2.25 resolution scale added.
- `fldGlobalBase.nameD` selects the place name, `D_0030D714` with
  `fldAutoMapFindPlace` and colour `0xF0DC6480`; `-1` there falls back to
  `nameU`, `D_0030AB54` with `fldAutoMapFindArea` and `0xA09DC380`. Both
  tables are `char [][28]`, already declared, and both are skipped when the
  matching `D_00567F50`/`D_00567EA0` length is below 2, read with an `(s16)`
  cast at the use site for retail's `lh`.
- The six `.lit4` entries from `D_0054C4D8` to `D_0054C4EC` are this
  function's, in the order 0.1, 0.1, 0.99, 0.01, 0.1, 0.1.

The `nameD` arm has to be written first (`if (nameD != -1)`), or gcc hoists
the `nameU` load above the test and emits the two arms in the other order,
which is 15 words worse. Case 3's clamp has to be written inverted,
`if (!(v > 1.0f)) { direction = 1; } else { v = 1.0f; direction = 0; }`, so
that the constant arm is the branch target; case 2 reads better the plain way
and measures the same.

What is left:

- The frame saves one callee-saved register too many, which is the whole
  2-word size difference. Retail keeps the font request handle and the string
  width in one register (`$17`): the handle is copied into the destroy call's
  argument and the register is immediately rewritten with `width + 8` in that
  call's delay slot. Two locals cannot reach that, and merging them into one
  `s32` with casts at the calls undershoots instead, 884 bytes with the width
  coalesced away entirely.
- Case 2 lacks retail's second `lwc1` of the counter and the `addiu $2, 0, -1`
  hoisted into the first `bc1f`'s delay slot. Retail reads the counter once per
  case and once more on the out-of-range path (in the range check's delay
  slot), which no placement of the read reproduces: before the switch, gcc
  common-subexpression-eliminates the per-case reads; after the switch, it
  replicates the load into all five predecessors instead of using the stored
  register, 20 bytes long; and an explicit `default: v = D_0054EEA4; break;`
  gets hoisted back out of the switch, 12 to 28 bytes long.

Measured and rejected: the two clamp arm orders in cases 2 and 3 (4
combinations), the two `nameD`/`nameU` arm orders, `&&` against nested `if`s
for the 0.99/0.01 range, three spellings of `x + 0x1EC - w`, the counter read
before, inside and after the switch, and the handle and width as one local
with casts.

### Ordinary sequence-start intermediate refinement

The retained `fldAutoMapSeqStart` draft now has 165 of 195 words matching at the
native 780-byte extent (previously 148). Splitting the same left-associated
Z-coordinate addition around the real up-vector copy restores its earlier
aggregate-copy position. No compiler constraint or artificial access is needed.
The remaining differences are floating-point allocation and closing store
scheduling; the assembly fallback stays in place and matching credit is unchanged.
All 36 matched peers are unchanged. Plain versus dump-enabled compiler output
agrees in both production and SKIP_ASM contexts; guarded production output is
byte-identical to baseline. Independent review and one million changed-block finite-float fixtures per
configuration pass at O0, O2 and with undefined-behavior sanitization. This tests
the changed arithmetic/copy block, not an emulator of the whole function.

### SeqStart live-zero allocation control (pass143)

The guarded draft now matches 172/195 words at the native 780-byte extent,
up from 165/195. This is **not an exact match** and retains its assembly fallback.
The positive-zero scalar is locally bound to `$f1` and constrained immediately
before its three real Y stores (field identifier, map position and target).
Retail materializes positive zero in `$f1` before those stores. No memory access,
float expression, callback, or additional consumer was introduced; the scalar
is initialized after the final callback and consumed before the vector copies.

Deletion controls at the same extent: binding without the live constraint stays
165/195; live constraint without binding reaches 166/195. Binding all three live
coordinates reaches 170/195 and is not retained. Ordinary scalar reuse of px/pz
adds four bytes; splitting the X addition reaches 161/195. Moving zero's assignment
and naming shared camera scalars do not improve the baseline. These controls do
not justify changing access types or inventing dependencies.

Residual differences now begin at +0x1F4: global-address registers, coordinate
arithmetic allocation and store scheduling still differ, as do the later light
constant registers. This is measured emitted-output progress, not proof of the
historical source or a finished decompilation.

### SeqStart field-ID address materialization (pass144)

The guarded draft advances from 172 to 175/195 words at the native 780 bytes by
materializing the real `fldAutoMapFieldId` address in a local, truthfully typed
pointer bound to `$3`. It supplies only the existing three field stores and
is never live across a call. This is a localized register-allocation aid, not
historical-source evidence. The same unbound pointer remains 172/195; an added
empty live constraint gives 174 and is omitted. Moving the assignment earlier
within the same closing block does not improve 175. All 36 matched peers remain
unchanged, and plain/dump-enabled assembly and object output agree in all three
compilation modes. The assembly fallback remains; this is not an exact match.
