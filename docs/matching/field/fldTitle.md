# Matching notes: field/fldTitle

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

A qualified `cc1probe.sh --sched --alloc --no-dumps` trace reproduces the
residual with hooked/unhooked and plain-output equality. In both scheduling
passes, phase load UID 1748 has priority 3 and cost 2; constant-one UID 1750
has priority 2 and cost 1. The load therefore issues first because of its
critical-path priority, rather than an original-order tie. The constant also
feeds the case-zero transition store. Replacing that transition's assignment
with a phase increment is behavior-equivalent on this path and leaves the
same two-word residual; the full owner preserves its seven existing matches
and data. This control is not retained.

Reproduce on the guarded body with `tools/cc1probe.sh <owner.c> --as-unit
src/smt3/field/fldTitle.c --func fldTitle_Process --cflag -DNON_MATCHING
--sched --alloc --no-dumps --out <output>`. No source-backed dependency that
places the constant before the phase load has been established.

A qualified `cc1probe` run explains the C dispatch order by critical-path
priority. In both scheduling passes, the phase load (UID 1748, cost 2,
priority 3) is chosen before the case-one constant (UID 1750, cost 1, priority
2). This is not an instruction-order tie; changing only the UIDs would not
reverse the choice. Plain, hooked and unhooked assembly agree. Reproduce with
`tools/cc1probe.sh src/smt3/field/fldTitle.c --func fldTitle_Process --sched --no-dumps --cflag -DNON_MATCHING --out /tmp/title-dispatch`.
The source premise behind retail's opposite order remains unknown.
