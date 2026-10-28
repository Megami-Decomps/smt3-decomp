# Non-matching drafts: kernel/dds3KernelProcess

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## dds3DrawProcess (kernel/dds3KernelProcess, 0x368)

157 of 218 words match, at the exact 872-byte size.

The frame prologue, the eleven `sdfModelDrawEnvLinkOt` calls over
`D_002F69B8` in steps of four `sdfGrOt_t *` against `dds3DrawEnv[frame]`'s
model environments in steps of 0x1B0, the `sdfGrOtClose` tail with the
`dds3DrawProcessFlag` flush and the `dds3DrawFlag & 0x2000000` arm all
reproduce, as do both 0x40 blur packets written as eight `u64` words and the
sprite primitive written as `s32` words with two `u16` zeroes at 0x2C and
0x4C (0x4C has to be written first in source).

What does not reproduce: retail keeps each `sdfGrAllocPacketBuff(0x40)`
result in `$2` and stores all eight words through it, then copies it to `$5`
for `sdfDrawTagLink`; ee-gcc moves it to `$8` first and stores through that,
which shifts the whole packet block. Retail also materialises the constant 3
once in `$19` where the draft keeps a second copy in `$20`, and its blur
scaling loop is `addiu $20, 1` against `bne $20, $7` with a walking pointer,
where the draft's `blur[i]` subscript turns into a countdown.

Ruled out: `dds3ScreenBlurOfsX` as an `extern s32 []`, sized or unsized, and
indexed `[0]`/`[1]` (16 bytes long; only `s32 *ofs = &dds3ScreenBlurOfsX;`
with `ofs[0]`/`ofs[1]` gets retail's `addiu $3, $28, %gp_rel` plus two `lw`
without growing); `i != 4` on the blur loop, with or without an explicit
walking pointer, both of which come out 16 bytes long; and two separate
pointer variables for the two packets, which is 117 words differing instead
of 61.

## dds3DrawFirstProcess (kernel/dds3KernelProcess, 0x58C)

**Reference implementation, not a match candidate.** Assembly remains production.

The guarded C records the render-texture, secondary-view and scene passes using
`dds3DrawBank_t`, the existing light/fog records and the VU0 vector/matrix macros.
It preserves the second render-texture query after the lighting callback, the
600-unit target construction and quake offset, the camera packet handoff, each
fog packet's lifetime, and the final default-light/default-fog installation.

The model ordering-table slots are initialized in native order: 0, field tables,
7, 8, 1 through 5, 9 and 10. The three Z-clear tags reach ordering tables 69,
80 and 82 through their freshly loaded callbacks. Frame selection, provider
signatures, draw-bank offsets and the two aligned VU-only local vectors were
checked against the native body and the matched providers.

The emitted address calculations and register lifetimes differ substantially.
Separating the three real packet-pair lifetimes restores the native frame size,
but does not reproduce the body. A matching follow-up needs evidence for the
native phase-local bank/address lifetimes; this source is retained to explain
the rendering flow rather than to suggest that the remaining difference is small.
