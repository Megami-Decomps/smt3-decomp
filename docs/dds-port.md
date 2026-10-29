# Porting dds-decomp's C into smt3

DDS1 and DDS2 run on the same sdf engine as SMT3. Their reconstructed C in
[dds-decomp](https://github.com/Megami-Decomps/dds-decomp) is useful evidence
for SMT3's remaining assembly fallbacks. Check the current
[progress report](progress.md) for coverage; the measurements below are dated
snapshots of the original reuse analysis.

`tools/dds_reuse.py` reads source, split assembly and symbol tables. It can
find candidate counterparts without a DDS executable, using retail sizes,
call counts and shared literals. Its confidence describes a pairing, not
whether donor C matches SMT3.

## Names and counterparts

Both projects now use many original names recovered from SMT3 HD. The early
observation that only 11 function names agreed is obsolete. A shared original
name is a useful starting point; compare the native assembly before copying
a body, because layouts, signatures and behaviour can differ between games.
See [Names](names.md) for the provenance of SMT3's names.

Older recovered names also differ: historical pairings called
`sdfTexListAdd` / `sdfMatInitSystem` on this side were named
`sdfAppendResourceListItem` / `sdfRegisterResourceQueueCallbacks` in DDS.
Use the current symbol tables rather than assuming those spellings persist.

Unit names and order help locate counterparts, but do not prove original
file boundaries. Literal strings and the code itself are stronger evidence
for an individual pairing.

## Method

Both games were built from near-identical sources by the same compiler, so the two
`.text` segments are two editions of the same book: mostly the same functions in
mostly the same order, with insertions, deletions and a few reorderings. That makes
it a sequence alignment problem, and the sequence that both sides can supply without
a binary on the donor side is the function size list.

1. **Size sequences.** For smt3, function boundaries and sizes come from
   `config/smt3/symbol_addrs.txt` plus the split asm. For DDS1, each C function's
   size is recovered from dds-decomp's own splat config and symbol addresses, so the
   donor sizes are retail sizes, not compiled-from-C sizes.
2. **Frame.** `difflib.SequenceMatcher` over the two size sequences. Runs of six or
   more consecutive equal sizes are the frame: at that length a coincidence is
   improbable and the run usually spans a whole translation unit.
3. **Gap filling.** Between frame blocks, a banded dynamic program aligns the
   leftovers on size closeness plus agreement in the number of calls each function
   makes. Call counts come from the asm on both sides and are cheap to compare, and
   they break ties that size alone cannot.
4. **Literal anchors.** Where an smt3 function references a string literal in
   `.rodata` and some dds-decomp C function contains the same literal, that is direct
   evidence rather than inference, and it overrides the alignment.

This aligned 4574 function pairs.

Confidence is then assigned per smt3 fallback:

- **high**: a shared literal anchor, or a frame run of at least six.
- **medium**: a frame run of three to five.
- **low**: a frame run of one or two, or a gap-filled pair.
- **none**: no counterpart, or a counterpart that dds-decomp has also left as asm.

## Numbers

Measured on 2026-10-20 against 2371 remaining smt3 fallbacks totalling 0x100DB4
bytes of `.text`. The fallback count drops as work lands, so rerun the tool
rather than quoting these:

    python tools/dds_reuse.py --dds-decomp <checkout>

| confidence | functions | bytes    | share of fallback bytes |
| ---------- | --------- | -------- | ----------------------- |
| high       | 63        | 0x00BF24 | 4.7%                    |
| medium     | 9         | 0x000C18 | 0.3%                    |
| low        | 155       | 0x010530 | 6.4%                    |
| none       | 2144      | 0x0E3D48 | 88.7%                   |

Served by C that already exists: 227 functions, 0x1D06C bytes, 11.3% of the
remaining work by size. Of the 2144 "none" rows, 2078 have no DDS1 counterpart
at all and 38 pair to code dds-decomp has not decompiled either.

The reuse tiers are nearly spent, and that is the headline rather than a caveat.
When this was first measured, on 2026-10-18 against 6930 fallbacks, the high
tier alone was 1779 functions and 25.7% of the remaining bytes. Porting and
ordinary reconstruction have since consumed almost all of it: 63 functions and
4.7%. The share has not fallen because the tier is holding up, but because the
denominator shrank faster than the tier did; in absolute terms the high tier has
gone 1779, then 154, then 63. What is left is overwhelmingly SMT3's own game
code, with no counterpart to port from.

Every count here is over functions whose `smt3_state` is `asm`, which is the set
of `INCLUDE_ASM` lines still in `src/`, not the set of `.s` files under `asm/`.
Those diverge: the split carries a file per function whether or not the function
is in C, so counting the split overstates the work left by thousands.

## Pair units, not functions

Five units' worth of experience says the per-function tiers are the weak part
of this tool and the donor *unit* identification is the strong part.

The tiers, as they have actually performed: on one unit 10 of 13
high-confidence pairings held; on another all 5 were different functions at a
similar donor size; a third unit's 5 were likewise unusable; and on
`event/evtEventViewer` both of its 2 high rows were different functions at a
similar size. One pairing has held exactly, `fldDrawSkyGradient` against
`fldSkyDrawFilter`, the same function at the same 0xAD8 size, which
transliterated to 682 of 694 words on the first compile.

What paid off instead, on the same unit where both named pairings failed:
dds-decomp's `dds1/event/evtEventViewer.c` is the same original file, so
fourteen bodies were lifted from its control flow and twelve of them matched,
37 functions in the unit overall. Every field offset, global and call name was
wrong and re-derived against retail, which is the standing caveat. Three of
them matched only on the donor's *literal* statement order, oddities included,
which is a reason to stop editing a transliteration that is close.

So the useful question is not "does this function have a high-confidence
pairing" but "does dds-decomp have the file this function came from".

**Find that file by its string literals.** On `event/evtEventViewerMenu` the
donor named from the unit's own name was the wrong one: `evtEventViewer.c` is
the viewer runtime, while that unit is the viewer menu, whose real donor is
`dds1/game/code_00235270.c`. Grepping dds-decomp for the menu strings
`VALUE CHANGE.` and `----- NEW FRAME -----` found it, and it aligns almost
function for function in the same order. 23 of 29 functions came out of it,
14 of them matching on the first compile from a literal transliteration. A shared
donor unit also tends to mean a shared type or helper, which is what makes the
rest of the family cheap once the first body is out.

The HD reconstruction ([Names](names.md)) also supplies original names and
source-file candidates that help locate the donor.

`build/dds_reuse.tsv` carries a row per function, not per fallback. Filter on
`smt3_state == "asm"` before counting anything: a tally over every row counts work
already finished and overstates what is left by several times.

The reuse is concentrated. `effect/*`, `event/*`, `interface/*`, `kernel/*` and the
`sdf/*` units are largely served; the two biggest unserved blocks are
`newbattle/nbMakePacket` (896 fallbacks, 870 of them with no counterpart) and
`code_001FAE80` (1157 fallbacks, 997 with no counterpart). Those are SMT3's own
game code, which is what one would expect: the engine is shared, the game is not.
`tools/dds_reuse.py` prints the full per-unit table and writes a TSV row per
function with the donor name, donor file, size, run length and the evidence used.

### How trustworthy the alignment is

Two independent checks, neither of which is an input to the alignment:

- **Literal anchors.** Of 92 functions anchored by a shared `.rodata` literal, the
  size alignment independently reproduced 65, missed 25 (no pair proposed) and
  contradicted 2. So among pairs it proposes it is nearly always right, and it is
  conservative rather than wrong: it fails to pair more often than it mispairs.
- **Unit names.** Restricted to pairs where both sides sit in a named unit, 1834
  agree on the unit name and 33 disagree, 98.2%.

Both checks are about the *pairing*. Neither says the donor C is byte-correct for
smt3, which it frequently is not: the same function often differs between the two
games by a field offset, an extra argument or an inlined call.

## Limits

- **The historical measurement did not use a DDS1 ELF.** The pairs above rest
  on sizes, call counts and literals. With a locally supplied DDS1 executable,
  `tools/pair_dds1.py` can compare masked instruction hashes. Treat that as
  stronger corroboration, then check operands and behaviour against both binaries.
- **Donor sizes are retail sizes.** dds-decomp's C is verified byte-identical for
  those functions, so a donor size equal to an smt3 size is meaningful. But a size
  match across two games is still weaker than a masked-hash match.
- **Size alignment cannot see reordering.** A function that moved between units
  shows up as a gap-filled or unpaired row, which is why the "none" tier contains
  some functions that do have a counterpart.
- **Literal anchors are sparse.** Only 92 of 6950 fallbacks reference a literal that
  also appears in dds-decomp's C, so they cannot carry the measurement on their own.
- **The confidence is about the pairing, not the port.** A high-confidence row says
  "this is the same function"; it does not say the C will compile to the same bytes.

## Port workflow

The split asm under `asm/smt3/` is the authority for what smt3 does. dds-decomp's C
is a hint about structure, never proof.

1. Pick a unit with many high rows and a single donor file. `dds_reuse.py`'s TSV
   gives `dds_file` per row, and a unit's rows usually concentrate in one or two
   donor files, so one file read covers a whole batch.
2. Read the donor C for shape: control flow, which fields are touched, which calls
   are made, what is inlined.
3. Read the smt3 asm for the same function and reconcile. Differences in field
   offsets, argument counts and inlining between the two games are normal and the
   asm wins every time.
4. Write it in smt3's own names and types from `include/`. Never import dds-decomp's
   recovered names or its local typedefs wholesale; they are a different game's
   recovery and would spread names this repo has not earned.
5. Verify. `python tools/check_unit.py <file>` must report 0 differ, and a full
   `ninja` must end on `check config/smt3/checksum.sha1`. Non-matching C can remain behind `#ifdef NON_MATCHING`, with the original
   `INCLUDE_ASM` fallback in its `#else` branch, in address order.

Families pay off far better than single functions. The sdf motion batch landed 36
functions, and most of them came in groups that share one body: 11 node generators
differing only in the vtable and the allocation size, 5 colour `InitBlend`
callbacks differing only in which material field is read, 5 colour `BlendSeek`
callbacks differing only in the setter called. Once the first of a family matches,
the rest are mechanical.

### What costs time

Not the structure, which the donor C gives away, but gcc 2.96 codegen. Things that
cost repeated attempts in the sdf motion batch:

- Branch polarity follows which side of the comparison the source writes.
  `if (x == 0) {A} else {B}` emits `bnel` to the else block; writing the test the
  other way around emits `beql` and swaps the block order.
- Reusing a loaded value through a named local, instead of re-reading the struct
  field, changes register allocation across the whole function. Hoisting one
  `frame0 = frameList[index];` out of a re-read cut `sdfMotGetKey` from 38 differing
  words to 9.
- `arr[i + 1]` reuses the pointer already computed for `arr[i]`; `arr[next]` with a
  named local recomputes the address from scratch. Retail does one or the other and
  the asm shows which.
- A 16-byte struct copy written in C emits unaligned `ldl/ldr/sdl/sdr`. Retail's
  `lq/sq` pair comes from `PS2_COPY_QUAD` in `include/ps2asm.h`.
- Passing a float literal through an `"r"` operand constraint makes gcc emit
  `lui/mtc1/mfc1` to get the value into a general register, which is what retail
  does when a VU0 macro wants it there.
- Varargs forwarders need `__builtin_va_list` with `__builtin_stdarg_start`, which
  is what spills the argument registers and the eight float registers into the
  frame. The three sdfDevCons printf entry points matched first try once the
  builtins were in place, and so did the `vsprintf` call that takes the list.
- Local layout and branch-likely are where small functions stall.
  `sdfDevConsDrawAll` reproduces its calls exactly but retail leaves 0x30 of
  frame below the 4KB glyph buffer that no arrangement of the locals accounts
  for, and `sdfPacFree` gets a `beql` guard plus a padding nop where retail fills
  the delay slot with the argument copy.

Block placement is where a function gets abandoned. `sdfMotionSeek` was reverted to
`INCLUDE_ASM` after several attempts: retail moves the whole simple-seek loop body
behind the blend block and enters it with a forward branch to a shared bottom test,
and no arrangement of the equivalent C reproduced that placement. Four others went
back the same way: `sdfModelGetNodeName` (a dead hoisted `%hi` and one padding nop),
`sdfDeliverDrawEnd` (retail takes the array base once and stores the high element
first, which no order of the two assignments reproduces), `sdfDevConsDrawAll` and
`sdfPacFree`. All five are small, and all five differ in placement or register
choice rather than in what the code does.
