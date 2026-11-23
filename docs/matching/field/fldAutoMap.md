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

Exact, 580 bytes (145 words). The function appends one textured icon quad.
It reserves a 0x20-byte tag, initializes it, links texture context 0, reads
the signed halfword lookup row, and calls `sdfPrimLinkFt4` with color
`0x80808080`, primitive mode `0x44`, and zero depth and allocator arguments.
The allocator parameter of the primitive is a callback; zero selects its
normal packet allocator.

The lookup occupies 0x100 bytes: 16 rows of eight halfwords. Entries
0/1 are the U/V origin, 2/3 the width/height, and 4/5 the anchor biases.
Both U and V coordinates use 16 times the stored halfword units. The
incoming screen coordinates are first multiplied by 16 and converted to
signed words; the X bias and extent scale by 16, and the Y bias and extent
scale by 8. Mode 4 swaps the axes of the anchor and extent and changes
the corner order; mode 8 flips the rows; other modes use the default
corner order and reload the geometry from the row. The coordinates use
32-bit wraparound, matching native `addu`/`subu`.

The known direct callers in `fldAutoMapDrawOneArea` pass icon-row choices
and coordinates calculated by `fldPoint3To2H`; the reconstruction
inherits the valid-row and representable float-to-word conversion domain.
The tail deliberately calls `fldDrawGetOt` twice. The first result is
the registration receiver, while the second supplies its function pointer.

The old compiler needs actual packet-request and row-offset operands to
retain the pre-allocation row scaling. The equivalent branch layout
retains the native first comparison and table address order. The three
geometry paths keep their actual loaded biases, converted coordinates,
and corner or extent values in the native live ranges. These constraints
emit no instructions and do not imply the retail source had the same
spelling.

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

226 of 226 words match at the native 904-byte size, with all eight
linked mode-table entries. The panel steps the area's name fade and
draws the place name or, if absent, the area name.

Modes 0 and 1 snap the alpha and clear the direction. Modes 2 and 3
step toward 0 and 1, respectively. Modes 4 through 7 continue the
current direction only strictly between 0.01 and 0.99. Direct case
reads and a default read retain the native alpha observations. The
real clamp tail resets the direction, writes alpha, then goes straight
to draw. The unclamped mode-2/3 tail instead stores the real direction
and shares the alpha publication. Keeping the two source paths separate
avoids the compiler's cross-jump into a combined publish and restores the
retail branch and delay-slot store. The two zero products in the
position interpolation are retained; the native fade state starts at
1.0 and remains in [0,1].

The text pointer is retained in its native callee-saved register.
The measured width replaces the already-consumed font request in a
union after passing the request to destruction. Width and the horizontal
offset use 32-bit modular arithmetic before conversion to the native
signed draw arguments. The two text paths preserve the width-precalc,
request, measure, destroy, panel and glyph call sequence and keep the
native place/area colors. The corrected font and glyph declarations use
the actual providers' pointer and return types. A prior empty float constraint
is no longer needed with the split clamp and publish paths.

All 38 functions in the owner match. The 13 object-level instruction
differences are their actual relocation immediates, not linked changes.
The full retail executable and the owner's allocated data bytes are
unchanged.

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

### SeqStart light-store ordering (pass145)

Writing the real light scale before its far field in the guarded draft advances
175 to 180/195 matching words at the native 780-byte extent. These are distinct
nonvolatile fields (+8 and +4), with no intervening callback or observer; retail
also stores scale before far. The ordinary source reorder removes all five
late-light register/store differences without adding a constraint or changing any
value, access width or consumer. The remaining 15 words concern the closing
coordinate/vector block. The assembly fallback remains and no exact credit is
claimed. Independent semantic review and randomized whole-storage fixtures pass.

A local `$f2` binding of the actual 200.0f Z offset then advances to 184/195.
The initialized constant has its sole real consumer in the existing Z addition,
with no intervening call; this is a localized reconstruction aid, not original
source evidence. The unbound scalar stays 180; adding an empty live constraint
regresses to 173, so none is added. Independent review confirms the native
0x43480000 constant and add.s lifetime. All 36 matched peers remain unchanged;
plain/dump assembly and object outputs agree in production, SKIP_ASM and
NON_MATCHING+SKIP_ASM modes. Eleven vector-block scheduling differences remain.

The following bounded pass reaches 186/195 through an ordinary reorder of the
map-position and field-ID three-store blocks. The distinct vectors retain the
same x/y/z accesses and values, with no intervening observer. The qualified
scheduler dump already put map-position stores ahead of field-ID stores; swapping
the source groups repairs two emitted store positions without new constraints.
Full owner checks preserve all 36 exact peers and three-mode assembly/object
parity passes. Nine scheduling words remain, starting at +0x234.

A subsequent local destination-operand constraint reaches 187/195, still at
780 bytes. A truthful `sdfVector4_t *eye` is initialized to `&fldAutoMapEye`
and constrained with an empty `+r` operand immediately before its existing
VU-copy consumer. No fixed register binding is needed and no value crosses a
call. This target-specific reconstruction aid leaves the native vector accesses,
including untouched W lanes, unchanged; it is not evidence of original source
spelling. The final target-Y and camera-X store positions now agree, while eight
address/zero/store scheduling words remain from +0x228 through +0x248. The
assembly fallback and matching credit remain unchanged. All 36 exact peers are
preserved, and production, SKIP_ASM and NON_MATCHING+SKIP_ASM each have identical
assembly and objects with and without compiler dumps. Ordinary pointer naming
alone stays at 186/195; fixed binding adds no benefit over the immediate operand.

### SeqStart ordinary field-ID pointer and dependency control (pass148)

With the current guarded draft, the ordinary `sdfVector4_t *fieldId` local now
produces exactly the same complete-owner assembly and object as the prior `$3`
binding. The binding is therefore removed. This supersedes the pass144 control
only for the current source: that older 172-word baseline did need the binding
for its measured gain. SeqStart remains 187/195 words at 780 bytes; it is not an
exact match. Production, guarded `SKIP_ASM`, and active
`NON_MATCHING`+`SKIP_ASM` outputs are individually unchanged, and each source/mode
pair has equal plain and dump-enabled assembly and objects.

The paired postreload dependency dumps also resolve the proposed alias-edge
control. The bound and ordinary-pointer variants have identical predecessor and
successor lists across all 164 instructions and 853 dependency edges. In
particular, each of the two field X/Z stores still has nine output-dependence
successors plus the anti-dependence to the first VU copy; camera address
materialization still has just that copy as its successor. The sole RTL-record
change is the field-address low instruction's `REG_EQUAL` note becoming
`REG_EQUIV`. Thus the dense store graph cannot be attributed to the explicit
register binding in this source context. These are parity-qualified dependency
dumps, not a fresh runtime priority/rank observation. Direct global field writes
instead of the pointer score 186/195 and are not adopted.

## Area title table

The area-title loader writes exactly 1,248 bytes, and the two rendering
consumers use a 26-byte stride. The writable C table preserves all 48
retail rows, including the spaces before some string terminators. It does
not add an index guard or change the native routines' supported area
assumptions. The previous interior labels were splitter boundaries, not
separately addressed objects. Text bytes that resemble addresses remain
literal, not relocations.

## NPC resource paths

The two existing NPC resource path arrays (288 and 576 bytes), followed
by the generic NPC path (24 bytes), comprise the final 888 bytes of the
original read-only area. These named path objects can match independently
of the earlier switch and vector constants. All paths and readers retain
their native bytes.

## Full-open diagnostic text

The assembly-backed `fldAutoMapDrawOneArea` passes this string to
`fldDbgPrint2D` when the full-open display diagnostic runs. The C array at
`0x005321C0` contains nine bytes including its terminator. The following seven
alignment bytes belong to the next 16-byte-aligned switch table placement,
not to the string. The existing function and its two string relocations
remain unchanged.

## Field model and blur slot storage

The native model cleanup visits 32 records at `0x567F90` in 0x70-byte
steps. It reads the effect pointer at offset 0x30 in each record. The
independent blur cleanup visits 64 four-byte handles at `0x568D90`
and passes each non-null handle to `sdfMemFreeDelayedDirect`.
Both arrays use the types already used by their consumers. Their
zero-fill storage ends at `0x568E90`, where `fldModelResPtr`
starts. The following resource and treasure storage remains raw.
The array definitions add no instructions or initialized data.

## Automap position storage

The native load routine writes both four-lane vectors at `0x567D00`
and `0x567D10`, then clears 32 model positions in 0xC-byte steps
starting at `0x567D20`. The C owner uses the same vector and XYZ
types as the consumers. Its 416-byte native zero-fill interval ends at
the untouched area-name lengths at `0x567EA0`. The preceding texture
handles and alignment remain separate, as do the following name lengths
and model slots. This does not add initialized data or code.

## Automap window texture handles

`fldAutoMapWindowTex` owns three texture-handle pointers at `0x567CF0`.
Automap initialization clears the three slots, resource loading fills them
from package entries 1, 3, and 5, and teardown queues each non-null handle
for release. The four bytes at `0x567CFC` remain in raw storage before
`fldAutoMapFieldId`. The sprite texture handles in small BSS are unchanged.

## Automap name-length storage

The three tables at `0x567EA0`, `0x567EF8`, and `0x567F50` hold the
area, floor, and place name lengths used by the automap. The native font
initializer writes 41, 41, and 24 halfwords, respectively. The definitions
keep the signedness of the existing declarations and the six alignment
bytes between each pair. The last 16 bytes before the model slots remain
raw. The three definitions add 212 bytes of typed zero-fill storage and
do not add code or initialized data.
