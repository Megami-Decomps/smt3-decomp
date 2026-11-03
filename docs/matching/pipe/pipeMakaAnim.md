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

## makaDrawNormalGauge (pipe/pipeMakaAnim, 0x2E0)

110 of 184 words match, at the native 736-byte size. This remains a
`NON_MATCHING` draft and retains its assembly fallback. The former volatile-array
draft emitted 720 bytes, with 18 of 180 prefix words matching. The historical owner
was `code_0029B780`, not `code_001FAE80`.

The ordinary-source reconstruction separates the real phases: capture the two
floating-point origin coordinates, build cells and positions, convert total width,
then compute sprite dimensions and color before drawing. The two-element origin
array holds only the actual x/y values and occupies the native stack slots; no
padding or unused entries were added. The existing cell and position extents are
unchanged. No compiler constraint or volatile RAM is needed.

Both native loops enter their bodies before testing the decremented index. This
is safe for every u16 value: the matching `makaGetBarWidth(s32)` provider returns
0..5. Index zero is visited, including when value is zero. The matching
`makaGetBarCell(s32,s32)` provider returns the cell value −1 for digit zero;
`cells[0]` then becomes 0. The subsequent index −1 is only the termination test,
never an array subscript. Digits 1..5 use positive divisors 1 through 10000.

Ordinary cells preserve the native initial byte store, returned-low-byte signed
comparison, and later unsigned/signed byte reloads for increment and previous
cell. There is no intervening call or escaped local-array pointer. Qualified
compiler dumps show the initial branch reload forwarded from the return value
between jump and CSE snapshots, while the later load survives; combine represents
its unsigned and signed uses as separate memory observations. A low prefix score
alone would not establish which loads survived.

The total-width expression retains the native float addition and float→u32→u16
conversion, before the later sprite-width multiplication and height calculation.
This ordering restores the native repeated multiplication across the unsigned
conversion branch. It introduces no input-range assertion or portable guarantee
for out-of-range float conversion. Position accumulation remains the same u16
wrapping behavior, with the original target-dependent s16 narrowing in the
position array. Provider ABIs, descending sprite-call order, arguments and texture
lookup are unchanged.

Bounded controls in the same full owner:

- Ordinary cells alone: 704 bytes, 9 of 176 prefix words.
- Explicit signed returned-cell snapshot: 704 bytes, 6 of 176 prefix words.
- Body-first loops, scalar origins and late color: 728 bytes, 42 of 182 words.
- Actual two-float origin pair: 728 bytes, 46 of 182 words.
- Total-width conversion before sprite-dimension locals: 736 bytes, 110 of 184 words.

All controls retain nine exact C peers and have no owned-data diagnostics. Plain
and diagnostic builds agree in normalized assembly, every allocated section,
non-file symbols and ordered relocations. The 90 shared dependency fingerprints
remain stable during measurement: 76 headers, five native assembly files, five
generated assembly files, the canonical owner source and three configuration
files. The private candidate sources intentionally differ. The final ordinary source
still differs in 74 words, including saved-register roles and initialization,
size/color/coordinate scheduling. It is not an exact match or a single residual
allocation tie. No register grid or volatile substitute was attempted.

## makaDrawBar (pipe/pipeMakaAnim, 0x2F8)

143 of 190 words match at the native 760-byte extent; the draft remains guarded.

The retained draft now uses the actual two-float origin, ordinary s8 cell and
s16 position arrays, and body-first build/draw loops. Signed conversion and
signed halfwidth behavior differ from NormalGauge and remain intact. A single
empty operand on the initialized 20-pixel s16 spacing retains a separate
conversion tail: qualified whole-owner RTL shows jump2 merging it without the
operand, while retail keeps it separate from the 14/16 path. This is a local
compiler reconstruction aid, not original-source evidence. It has no fixed
register, memory barrier or call-spanning binding.

Target-only output is 143/190 words at the native 760-byte extent. The 47-word
residual remains guarded and earns no matching credit. All 9 exact peers and
owned data remain intact; NormalGauge stays 110/184 at 736 bytes. Three strict
compilation modes pass, and final plain/dump parity plus unchanged production
object contracts were checked.

The full u16 domain gives width 0..5; digit 0 is the -1 cell value, not a negative
index. Both arrays retain their actual 16 elements. Signed narrow step/position,
LB/LBU observations, the 13-argument sprite ABI, and per-call MakaTex[1].th reloads
are preserved. The inspected callers use scales 1.3..1.8 and normal negative-maka
animation coordinates; arbitrary scales, malformed caller lifecycle and
out-of-range float casts remain outside the safety claim.

Final integration preserves both guarded targets and all nine exact peers.
Production and retained-C plain/dump parity, three strict modes, full retail
byte comparison/checksum/no-op, 450 tests, 817 layout assertions, fresh stable
141-owner/481-draft qualification and seven IRX checks pass. Reports are
unchanged; this batch claims zero exact functions or bytes.
