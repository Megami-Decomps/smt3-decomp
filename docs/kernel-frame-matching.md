# Kernel frame draw process

`dds3DrawProcess` at `0x00102448` is exact ordinary C (872 bytes). The source
adapts Megami-Decomps/dds-decomp's `kwlnRenderFrame` in
`src/dds1/game/code_00101B08.c` and the two-corner payload from
`include/kwln_sprite.h`, at commit
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`. The donor worktree differs in this
unit only by a trailing blank line; its function and payload header are the
committed donor, not a new SMT3 contribution.

## Native and provider contracts

- SMT3 calls `dds3DrawDistRipOverError`, `dds3FadeUpdate`, `dds3DrawFade`, and
  `dds3BlurUpdate`. DDS's two extra background-fade calls are not imported.
- SMT3 visits eleven model groups, not DDS's thirteen: the native loop starts
  at ten, steps the four-pointer `sdfModelOt_t` by `0x10`, and steps
  `sdfModelDrawEnv_t` by `0x1B0`. `sdfModelDrawEnvLinkOt` in `sdf/sdfModel.c`
  consumes exactly those four OT pointers. The existing `dds3DrawBank_t`
  supplies stride `0x1BE0` and the real `stdEnv` at `+0x1300`.
- Each 64-byte state packet contains a DMA header, a VIF FLUSHA/DIRECT pair,
  a GIF A+D tag, and two A+D register writes. The split shifts/or operations
  constructing VIF commands also appear in the matched provider
  `sdfGrLinkDispFrameTex` (`sdf/sdfDraw.c`). No opaque instruction surrogate
  or artificial constant consumer is introduced.
- `sdf2DAllocSprite(1)` allocates a five-register GIF primitive and specifies
  `0x53531`: RGBAQ, UV, XYZ2, UV, XYZ2. `sdf2DGetPrimPtr` returns the payload
  after the 32-byte packet header. This proves the local 80-byte payload and
  its two 32-byte corner records. Native stores RGBA at `+0..+0xC`, UV at
  `+0x10/+0x14` and `+0x30/+0x34`, XYZ at `+0x20/+0x24/+0x28` and
  `+0x40/+0x44/+0x48`, and clears the 16-bit ADC lanes at `+0x2C/+0x4C`.
  Unwritten lanes are real ignored GS payload space, not frame filler.
- `dds3ScreenBlurOfsX[2]` follows the already matched adjacent X/Y read/write
  contract in `interface/itfTestMesManager.c`. Four actual edge distances
  are scaled by signed Q12 before converting X/Y to their respective 16/8
  GS coordinate units. The donor's ordinary increasing four-element loop
  now reproduces the native loop without a fabricated pointer alias.
- The closing draw tag is stored in `dds3ClosedDrawTag`. The optional flush
  uses its real `head` address; its declaration agrees with the matched
  `sdfGrFlushOtChain(u32 *)` provider. The one-shot skip bit is `0x02000000`,
  and `sdfGrReqDraw` receives the actual pre-draw chain `D_002F6A70`.

The old 61-word residual used a flattened sprite and different state-packet
source. This donor translation matches all 218 words on its first valid
compile. No scheduling search, flags, aliases, asm, or source-only compiler
constraints were added. The guarded `dds3DrawFirstProcess` draft is unchanged.

The separately qualified [null draw gate correction](null-draw-gate.md)
provides the truthful canonical `u32 dds3NullDrawProc(void)` signature used
here and by all existing C gates. Its unused process-ID parameter had no
native argument-read or registration evidence. That cleanup earns no new
matches and is a prerequisite, not part of the 872-byte implementation.

## Qualification

On SMT3 base `e9255dbba166bb86f8cf8de9792c9ef785deb055`, the complete owner
checks `4 match, 0 differ`. `ninja` reproduces retail SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`. No data ownership, header, compiler,
SDK mode, or other owner's source is changed.
