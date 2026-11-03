# Compiler decision traces

`tools/cc1probe.sh` runs the pinned ee-gcc 2.96 compiler under GDB and logs
the decisions behind scheduling, allocation, CSE, reload and delay-slot
filling. It compiles through this repository's `tools/cc.sh`, including its
canonical unit paths, per-unit flags and pinned 32-bit runtime.

The probe and its GDB hooks come from
[dds-decomp](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/cc1probe.md).
The hooks use the same compiler binary in both projects. The wrapper here
uses the `smt3` target and `SMT3_*` environment variables. The linked guide
documents the decision families and how to read them; its DDS examples are
compiler evidence, not measurements of SMT3 functions.

## Requirements and use

Run `python3 tools/download_tools.py` first. Hooked mode requires GDB with
Python; `--nohook` does not require a debugger. Both modes refuse a compiler
whose SHA-256 differs from
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
It also refuses units assigned another compiler in `compilers.txt`; the hooks
are specific to 2.96. Use `check_unit.py` and the integrated build for the
older SDK compiler. It is diagnostic only; the normal build does not use it.

```sh
tools/cc1probe.sh src/smt3/kernel/dds3Randomize.c \
    --func dds3InitRandCB --all --out /tmp/smt3-rand-probe

tools/cc1probe.sh /tmp/candidate.c \
    --as-unit src/smt3/kernel/dds3Randomize.c --version smt3 \
    --func dds3InitRandCB --sched --alloc --no-dumps --out /tmp/smt3-candidate-probe
```

Choose the actual unit and function under investigation. A unit containing
assembly fallbacks needs the usual configured build inputs. `--cflag FLAG`
adds an experimental compiler option; this does not authorize changing the
normal unit flags. `--func` restricts logging, not compilation.

The default output is under `${TMPDIR:-/tmp}/cc1probe/`. `--out` selects a
disposable output directory, whose previous probe outputs are replaced. A
same-length mirror under the temporary directory preserves the path lengths
that can affect this compiler's heap. Use separate output directories for
concurrent runs.

## Qualify the observation

Every normal probe compares hooked assembly and every RTL dump with an
unhooked compile. When dumping is enabled it also compares the unhooked
`-da` assembly with plain `tools/cc.sh`, since dumping can itself change the
compiler's heap and decisions. Both compiler runs must succeed. Require the
final `verify: OK` result before citing a trace. `--no-verify` produces
unqualified diagnostics. `--nohook` only produces ordinary assembly and, unless
`--no-dumps` is selected, compiler dumps. It still validates the selected
compiler and its pinned hash, but produces no decision trace or `verify: OK`
result. Its exit status 0 means the compiler runs completed, not that dumped
and plain assembly matched: compare those outputs independently before
claiming parity.

If dumping changes code, rerun with `--no-dumps`. The log's instruction IDs
then describe the plain compilation, without corresponding RTL files.
Debugger breakpoints can also interfere with execution even when their
handlers only read memory; output parity must be established for every new
source, hook set and execution route. Keep the redundant
`fill_eager_delay_slots` entry hook disabled: DDS measured interference at
its adjacent return-hook boundary.

A trace citation records the command and parity result, deciding pass and
instruction IDs, the compiler property required, and the ordinary C that
supplies that property. Follow the [canonical matching policy](CONTRIBUTING.md#matching-rules).
The complete-unit check and full retail checksum remain necessary after a
source change; a neutral observation is not a matching result.

## Wrapper qualification

On `src/smt3/kernel/dds3Randomize.c`, all decision families ran with identical
hooked and unhooked assembly and RTL dumps. The same check passed without
dumps. This qualifies the wrapper on that unit, not every future trace.

## QEMU observation without GDB

The upstream DDS [standard-library observer guide](https://github.com/Megami-Decomps/dds-decomp/blob/648e39e3cdda433ec839232f5b4d65f7d8b8022a/docs/compiler-live-observer.md)
and [entry point](https://github.com/Megami-Decomps/dds-decomp/blob/648e39e3cdda433ec839232f5b4d65f7d8b8022a/tools/ee_gcc_observe.py)
provide allocation and scheduler observation through QEMU without GDB.
Use an upstream checkout at that revision; these tools are not vendored in
SMT3. This is not a drop-in replacement for all GDB probe modes, and operand
mode has not been qualified on SMT3.

Keep the compiler pin above unchanged. Freeze the actual wrapper-resolved
argv, working directory, environment and hash closure, including source,
headers, fallback assembly, configuration, tools and runtime. Require fresh
ordinary/direct plain/direct dump equality and fresh baseline/observed
artifact equality for each case, plus opcode, target-discovery and
completion guards. Use only supported Unix transport or strictly owned
loopback transport, never an all-interface fallback; verify child/listener
cleanup. An exit-zero `--nohook` run remains insufficient evidence of parity.

The `dds3InitRandCB` allocation smoke case qualified 138 inputs and 28 fresh
artifacts, with 85 events and 24 exercised hooks, not all 45 hooks. Later
[GroupCenter cases](matching/effect/effBattleMisc.md#later-qualified-scheduler-and-allocation-observations-2026-11-03)
each qualified 5,565 inputs, 28 artifacts and 34 exercised hooks. These are
case-specific observations, not blanket neutrality or matching claims.
