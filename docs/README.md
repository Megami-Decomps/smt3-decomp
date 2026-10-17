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
- [smt3hd-reconstructed-sources](https://github.com/smt-ret/smt3hd-reconstructed-sources):
  reconstructed headers and source file names from SMT3 HD.
- [smt-ret/smt3 `CONTRIBUTING.MD`](https://github.com/smt-ret/smt3/blob/main/CONTRIBUTING.MD):
  the naming and type conventions this repo follows.
