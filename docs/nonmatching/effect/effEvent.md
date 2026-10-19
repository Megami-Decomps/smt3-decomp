# Non-matching drafts: effect/effEvent

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## effEveBEDCutDraw (effect/effEvent, 0x1DC)

117 of 119 words match, at the exact 476-byte size. The two differing words are
adjacent and hold the same two instructions in the other order: retail puts
`addiu $2, $0, 5` in the `beqz`'s delay slot and `lbu $3, 0x8($18)` after it,
the draft the reverse. The registers, both operands and every other word are
identical, so what is left is which of two independent instructions the
delay-slot filler takes.

The two are coupled and pull against each other. Reading the compared byte into
a local *above* the `works[0] != NULL` test gives retail's registers (the
constant in `$2`, reusing the dead pointer's register, and the byte in `$3`) and
the wrong order, because the `lbu` is then the last instruction before the
branch and the filler takes it. Reading it *below* the test gives retail's order
and swaps the registers, since the byte's pseudo is then created first and wins
`$2`. The local's type does not matter: `u8`, `s8`, `u16`, `u32` and `s32` all
give the same two spellings.

Ruled out, all 3 of 119 or worse: `5 != kind` and `(u8)5 != kind`;
`!(kind == 5)`; `kind - 5 != 0`; the assignment inside the condition, either
operand order; a `markKind = 5` local, above and below the test, which gcc
hoists out of the loop; an `aim = &entry->aimParams` pointer local (20 words); a
`dds3EffectPart_t *part` local for the tested slot; `*works` for `works[0]`;
reading the byte at the top of the loop body, before the `zeroKindCount` test
(78 words); a `goto` out of the `works[0]` test and a `goto` into the mark arm;
and both switch shapings of the dispatch, `default` first with a `goto` back
(78 words) and `case 5` falling into `default` (41 words).

What the draft establishes, since the rest is exact:

- `D_005343D0` is a quaternion `Mult` combines with the record's own
  `init.orientation` quadword at +0x10, as `D_00532EA0` is in the sibling
  `effBTLCutDraw`. The frame is `sdfVector4_t position` at 0, `sdfMatrix4_t
  base` at 0x10 and `sdfMatrix4_t marked` at 0x50, in that declaration order.
- `effEveBEDCut_t.actor` is an `evtUnit_t *`: the mark arm reads
  `work->actor->owner->inner` and hands it to `sdfModelGetMarkPos` and
  `sdfModelGetMarkPosMatrix`, which pins +0x8C as `evtUnit_t.owner` and +0x18 as
  `mdlObj_t.inner`. The draft needs `include/event.h` and `include/model.h`,
  which the unit did not include before.
- The mark arm is the `else`, not the `if`: written `scaleKind == 5 &&
  actor != NULL` with the mark work in the `then` arm, gcc lays the mark block
  out first and branches over it, where retail falls into the generic block.
  Inverted to `scaleKind != 5 || actor == NULL` it is laid out retail's way,
  which alone moved 30 words.
- The tail is `cut->currentFrame++` with no `effBTLCutSetMax`, and the
  trigger-frame callback is tested as `cut->triggerFrame == frame &&
  cut->triggerFrameCallback != NULL` against the frame read at entry, with
  `currentFrame` re-read afterwards.

The draft depends on `SDF_VU0_STOREMATRIX_RWADDR`, added to `include/ps2asm.h`
for it. The plain and `_NOMEM` matrix stores both share one address pseudo with
the following call's `&marked` argument; that pseudo is loop invariant, gets
hoisted into the preheader, takes the last callee-saved register and pushes
`color` into a stack slot, which is 71 of 119 words and a 16-byte larger frame.
With the address as a read-write asm operand cc1 gives the asm its own copy, so
both uses are rematerialised as retail's two `addiu rd, $29, 0x50`, and the
frame and every register fall into place. `SDF_VU0_STOREMATRIX_MEMOPS` also
frees the register but addresses the frame directly, so the four `sqc2` and the
whole tail shift by one word (46 of 118). A `sdfMatrix4_t *mp = &marked;` local
with either existing macro does not reach it: cse folds the local back into the
shared pseudo.
## effEVECombParSmokeDraw (effect/effEvent, 0xB34)

493 of 711 words differ at 2844 of retail's 2868 bytes, so the draft is six
words short. That positional count overstates the distance: aligned against the
retail listing instruction by instruction, 258 of 718 instructions differ, and
about 100 of those are the same instruction with one register name changed. The
whole prologue, the packet and transform setup, the particle update, the spill
slot assignment and the frame size are exact; what is left is one float
register rotation, the instruction order inside the two vertex blocks, and four
branch annul bits.

This is the batched sibling of `eff_1AEC58`, which matches. Everything from the
`aViewTarget`/`aViewEye`/`aViewDown` cross product down to the age and repeat
tail is the same code, and that half of the function transferred unchanged from
`eff_1AEC58`'s source, including the growth factor, the three leading reads and
the spill slots `repeat` at `0x880` and the particle count at `0x888`. The new
half writes two 0x400-byte `sdfBillBoardBatch_t`-shaped stack batches instead
of calling the `dds3Billboard*` setters, and links them through
`sdfGrOtTable[48]` and `[49]`.

What the draft establishes:

- The two batches are stack aggregates at `0x0` and `0x400` with the layout
  `include/billboard.h` already declares for `sdfBillBoardBatch_t`: centres at
  `+0x0`, colours at `+0xF0`, texture coordinates at `+0x12C`, corner offsets
  at `+0x21C`, count at `+0x3FC`. The frame follows declaration order: the two
  batches, the identity matrix at `0x800`, then `offset`, `origin` and `side`
  at `0x840`, `0x850` and `0x860`, then thirteen four-byte reload spill slots
  packed from `0x870`.
- `effEventBillParams_t + 0x74` and `+ 0x78` (`colorStartAge`,
  `colorTransitionFrames`) are only read here. They select the primary
  texture's colour in three arms: below `colorStartAge` the blend base is
  `color & 0xFF000000`, within the next `colorTransitionFrames` it is
  `effColorBlend(color & 0xFF000000, color, rise)` with `rise` the clamped
  fraction, and after that it is `color` itself. Retail's `daddu $4, $0, $0` in
  the first arm is `(color & 0xFF000000) & 0xFFFFFF` constant-folded, which is
  what proves all three arms end in the same
  `effColorBlend(base & 0xFFFFFF, base, ratio)` written out in full.
- The texture coordinates come from the handle's `width` and `height` through
  the same idiom `BillboardLoadTex` in `effect/billManager` uses,
  `(s16)((u16)tex->width << 4)`, and the half extent is
  `(uvWidth >> 5) * 0.0625f` scaled by the quad's own scale. Retail's shared
  `sll 20` with an `sra 21` and an `sra 16` off it is that pair of expressions
  and nothing else.
- The GS packet written after the loop is eight `sd` of a 0x40-byte buffer: a
  DMA tag carrying two VIF codes, a GIF tag with `NLOOP` 2 and one `A+D`
  register, then `TEST_1` (0x51801 or 0x31801 on `work->flag`) and `ALPHA_1`,
  0x44 for the first ordering table and 0x42 for the second.

Two levers found here, both already added to
[Matching levers](matching-levers.md):

- A run of stores that each repeat the whole subscript needs the row to be one
  flat array, not an array of two-field structs. Written
  `batch.uv[n][k].u`/`.v` with `UV_t`, gcc builds two base pseudos, `$29 + 0xC`
  for the `.u` stores and `$29 + 0xE` for the `.v` stores, cannot share either,
  and materialises a full address per store with a zero displacement. Declared
  `s16 uv[15][8]` and written `batch.uv[n][k]`, it builds the single
  `$29 + 0xC` base retail has, with the `0x120` to `0x12E` displacements and
  the seven `daddu rd, rs, $0` copies that give each `sh` its own register.
  That one change was worth 30 words and both base registers.
- The index of such a run belongs in a local read once, `n = batch.count;`, not
  in the subscript. `batch.uv[batch.count][k]` re-reads the counter at every
  one of the three scaled forms (`sll 2`, `sll 4`, `sll 5`) where retail reads
  it once into `$19` and shifts that; the increment afterwards still re-reads
  the field. One local reused for both batches is what gives retail's `$19`.

Ruled out:

- The float register rotation is the largest single residual and nothing
  reached it. Retail gives `$f20` to the fade ratio and `$f21`/`$f22` to the
  two scales; the draft gives `$f20`/`$f21` to the scales and `$f22` to the
  ratio, and every later float register is already correct. All six
  declaration orders of `ratio`, `scaleX` and `scaleY` emit byte-identical
  code, as does making the ratio local do double duty as the cosine (the
  registers rotate together and it costs one word). Four spellings of the
  scale pair (the base scale read twice, the two assignments swapped, a `+=`
  for the second) are identical as well.
- Two pointer arrays, `sdfDrawTag_t *tag[2]` and `sdfTexHandle_t *tex[2]`, for
  the four pointers retail holds in the adjacent slots `0x870` to `0x87C`.
  gcc rounds each stack aggregate up to 16 bytes, so they land at `0x870` and
  `0x880` with an eight-byte hole and the frame grows by 16. They are ordinary
  reload spill slots: four plain locals give the packed layout once the rest of
  the register pressure is right.
- Deriving the lower ordering table entry from the upper one, `(&ot2)[-1]`,
  for retail's `addiu $4, $16, -0x20`. Two `PS2_SYMBOL_AT` aliases and that
  spelling emit the same code; gcc shares one `%hi` across both entries either
  way, where retail materialises the first entry's address on its own and
  derives it from the second only at the end.

The draft depends on three macros added to `include/ps2asm.h` for it,
`SDF_VU0_LOADVECTOR_RWADDR`, `SDF_VU0_STOREVECTOR_RWADDR` and
`SDF_VU0_COPYVECTOR_RWSRC`, and on the `effEveBillBatch_t` type declared in the
unit above the function. The four VU0 accesses to the `offset` vector inside
the loop share one address pseudo under the plain and `_NOMEM` forms; that
pseudo is loop invariant, is hoisted into the preheader as
`addiu $22, $29, 0x840`, takes a callee-saved register and pushes the second
draw tag out of its spill slot, which is 524 of 706 words and a 16-byte larger
frame. With the address as a read-write asm operand, passed through a pointer
local as `effEveBEDCutDraw` does for `SDF_VU0_STOREMATRIX_RWADDR`, cc1 gives
each asm its own copy, retail's four separate `addiu rd, $29, 0x840` come back,
and the frame size, every saved register and all thirteen spill slots fall into
place.
