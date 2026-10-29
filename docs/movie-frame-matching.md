# Movie frame sprite — 596 bytes

`release_movieobj2` at `0x002C5170` is one ordinary C packet constructor with two
instances of the shared SDK bit-length primitive. The zero-credit primitive
alignment is documented in [levers/asm-macros.md](levers/asm-macros.md); it is
not a target-only fixed-register helper.

## Real storage and semantics

The native allocator receives `0xB0`: four DMA/VIF/GIF header words and nine
A+D register writes. The local `sdfMovieDrawPacket_t` describes exactly that
packet, using the existing `sdfGifTag_t` and `sdfGrTagStdReg_t` records. Its
explicit DMA/VIF words follow the same full-header representation used by
matched `sdfPrim` constructors; no union alias or padded view is added.

The two UV/XYZ2 pairs have real source and destination rectangle endpoints.
The decoder's width and height are loaded as unsigned halfwords. Native draws
the stored viewport X/Y/width/height as signed halfwords; explicit `s16`
conversions interpret those existing 16-bit storage fields without changing
the shared movie record's declaration. Signed endpoint additions cannot
overflow s32 (each input is s16); shifting is performed after conversion to
u32, avoiding undefined signed-negative shifts. The final UV word converts
back through s32 before the u64 write, preserving native's sign-extended word
transport, including its unused upper bits. RGBAQ likewise contains the native
sign-extended `0x80808080` word, not a zero-extended guessed 64-bit value.

The two bit-length values use the real decoder dimensions minus one. For a
u16 dimension, that input is in `[-1, 65534]`; the native
`31 - (PLZCW & 0xFF)` result is in `[0, 16]` (zero and one both yield zero).
Thus the signed `widthLog2 << 26` cannot overflow. TEX0
retains native TCC, TW/TH, TBW forced to at least one, the resource base pointer
and the RGB-mode PSM selection. The GIF register sequence is TEX1, TEX0,
CLAMP, TEXA, RGBAQ, UV, XYZ2, UV, XYZ2. TEXA is `0x4000000080`, as proved by
native `0x40 << 32 | 0x80`, not the old draft's `0x4000000000000080`.

Playback below phase two draws nothing. PAC sound starts once when buffered
bytes reach `0x1000`, setting the real callback/owner and channel. SMT3 has no
DDS alternate frame-mode branch; that donor-only branch is not imported.
The packet is linked to a newly allocated draw tag and registered through the
actual ordering-table callback. Explicit local declarations follow the real
packet, callback-address and sound providers; existing integer-address APIs
receive explicit EE-width address conversions, not invented prototypes.

## Source credit and qualification

PiM adapted DDS1 `sdfMovieDrawFrame` from
`src/dds1/game/code_002E9708.c:1910` at Megami-Decomps/dds-decomp commit
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`, with the existing SMT3 draft and
native body as independent references. The existing SDK primitive is
`EE_MMI_BIT_LENGTH` in DDS `include/ee_mmi.h:441`; its canonical policy and
independent SMT3 texture uses justify the separate shared correction. Donor
source, prior matched bodies, shared record work and the primitive correction
receive no new function credit.

Final complete owners are movie 12 match / 0 differ and texture 39 / 0; the
primitive-only stage retains movie 11 / 0 and texture 39 / 0. Native data and
both retail images must retain SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`. Both guarded consumer owners are
compiled at the prerequisite stage. Final stock plain/`-da` whole-owner output
parity, all 25 tool scripts and full EE/IRX draft checks qualify the combined
source. No hooked decision trace is claimed. New credit is **one function,
596 bytes**.
