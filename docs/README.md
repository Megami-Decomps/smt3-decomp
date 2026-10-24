# Documentation

See the [project README](../README.md) for the supported version and the
quickstart.

## Code reconstruction

- [Contributing](CONTRIBUTING.md): the matching workflow, matching rules, and
  naming and type conventions.
- [Toolchain](toolchain.md): the original compiler and assembler, the evidence
  for them, and the pinned 32-bit runtime.
- [Names](names.md): where the function, variable and file names come from, and
  what is not yet recovered.
- [Type recovery](type-recovery.md): how a repo struct is matched to the
  recovered type it stands for, what has been renamed, and what is still
  invented.
- [IDB corrections](idb-corrections.md): names and layouts in the IDA database
  that the retail code proves wrong, and the proof for each.
- [Building the IRX modules](irx-modules.md): how the three IOP modules in
  `USERIRX/` build byte-identical from source, the container format, and where
  to put C.
- [The SDF IRX modules](sdf-irx-modules.md): the three IOP modules in
  `USERIRX/`, the sound driver's command table and work area, the two SIF RPC
  services the EE hosts for them, and the .SMG and ADPCM headers.
- [The Japanese builds](maniax-build.md): how `SLPM_654.62` and `SLPM_666.81`
  compare to retail, how the USA names were carried onto them, and what the two
  builds are and are not worth.
- [VU0 constructs](vu0-constructs.md): the vector and MMI operations the
  assembly macros cover, and what an equivalent has to preserve.
- [VU microcode](vu/README.md): the VU1 and VU0 microcode, assembled from the
  annotated listings in `src/smt3/sdf/vu/` rather than included as a blob, plus
  the pseudo-C listing, the hand transcription of all 1884 VU1 instructions, and
  the data memory the upload packet seeds.
- [Naming drift](naming-drift.md): the audit of member, enum, define, function
  and global names against the IDB, and the queue of fixes it found.
- [HD alignment](hd-alignment.md): parameter names taken from the IDB
  prototypes, and type and member names recovered by reading our function
  bodies against SMT3HD's, with the checks that turn a report into a rename.
- [SDK and C runtime](sdk-matching.md): the 126,120 bytes of Sony library,
  newlib and libgcc code at the end of `.text`, which archive object each
  function came from, and the compiler that built them.
- [Matching levers](matching-levers.md): what to change in the source when a
  function is close but not exact, and what has been ruled out.
- [Non-matching drafts](nonmatching.md): the reconstructions kept behind
  `#ifdef NON_MATCHING`, how close each is, and what has been ruled out.
- `config/smt3/claims.tsv`: who is working on which unit, so the same one is
  not reconstructed twice.
- [Reading progress](progress.md): how progress would be measured and what
  counts.

## Builds

- [Building](building.md): requirements, build targets, generated files, and
  checksum troubleshooting.

## External references

These live in sibling projects rather than here, because the work was done
there and SMT3 is the engine DDS was built on:

- [dds-decomp `docs/idioms.md`](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/idioms.md):
  source shapes confirmed to produce specific ee-gcc 2.96 output.
- [dds-decomp `docs/compiler-decision-atlas.md`](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/compiler-decision-atlas.md):
  mismatch diagnosis and stop rules.
- [smt3-reconstructed-sources](https://github.com/smt-ret/smt3-reconstructed-sources):
  reconstructed headers and source file names from SMT3 HD.
- [smt-ret/smt3 `CONTRIBUTING.MD`](https://github.com/smt-ret/smt3/blob/main/CONTRIBUTING.MD):
  the naming and type conventions this repo follows.
