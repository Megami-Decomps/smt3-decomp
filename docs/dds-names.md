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

Through pairs whose masked instruction streams are identical, a data reference in
SMT3 (a `lui`/`%lo` pair or a `$gp`-relative access) sits at the same instruction
index as the DDS reference, so the named SMT3 global maps to a DDS address with a
vote per reference. Measured on 2026-10-22: 626 named SMT3 globals reach DDS1 and
625 reach DDS2 with unanimous votes, two or one conflicting. About half land on an
address dds-decomp has not symbolised and half on one of its inferred names. Not
applied; it needs the same provenance treatment as the functions and a check that
the DDS object has the same extent.
