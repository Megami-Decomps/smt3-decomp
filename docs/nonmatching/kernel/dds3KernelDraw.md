# Non-matching drafts: kernel/dds3KernelDraw

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## dds3InitZClearDrawTag (kernel/dds3KernelDraw, 0x15C)

Sixty-six of 87 words match, at the exact 348-byte size, and every
instruction's opcode and offset is right: all 21 differing words are register
names. Retail puts the two XYZ2 payloads in the callee-saved pair,
`$17 = 0x79007000` and `$16 = 0x87009000`, and the TEST_1 and the second
GIFtag in temporaries, `$25 = 0x32001` and `$24 = 0x1003400000000004`; the
draft swaps those two pairs. The same swap also puts the address induction
variable in `$6` and the constant 5 in `$5` where retail has them the other
way round. Nothing in the loop body, the two counters, the `sll $2, state, 5`
addressing or the constant materialisation sequences differs.

The shape is settled: a `for (state = 0; state < 3; state++)` over an inner
`for (buffer = 0; buffer < 2; buffer++)` whose index the body does not use, so
it becomes retail's `addiu -1` / `bgez` downcounter, with the packet addressed
as `dds3ZClearDrawTag[state][buffer]` written out at each field. Indexing the
element inside the inner loop is what gives retail's `sll $2, $5, 5` with the
outer induction variable stepping 0xB; a row pointer hoisted to the outer loop
(`packet = dds3ZClearDrawTag[state]` plus `packet++`) makes gcc step a single
biv by 352 instead and costs two words as well.

Ruled out, all 21 or worse: both orders of `.data` and `.reg` within every
A+D register entry, independently for the TEST, colour, position and blend
pairs (16 combinations) - the scheduler normalises the position pair either
way, so the emitted order is not evidence of source order there; a
`dds3ZClearPacket_t *packet` local assigned inside the inner loop, assigned as
its initializer, and as `dds3ZClearDrawTag[state] + buffer`; a row pointer
with `row[buffer].field`; `(u8 *)dds3ZClearDrawTag + state * 0x160` and the
same through a `u32 ofs` local; a 32-byte `sdfGrPacketHeader_t *` cast indexed
`[state * 11]`; `u32` for either loop counter (either one costs a word);
`while` loops; one combined declaration; an explicit `buffer = 1; buffer >= 0`
downcounter; and seven orderings of the eleven field groups. Writing the two
position entries before the colour entries reaches 19 words and fixes the
saved-register pair, but reorders the stores in the body, so it trades six
register names for six store offsets. Two `u64` locals holding the position
payloads, declared before the loop with and without initializers, is 29.

The remaining difference is an allocation tie: every one of these constants is
hoisted to the preheader, referenced once per iteration and live across the
whole loop, so `allocno_compare` cannot separate them on refs, frequency or
live length. Someone attacking this needs to change the order the preheader
pseudos are created in while leaving the body's store order alone.

## dds3DrawFade (kernel/dds3KernelDraw, 0x2A8)

A hundred and eighteen of 170 words match, at the exact 680-byte size, with
every instruction's opcode, operand shape and position identical: all 52
differing words are register names. Retail puts the frame id in `$5`, the
`0x80 - fadergba[3]` difference in `$3` and the packed colour in `$7`, and
reuses `$5` for the address temporary each spill is computed in; the draft
gives the frame id `$7`, the difference `$5`, the colour `$6` and the address
temporary `$7`. The eight stack spill slots at `0x0` to `0x1C`, the frame, the
twenty-six packet stores and the three calls all land where retail has them.

Two things had to be found. The packet must be addressed as
`dds3FadePacket[frameId].field` at every store, not through a
`dds3FadePacket_t *` local: the pointer form folds the whole body into
displacements off one base and emits 105 words instead of 170, where retail
computes `(frameId * 0xB0 + offset) + base` per field and spills eight of those
addresses. And the colour's or-tree is `((alpha << 24) | r) | ((b << 16) | q)
| (g << 8)`, which only the flat source order
`0x3F80000000000000 | (g << 8) | r | (b << 16) | (u64)(alpha << 24)` reproduces;
the explicitly parenthesised form that spells retail's tree out is 120 words
out, and the five other flat orders tried are 57 to 120.

The packet is a unit-local `dds3FadePacket_t`: a DMA tag, a GIFtag, four A+D
registers (`0x42` = 1, `0x49` = 0, `0x4A` = 0, `0x47` = 0x31003), a second
GIFtag and four more (RGBAQ, XYZ2, RGBAQ, XYZ2), 0xB0 bytes, two entries. The
alpha byte and the three colour bytes are `fadergba[3]` down to `fadergba[0]`:
retail addresses all four off one base register holding `&fadergba[3]`, which
is what reading `fadergba[3]` first produces.

Ruled out, all 52 or worse: nine declaration orders and the `u32`/`s32`
spellings of the frame id (identical at 52, except `u32 alpha`, which is 149);
the guard as an early `return`, as `(0x80 - fadergba[3]) < 0x80` with the
difference recomputed inside, and with the shift written out at its use; and
computing the difference before the `dds3GetFrameID` call, which costs a word.
Both orders of `.data` and `.reg` within each A+D entry were measured: `.reg`
first is the 52, `.data` first is 151.

The draft depends on `dds3FadePacket_t`, defined in the unit beside the
`fadergba` externs, so a harvest of the body alone loses the type.

## dds3SceneLightUpdateFirst (kernel/dds3KernelDraw, 0x5FC)

A hundred and forty-five of 383 words match, at the exact 1532-byte size. The
frame, the nine saved registers and their slots, the seven 16-byte colour
carriers at `0x0` to `0x60`, every VU0 block, both calls, all six `div.s`
groups and the whole control flow land where retail has them. What is left is
two things, and they interact.

The function is a three-light loop (not four: `slti $2, $30, 3` with the
counter incremented in the body) followed by four flag-gated blends and the
soften/bias pair. The loop needs a running mask, `for (i = 0, colorFlag = 4;
i < 3; i++, colorFlag <<= 2)` with the direction flag written as
`colorFlag << 1` at each of its two uses: `4 << (i * 2)` gives `sllv` from a
constant in a register and loses retail's `sll $18, $18, 2` at the loop
bottom, and a named `dirFlag` local computes the shift once where retail
computes it twice. Nothing inside the loop may clobber memory. `dds3DrawFlag`
lives in `$7` across the whole loop and is reloaded only after `Lerp` and
`ToMatrix`, so both unpacks are `MMI_RGBA_UNPACK_NOMEM` and both stores are
`SDF_VU0_STOREVECTOR_NOMEM`; a `"memory"` clobber on any one of them adds a
`lw` reload per clear and costs 120-odd words (measured: unpacks with the
clobber 360 of 381, the colour store with it 360 of 379).

First remaining difference: the five saved-register pointers are one rotation
off. Retail assigns `$19` the `dds3LightParam` base, `$20` the word index,
`$21` `D_00557270`, `$22` `D_00557240` and `$23` the `0x3C000000` unit; the
draft gives `$19` the word index, `$20` the base, `$21` the unit, `$22` and
`$23` the two vector tables. The cause is one instruction: retail reloads
`lw $2, 0($19)` before the colour `sqc2` as well as at the loop top and in the
direction block, three references, where the draft CSEs the loop-top load into
`$8` and leaves the base with two. With three the base outranks the index in
`allocno_compare` and the unit falls to last, which is retail's order. Nothing
tried broke that CSE without also clobbering memory and losing `$7`: a
`sdf3DLight_t *` local for the null test (identical at 238), `u32 i` (239),
`i != 3` (241), and the store written through the subscript, which is what the
draft already does.

Second remaining difference, and the larger half of the 238: in the ambient
and fog-colour blocks retail materialises the `1/128` unit as
`lui 0x3C00` + `mtc1` + `mfc1`, four times, as ordinary schedulable insns
interleaved with the ratio divide, so the float constant is sitting in an FP
register and cc1 moves it to a GPR for the `qmtc2`. A float literal passed to
the macro's `"r"` operand folds to a single `lui` instead, because cc1's
`movsf` has a GPR-from-constant alternative and `regclass` prefers
`GENERAL_REGS` when the only other reference is an `"r"` asm operand. The
draft spells it `PS2_FLOAT_BITS(unit, 1.0f / 128.0f)` before each unpack,
which produces the right three instructions but as a volatile asm, so the
`mfc1` cannot be scheduled before the `div.s` and the carrier address cannot
be scheduled before it: the ambient and fog blocks come out shifted by two
positions, which is why they are almost entirely differing words. Adding a
dummy `"f"` input beside the `"r"` one does flip `regclass` to `FP_REGS` and
reproduces retail's schedule exactly in the ambient block, but cc1 then CSEs
the two constant loads inside each block into one FP register and the function
comes out four words short at 1512 bytes. `"+f"` on a per-invocation local
recovers two of the four (1516) by forcing a reload each time, but moves the
GPR copy instead of the constant. Whoever picks this up needs the spelling
that puts an SFmode constant in an FP register *and* leaves the two loads
uncommoned; that single fact is worth about ninety words.

Ruled out at 238 or worse: `1.0f / 128.0f`, `0.0078125f` and
`(f32)(1.0 / 128.0)` as the ambient and fog unit (all fold to one `lui`, 376
words); the same as a `double` (378, `li` + `dsll32`); an `f32` local assigned
the constant (cc1 propagates it); a `static` helper taking `(u32 *, f32)`
(1452, not inlined the way the macro is); the integer `0x3C000000` in the
ambient and fog blocks as well as the loop (1516 with the clobber, 1496
without); all six orders of the ratio, carrier and unit statements at the head
of the ambient and fog blocks (238, 240, or 1540 when the unit goes first);
both orders of the colour carrier and the ratio in the loop (238); both orders
of the soften and bias stores (238 and 247); hoisting `1.0f - ratio` into a
local in the soften block (238); and spelling the counter step as
`u16 n = now + 1; now = n;` instead of `(f32)(u16)++now` (243).

The draft depends on one new symbol only in its discarded form: addressing the
fog colour word as `dds3LightParam.fog.color` costs an extra `addiu`, and
`PS2_SYMBOL_AT(u32, ..., dds3LightParam, 0x30)` removes it, but that variant
measured 245 and is not what is kept. The kept draft adds no types and no
macros; it does need `lightcolornow`, `lightcolormax`, `lightdirnow` and
`lightdirmax` declared `u16` rather than `s16`, since every read here is `lhu`
followed by `andi 0xFFFF`.
