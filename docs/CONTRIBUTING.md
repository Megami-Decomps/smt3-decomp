# Contributing

Anyone can contribute. This guide covers the whole loop for one function: pick
it, decompile it, verify it, and name it.

## Matching rules

The [dds-decomp matching rules](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/CONTRIBUTING.md#matching-rules)
are the canonical policy for this project and apply here in full, including
both their permissions and their evidence requirements. The linked policy is
the source of truth for every condition summarized below.

Plausible, ordinary source must compile to the exact retail bytes. Matching
establishes the reconstructed behavior and compiler properties; it does not
establish the original source spelling or variable names. In particular, the
policy permits:

- Ordinary C justified by a qualified trace of the real compiler, including
  live scratch scalar reuse across disjoint phases under its semantic-review,
  truthful-type and output-parity requirements.
- Evidence-backed unused aggregates explaining frame gaps, a statement after
  the final call that blocks a tail call, and dead code whose trace survives
  in retail, under the specific conditions in the canonical rules.
- Volatile VU0 scratch vectors and matrices under the canonical
  [bare-address hardware hand-off idiom](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/idioms.md),
  where only VU0 assembly observes their element stores.
- Real constant endpoints in floating-point interpolation when the algorithm,
  native operations and matched-family evidence meet the canonical rules.
- A one-pass block or forward branch when a compiler trace or matched sibling
  establishes the required block shape.
- Bounded search as a diagnostic aid. Reduce a hit to readable, ordinary C,
  explain the compiler property, and qualify it independently before landing.

These permissions retain the prohibitions on fakematches, register pinning,
artificial dependencies, false types, dummy uses and unsupported compiler flags.
A close reconstruction remains `INCLUDE_ASM` with its best draft preserved.

For SMT3, use the existing COP2/VU0 and MMI macros in `include/ps2asm.h` and
check the complete affected units with `tools/check_unit.py`, then the full
retail checksum build with `ninja`. An unchanged DDS donor does not need
donor-side requalification; changes made to DDS itself must pass its own checks too.

Before starting, read
[dds-decomp `docs/idioms.md`](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/idioms.md)
for confirmed source shapes, and [the local matching levers](matching-levers.md)
for SMT3 measurements. The [compiler trace guide](cc1probe.md)
explains `tools/cc1probe.sh` and output-parity qualification; a trace measured
on DDS is a lead until the relevant property is established for the SMT3 unit.

## 1. Pick a function

Every retail function belongs to a C unit under `src/smt3/`. A function that is
not done yet is an `INCLUDE_ASM(const s32, "<dir>/<unit>", NAME);` line, and
its assembly is in `asm/smt3/nonmatchings/<dir>/<unit>/NAME.s`.

The `main` segment is split into named translation units where the evidence
supports them, with `code_<vram>` placeholders for the rest. Preserve the
configured boundaries; see [TU recovery](tu-recovery.md). Sony SDK and runtime
units under `src/smt3/sdk/` use their configured compiler and remain outside
game-code progress; see [SDK matching](sdk-matching.md).

```sh
python tools/progress.py
```

reports how many functions and retail code bytes no longer use `INCLUDE_ASM`,
and, on a second line, how many of the functions that are left have a
non-matching draft. It does not verify matching; see
[Reading progress](progress.md).

## 2. Decompile

```sh
python tools/decompile.py FUNCNAME     # m2c draft with the unit's context
```

`decompile.py` runs through `tools/m2c_ee.py`. The adapter preserves the EE
compiler's eight-byte slots for known trailing scalar arguments: a twelve-word
packet call uses eight GPRs followed by stack offsets 0, 8, 16 and 24. It also
decodes raw `cvt.w.s` words in text sections so m2c retains their conversions
and dependent arithmetic. Data words and the input files are preserved. The
installed m2c dependency is unchanged; `tools/test_m2c_ee.py` checks actual caller,
callee and conversion output. Local unit types and provider signatures still
need to be supplied when the header-only context does not contain them.

Replace the `INCLUDE_ASM` line with your C. Keep functions in retail order, and
leave `INCLUDE_RODATA` and `INCLUDE_SDATA` lines where they are: `configure.py`
recomputes their placement.

- Float constants are literals. ee-as builds the unit's `.lit4` pool.
- String literals compile into the unit's own `.rodata`/`.sdata`. Keep
  `extern char D_X[]; /* "text" */` only when the unit check says the literal
  is shared, padded or merged.
- `switch` statements compile their own jump tables, and every entry is checked.

decomp.me works for single functions (compiler `ee-gcc2.96`, flags `-O2`);
`tools/m2ctx.py` writes the context. asm-differ reads `diff_settings.py`, and
decomp-permuter reads `permuter_settings.toml`.

When a function reproduces all but a few words, two references usually say more
than another permutation:

- [smt3-reconstructed-sources](https://github.com/smt-ret/smt3-reconstructed-sources)
  gives the real parameter list, the calls a body makes and in what order, and
  the field name behind an offset. The bodies are transpiler output with
  arguments often flattened to `0`, so they are a lead, not source. See
  [Names](names.md#using-it-on-a-function-that-will-not-match).
- [dds-decomp](https://github.com/Megami-Decomps/dds-decomp) already has C for
  part of this engine. `tools/dds_reuse.py` pairs it against the remaining
  `INCLUDE_ASM` functions; [the port notes](dds-port.md) cover how far the
  pairing can be trusted.

### Finding existing matching analysis

Optional per-unit findings live under [matching/](matching/README.md), alongside
[matching levers](matching-levers.md). They record compiler measurements and
source shapes already tried; a new draft does not need a page or index entry.
When editing those findings or the lever pages, run `python3 tools/docs_index.py`
to refresh their searchable indexes.

[Matching levers](matching-levers.md) is the list to work through when a
function is close but not exact, including the shapes that have been ruled out
and are not worth a second attempt.

Before starting a unit, claim it in `config/smt3/claims.tsv` and set the row
to `done` when you stop, finished or not. The file is advisory and nothing in
the build reads it; it exists so two people do not reconstruct the same unit
twice. Check it before you pick a unit.

### Non-matching C

Keep the C in its own unit with its assembly fallback in native order:

```c
#ifdef NON_MATCHING
void unfinished(void) {
    /* Reconstructed behaviour. */
}
#else
INCLUDE_ASM(const s32, "unit/path", unfinished);
#endif
```

The normal matching build selects the assembly. Builds defining NON_MATCHING
compile the C. The guard is the marking: no closeness comment, write-up,
per-function metadata or separate status is required. Non-matching C earns no
matching credit. Preserve retail's behaviour, including its bugs, and declare
every callee before first use through a header or its real prototype.

`python3 tools/check_nonmatching.py` compiles every guarded unit
with NON_MATCHING and SKIP_ASM. Implicit function declarations
are errors in this compile only. It checks compilation and nothing else.
`tools/revert_differing.py` also emits this plain form.

## 3. Verify

```sh
python tools/check_unit.py src/smt3/<dir>/<unit>.c [-v] [--func NAME]
```

The unit is clean when this ends in `N match, 0 differ`. Always check the whole
unit after an edit: a changed declaration, or even a new name, can change how
other functions in the same unit compile. Then run the build and the checks
used by CI:

```sh
ninja     # fails unless the ELF is byte-identical to retail
(for test in tools/test_*.py; do python3 "$test" || exit; done)
python3 tools/check_nonmatching.py
python3 tools/check_irx_nonmatching.py
```

A unit that checks clean on its own but breaks the build is usually a data
placement problem, not a codegen one. [Building](building.md) covers reading the
checksum failure.

## 4. Name

The ELF ships no symbols, so every name comes from somewhere else or from a
contributor.

```sh
python3 tools/names.py apply smt3 names.tsv
python3 configure.py --force-split      # so the assembly follows the rename
```

Each row is `<old name|0xADDR> <new> <evidence|inferred> <note>`. [Names](names.md)
explains the available sources and what counts as evidence. Keep an original name
whenever a source supplies one; renaming for consistency is a later job. Re-check
every unit you renamed in: a new name can change how neighbouring functions
compile.

## Permuter (optional)

For a draft that is close but holds a residual the source shape does not
explain, a search can tell you which change supplies the missing property.
`tools/perm_unit.py` runs [decomp-permuter](https://github.com/simonlindholm/decomp-permuter)
inside the real unit, so the candidate sees the unit's small-data, no-throw and
flag facts instead of a standalone file's.

```sh
export DECOMP_PERMUTER=../decomp-permuter       # default: next to this repo
python3 tools/perm_unit.py import src/smt3/<dir>/<unit>.c NAME draft.c
$DECOMP_PERMUTER/permuter.py build/permuter/smt3_NAME -j8 --best-only --stop-on-zero
```

`draft.c` holds your C for `NAME` (declarations ahead of it are kept), and the
unit must still have the function as `INCLUDE_ASM`. Score 0 means the candidate
assembles to the retail function including frame and local offsets.

A hit is a hint, not a landing. Reduce it to the smallest change a person would
write, then verify it the normal way: `tools/check_unit.py` on the complete unit
and `ninja`. Raw permuter output does not count as decompiled, whatever it
scores. Only mutations that keep source ordinary are enabled; `permuter_settings.toml`
serves the permuter's own `import.py` for the standalone flow.

## Naming and type conventions

These follow [smt-ret/smt3's contributor guide](https://github.com/smt-ret/smt3/blob/main/CONTRIBUTING.MD),
the existing SMT3 decompilation by the same community. Carried over so that
code and names can move between the two projects without a translation step.

Use the original names whenever possible.

### Primitives

| Type | Use |
|---|---|
| `s8` | byte values |
| `s16`, `u16` | signed and unsigned 16-bit values |
| `s32`, `u32` | signed and unsigned 32-bit values |
| `s64`, `u64` | signed and unsigned 64-bit values |
| `f32`, `f64` | single and double precision floating point |
| `char` | character arrays used for text |

### Structs

Struct types are typedef'd with a `_t` suffix:

```c
typedef struct
{
    ...
} foo_t;
```

### Enums

Enum members use the uppercase name of the enum as a prefix, separated by an
underscore:

```c
enum foo
{
    FOO_BAR,
    FOO_QUX,
};
```

### Functions

Publicly accessible functions take a short (3 to 4 letter) module prefix in
lowercase, followed by CamelCase:

```c
void fooBar(void);
```

Private (`static`) functions are TitleCase with no module prefix:

```c
static void Bar(void);
```

A function reached through a dispatch table keyed by an enum sometimes takes an
underscore suffix naming the entry (`fooBar_A`, `fooBar_B`).

### Variables

`camelCase` for variable names.
