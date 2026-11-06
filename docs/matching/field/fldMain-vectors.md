# Matching notes: field position vectors

## fldAnaStart and fldWaveStart (field/fldMain, byte-neutral)

These callers now construct their positions as the existing `sdfVector4_t`
type accepted by `dds3SetPosPoseSet`, using its `x`, `y`, `z`, and `w` members.
The canonical provider copies the full 16-byte vector into the object's pose.
The former four-float arrays represented the same payload but did not satisfy
that pointer contract.

Each caller still clears 16 bytes, sets `w` to 1, and assigns all three position
components before passing the vector's address. The existing file checks,
wave-buffer selection, calls, and early returns are unchanged. There is no
cast, new type, prototype change, added alignment, or additional operation.

Complete production, SKIP_ASM, and SKIP_ASM+NON_MATCHING compilation contexts retain
identical assembly and object output. Both pointer warnings disappear in all
three contexts, and the stricter retained-C compile passes. Raw strict
production and SKIP_ASM compiles still report the pre-existing implicit
`memset`, `strcpy`, and `memcpy` declarations. The ordinary retained-C gate
already passed before this repair. Complete caller and provider checks and
the full retail build remain required. No additional function receives
matching credit, and the unresolved `fldAnahoriSelectInit` draft and assembly fallback are unchanged.
