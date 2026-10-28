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

`dds3DrawFirstProcess` in the same unit is still `INCLUDE_ASM` and untouched.
