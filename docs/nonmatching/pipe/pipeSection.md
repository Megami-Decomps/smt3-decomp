# Non-matching drafts: pipe/pipeSection

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## ActionZMotion (pipe/pipeSection, 0x140)

74 of 80 words match at the native 320-byte size. Production remains
assembly-backed.

The draft snapshots the original count, clamps only its upper blend limit,
interpolates the two position components, and decrements a positive original
count. Negative input bypasses both interpolation and the post-decrement zero
clamp. The scale window uses the clamped pre-decrement count and inclusive
endpoints. Exact binary32 literals preserve the non-integer boundary and the
slightly sub-0.2 additive term.

All six residual words concern the order of independent complementary/count
float conversions and their first products. Swapping cast statements, separating
the blend and scale locals, or naming the weighted data component leaves the
same result. A one-pass conversion block also changes constant hoisting and
was discarded.

The single post-call local reset follows the matching-rule exception and the
matched ObjectDraw pattern in this unit. Qualified candidate/control traces
show reset187 after sibling selection and absent in life13; removing it gives
a tail call and twelve extra bytes. Hooked/unhooked and dump/plain outputs
agree. No extra loop block or dummy copies are retained.
