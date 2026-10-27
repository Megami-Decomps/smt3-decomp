# Non-matching drafts: sdf/sdfQuat

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## FromEulerXYZ (sdf/sdfQuat, 0x118)

60 of 70 words match, at the exact 280-byte size.

## FromEuler (sdf/sdfQuat, 0x114)

59 of 69 words match, at the exact 276-byte size.

Both are the same function with and without the `sdfClipAngleValuePI` clamp on
each input angle, and both leave the same ten words. Every instruction is
retail's; what differs is which fp register each of the four quaternion
components lands in, and the store order that follows from it. Retail numbers
them `$f0` x, `$f20` y, `$f2` z, `$f3` w and stores `0`, `4`, `8`, `0xC` in
index order; the draft gives z `$f1`, which makes the scheduler emit the four
`swc1` as `8`, `0`, `0xC`, `4`.

Confirmed on the way: each half angle is a single reused `f32` local, not three,
which is what gives retail one callee-saved register holding the value across
both the `sdfCos` and the `sdfSin` call and then reusing it for the next axis;
`0.5f` is one pseudo kept in a callee-saved register for the whole function,
which the three `* 0.5f` uses CSE into; all three angles are negated at entry;
and the components are

    x = sz * sy * cx + cz * cy * sx
    y = cz * sy * cx - sz * cy * sx
    z = sz * cy * cx + cz * sy * sx
    w = cz * cy * cx - sz * sy * sx

with the four pair products `cz*cy`, `cz*sy`, `sz*cy`, `sz*sy` left to CSE and
each triple grouped left to right, which is retail's `mul` order exactly.

Ruled out: all 24 orders of the four component assignments (the best is index
order, and 2, 0, 3, 1 - the order retail's adds complete in - is eight words
worse); explicit `czcy`/`czsy`/`szcy`/`szsy` locals for the pair products, which
costs 23 more words by pinning them to callee-saved registers; and `cx` written
as the first factor of each triple, which costs 29.
