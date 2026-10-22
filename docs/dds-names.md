# Carrying the SMT3HD names to DDS1 and DDS2

The reverse of [dds-port.md](dds-port.md): that page brings dds-decomp's C into
this repo, this one carries this repo's names into dds-decomp. The names are the
HD-recovered originals in the `SLUS_209.11` IDB ([Names](names.md)); dds-decomp's
names are its own inferred descriptions, so where a DDS function is the same code
as a Nocturne function the Nocturne name wins there as it does here.

The method is `tools/pair_build.py`, measured in [maniax-build.md](maniax-build.md),
plus `tools/dds_names.py`, which turns three pairing runs into tables dds-decomp's
`tools/names.py` applies. The rules, the exclusions and the numbers are documented
on the receiving side, in dds-decomp's `docs/smt3-names.md`, so that the record
sits next to the names it explains. This page keeps what was learned about the
pairing itself.

## Where the masked pairing is not enough

`pair_build.py` masks every relocatable immediate, which is right for relinked
builds of one game but too coarse between games: 1,726 of the 4,371 high-tier
DDS1 pairs had a masked hash shared with other functions, because a function
family differing only in an allocation size or a constant collapses to one hash.
`dds_names.py` adds a second, relocation-aware hash that masks only the fields
that form addresses (`lui` halves, the `%lo` or `$gp` immediate that completes
them, `j`/`jal` targets) and keeps every other constant. That alone proves 1,888
DDS1 pairs the masked hash could not, and the remaining shared-hash pairs are
kept only inside a constant-delta run.

The syscall stub table bites exactly as [maniax-build.md](maniax-build.md)
section 5 says: 64 forced byte-identical pairs disagreed with the alignment in
each game, all of them stubs in `[0x002DBCC0, 0x002DC2C0)` where only the
service number differs. Byte-identical pairs override the alignment, and a
shared masked hash is otherwise rejected, so the stubs are named by their bytes.

## Names in this IDB that DDS proved wrong

- The soft-float runtime. dds-decomp links its C against functions the compiler
  references by name, and the build is byte-identical, so its `dpcmp`, `dptofp`
  and `dptoul` are at the right addresses. The IDB names their SMT3 twins
  `dpdiv`, `sdfDoubleToULong` and `sdfDoubleToUInt`, which cannot be right for a
  routine the compiler calls as `dpcmp`. Not yet corrected in the IDB; see
  [idb-corrections.md](idb-corrections.md) for the bar a correction has to meet.
- `sdfAddHandler` carries its own name in the string
  `sdfAddHandler : unknown type %d`; the IDB calls it `DmacAddHandler`, which is
  the HD name. dds-decomp keeps the evidence name, and the port never replaces an
  evidence name.
- HD tags of the form `fcl_213998` and `moviePlayFile_27041744` are not names,
  and compiler markers such as `gcc2_compiled__7` are not functions. 116 of the
  paired DDS1 functions carry one of these and keep their dds-decomp name.

## Second stage: globals

`tools/dds_data_names.py` carries the data symbol names the same way, and it is
done: 352 globals on DDS1 and 356 on DDS2. The evidence is positional rather
than structural. In a pair of functions whose masked instruction streams are
identical, the instruction at a given index is the same instruction with the
same registers, so the object it addresses corresponds; each reference is a
vote, and only unanimous votes are accepted.

What limits the yield is not the evidence but the recipient. About 260 named
SMT3 globals per game vote for a DDS address that dds-decomp has not
symbolised, because splat emits the surrounding bytes as one `D_` blob. Those
are reported as `not-a-dds-symbol` and dropped: the pass renames objects and
never invents them. Splitting those blobs into real symbols would roughly
double what this evidence can carry, and is the obvious next piece of work.

Three globals could not be carried at all. ee-gcc's GCSE hashes a `SYMBOL_REF`
by name, so renaming a global can change a register allocation tie in a
function that reads it, and in `dds3Particle_SparkBuffInit`'s case the old
spelling is what the match was built on. The rules, the counts and those three
cases are recorded on the receiving side, in dds-decomp's
`docs/smt3-data-names.md`.

## Third and fourth stages: types and the VU0 macros

`tools/dds_type_names.py` carries type names across the bridge the function
pass built: a function named the same in both repos takes the same parameters,
so each position votes for a type pairing. 59 types landed in each game, the
same set, since dds-decomp shares one `include/` between the games. The gates
are four agreeing positions, four times the runner-up, agreeing pointer depth
and agreeing struct extents where both are declared. This is the method
[Type recovery](type-recovery.md) already uses inside this repo, pointed
outward.

`tools/dds_vu_macros.py` carries the VU0 macro names. The evidence is indirect
but solid: HD's transpiler could not inline the engine's macros, so its
metadata keeps each as a function named `SDF_VU0__<OP>`, and `include/ps2asm.h`
adopted those names. Two macros that expand to the same instructions with the
same operands and clobbers are the same operation, so the name carries; the
tool then requires the name to exist in the HD reconstruction, which stops a
contributor's variant suffix from travelling. 15 macros were renamed and 24
already agreed.

Neither pass can change either game's code. A type name and a macro name are
both gone before the compiler runs; only a struct's layout and a macro's
expansion reach it, and those are what the pairing proved equal.

What is left on the macro side is a reconciliation rather than a port: 108 of
dds-decomp's macros match no body here, and 195 of this repo's bodies have no
counterpart there. Each is an idiom one repo has written and the other has not,
or the same idiom under a different clobber list.
