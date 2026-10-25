# Building the IRX modules

Seven IOP modules build byte-identical from source, with their own compiler,
linker script and container packer: the three Atlus wrote in `USERIRX/`, and
the four of Sony's in `SOUNDIRX/` that the sound driver needs. The build is
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
| `MODHSYN.IRX` | 59501 | 232 | 213 |
| `LIBSD.IRX` | 28573 | 95 | 79 |

The remaining chunks are import stubs and the tables between them. Both
numbers are what `tools/irx_split.py` prints, so the function count
includes those stubs; the table under Writing C counts only the functions
that have a body.

Sony's four are `MODHSYN`, the SPU2 synthesiser, `MODMIDI`, the sequencer that
feeds it, `MODMSIN`, the message queue between them, and `LIBSD`, the SPU2
register and transfer library all of them sit on. They are what plays a
`.SMG` bank, so the sound driver's bank path is almost entirely calls into them,
and the USA and Maniax discs ship the same files byte for byte. All four are
complete: every function has C.

## The toolchain

`iop-gcc 2.8.1` from the decomp.me archive, with its own `cpp`, `cc1` and `as`.
Its cc1 does not preprocess, so the build runs cpp first, the way the 2.8
driver does. `-O2` reproduces the code; the first function tried,
`sdfSndIsqrt`, came out byte-exact with no coaxing, including register
allocation and the `div` trap expansion.

The same compiler and flags reproduce Sony's four modules, which is not a
given, since those came from Sony's own build rather than Atlus's. Sign
extension, two-branch clamps and delay-slot filling all come out of 2.8.1
unaided, so `iop-gcc 2.95.2-102` has not been needed for them either.

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
and writer round-trip all seven retail files byte for byte, which is what pinned
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
  are the `D_<address>` chunks in the module's C file. The import tables are no
  longer among them; see below.
- Jump table entries get a `L_<address>` label rather than gas's local `.L_`
  form, because they have to resolve from `.rodata`.

`bss.s` reserves its space in a `.bss.reserve` input section that the linker
script folds into `.bss`, byte aligned so gas cannot round its size up, with
the empty `.bss` beside it giving the output section its alignment of 16. See
the container section above for why the rounding matters.

## The import tables are source, not chunks

An imported function is reached through an 8-byte stub that the loader rewrites
into a jump to the resolved export:

    jr    $31
    addiu $0, $0, <ordinal>

The ordinal says which export, so the stub is fully determined by the library
and the ordinal, and `IRX_IMPORT(intrman, CpuSuspendIntr, 0x11)` in `irx.h`
emits it. The table around it is just as determined: a 20-byte header of the
magic `0x41e00000`, a zero, the version, and an eight byte name field, then the
stubs, then an 8-byte zero entry. So a whole table reads as

    IRX_IMPORT_TABLE(intrman, 1, 2);
    IRX_IMPORT(intrman, CpuSuspendIntr, 0x11);
    IRX_IMPORT(intrman, CpuResumeIntr, 0x12);
    IRX_IMPORT_TABLE_END;

with `ALIGN_TEXT(4)` in front, because the first table in a module is aligned to
16. This replaced 152 stubs and 46 tables across the seven modules and left
`SDFIOP` with no `INCLUDE_ASM` at all. It is the same design as ps2sdk's
`DECLARE_IMPORT`, and the symbol keeps its library prefix so two libraries
exporting the same short name stay distinct.

Writing the ordinal in the source is what makes a wrong import name visible,
because a stub's name can then be checked against the table that contains it.
Doing that once found two mislabelled imports that had been invisible for as
long as the tables were opaque data; both are in `docs/idb-corrections.md`.

Three notes on gas 2.8, which assembles these. It rejects `. - label` inside
`.rept` and inside `.if`, accepts it in `.space` but warns when the count comes
out zero, and accepts it silently in `.fill`. The name field therefore pads with
`.fill`, since an eight character library name such as `loadcore` needs no
padding at all.

The export table is still a `D_<address>` chunk. It holds relocated function
pointers rather than ordinals, so it is a larger job than the import side.

## Names

A module's exports carry its own prefix and its internals do not:

| Form | Meaning | Example |
| --- | --- | --- |
| `<module>_sce<Lib><Name>` | an export whose Sony name is known | `libsd_sceSdVoiceTrans`, `modhsyn_sceHSyn_Init` |
| `<module>_e<N>` | an export whose purpose is not known yet, by ordinal | `modhsyn_e19` |
| `sce<Lib><Name>` | an internal function, named for what it does | `sceHsynCmdNoteOn`, `sceSdTransferOp` |
| `<library>_<Name>` | an import stub, named for the library it calls | `intrman_CpuSuspendIntr` |

So a bare `sce` name is internal and a prefixed one is exported, which reads
backwards until you know it. LIBSD follows it with `sceSd`: `libsd_sceSdInit` is
exported, `sceSdInitCores` is not. Note that `sceSd` is the Sound Device
library; `sceCd` is Sony's unrelated CD/DVD prefix and appears nowhere here.

Names ported from the Persona 3 FES `LIB31` builds are evidence, not fact, since
those are a later version of the same libraries. Verify against the retail code
before applying one, and never run those binaries against this driver.

## Writing C

`src/irx/<module>.c` owns `.text`. It started as nothing but `INCLUDE_ASM`
lines in link order; decompiling a function means replacing its line with the
C body in the same place, so the function stays where it was. Every function
in all seven modules now has C.

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
`sceHsynEnvRateLookup` went that way: IDA had it as `int(int, int, int)`, the
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
| `MODMSIN.IRX` | 9 | 9 | 0 |
| `MODMIDI.IRX` | 54 | 41 | 13 |
| `MODHSYN.IRX` | 188 | 152 | 36 |
| `LIBSD.IRX` | 60 | 37 | 23 |

### Where LIBSD stands

Every function has C: 37 match and 23 are equivalent under `NON_MATCHING`;
nothing is bare. The export table at `.text` 0 stays assembly, since it holds
relocated function pointers rather than ordinals.

Four of the 23 cannot match while `.rodata` is assembly, their switch or
dispatch compiling to a jump table whose `L_<address>` case labels the assembly
`.rodata` already defines: `libsd_sceSdProcBatch`, `libsd_sceSdProcBatchEx`,
`libsd_sceSdGetCoreAttr` and `sceSdTransferOp`.

`start` is its own class. gas decides whether to fill a `jal`'s delay slot
while parsing, so the `la` of the export table loses retail's `nop` to the
`addiu` because the table is assembled into the same translation unit; the
identical source matches in MODHSYN, whose library table is in `.data`.

The rest are the familiar handful of instructions: which callee-saved registers
four live values land in (`libsd_sceSdVoiceTrans`, `sceSdBlockTransRead`,
`sceSdResetVoices`), a scheduler permutation of one block
(`sceSdSetSpdifMode`, `libsd_sceSdSetAddr`, `libsd_sceSdClearEffectWorkArea`,
`sceSdInitState`), a load retail recomputes (`sceSdIrqHandler`,
`sceSdVoiceTransIo`), cross-jumping (`sceSdDmaIntrHandler`,
`sceSdResetCores`), gas refusing to move the second half of a `sw reg, sym`
into a following delay slot (`libsd_sceSdInit`, `sceSdBlockTransWrite`), and
two prototypes that are right for the API but not for retail's own translation
unit, which had none in scope: `libsd_sceSdGetParam` returning `u16` costs
`libsd_sceSdSetCoreAttr` a register mask, and `libsd_sceSdPitch2Note`'s `u16`
parameters cost it a mask and a spill.

What `iop.h` says about LIBSD is otherwise right. One thing it does not say:
`libsd_sceSdBlockTrans` reads a fifth argument through `va_arg` when
`mode & 3 == 3`, which is the one instruction that version is over. The
declaration's four parameters are right for every caller in this game, since
MODHSYN's wrapper passes four and matches, so the faithful spelling would be
four parameters plus an ellipsis. Left alone, because the other modules'
matching builds depend on that declaration.

### Where MODHSYN stands

Every function has C. 152 match and 36 are equivalent under `NON_MATCHING`;
nothing is bare. The remaining chunks are the IRX import stubs and the tables
between them, which never become C.

Six of the 36 cannot match at all while `.rodata` is assembly: their switch
compiles to a jump table whose `L_<address>` case labels the assembly
`.rodata` already defines, so a C body leaves them undefined and the *link*
fails. They are `sceHsynLfoStep`, `sceHsynVoiceEventDispatch`,
`sceHsynLfoSetupA`, `sceHsynLfoSetupB`, `sceHsynVoiceSetParamRamp` and `sceHsynCmdDispatch`.

The other 30 break down as:

| Function | Why |
| --- | --- |
| `sceHsynInitCores` | four instructions out of place: gcc schedules the outer loop's preheader after the list-head stores |
| `sceHsynVoiceVolumeCommit` | one short: gcc keeps `envB.level` live into the second arm where retail reloads it |
| `sceHsynVoiceUpdatePathB` | one short: gcc keeps `v->vag` live across two halfword stores |
| `sceHsynVoiceStart` | byte-identical but for one register pair |
| `modhsyn_sceHSyn_Load` | six short: gcc cross-jumps the two identical bank-clearing blocks |
| `sceHsynChannelNoteOn`, `sceHsynVoiceStartPathA`, `sceHsynVoiceStartPathB` | the retail bodies spill a dozen values to the frame; a C body of this shape keeps them in registers |
| the other 22 | inherited from earlier agents |

Three of the four large note-on paths (`sceHsynVoiceStartPathA`, `sceHsynVoiceStartPathB`,
`sceHsynChannelNoteOn`) share one shape: walk the program's key list, the sample
set and the sample entries, resolve a VAG, free a voice by one of four
escalating strategies, then fill the voice and call `sceHsynVoiceUpdatePathA` or
`PathB`. Reading one makes the other two quick.

`sceHsynVoiceStart` is settled: it is a nine-argument forwarder whose `path`
selects `sceHsynVoiceStartPathA` or `sceHsynVoiceStartPathB`, and the recovered parameter types
are in [IDB corrections](idb-corrections.md).

### Where MODMIDI stands

All 62 functions have C: 41 match and 13 are equivalent under `NON_MATCHING`.
Those 13 break down as two that cannot match at all (`sceMidiPlaySong`,
`sceMidiPlayEnv`, both jump tables), five whose only difference is gcc
cross-jumping a tail retail keeps twice (`sceMidiParseMetaEvent`, `modmidi_e8`,
`sceMidiSongRepeat`, `sceMidiSearchPlay`, `sceMidiScanLoops`), and six inherited from
earlier agents.

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

What the compiler wants, shape by shape, is in
[What iop-gcc 2.8.1 wants](irx-idioms.md). Read it before grinding a function by
hand: the remaining difference is usually one of the thirty-odd entries there.

A guarded draft is never compiled by the normal build, which takes the assembly
instead, so one can carry an error indefinitely. `tools/check_irx_nonmatching.py`
compiles every module's C with `NON_MATCHING` using the host compiler and is a
syntax check rather than a matching one. It found a missing declaration, three
bare returns in a function the database called `int`, and a cast on the left of
an assignment that had been there since the first commit.

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
