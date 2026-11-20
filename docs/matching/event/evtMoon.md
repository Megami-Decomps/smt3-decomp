# Matching notes: event/evtMoon

## evtMoonProcess2 (event/evtMoon, 0x119C)

All 1,127 words match at 4,508 bytes, including all six generated 16-entry
phase tables and their default edges. The owner's other fifteen functions and
the original read-only and small-data layout are unchanged.

The three no-argument gates are `fmProcessChk`, `fldChkVortex` and
`dds3NullDrawProc`. The process ID is used only by the subsequent data lookup.
The function updates and clamps `gMoonCnt`, then uses the dead-moon path if its
texture is present. Otherwise it computes the original interpolated coordinates.
The normal draw and animation code runs only with the second moon draw bit set.

The real work and phase reads stay on their respective sides of the draw
callbacks. In particular the optional paired overlay can replace the global
work pointer before the glow stage. Valid phases with no overlay still enter
the glow dispatch through its own phase load; an out-of-range overlay phase
instead goes straight to the shared alpha restore. The palette and glow calls
keep their phase-specific alpha, color, coordinates, scales and real
texture handles. The movement overlay retains its distinct timing, color and
geometry, and the tail reads and updates the four actual animation clocks
after callbacks.

The local compiler operands preserve native phases for initialized values with
real consumers, including the outgoing packed palette color, the real
overlay-to-glow pointer, sprite inputs and animation math. They emit no
instructions or memory barriers. Literal spellings represent the actual
retail float words under the pinned EE compiler; in particular the timer
threshold has bits `0x3e999999`.

The existing camera-name pointer's standalone skip-assembly check reports an
unresolved section-relative relocation before and after this match. In the
complete owner the original pointer still resolves to the same 16-byte camera
name at the same retail address. The full linked retail image verifies both
that pointer and the new function's code, tables and literal layout.
