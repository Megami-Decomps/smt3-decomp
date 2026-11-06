# Matching notes: kernel/dds3KernelDraw

## dds3DrawFade (kernel/dds3KernelDraw, 0x2A8)

A hundred and eighteen of 170 words match at the exact 680-byte size. The
52 differences are not all register names: the colour-component loads and
OR dependencies also differ. Native first loads blue at `fadergba + 2`, then
red at `+0`, then green at `+1`; the draft loads green, red, then blue. Native
builds `((shiftedAlpha | red) | (blueShift | q)) | greenShift`, while the draft
builds `((greenShift | shiftedAlpha) | (red | q)) | blueShift`.

Retail puts the frame id in `$5`, the `0x80 - fadergba[3]` difference in `$3`
and the packed colour in `$7`, and reuses `$5` for spill-address temporaries;
the draft uses `$7`, `$5`, `$6` and `$7` respectively. The eight spill slots
at `0x0` to `0x1C`, frame size, packet destinations and calls still agree.

Two things had to be found. The packet must be addressed as
`dds3FadePacket[frameId].field` at every store, not through a
`dds3FadePacket_t *` local: the pointer form folds the whole body into
displacements off one base and emits 105 words instead of 170, where retail
computes `(frameId * 0xB0 + offset) + base` per field and spills eight of those
addresses. The retained flat colour expression reaches the 118-word match,
but does not reproduce native's OR tree. Historical explicit-parenthesis and
five other flat-order controls were 57 to 120 words different; they are not
new leads and were not repeated in the current qualification.

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

### Qualified colour data flow and signed packing (2026-11-06)

Fresh complete-owner output confirms the 118/170 baseline. Its initial RTL
already groups green with shifted alpha (instructions 40/42/43/44), red with
Q (48/50/51), and adds blue last (56/58/59). Thus the differing colour tree is
present at expansion; it is not merely a final scheduling or register-renaming
residual. The Q operand is the integer bit pattern `0x3F80000000000000`, not a
floating-point operation. This is measured compiler output, not a claim about
historical source spelling or the internal cause of reassociation.

The retained draft now shifts alpha through `u32`, converts that low word to
`s32`, then widens to `u64`. Native `sll` at `0x00105340` sign-extends its
32-bit result before the 64-bit ORs. The canonical source is `u8 fadergba[4]`;
`dds3FadeSet` stores its argument without a clamp. Across all 256 stored bytes,
alpha spans -127..128 and the existing guard skips only 128. Negative values
must therefore retain native's sign-extended high word rather than invoke a
negative signed-left-shift or zero-extend the packed alpha. Provider types,
reads, guard, calls and packet stores are unchanged. The unsigned-to-signed
narrowing is target-specific and verified with the configured compiler; this
is not a claim of portable signed-conversion behavior.

Original and repaired complete assembly and entire ELF objects are identical
in production, SKIP_ASM and NON_MATCHING+SKIP_ASM contexts, preserving all 87
peers, owned sections, symbols and relocations. The active ordinary comparison
also remains byte-identical and nonmatching. Twelve strict builds pass without
diagnostics. All 256 input bytes and 783,360 colour lane/corner combinations
agree with an independent native SLL/sign-extension model under host UBSan;
this supplements, rather than replaces, the native instruction review.

Eighteen complete-owner contexts independently qualify plain/dump and
wrapper/direct assembly/object parity with frozen actual-runtime inputs.
A bounded alpha binding changes instruction placement, and a colour binding
with a real live operand gives 117/170 words at 680 bytes. Neither is retained.
The ordinary colour-dependency and allocation residual remains; no new match
credit or changed compiler flags are claimed.
