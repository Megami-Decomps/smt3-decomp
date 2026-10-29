# Automap input and view update

`field/fldAutoMap::fldAutoMapCalc` is 1,588 bytes at `0x00149290`.
The source adapts `fldUpdateAutomapInput` in
`Megami-Decomps/dds-decomp/src/dds1/game/code_001411F0.c`, credited to
**Megami-Decomps/dds-decomp / redthing1**. The donor complete-file SHA-256 is
`e5e3c3c88c1b9042f7de5428158be0163fa633856b74155ab634046591c66415`.
SMT3's native body, rather than the donor title's behavior, determines each
port difference.

## Input and geometry contracts

- Inactive automap state returns immediately. A close request does not advance
  the input-delay counter. Exit/reset button handling, sound, restored center
  coordinates and sequence state retain the native callback ordering/reloads.
- Stick-repeat and button-repeat paths retain distinct conditions. A held
  diagonal consumes the direction choice even without its Z-repeat bit; it
  must not fall through into a cardinal step. The existing button bytes and
  the kernel's real 2-by-2 stick-status array are used without invented flags.
- SMT3 moves and quantizes in **1,000-unit** steps, not DDS's 600. The existing
  `fldAutoMapGetBBox` provider's four outputs are min-X, max-Z, max-X, min-Z.
  Native endpoint subtraction/addition, float division, integer quantization
  and all four independent float comparisons are preserved, including the
  original signs on the Z endpoints. No integer-only clamp or new bounds
  policy is substituted.
- Eye/focus/up occupy native frame offsets `0x10`, `0x00`, `0x20`; the four
  actual bounding-box outputs occupy `0x30` through `0x3C`. Together with
  saved registers these reproduce the `0x60` frame. The up vector copies the
  existing `D_00532240` value `{0, 0, -1, 1}`. Eye/focus W lanes are not newly
  initialized: the implementation preserves the native three-component writes.
- The native eye height is `-24000.0f`, overridden by `-60000.0f` when button
  byte `0x2B` is nonzero. This SMT3-only branch is absent from the DDS donor.
  Both heights and `20000.0f` reproduce this target's literal-pool words.

The existing `SDF_VU0_COPYVECTOR` primitive copies the actual aligned quadwords
and carries its existing memory clobber. The six copies feed the automap's
three vector records and then the real `aViewEye`, `aViewTarget`, `aViewDown`
records; no new assembly or memory-operand convention is introduced.

## Existing record and storage owners

The light/fog record at `dds3LightParam2 + 0x20` already has the unit-local
`fldAmLightParam_t` view used by `fldAutoMapSeqStart`. Its four floats and color
word retain their actual offsets. The reconstruction sets 255, 1000, 255,
20000 and `0x808080` in the donor's ordinary field-writing sequence; it neither
splits this owner into artificial globals nor changes its layout.

`fldAutoMapEnable` is a real standalone four-byte word at `0x002FC118` in the
native `.data` section (`asm/smt3/data/data.data.s`). Its declaration states
that existing storage section, following other native `.data` flags in this
project. It is not declared as a fabricated array to influence code generation.
Fade and sound declarations match the existing one-argument fade and
three-argument sound providers. No shared header or provider is changed.

## Qualification

Starting from public source baseline
`0f799620a87bd2b4daae393a5260aebe6de5efc6`:

- The first substantive native-adapted donor port gives 33 match, 0 differ.
  The final Calc/Proc family gives **34 match, 0 differ**, including the
  target's literal pool and existing data.
- All five unrelated guarded drafts compile and remain assembly-backed.
- Full retail SHA-1: **`5facfbf7916836613449c922e26a4ec412207b32`**.
- Plain and ordinary `-da` assembly are identical after normalizing only the
  temporary initial `.file` line. This is not a hooked compiler trace.

`fldAutoMapProc` (72 bytes at `0x0014A1A0`) independently closes with the real
Calc definition visible. Its published C body is unchanged: two field-ID
bounds checks and the sequence-state gate remain exactly as before. Only its
old guard/comment/ASM fallback are removed. The previous three branch-annul
residuals disappear without a fake callee, extra provider definition, prototype
trick, or otherwise modifying that consumer. Both functions are qualified
together: **2 functions / 1,660 native instruction bytes**.

No arbitrary source-order grid, register pinning, dummy dependency, compiler
option change, layout padding or unrelated fallback promotion was used.
