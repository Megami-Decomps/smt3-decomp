# Contributing

Anyone can contribute. This guide covers the whole loop for one function: pick
it, decompile it, verify it, and name it.

## Matching rules

A function counts as decompiled only when it compiles to exactly the retail
bytes, and it has to reach that the way the original developers would have.

- **No fakematches.** Inline asm is allowed only for COP2/VU0 and MMI
  sequences gcc 2.96 cannot emit from C, kept small inside real C. Not
  allowed:
  - register pinning (`register x asm("$n")`)
  - computed gotos or label tables standing in for a `switch`
  - dummy variables, dead stores or `volatile` added to steer codegen
  - one-off inline wrappers that exist only to change codegen
  - self-cancelling statements (`x++; x--;`), `^ 0`, `* 1`, full-width masks,
    chained assignments that only share a register
  - reading a typed struct through a flat word array or cursor
  - permuter output landed as it came out. A decomp-permuter hit is a hint:
    reduce it to the smallest change that still matches, land it only if it is
    code a person would write, and say in the commit message what the change
    gives the compiler. `tools/perm_unit.py` runs the permuter inside the real
    unit with only those kinds of mutations enabled.
- **Plausible source.** Use structs for recurring layouts, real types (no `u64`
  unless the value is 64-bit), meaningful parameter and local names, and
  natural loops and switches.
- **Compiler flags need evidence.** Every unit builds with `-O2 -G8` until a
  contiguous run of functions in one file is shown to need something else. One
  function that matches under some option is not evidence.
- **Close is not matched.** A near miss stays `INCLUDE_ASM`. Save its best C
  outside the build with a note on the remaining difference.

Narrow exceptions exist for frame gaps from unused locals, a statement that
blocks a tail call, and dead code that retail still shows. They are written up
with their conditions and required citations in
[dds-decomp's matching rules](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/CONTRIBUTING.md#matching-rules);
the same rules apply here. Before starting, read
[dds-decomp `docs/idioms.md`](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/idioms.md),
which lists the source shapes confirmed to produce specific ee-gcc 2.96
sequences on this engine.

## 1. Pick a function

Every retail function belongs to a C unit under `src/smt3/`. A function that is
not done yet is an `INCLUDE_ASM(const s32, "main/code_<vram>", NAME);` line, and
its assembly is in `asm/smt3/nonmatchings/main/code_<vram>/NAME.s`.

`.text` is currently one `main` segment split into `code_<vram>` units: no original
file boundaries have been recovered in this repo yet. See [Names](names.md) for
what is known about the original file names and why they are not used as unit
names yet. The Sony SDK libraries are prebuilt archives and stay assembly.

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

[Matching levers](matching-levers.md) is the list to work through when a
function is close but not exact, including the shapes that have been ruled out
and are not worth a second attempt.

Before starting a unit, claim it in `config/smt3/claims.tsv` and set the row
to `done` when you stop, finished or not. The file is advisory and nothing in
the build reads it; it exists so two people do not reconstruct the same unit
twice. Check it before you pick a unit.

A function that comes close but does not match belongs behind
`#ifdef NON_MATCHING`, which `tools/revert_differing.py` does for you, with
one line in the source and the detail in
[Non-matching drafts](nonmatching.md). See
[when to stop](matching-levers.md#when-to-stop-and-keeping-the-draft).

## 3. Verify

```sh
python tools/check_unit.py src/smt3/main/code_<vram>.c [-v] [--func NAME]
```

The unit is clean when this ends in `N match, 0 differ`. Always check the whole
unit after an edit: a changed declaration, or even a new name, can change how
other functions in the same unit compile. Then build:

```sh
ninja     # fails unless the ELF is byte-identical to retail
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
python3 tools/perm_unit.py import src/smt3/code_<vram>.c NAME draft.c
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
