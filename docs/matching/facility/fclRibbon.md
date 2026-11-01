# Matching notes: facility/fclRibbon

## DrawElem (facility/fclRibbon, 0x368)

218 of 218 words match at 872 bytes in ordinary C. The activation effect
reuses the real `spread` scalar for `ctr_act`; the later pulse effect
overwrites it with `0x40 - timer` before use. Both phases initialize the
value they consume. The change removes the activation-only `timer` local,
not a computation or an observation of ribbon state. This is live scalar
reuse across separate phases, with no register constraint. This describes
the landed source and does not claim historical source spelling.

The initial loop snapshots the element pointer and count, draws each
inactive sprite, then observes the active index after that sprite callback
before optionally drawing its active overlay. The deactivation overlay,
alpha-mode switch, activation expansion, pulse expansion and final alpha-mode
reset remain in order. Each later phase obtains its own current state and
element selection; the shared scalar does not cache state across phases.

Alpha products retain explicit unsigned 32-bit multiplication followed by
target signed conversion and arithmetic shift. The initial alpha setup
likewise retains its unsigned shift before signed conversion. These preserve
the target's bit-level arithmetic rather than asserting universally portable
signed conversion or right-shift behavior. Coordinate arithmetic and the
existing valid ribbon/element contract are unchanged; no fabricated bounds
or guarantee for arbitrary corrupted state is introduced.

Source commit `5355c5c1f8353e29009d22e369f2b0e9f35cb66c` has successful
exact-source hosted [CI 41010811151](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41010811151).
The recorded landing check was 13 match, 0 differ for the complete owner.
This historical result is not a new local validation run.

## Qualified allocation evidence (2026-10-30)

Paired complete-owner compiles of the published form and the precommit
activation-only `timer` establish the allocation dependency. Each ordinary
compile was compared against the same compile with `-da -dumpbase` through
`tools/cc.sh`, using `SMT3_AS_UNIT=src/smt3/facility/fclRibbon.c` and pinned
cc1 SHA256 `d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
Both pairs preserve assembly after canonical scratch-directory normalization,
all allocated-section headers and bytes, relocation entries and symbol contracts,
and the function inventory. These are actual compiler pass dumps, not GDB hooks.

In the published expansion, activation UID322 and pulse UID461 both define
pseudo97. Their last consumers have separate REG_DEAD notes (UID368 and UID507).
Local allocation records seven uses, two sets and two deaths across blocks,
and leaves this pseudo for global allocation, which assigns it hard register9.
With the separate activation timer, pseudos158 and97 are each confined to one
block and local allocation assigns both hard register11; neither enters global
allocation. The separate form has 20 differing DrawElem words at the same
872-byte extent. All other compiled functions and allocated data are unchanged.

The final complete-input owner checks are 13 match, 0 differ for published
source and 12 match, 1 differ for the separate form. The checker needs the
unchanged generated `asm` tree as well as source, configuration and retail input
to bound neighboring data. This evidence establishes shared-pseudo allocation,
not an unobserved scheduling tie-break or original variable spelling. The two
real spread lifetimes remain disjoint and each branch initializes its value.
