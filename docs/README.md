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
- [VU0 constructs](vu0-constructs.md): the vector and MMI operations the
  assembly macros cover, and what an equivalent has to preserve.
- [Naming drift](naming-drift.md): the audit of member, enum, define, function
  and global names against the IDB, and the queue of fixes it found.
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
