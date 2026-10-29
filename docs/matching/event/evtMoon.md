# Matching notes: event/evtMoon

## evtMoonProcess2 (event/evtMoon, 0x119C)

The reconstruction emits 3,984 bytes against 4,508 native bytes. In the emitted
prefix, 13 of 996 words match; 983 differ and 524 bytes remain uncovered.
The frame is 0xA0 instead of 0x90, and generated jump tables/read-only layout
also differ. This is a broad code-generation residual. Production retains
assembly; all 15 existing C functions stay exact.

The three no-argument gates are `fmProcessChk`, `fldChkVortex` and
`dds3NullDrawProc`. The process ID is used only by the subsequent data lookup.
Update and clamp `gMoonCnt`, then prefer `evtDeadMoonDraw` when its texture is
present. Otherwise compute the original float interpolation coordinates and
run the entire normal draw/animation block only while moon draw bit2 is set.
The flag-off path does not draw, decrement the move timer or advance phases.

All six 16-entry phase dispatches are preserved, with fresh phase reads across
provider calls. The optional paired overlays in the fourth dispatch are followed
by the fifth dispatch on every path, including phases that draw no overlay.
Do not nest that latter stage only inside the 5/11 case. The fifth dispatch uses
RGB 0x4A40FF, 0x6060B0, 0x7070A0 and 0x808090 for phases 0, 1/15, 2/14 and 3/13,
then 0x808080 for the other phases. Phase 7/9's second glow has final scale100,
while its preceding glow uses80. Other phase arguments follow their individual
native call sites. Phase values outside0..15 retain the native default behavior.

`evtDrawDeadMoonSprite` has twelve integer/pointer and five float parameters:
its first eight words use integer registers, color/texture/X/Y use stack slots
0/8/0x10/0x18, and the float bank is independent. `DrwSprite2D` has ten parameters,
with color and texture at stack0/8. Preserve those color arguments and unsigned
packed-alpha shifts. The final movement overlay uses RGB0xFFFFFF.

The tail updates four animation globals only after the normal draw sequence.
Their live reads and those of the phase/texture providers preserve callback
ordering. No guessed padding, extra phase clamps or invented initialization is
used. A fresh complete-owner promotion and full data qualification are required
before replacing assembly.
