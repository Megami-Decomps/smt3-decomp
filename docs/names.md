# Names

`SLUS_209.11` has no `.symtab` and no `.strtab`, and a scan of its strings
finds no `__FILE__` paths or any other `.c` filename. Every name in this repo
therefore comes from another build of the game or from a contributor.

Struct, union and enum types have their own page, [Type recovery](type-recovery.md).
The audit of member, enum, define and function names against the IDB is in
[Naming drift](naming-drift.md).

## Sources

| Source | What it gives | State |
|---|---|---|
| SMT3 HD symbol recovery, applied to the PS2 IDB | 9,470 of 9,571 function names at PS2 addresses | seeded into this repo |
| [smt3-reconstructed-sources](https://github.com/smt-ret/smt3-reconstructed-sources) | 272 headers, 647 source file names, struct layouts and field names | not imported yet |
| Nocturne JP debug build `SLPM_652.42` (Dec 26 2002) | `__FILE__` paths and the code that references them | no copy in this repo |

### SMT3 HD symbol recovery

SMT3 HD is a transpiled port that ships il2cpp metadata, so its function and
type names survive. Recovering them and carrying them back to the PS2 build is
what makes SMT3 unusually well named for a stripped PS2 binary: the IDB for
`SLUS_209.11` names 9,470 of its 9,571 functions, with the engine's original
module prefixes intact (`dds3*`, `sdf*`, `btl*`, `fld*` and so on).

`tools/ida_symbols.py` exports them from the open IDB into
`config/smt3/symbol_addrs.txt` in splat's format, read-only on the IDB side. They are good
names, but they are not symbols read out of this ELF: the mapping from the HD
build back to PS2 addresses is a recovery step, and a wrong pairing produces a
confidently wrong name. Record each name's provenance and treat anything that
is not a name this binary carries for that function itself as inferred.

### Reconstructed sources and headers

`smt3-reconstructed-sources` reconstructs the original source tree from the
same HD metadata: 272 headers under `include/`, 647 `.c` files under `src/`,
and a `symbols.txt` address map of 11,595 functions with the source file each
belongs to. That file map is the best available list of the game's original
translation unit names (`ScrScriptProcess.c`, `rbRingBuf.c`, `sdfTmx.c`,
`fldTitle.c` and so on), and its headers are the best starting point for struct
layouts and field names.

Its addresses are HD build addresses, not PS2 addresses, so the file map does
not place a PS2 function in a file by itself. Pairing the two is open work.

The repository was renamed from `smt3hd-reconstructed-sources`; the old URL
still redirects.

#### Using it on a function that will not match

The bodies are transpiler output, not source. Call arguments are often
flattened to `0` and some control flow is gone, so a body cannot be copied.
What survives is worth checking whenever a reconstruction is close but not
exact:

- the real parameter list, where the assembly only shows registers,
- which functions the body calls, and in what order,
- which field name an offset corresponds to, as a second opinion before adding
  a field to a shared header,
- which translation unit the function belonged to, by grepping `src/` for its
  name.

The IDB stays authoritative over all of it, and a name or layout this ELF
proves wrong is wrong here too. Treat it the way
[the dds-decomp pairing](dds-port.md) is treated: a lead to verify, never
evidence on its own.

### The Nocturne debug build

A JP debug build of Nocturne, `SLPM_652.42`, dated Dec 26 2002, keeps
`__FILE__` strings and the references to them. dds-decomp used it to recover
172 file paths with 1,478 code references, and projected those names onto the
USA retail build and from there onto DDS. Those projections are recorded in
[dds-decomp `docs/tu-names.md`](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/tu-names.md),
including the file names and directory layout it recovered for this exact ELF.

This repo does not have a copy of `SLPM_652.42`, so none of that has been
re-derived here. Anyone who has that build can recover real unit boundaries for
`SLUS_209.11` directly, which is the single highest-value missing input.

## Unit names today

**No translation unit boundary recovery has been done in this repo.** `.text`
is one splat segment, and its C units are named `code_<vram>` after the first
address in the unit. Those names carry no claim about the original source
layout: a `code_<vram>` unit is a chunk of the address space, not a file.

Changing that needs proven boundaries, from the debug build's `__FILE__`
references or from a solid HD-to-PS2 function pairing. Until then, do not
rename units after reconstructed file names: a wrong boundary silently moves
`.rodata` and `.lit4` ownership between units and makes later splits harder to
verify.

## Tools

```sh
python3 tools/ida_symbols.py              # IDB names -> config/smt3/symbol_addrs.txt
python3 tools/ida_types.py                # IDB types -> config/smt3/ida_types.txt
python3 tools/check_names.py              # tree type names vs that file
python3 tools/inline_strings.py --list    # string placeholders still convertible
python3 tools/names.py apply smt3 names.tsv
python3 tools/find_tus.py smt3            # TU evidence from the split assembly
```

`tools/find_tus.py` collects translation unit evidence out of the split
assembly itself, without any other build: shared string literals, jump tables
and `.lit4` entries put functions in the same unit, and a repeated string
proves a boundary. It writes `build/smt3/tu_evidence.txt`. That is the path to
real unit boundaries that does not need `SLPM_652.42`, but the evidence has not
been collected or acted on in this repo yet.

## Recording provenance

Keep two classes of name apart:

- **evidence**: the binary itself names the function, in a message the function
  carries (`sdfAddHandler : unknown type %d`). None has been harvested from
  `SLUS_209.11` yet.
- **inferred**: everything else, including every HD-recovered name, every name
  taken from the reconstructed sources, and every contributor's description.

`tools/names.py apply` records both: the name goes into
`config/smt3/symbol_addrs.txt` as a curated row, which wins over the generated
block, and its provenance into `config/smt3/name_sources.txt`. Rename C
references with the same command, then run `python3 configure.py --force-split`
so the assembly files follow, and re-check the units you renamed in.

Conventions for new names are in
[Contributing](CONTRIBUTING.md#naming-and-type-conventions).
