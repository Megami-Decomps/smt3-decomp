
## fldPlayerMotionSe (field/fldPlayer, 0x5B0)

363 of 364 instruction words match at the native 1,456-byte size. Production
remains assembly-backed. The single residual is at +0xF8: C emits `bne`, retail
`bnel`; both use the same decrement in the delay slot and select the same
surface sound category. Reversing the source arms or using a conditional
expression leaves that residual. A separate override snapshot is also inert;
updating the category snapshot in place changes three register words. Loading
the default category before testing the override changes fifteen words.

The body snapshots the current model motion frame and the previous field frame,
then updates the field frame before dispatch. Old motion 13/11 has independent
15/20-frame crossing sounds. Other motions select both surface sound bases,
calculate distance-clamped or randomized volume, and retain random-call order,
the direct base+3 transition, separate left/right variants and crossing gates.
The distance is the square root of three squared coordinate differences, minus
500, clamped to 0..7000. The two late motion variants re-roll their volume.

The final volume reset is the single permitted post-call statement for the
shared sound-dispatch tail. Native uses ordinary calls plus an epilogue there,
but tail calls for the two early special sounds. Removing the reset generates
1,820 bytes with duplicated sibling-call epilogues; keeping it emits the native
size. A qualified `cc1probe --reorg` run on this source shows reset instruction
1052 (volume pseudo 98) after sibling selection in `rtl.01.sibling`, and absent
from `rtl.13.life`. The early special sound calls still select sibling calls.
Hooked/unhooked assembly and all dumps agree, and dumped/plain output parity
passes with compiler SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.

The same qualified trace diagnoses the remaining branch: the category decrement
is available before the branch and is accepted for its ordinary delay slot.
It does not establish a source property that requires the native annulled slot;
no branch-forcing exception or compiler option is used.
