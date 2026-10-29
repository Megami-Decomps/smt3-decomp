# Matching notes: pipe/pipeMakaAnim

## makaDrawMulti (pipe/pipeMakaAnim, 0x494)

291 of 293 instruction words match at the native 1,172-byte size. Production
remains assembly-backed. The two residual words are +0x248 and +0x250: native
materializes zero in `$f4`, then copies it to the saved fade register `$f20` in
the phase-test delay slot. C materializes zero directly in `$f20` and leaves
the delay slot empty. No synthetic copy or fixed-register constraint is used.

The seven easing samples are copied into a local array. The object count is
subtracted from sixteen and narrowed to a signed byte; the amount is captured
before the blend-mode callback, then narrowed to the bar provider's unsigned
halfword input. Narrowing at each bar call preserves the native common value
lifetime. A separate halfword value assigned before the drawing branch hoists
the narrowing into the earlier fade dispatch and loses 117 matching words.

The routine changes blending mode from zero to one after its first bar draw,
then draws a ghost bar, the multi gauge and the expanded single gauge. Each
stage has its own native fade window and scale formula; the scalar opacity and
easing locals are reused across those stages. Alpha passes through unsigned
word conversion before the provider's byte narrowing. Callback-visible object
positions are read again for each bar; subsequent gauge origins are fixed.

A conditional expression in the ghost fade's active window emits 1,188 bytes. Independent phase-seven and phase-eight tests
retain the native size but change three words. The ordinary three-arm fade is
the preferred source for this still-unmatched function. The neighboring single
renderer is now matched with the localized constraint documented below; that
result does not establish a match for `makaDrawMulti`.


## makaDrawSingle (pipe/pipeMakaAnim, 0x3D0)

244 of 244 words match in `f6648fa8`: 976 bytes, complete owner 8 match / 0 differ with data
checks; the actual published commit rebuilt the exact retail image and a second
`ninja` reported no work, with a clean tracked tree and validated link inputs.

The ordinary reconstruction had five differing words: it assigned ghost
alpha zero directly to `$f20` and hoisted the phase comparison. Reusing the
finished easing scratch for the real zero endpoint, with a local nonvolatile
empty `"+f"` operand constraint, retains the native `mtc1` to `$f4` and `mov.s`
to `$f20` in the branch delay slot. The value is initialized after the first-bar
calls, consumed by alpha when phase is below seven, and overwritten before the
later gauge stage. There is no hard register binding or constrained call-crossing
lifetime. This is an SMT3 reconstruction aid, not original-source evidence;
the qualified stock diagnostic run preserved production code/data output.
