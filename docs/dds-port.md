# Porting dds-decomp's C into smt3

DDS1 and DDS2 run on the same sdf engine as SMT3, and
[dds-decomp](https://github.com/Megami-Decomps/dds-decomp) has most of that engine
in C already: 9343 of the 9566 functions in its main `.text` are written out, with
145 still `INCLUDE_ASM`. SMT3 has 9698 functions and 6930 of them are still
`INCLUDE_ASM`. The question this doc answers is how much of that remaining work
dds-decomp's C can serve, and how to use it without letting a guess into the build.

`tools/dds_reuse.py` produces the numbers. It reads only source: this repo's
`INCLUDE_ASM` lists and split asm, and dds-decomp's `.c` files and `asm/` tree. No
DDS1 binary is involved, which is the main limit (see Limits below).

## Why names do not work

dds-decomp's symbols are its own recovered names, not the original ones. Exactly 11
function names are shared between the two trees, so name matching finds nothing.
The sdfMat example from the table below is typical: smt3's `sdfTexListAdd` is
dds-decomp's `sdfAppendResourceListItem`, and smt3's `sdfMatInitSystem` is
`sdfRegisterResourceQueueCallbacks`. Pairing has to go through addresses or code
shape.

Unit names are the one exception that does carry over, because
`config/smt3/tu_candidates.tsv` already holds translation unit names recovered on
the DDS side. They are used as a check on the alignment, not as an input to it.

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

Measured on 2026-10-18 against 6930 remaining smt3 fallbacks totalling 0x1ADE98
bytes of `.text`. The fallback count drops as work lands, so rerun the tool rather
than quoting these.

| confidence | functions | bytes    | share of fallback bytes |
| ---------- | --------- | -------- | ----------------------- |
| high       | 1779      | 0x058874 | 25.7%                   |
| medium     | 183       | 0x005728 | 2.6%                    |
| low        | 579       | 0x01DA28 | 8.4%                    |
| none       | 4389      | 0x1324D4 | 63.3%                   |

Served by C that already exists: 2541 functions, 0x7B9C4 bytes, 36.7% of the
remaining work by size. Of the 4389 "none" rows, 4298 have no DDS1 counterpart at
all and 91 pair to code dds-decomp has not decompiled either.

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

- **No DDS1 ELF and no DDS1 IDB in this session.** Every pair above rests on sizes,
  call counts and literals. With a DDS1 binary, `tools/pair_dds1.py` can hash each
  function with register and immediate fields masked out and pair on the hash, which
  is near-proof per function and would collapse the medium and low tiers into either
  high or none. That is the single thing that would unblock byte-level pairing.
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
   `ninja` must end on `check config/smt3/checksum.sha1`. Anything that will not
   match goes back to `INCLUDE_ASM` in address order.

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
