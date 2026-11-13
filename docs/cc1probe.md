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

## Stock dumps and exact-binary inspection

Stock dumps can settle a compiler question without an execution observer.
Capture the wrapper's actual source/output paths and resolved compiler and
assembler commands, then replay plain, dump and strict retained-C builds at
those paths. Compare complete assembly and raw assembled objects within each
context. Preserve production, `SKIP_ASM` and retained-C contexts separately;
do not normalize away differing instructions, data or relocations. Freeze the
source, generated inputs, compiler and actual runtime before and after.

The pinned 2.96 executable also retains STABS source lines, local names and
structure information. Static disassembly can identify an exact branch or
field in that executable. It cannot establish a runtime argument, cache hit
or intermediate instruction order. Correlate it with qualified stock output,
and state which facts remain unobserved rather than substituting a nearby
upstream GCC implementation.

### Empty constraints still spend scheduler issue slots

In the qualified `ActionZMotion` reconstruction, the first-product empty
operand has no functional unit and emits no opcode, but consumes the second
issue slot in a two-slot scheduler cycle. The already-ready offset store must
wait. In the next cycle the Y product's arithmetic critical-path priority
beats the store, leaving the two native instructions exchanged.

The pinned `schedule_block` decrements `can_issue_more` for that scheduled RTL
instruction. Its lack of emitted bytes does not exempt it from the issue
budget. Before adding another operand constraint, inspect the actual ready
queue, release times, issue budget and consumer priorities. A constraint can
fix one ordering and introduce another. Removing an operand that has become
redundant is a useful paired control; adding fabricated dependencies is not.

### Equal priority does not identify the deciding tie-break

The shaped-model packet's post-call return copy and VIF constant can be ready
together with equal priorities and successor sets. In the pinned comparator,
their dependence classes relative to the last scheduled call decide the
order before successor count or final instruction order: the true cost-one
return edge outranks the zero-cost anti-dependence. Post-reload comparison
skips the register-weight tier.

An actual return-value operand changed this case's release/issue sequence and
recovered the native VIF-before-copy order, while introducing other scheduling
differences. It remained nonmatching. This is a causal source lead, not a
general remedy or evidence of the original spelling. Similarly, work released
within a cycle can issue before the next sorted ready-list boundary; do not
infer a sorted tie from the final instruction order alone.

### Delay-resource masks need their actual provenance

The pinned call-resource collector can omit a call argument `USE` when its own
delay slot has a `SET` to an equal destination, subject to the compiler's full
RTL equality test. However, its forward dead/set scan checks `can_throw`
first. A call without `REG_EH_REGION` can stop that scan before either the
argument use or the slot definition is processed, including inside a
`SEQUENCE`.

The live-resource helper also uses cached masks and barrier-based block
discovery followed by simulation. A displayed basic-block live-in set alone
does not establish the mask used by an annul decision. The `frAnalyzeTag`
audit identifies these exact-binary paths but leaves the deciding mask,
cache state and intermediate order unobserved. It does not justify fake
liveness, a `nothrow` declaration, or moving a provider into the caller's unit.

For a different class of residual, the SDK compiler's first CSE changes
`sceCdStRead`'s literal-zero early return to a copy of the tested hard register
known to contain zero. That wrong native word exists before scheduling.
Paired stock dumps qualify this SDK observation independently; the 2.96
probe's hooks do not apply to the SDK compiler. Locate the first differing
pass before trying another scheduling control.

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

### A ready address can lose on priority, then dependent count

A qualified retained-C `fldAutoMapSeqStart` observation distinguishes three
causes inside its remaining scheduling window. The eye-address high half has
priority 8 and outranks the field-ID low half at 7. The already-ready camera
stack address has priority 6; it first loses to those higher-priority nodes,
then loses to the two field-ID stores at equal priority through the
comparator's dependent-count tier. Neither decision is a final source-order
tie. By contrast, the positive-zero materialization and eye-address low half
both have priority 7 and their comparison reaches the final instruction-order
tier. Moving pointer declarations indiscriminately therefore targets the
wrong mechanism for most of this window.

The observation qualified the ordinary wrapper, direct plain, direct dump
and observed compiler assembly and objects in the same retained-C context;
6,471 events covered 855 frozen inputs. It is evidence about this nonmatching
candidate, not a reconstruction of the original compiler's runtime decisions.
Determining the particular successor edge that supplies each critical-path
priority remains separate work. In this schedule, empty operands consume issue slots;
adding fabricated consumers is not a legitimate way to raise a priority.

On the configured ee-gcc 2.96 `-O2` owner with `NON_MATCHING` and `SKIP_ASM`,
the baseline remains 187/195 words at the native 780-byte size. Immediate
real camera-address operands did not improve it; pairing the eye and camera
operands fell to 184/195. A real field-base operand fell to 171/195, and a
position-base operand to 173/195. These are bounded negative controls, not
matching gains or permissions to fabricate dependencies. The unchanged
fallback remains required.
