# Matching notes: pipe/pipeMakaAnim

## makaDrawMulti (pipe/pipeMakaAnim, 0x494)

293 of 293 words match in `5abcc45e`: 1,172 bytes, complete owner 9 match / 0 differ
with data checks. The actual published commit rebuilt the exact retail image;
a second `ninja` reported no work, with a clean tracked tree and validated link
inputs.

The ordinary reconstruction had two residual words: it materialized ghost-fade
zero directly in `$f20` instead of using `$f4` and a branch-delay-slot copy.
Reusing the finished easing scratch for the real zero endpoint, with one local
nonvolatile empty `"+f"` operand constraint, preserves the native sequence. The
value is initialized after `eglLinkAlpha(1)`, consumed by alpha when phase is
below seven, and overwritten before the multi-gauge stage. There is no hard
register binding or constrained call-crossing lifetime. This is an SMT3
reconstruction aid, not original-source evidence; the qualified stock diagnostic
run preserved production code/data output.

The seven easing samples are copied from native `D_00548FF8` into a typed local
aggregate. Retaining the imported table preserves its exact values and padding;
a literal local initializer would duplicate the imported data. The object count
is subtracted from sixteen and narrowed to a signed byte; the amount is captured
before the blend-mode callback, then narrowed to the bar provider's unsigned
halfword input at each call to preserve its native lifetime.

The routine changes blending mode from zero to one after its first bar draw,
then draws a ghost bar, the multi gauge and the expanded single gauge. Each
stage has its own fade window and scale formula. Alpha passes through unsigned
word conversion before the provider's byte narrowing. Callback-visible object
positions are read again for each bar; subsequent gauge origins are fixed.


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
