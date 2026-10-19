# VU microcode

`.vutext` holds two VIF packets and nothing else that executes: `vu0MicroPacket` at `0x002F1350`
and `vu1MicroPacket` at `0x002F18E0`. `sdfCoreDrawInit` DMAs the first to channel 0, so VIF0
uploads 177 instructions to VU0 micro memory, and `sdf3DLinkInitPacket` links the second into the
draw chain as a REF tag, so VIF1 uploads 1884 instructions to VU1 and then unpacks the data memory
they start from. Everything the game draws in 3D runs through the VU1 image.

**The build assembles it from source.** `src/smt3/sdf/vu/vu1_micro.vsm` and `vu0_micro.vsm` are
annotated assembler listings, one line per instruction pair, and `tools/vuas.py` turns them into
the bytes `.vutext` holds. Change a mnemonic and the linked image stops matching the retail SHA-1,
so the listings are the microcode rather than a description of it. Everything in this directory is
for reading.

| file | what it is |
|---|---|
| [`../../src/smt3/sdf/vu/vu1_micro.vsm`](../../src/smt3/sdf/vu/vu1_micro.vsm) | the VU1 image as assembler source, what the build assembles |
| [`../../src/smt3/sdf/vu/vu0_micro.vsm`](../../src/smt3/sdf/vu/vu0_micro.vsm) | the VU0 image, likewise |
| [`vu1_image.c`](vu1_image.c) | the VU1 image as pseudo-C, one statement per instruction, generated |
| [`vu1_annotated.c`](vu1_annotated.c) | the same 1884 instructions written out by hand, in 77 routines with no gaps |
| [`vu0_micro.c`](vu0_micro.c) | the VU0 micro-mode image as pseudo-C, generated |
| [`vu1_names.txt`](vu1_names.txt) | the names `vu2c.py --names` prints for fixed addresses and routines |
| [`memory-map.md`](memory-map.md) | what each named VU1 address holds, with the instruction that proves it |

## The source the build assembles

Each line is one instruction pair as dvp-as writes it, the upper half then the lower, and the
comment after it carries the micro address, both instruction words and what the pair does:

```
; ---- lightVertex, 0x0132..0x0172: the three-light sum, two vertices an iteration
lightVertex:
	MULAx.xyzw ACC, vf20, vf05  LQ.xyzw vf07, 800(vi04)  ; 0132 01E0A1BC 01E72320  ACC.xyzw = ...
```

Labels are the routine names from `vu1_names.txt` and the entry symbols the IDB carries, the
banners come from the transcription's declared ranges, and the pseudo-C comes from the same decode
`vu1_image.c` uses. `tools/vudis.py` writes the whole file:

```bash
tools/vudis.py --vu 1 -o src/smt3/sdf/vu/vu1_micro.vsm
tools/vudis.py --vu 0 -o src/smt3/sdf/vu/vu0_micro.vsm
tools/vudis.py --vu 1 --verify         # diff dvp-as's bytes against the ELF
```

`tools/vuas.py` is the assembler the build uses. It takes dvp-as syntax for the forms this game
uses, in Python, so a matching build needs no toolchain beyond the one `download_tools.py` already
fetches. The syntax is not invented: `dvp-as` itself, the assembler the `.DVP` overlay tables in
the ELF show the microcode was built with, assembles these same files to the same bytes, and
`tools/test_vuas.py` checks that on every run. `tools/download_tools.py` fetches that assembler:
ps2dev publishes no DVP-only artifact, so it streams their smallest bundle and takes the one
binary, 853 KB of a 37 MB download, reading 28 MB of it before it can stop. That bundle carries
GNU as 2.14 from 2003 rather than a modern rebuild, which is the generation that built the game.
`$DVP_AS` overrides the path.

`tools/vu_include.py` runs after splat and rewrites `asm/smt3/data/vutext.s` so the unit includes
the assembled payloads and takes only the VIF headers, the debug font texture and the packet's data
from the extracted asset. `tools/vutext.py --packets` prints the walk those offsets come from.

## Regenerating the reading material

```bash
tools/vutext.py --jsonl build/vu/images.jsonl
tools/vu2c.py build/vu/images.jsonl --program 914D --clean --flow --keep-nops \
    --names docs/vu/vu1_names.txt > docs/vu/vu1_image.c
tools/vu2c.py build/vu/images.jsonl --program FA33 --vu 0 --clean --flow --keep-nops \
    > docs/vu/vu0_micro.c
```

There is no timestamp in either listing: one ELF and one set of options always give the same file.
`--keep-nops` is not optional. Without it 81 of the VU1 image's 1884 addresses are omitted, every
range in the transcription reads as incomplete, and `vucheck.py` reports gaps that are not there.
`--clean` folds matrix multiplies, cross products, constants, clamps and pointer steps, `--flow`
shapes branches into loops, if/else and calls, and both keep the instruction words on the line, so
a fold is checkable where it stands. `tools/vu2c.py <dump>` with no options prints one statement
per instruction, which is what to diff a suspicious fold against; `--audit` prints the statements
each fold replaced.

## What is checked

- `ninja` is the main check: the linked image has to match the retail SHA-1, and every instruction
  in both listings is in it. `tools/test_vuas.py` is the same check without a build, and adds the
  dvp-as cross-check and the error cases.
- `tools/vutext.py --check` walks both packets code by code and fails if a payload runs past the
  packet, if an MPG lands anywhere but where the already-filled micro memory ends, or if the
  reconstructed image does not match its recorded sha1. It also checks every upload against
  `.DVP.ovlytab`, the overlay table dvp-as left in the ELF, which independently records each
  chunk's source address and micro address.
- `tools/vucheck.py docs/vu/vu1_annotated.c docs/vu/vu1_image.c` reports any address in a declared
  range the transcription never mentions, and any instruction it cites at an address that does not
  have it. All 77 ranges are complete and nothing is miscited.
- `tools/vu2c.py --selftest` runs every simplification against the shape it folds and the near miss
  it has to refuse: 32 cases, 0 failed.
- `gcc -fsyntax-only -std=c11 -Wall` is silent on both generated listings, and
  `gcc -fsyntax-only` on the transcription, which warns only about values it sets and never reads.
  That is a real check; it has caught a helper with the wrong return type and an unclosed comment.

What is not checked: the pseudo-C the comments carry, beyond the mnemonic it was derived from.
`vu2c.py --check` re-derives every mnemonic from the words and diffs it against PCSX2's own
disassembly; that needs a dump taken from PCSX2, which the VU1 image has (see below, 0 mismatches
over 1884 instructions) and VU0 does not. What covers VU0 instead is the round trip: both
assemblers turn its listing back into the retail bytes, which no wrong mnemonic survives.

## The VU1 image is the image a trace of the retail game captures

`vu1_annotated.c` was written in sibling project [sdfhook](https://github.com/tge-was-taken/sdfhook)
against a PCSX2 VU trace, not against this ELF. It transfers with proof rather than by assumption:
the eight MPG uploads in `.vutext` reconstruct to a 15072-byte image whose sha1 is
`914D6629023EA313`, and that is the traced image byte for byte. The generated listing reproduces
sdfhook's `docs/vu1/vu1_image.c` line for line as well, differing in three strings: the identity
hash, which is PCSX2's there and the sha1 here, the entry named after it, and the path of the names
file.

`tools/vu2c.py` and `tools/vucheck.py` are copied from sdfhook, which copied `vu2c.py` from
pcsx2-hook, where the maintained one lives. The only local change is `--vu N`, which sets the name
in the emitted entry and the word in the preamble, so the VU0 listing does not call itself VU1.

## The uploads

| packet | source | overlay id | micro | instructions |
|---|---|---|---|---|
| `vu0MicroPacket` | `0x002F1358` | `3830929603` | `0x0000..0x00B0` | 177 |
| `vu1MicroPacket` | `0x002F18E8` | `48712974` | `0x0000..0x00FF` | 256 |
| `vu1MicroPacket` | `0x002F20F0` | `3214916938` | `0x0100..0x01FF` | 256 |
| `vu1MicroPacket` | `0x002F28F8` | `3355168705` | `0x0200..0x02FF` | 256 |
| `vu1MicroPacket` | `0x002F3100` | `3355168705` | `0x0300..0x03FF` | 256 |
| `vu1MicroPacket` | `0x002F3908` | `3355168705` | `0x0400..0x04FF` | 256 |
| `vu1MicroPacket` | `0x002F4110` | `3963728536` | `0x0500..0x05FF` | 256 |
| `vu1MicroPacket` | `0x002F4918` | `2657164303` | `0x0600..0x06FF` | 256 |
| `vu1MicroPacket` | `0x002F5120` | `2657164303` | `0x0700..0x075B` | 92 |

`tools/vutext.py --manifest` prints it. The VU1 image arrives as seven full MPG chunks of 256
instructions and a tail of 92, because a VIF MPG command carries at most 256. The overlay column is
the middle field of the name dvp-as generated. It repeats: the three chunks covering
`0x0200..0x04FF` share one value and the two covering `0x0600..0x075B` share another, which groups
the chunks by whatever they were assembled from. Nothing in the names identifies a source file.

## The MSCAL entries

| MSCAL | symbol | reaches | routine |
|---|---|---|---|
| `0x0000` | `vu1_entryItop30` | JR through VIF1_ITOP |  |
| `0x0004` | `vu1_entryCopyHeader` | `0x0708` (JR) | `copyHeader` |
| `0x0008` | `vu1_entrySpriteDispatch` | JR through VIF1_ITOP |  |
| `0x000C` | `vu1_entryLighting` | `0x01B9` (BAL) | `drawEntry` |
| `0x0010` | `vu1_entryRamp` | `0x041F` (BAL) | `drawEntry2` |
| `0x0014` | `vu1_storeFog` | the routine itself |  |
| `0x001E` | `vu1_entryResetBatch` | `0x0030` (B) |  |
| `0x0020` | `vu1_entryLoadCamera` | `0x0039` (B) | `loadCamera` |
| `0x0022` | `vu1_entryUnpackTexture` | `0x00C2` (B) | `unpackTexture` |
| `0x0028` | `vu1_entryUnpackTwo` | `0x00D3` (B) | `unpackTwo` |
| `0x002A` | `vu1_entryLightSetup` | `0x008F` (B) | `lightSetup` |
| `0x002C` | `vu1_entryCopyBlock` | `0x0081` (B) | `copyBlock` |
| `0x002E` | `vu1_entryComposeMatrices` | `0x004F` (B) | `composeMatrices` |

`tools/vutext.py --entries` prints it, resolving each stub's branch from the words. EE code kicks
the image with `MSCAL <micro address>`, and the symbol column is the name the IDB carries for that
address, which is how an EE call site names the entry it uses. Most entries are a two-instruction
stub that branches straight to its routine; `vu1_storeFog` is the routine itself; the two that jump
through a register the VIF's `ITOP` feeds dispatch on a value the EE side chose and have no static
target.

This is also a cross-check of two naming efforts that did not see each other. The routine column
comes from `vu1_names.txt`, written from the microcode in sdfhook, and the symbol column from the
IDB, written from the EE side. They agree on seven: `copyHeader`, `loadCamera`, `unpackTexture`,
`unpackTwo`, `lightSetup`, `copyBlock` and `composeMatrices`. `vu1_entryResetBatch` is an eighth in
substance: it branches to `0x0030`, inside the block the transcription declares as "the MSCAL
entries and the per-batch reset".

Two IDB names do not survive the same reading. `vu1_entryLighting` branches to `drawEntry`, which
unpacks the per-draw state and runs the whole batch, and `vu1_entryRamp` branches to `drawEntry2`,
the second pass over the same batch. Both are draw entries rather than a lighting or a ramp routine.
They are left alone here: the name may still describe what the EE caller uses the entry for, and
[the correction rule](../idb-corrections.md) wants the retail code to prove a name wrong, not just
leave it unexplained.

## The VU0 image

177 instructions, uploaded at init and never named by anything. No `vcallms` or `vcallmsr` appears
anywhere in the sdf region (`0x002B0000` to the end of `.text`), so whatever starts it is not an EE
micro-mode call there; a VIF0 `MSCAL` in a packet built at run time is the remaining candidate and
has not been found. What the code does is a transform: the `mul4x4` against `vf28..vf31` at
`0x0004`, a perspective divide, `FCGET` on the clip flags, and quadword stores through `vi11`, which
is the shape of a VU0 vertex pipeline rather than of the COP2 macro-mode helpers in
[`docs/vu0-constructs.md`](../vu0-constructs.md). It is listed here because it is in `.vutext` and
the same tooling reads it, not because anything in the game has been traced through it.
