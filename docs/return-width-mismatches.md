# Return-width mismatches

A function defined with a narrow return type (`s8`, `u8`, `s16`, `u16`) but
declared `extern` with a wider type (`s32`, `u32`) at some call site. Retail
never shows it: the EE's own return sequence leaves the full `$v0` correctly
sign- or zero-extended regardless of what the caller's declaration says, so
every build on this hardware gets the right value no matter which type the
caller wrote down. A native port's host ABI makes no such promise: it only
guarantees the low N bits of the return register when the true definition
returns an N-bit type, so a caller that inspects the full register per its
own wider declaration can read garbage in the bits above it.

This was first found the hard way: `dds3AdminiGetPrevHistoryFlag` cost a day
of port debugging before anyone suspected a return type (see
`e5c2ddda`, which widened the definition instead of the caller, because
narrowing the one caller's declaration to match would have cost an extra
`andi` at its call site that retail does not have; see the comment in
`include/kernel.h`). A later audit swept every function whose definition and
some caller's extern declaration disagree on return width. That audit's
full list, and what reading the callers settled for each, is below.

## Method

Decide the correct spelling from the true definition, not the other way
around: find the function's own definition, read its declared return type
there, then check whether that matches every `extern` declaration seen by a
caller. Where a declaration is wrong, fix the declaration to the true type
rather than widen the definition. Widen the definition instead only when
fixing the declaration would itself change codegen (as with
`dds3AdminiGetPrevHistoryFlag` above); that is the exception, not the
default.

The real discriminator is whether any caller uses the value at all, not
whether the use looks like a sign check or a magnitude check or a boolean
test. A narrow return read through a wide declaration has undefined upper
bits on a native build. Nothing about `==`, `!=`, array indexing, or a bare
`if (x)` inspects fewer of those bits than a `< 0` sign check does; garbage
in the upper bits makes a logically-zero value test as non-zero (or the
reverse) under any of them equally. There is no safe "boolean-looking" use,
because the ABI does not know the caller only meant to ask a yes/no
question.

- **Dangerous**: the value is used by the caller in any way at all:
  comparison (`==`, `!=`, `<`, `>`), array indexing, storage into a wider
  field with no narrowing in between, or bare truthiness (`if (x)`,
  `if (!x)`). Whether the failure shows up as a wrong sign check or a wrong
  magnitude check is how it manifests, not what makes it dangerous; both come
  from the same undefined upper bits.
- **Safe**: either nobody reads the return value, or some C-level operation
  between the call and the value's use re-narrows it to the true width in a
  way the compiler must honor, e.g. assignment into a variable declared with
  the true narrow type, or an implicit truncation on return through a
  function whose own declared return type is the true narrow width.

**The first triage used the weaker criterion and was wrong on four of six.**
It separated dangerous from harmless by whether a caller's use "depended on
sign or magnitude," and judged six callers safe because they used the value
as a boolean or discarded it. A second pass over those six found four of
them dangerous: a boolean-looking `== 0` or bare `if (x)` is exactly as
exposed to garbage upper bits as a sign comparison is. The one case that
held up, `rstAddSkillToSlot`, is safe for an unrelated reason: its only
mismatched-declaration caller discards the return value outright, not
because its use was boolean. A future porter reading this should not repeat
that mistake: "the caller only checks truthiness" is not a reason to call a
mismatch safe.

Every fix below keeps the image byte-identical to retail
(`5facfbf7916836613449c922e26a4ec412207b32`), verified before, not after,
each commit.

## The audit's list

| Symbol | True type | Mismatched declaration | Dangerous? | Status |
| --- | --- | --- | --- | --- |
| `itfMesMngGetPhase` | `s8` (`itfMesManager.c`) | `extern s32` in `nbNegoProcess.c` | yes | fixed, `00dfde91` |
| `itfMesMngGetSelPhase` | `s16` (`itfMesManager.c`) | `extern s32` in `nbNegoProcess.c` | yes | fixed, `00dfde91` |
| `mdlPibGetEffectCount` | `u16` (`mdlEffect.c`) | `extern s32` in `mdlManager.c` | yes | fixed, `06d375b9` |
| `mdlPibGetMotionCount` | `u16` (`mdlEffect.c`) | `extern s32` in `mdlManager.c` | yes | fixed, `06d375b9` |
| `itfMesMngGetSelectNo` | `s16` (`itfMesManager.c`) | `extern s32` in `nbNegoProcess.c` and `fclMisc.c` | yes, worse than the rest: the `nbNegoProcess.c` result lands in `n->nowselect` (`s32`, no narrowing) and that field is later used as an array index (`NbNegoFlowMukan.c:632`), so the native failure mode is an out-of-bounds read, not just a wrong comparison. The `fclMisc.c` call site is harmless on its own (its result returns through `fclGetSelMessagePos`, declared `s8`, which narrows it on the way out), but the symbol still needed the fix for the dangerous site. | fixed, `ab2dbdcf` |
| `dds3BillboardGetAnimState` | `u16` (`billManager.c`) | `extern s32` in `dds3EffectObjectBasic.c` | yes, compared directly against `0` with no narrowing in between | fixed, `319214ea` |
| `dds3GetBasicArrayCount` | `u16` (`dds3WorldBasic.c`) | `extern u32` in `dds3AreaBasic.c`, `extern s32` in `dds3UnitObjectBasic.c` | yes, direct `== 0` comparisons in both files (three sites in `dds3AreaBasic.c`, one in `dds3UnitObjectBasic.c`) | fixed, `fe834247` |
| `dds3ObjectBaseCheckFlag` | `u8` (`dds3ObjectBase.c`) | `extern s32` in `fldTest.c` and `fldPlayer.c` | yes: `fldTest.c` compares directly against `0` (`!=`/`==`), `fldPlayer.c` tests it with bare truthiness | fixed: `fldPlayer.c`'s half landed, by a staging mistake, inside `319214ea` (that commit's message names only `dds3BillboardGetAnimState`); `fldTest.c`'s half is `472c96a2` |
| `evtCheckPolygonMovieFlag` | `u8` (`evtPolygonMovie.c`) | `extern s32` in `evtCommand.c` | yes, its one caller compares the result against `0` with `!=` | fixed, `55173e5a` |
| `rstAddSkillToSlot` | `s8` (`fclCombineInit.c`) | `extern s32` in `rstupdate.c` | no: the one cross-unit caller (`rstupdate.c:71`) discards the return value outright | not fixed, no fix needed |

Nine of the ten had a caller that reads the value and are now fixed by
correcting that caller's declaration to the true type; the tenth,
`rstAddSkillToSlot`, is safe only because its one mismatched-declaration
caller never reads the value at all, not because of anything about how it
would have used it. The commits above also hold the per-symbol reasoning and
the checksum confirmation.
