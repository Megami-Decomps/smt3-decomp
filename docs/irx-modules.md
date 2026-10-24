# Building the IRX modules

Six IOP modules build byte-identical from source, with their own compiler,
linker script and container packer: the three Atlus wrote in `USERIRX/`, and
the three of Sony's in `SOUNDIRX/` that the sound driver needs. The build is
separate from the main ELF's and touches none of it.

```
python tools/download_tools.py          # brings in iop-gcc 2.8.1
python tools/irx_split.py               # asm from orig/irx/*.IRX
python tools/irx_configure.py
ninja -f build-irx.ninja
```

`ninja` ends in a `verify` step per module that compares the packed file with
`orig/irx/<module>.IRX` and fails the build on any difference, so a green build
is the proof. What the modules do is in
[The SDF IRX modules](sdf-irx-modules.md).

| Module | Bytes | .text chunks | Real functions |
| --- | --- | --- | --- |
| `SDFIOP.IRX` | 2857 | 32 | 5 |
| `SDFIOPFL.IRX` | 3113 | 28 | 6 |
| `SDFSDMAN.IRX` | 39981 | 157 | 72 |
| `MODMSIN.IRX` | 1929 | 17 | 12 |
| `MODMIDI.IRX` | 21965 | 71 | 62 |
| `MODHSYN.IRX` | 59501 | 238 | 218 |

The remaining chunks are import stubs and the tables between them.

Sony's three are `MODHSYN`, the SPU2 synthesiser, `MODMIDI`, the sequencer that
feeds it, and `MODMSIN`, the message queue between them. They are what plays a
`.SMG` bank, so the sound driver's bank path is almost entirely calls into them,
and the USA and Maniax discs ship the same files byte for byte. `MODMSIN` is
complete; the other two are part way.

## The toolchain

`iop-gcc 2.8.1` from the decomp.me archive, with its own `cpp`, `cc1` and `as`.
Its cc1 does not preprocess, so the build runs cpp first, the way the 2.8
driver does. `-O2` reproduces the code; the first function tried,
`sdfSndIsqrt`, came out byte-exact with no coaxing, including register
allocation and the `div` trap expansion.

The same compiler and flags reproduce Sony's three modules, which is not a
given, since those came from Sony's own build rather than Atlus's. The four in
`MODHSYN` that have C were byte-exact as written, sign extension, two-branch
clamps and delay-slot filling included, so `iop-gcc 2.95.2-102` has not been
needed for them either.

Linking uses the decompals `ld`, since iop-gcc ships none. Registers are
written numerically in the asm: gas 2.8 predates the o32 aliases, the same
reason `config/smt3/SLUS_209.11.yaml` sets `mips_abi_gpr: numeric`.

`iop-gcc 2.95.2-102` is also on the archive if 2.8.1 ever fails to match
something. It is a Windows binary and needs wine.

## The container

An IRX is an ELF of type 0xFF80 that keeps its relocations and drops its
symbols. The loader puts the module anywhere and walks `.rel.*` adding the load
base, so every relocation carries symbol index 0 and the file is a complete
image of a module linked at 0. Only four relocation types appear: R_MIPS_32,
R_MIPS_26, R_MIPS_HI16 and R_MIPS_LO16.

`tools/irx.py` reads and writes the container. Its layout is fixed:

- the ELF header, two program headers, then `.iopmod` at 0x74
- the load segment right after `.iopmod`, rounded up to 16, so every
  allocatable section sits at its address plus that offset. `.iopmod` is
  0x1A bytes plus the module name and its NUL, so where the segment starts
  depends on how long the name is: `sdf_sound_driver` is exactly 18 bytes and
  lands the segment on 0xA0, while Sony's longer names push it to 0xB0
- `.shstrtab`, the section header table, then the `.rel` sections in section
  order, then a one-entry `.symtab` and an empty `.strtab`

A NOBITS section records the end of the file data as its offset rather than its
own address plus the delta. `.iopmod` carries the module info pointer, the
entry point, `gp`, and the text, data and bss sizes; `gp` is 0x7ff0 past the end
of `.data` in every module, which is where `.sdata` would start.

Three details only Sony's modules forced, all of which the three SDF modules
satisfied by accident:

- a module carries a `.bss` section header even with nothing in it, which
  `MODMSIN` is. Every other section is dropped when it comes out empty.
- the `.bss` reservation in `bss.s` is byte aligned, because gas rounds a
  NOBITS section's size up to its own alignment and `MODHSYN`'s `.bss` is
  0x3842 bytes, which four-byte alignment turns into 0x3844. Symbols inside it
  are placed by the `.space` runs, not by the alignment.
- a `%hi`/`%lo` pair's two relocations sit together in the table, before the
  relocation of any instruction physically between them. That happens when the
  `%lo` lands in a jump's delay slot: the compiler writes `lui`/`addiu`/`jal`,
  the assembler moves the `addiu` down into the slot, and the relocations keep
  the order from before the move. Assembly under `.set noreorder` is already in
  final order, so gas emits strictly by offset and the packer puts each `LO16`
  back beside its `HI16`. It is a no-op once a function is C and gas fills the
  delay slot itself.

`python tools/irx.py info orig/irx/SDFSDMAN.IRX` prints all of it. The reader
and writer round-trip all six retail files byte for byte, which is what pinned
the layout down before any of it was built from source.

## Splitting

`tools/irx_split.py` disassembles with rabbitizer and writes one `.s` per
`.text` chunk under `asm/irx/<module>/text/`, plus `rodata.s`, `data.s` and
`bss.s`.

It uses the module's own relocation table rather than guessing which immediates
are addresses. For a module based at 0 that distinction matters: `0x510` is
both a plausible constant and the address of a string, and only the relocation
table says which. Each relocated operand becomes `%hi`, `%lo` or a symbol
reference, so the assembler and linker rebuild the relocation tables from the
source and a byte-identical `.rel.text` is evidence the code is right.

Two details the retail files forced:

- `.text` is not all functions. The IRX import and export tables sit between
  the stub functions, so anything no function covers is emitted as data. They
  are the `D_<address>` chunks in the module's C file.
- Jump table entries get a `L_<address>` label rather than gas's local `.L_`
  form, because they have to resolve from `.rodata`.

`bss.s` reserves its space in a `.bss.reserve` input section that the linker
script folds into `.bss`, byte aligned so gas cannot round its size up, with
the empty `.bss` beside it giving the output section its alignment of 16. See
the container section above for why the rounding matters.

## Writing C

`src/irx/<module>.c` owns `.text`. It started as nothing but `INCLUDE_ASM`
lines in link order; decompiling a function means replacing its line with the
C body in the same place, so the function stays where it was. Every function
in the three SDF modules now has C; Sony's three are nearly all still
assembly.

What stays `INCLUDE_ASM` with no C, and always will: the IRX import stubs
(`jr $ra` plus the import id) and the import and export tables between them,
the `D_<address>` chunks. They come from the linker's IRX macros, not from a
compiler.

### Types and prototypes come from the IDB

A module whose config names an `idb:` also has a generated
`include/irx/<module>.h`, which its `.c` includes in place of `iop.h`:

```
python tools/irx_headers.py MODHSYN
```

It carries the module's recovered structs, the prototype of every function the
database has a type for, and its named data. Every struct comes with a
compile-time size check, so a layout the compiler reads differently than IDA
does is a build error rather than a quietly wrong decompilation. IDA's own
spellings are translated on the way out, since `__fastcall` and `_DWORD` mean
nothing to iop-gcc 2.8.1 and `#pragma pack` postdates it.

The database is the source of truth, so the loop runs in both directions. A
prototype IDA guessed and the retail code disproves shows up as `conflicting
types for ...` the moment the C is written against the truth, and the fix is to
correct the database and regenerate, not to edit the header or bend the C.
`hsynEnvRateLookup` went that way: IDA had it as `int(int, int, int)`, the
matching C is `s16(s16, s32, s32)`, and the byte-identical build is what
settles it.

A function whose C is equivalent but does not reproduce the retail
instructions sits under `#ifdef NON_MATCHING`, with the `INCLUDE_ASM` the
default build uses in the `#else`, and a comment saying how far off it is:

```c
#ifdef NON_MATCHING
/* One instruction short: gcc fills the branch delay slot ... */
s32 sdfSndUpdateVoiceMap(void)
{
    ...
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndUpdateVoiceMap);
#endif
```

So the default build is still byte-identical, and `-DNON_MATCHING` compiles
the C instead (it will not link for `SDFSDMAN`, whose jump tables live in the
assembly `.rodata`).

| Module | Functions | Matching C | Under `NON_MATCHING` |
| --- | --- | --- | --- |
| `SDFIOP.IRX` | 5 | 5 | 0 |
| `SDFIOPFL.IRX` | 6 | 6 | 0 |
| `SDFSDMAN.IRX` | 72 | 33 | 39 |
| `MODMSIN.IRX` | 12 | 12 | 0 |
| `MODMIDI.IRX` | 62 | 31 | 5 |
| `MODHSYN.IRX` | 218 | 118 | 15 |

Two things only Sony's modules turned up, both about delay slots and neither
fixable from the C:

- `gas` refuses to move the second half of a `sw reg, sym` macro expansion
  into a following branch's delay slot, where the retail assembler put it. Any
  function whose last act before a test is a store to a global is a nop long;
  `modhsyn_e16` is the smallest case.
- where retail fills a slot by moving the fall-through's first instruction
  down, gcc sometimes copies the branch target's instead and keeps both, which
  costs one instruction.

The three that cannot match while `.rodata` is assembly are
`sdfSndGetVolTable`, `sdfSndLoadSmg` and `sdfSndDrvRpcInit`: each compiles its
switch to a jump table, and a table emitted from the C file would land at the
front of `.rodata` and move every string after it. The rest are off by a
handful of instructions, usually the order of two basic blocks or which
register a value lands in.

`tools/irx_diff.py` is the loop: build, then

```
python tools/irx_diff.py SDFSDMAN --sizes     # chunks whose size changed
python tools/irx_diff.py SDFSDMAN <function>  # retail and built side by side
```

Sizes first: while any chunk is the wrong size everything after it moves, so
the byte comparison only means something once `--sizes` reports no change.

Three things the retail code settled that are worth knowing before adding
more C:

- `-G 0`. The default small data threshold puts a four byte extern in `.sbss`
  and reaches it through `gp`; the retail code uses `%hi` and `%lo`
  everywhere.
- `sdfSdManInit` was a translation unit of its own. The linker aligned what
  follows it to 16, which is the stray `nop` at the end of the function; the
  C file reproduces it with an `__asm__(".align 4")`.
- The ADPCM state is one struct (`sdfAdpcm`), not twenty globals. The retail
  code reaches `fadeStep` at 14 off the same base register it loaded for
  `playing`, which only happens for fields of one object.

## Config

Each module has its own config directory, laid out like `config/smt3`:

```
config/irx/SDFSDMAN/SDFSDMAN.yaml      the module: sha1, .iopmod fields, paths
config/irx/SDFSDMAN/symbol_addrs.txt   splat's symbol_addrs format
```

The yaml is authored. It names the `.iopmod` fields that are not derived from
the link (the module name, its version and the entry symbol), the sha1 of the
retail file, and where the module's asm, source and build output live.
`tools/irx_config.py` reads it and every tool takes its paths from there, so
adding a module is a directory and a yaml.

It may also carry an `idb:` key naming the database its symbols come from, as
a path fragment rather than a module name. Sony's sound modules need it:
several builds of each are usually open at once, Nocturne's and whichever
later one came with another game, and the module name alone picks the wrong
one or refuses as ambiguous.

`symbol_addrs.txt` is generated from the IDB by `tools/irx_symbols.py`. A
function IDA has found but not named comes across as `func_<address>`, because
its bounds are what give the splitter one chunk per function; unnamed data is
left out, since the splitter names what it finds there itself.

There is no checked-in chunk order. `tools/irx_split.py` writes one under
`build/` to bootstrap the C file, and after that the `INCLUDE_ASM` lines in
`src/irx/<module>.c` are the order, because a decompiled function replaces its
line in place.

Why not splat itself: splat cannot read an IRX, so the container work in
`tools/irx.py` is needed either way, and spimdisasm would infer symbols where
the relocation table states them. The symbol file format is shared, which is
the part worth sharing.
