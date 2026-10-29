# Non-matching drafts: field/fldCamera

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## resetCamNormal (field/fldCamera, 0x374)

208 of 219 words differ, at 876 bytes against 884, but every instruction after
the head is identical: the body sits at a uniform 5-word shift, and the only
real difference is that retail rematerialises three values inside the OOYDEG
block that ee-gcc shares with the rest of the function.

Retail computes `&fldGlobalBase` twice, once into `$2` for the two `camAngle`
stores and again into `$16` after `dds3GetPosPoseSet`, and materialises
`180.0f` and `3.14f` twice each, which is why the unit's `.lit4` pool holds
`3.14f` at both `0x0054C2C0` and `0x0054C2C4`. The draft emits one address
pair in the prologue and one of each constant in `$f20`/`$f21`, so it runs
five words short in the head and three nops long in the tail.

The pool count matters beyond this function: a draft that emits one `3.14f`
where retail has two shifts every later entry, so this has to stay behind
`INCLUDE_ASM` until the split is reproduced.

Ruled out: a `fldAreaWork_t *gb` local covering the post-STOREVECTOR half, the
same local covering the `camAngle` half instead, `-fno-gcse`,
`-fno-cse-follow-jumps`, and an `__asm__ volatile ("" ::: "memory")` barrier
between the `camAngle` stores and the call. None of them splits the pseudo,
which puts the mechanism in cse running over the function's single basic block
rather than in anything the source can state.

Confirmed on the way: the frame is exactly seven `sdfVector4_t` plus the six
saved GPRs and three saved FPRs, the prologue and frame size are exact, and
`fldHitChkCamera` takes its two vectors as pointers to fresh `lq`/`sq` copies
with the target vector first.

## resetCamNormal2 (field/fldCamera, 0x360)

207 of 215 words differ, at 860 bytes against 864. The same function one
statement group lighter: it writes only `camAngle`, carries no
`fldHitChkMatter` or `fldHitChkSlope` reset, and lerps `fldCamDist` toward
`fldCamDirLock` by `0.0625f` instead of adding it. It fails for the reason
above, with one `3.14f` where retail has `0x0054C2C8` and `0x0054C2CC`.
Matching `resetCamNormal` should close this one unchanged.
