# Matching notes: sound/sndManager

## dds3TitleDrawFade (sound/sndManager, 0x4F8)

318 of 318 words match: 1,272 emitted and native bytes at `0x00226EC0`.
The complete owner check passes all 14 C functions and its owned data.
Source commit: `224abeee0e9ef9e6367c74bc6cf534de158535db`.
The exact-source local gates also pass: retail checksum/provenance, no-op build,
149 guarded owners (612 drafts), seven IRX modules, `test_cc`, and 26 Python
tool suites (427 unittest cases).
Hosted [CI run 40949890330](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40949890330)
passed for this exact source commit, including build verification, source
reports/scope checks, site build and deployment.

The ordinary C uses one live `age` scalar for the successive clamped and
periodic time calculations, and one `alpha` scalar for the initial box opacity
and later sprite opacities. Each value has a real draw consumer; there are no
constraints, dummy operations or dead computations. Color packing remains an
inline `((u32)alpha << 24) | 0x00808080` argument. The counter increment uses
explicit unsigned wrapping before conversion back to `s32`.

The two animated sprite phases have independent guards. After the first draw,
reload `titleWork->unk_30` before testing the second phase: a callback can change
both the pointer and counter. Preserve the intervening calls and texture loads.
Within each periodic branch, `age -= 0x78; age %= 0x42;` retains the native
subtraction/remainder def-use chain rather than a separate remainder scalar.

The first periodic rise is `0x19 + age * 0x66 / 0x13`. The old non-matching
source draft incorrectly used `0x33`; retail's shift/add chain computes
`3 * age * 17 * 2`, or `102 * age`. This corrects the draft to retail behavior,
not the game. The other phase's `0x33` multiplier is distinct and remains intact.

The already-matched `dds3TitleDrawMenu` sibling supplied useful examples of
real scalar reuse, signed arithmetic and packing, but is a separate existing
match. Its retained dead division was not copied: this target needs none.
