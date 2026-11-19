# Matching notes: field/fldSky

## fldSkyDrawFilter (field/fldSky, 0xAD8)

Matches all 694 words at the native 2,776-byte size. A small ordinary
`static inline` helper evaluates the same signed blend for all 16
channels. The stock compiler inlines the helper, leaving exactly the
native order of temporary stack slots and all other instructions. The
table pointer is reloaded after calls to the draw routine, as in retail.

Adapted from Raikaru's DDS1 sky-gradient reconstruction in commit
`15fdee1692`, using the [published DDS source](https://github.com/Megami-Decomps/dds-decomp/blob/bd9c319dbe656fa4c440baa1b9984ca6df7f43e1/src/dds1/field/fldFileResolver.c#L5671).
SMT3 uses `fldAutoMapChkMode`, OT offset 0x39 and `aDrawBoxG`; its current and
previous filter indices, fade timing, integer channel extraction and alpha
scaling were checked independently against native code. The first three
corner alphas are clamped, but the fourth is not, for both filter grids.
The 292-byte record contains its mode and an 8-by-9 color grid; seven rows
of eight quads consume neighboring corners.

### Earlier 682-word draft and the three spill slots

Traced, both with `verify: OK`:

    tools/cc1probe.sh /tmp/base.c --as-unit src/smt3/field/fldSky.c --version smt3 \
        --func fldSkyDrawFilter --reload --no-dumps
    tools/cc1probe.sh /tmp/base.c --as-unit src/smt3/field/fldSky.c --version smt3 \
        --func fldSkyDrawFilter --gcse --no-dumps

`alter_reg` hands out stack slots in ascending pseudo number, four bytes at a
time from +0x60, so the three offsets are only a pseudo numbering. The three
pseudos are gcse PRE reaching registers: `pre_delete` replaces
`(mem:SI "fldSkyDataTable")`, `(ashift:SI (reg row) 3)` and
`(ashift:SI (reg row) 5)` with new registers it creates on first need, while
walking the expression hash table. The draft creates them in the order
`row*8`, `row*32`, table pointer; retail needs the table pointer first.

The walk is bucket order, and the bucket is `hash_expr` modulo the expression
table size, which gcse derives from the function's insn count at entry. Two
measurements separate the two inputs:

- The register number is not a lever. Shifting every local pseudo by 1 through
  16, by moving that many never-spilled locals ahead of `column`, leaves the
  `pre_delete` order and all twelve words unchanged, and so do all 48
  placements of the `fldSkyDataTable` declaration in the unit's extern block.
  The four spilled loop locals keep +0x60..+0x6C in every variant, so a
  uniform shift is invisible in the frame as well.
- The insn count is the lever, and it is not addressable. Dropping the
  `gFilterAlpha` block moves the fourth PRE register,
  `(high:SI "fldRunDanteGunFlag")`, from first to last in the same order,
  which is the table size rotating the buckets.

This did not make the rotation inherent: factoring the repeated
channel calculation into an inline helper changed intermediate compiler
behavior without changing the arithmetic or any other final instruction.
Applying the helper consistently to all 16 channels restores the native
sequence. The inline shape does not establish the original source spelling.

The native table pointer is reloaded after drawing callbacks. A function-wide
snapshot would lose that behavior and is not a replacement for the direct
global accesses. A per-cell table local leaves the same twelve differences.
Swapping row/column declaration order changes their own stack locations and
adds twelve differences without fixing the temporary slots. No frame padding,
false aggregate or compiler constraint is used.
