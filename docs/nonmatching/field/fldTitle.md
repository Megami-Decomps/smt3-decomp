# Non-matching drafts: field/fldTitle

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## fldTitle_Process (field/fldTitle, 0xBD4)

755 of 757 instruction words match at the native 3,028-byte size. Production
remains assembly-backed. The only residual is instruction order at +0x4C and
+0x50: native loads the case-one constant before the global phase, while C
loads the global phase first. The remaining 755 words match, including all
sprite arguments, callback-visible frame reloads and animation transitions.

The entry background shrinks vertically, followed by overlay fades, horizontal
strips moving at 56 units per frame, and the title's white and colored layers.
The fixed layout keeps its native interleaved upper/lower strip order. Exit
strips move at 72 units per frame; their lower displacement is formed before
negation. Drawing stages use separate color lifetimes. No padding, fixed
registers or artificial dependency is used to alter scheduling.

A local phase snapshot and direct global switch produce the same residual.
