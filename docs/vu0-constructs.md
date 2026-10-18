# The VU0 and MMI constructs, and what a port has to preserve

`ps2asm.h` and the units define 246 distinct assembly macros. They collapse to
about 40 distinct operations: the same transform, cross product, normalize and
colour unpack recur under different register pairs. This page lists those
operations and records the places where the obvious C equivalent is wrong, so a
port translates whole operations rather than instructions and still gets the
same numbers.

Nothing here is a port. The macros stay as assembly in this repository; this is
the inventory a port would work from.

## The state the macros share

Across every macro the state touched is `vf0`-`vf16`, `vf20`-`vf31`, `ACC`, `Q`,
`$vi18` (the clip register) and `$vi22` (`Q` read back as an integer). No VU0
integer register, no VU0 data memory, no `vcallms`: the game drives COP2 in
macro mode from the EE and never runs a VU0 microprogram.

`ACC` never stays live past a macro's last instruction, checked across all 246,
so it is internal to each operation. `Q` is not: `VU0_CLIP_RATIO_X` in
`code_002C6F38.c` leaves it pending and the paired
`VU0_CLIP_SEGMENT_UPDATE_FIRST` consumes it after a `vwaitq`. `vf12` and `vf2`
carry vectors across calls, and `vf1` is written in one function and read in
another, so the register file is shared state and not per-function.

## `vf1` is read by code that does not set it

`VU0_TRIANGLE_FACING` and `VU0_INTERPOLATE_CLIP_VERTEX` in `code_002B5CD0.c`
read `vf1` without writing it. `VU0_TRIANGLE_FACING` finishes a dot product with
`vmaddz.x vf2, vf1, vf2z`, which needs `vf1.x` to be 1; `vf0.x` is hardwired to
0 and cannot serve. The producer is `sdfCorePacketGen`, still hand-written
assembly, which sets `vf1 = (1, 1, 1, -1)`:

```
vaddw.xyz $vf1, $vf0, $vf0w    # xyz = 0 + 1
vsubw.w   $vf1, $vf0, $vf0w    # w   = 0 - 1
```

Anything that runs those two macros without that initialisation drops the z term
of the dot product and reports the wrong facing. The `vf1` uses in
`code_002BD340.c` are unaffected: each of those blocks sets `vf1` itself first.

## Every macro parameter is a value

No macro takes a register name, an axis letter or an assembly string. Each
operation is named for the registers it touches, so a C equivalent is a function
per name with no preprocessor pasting, and `grep vf12` finds every piece of code
that touches `vf12`.

21 macros used to take a `vf` or axis token across 229 call sites. Converting
them cost less than keeping them: 36 of the 50 register tuples in use already had
an identical fixed-register macro, so the tree ended up with 21 definitions
removed and 14 added. Before that conversion `VU0_MOVE_VF(vf11, vf10)` and
`SDF_VU0_COPYVECTOR_A_TO_B` were both defined and meant the same thing, which is the
worst of both schemes.

Two distinctions a port must not flatten, both of which cost a match when they
were flattened here:

**A basic asm is not an extended asm with no operands.** `__asm__ volatile
("...")` and `__asm__ volatile ("..." : :)` are scheduled differently by cc1
2.96. `VU0_COPY_VF10_TO_VF11_EXTENDED` and `VU0_SUB_VF10_VF11_EXTENDED` carry the
second spelling because `mdlSetBlur` only matches with it: with the plain form,
the float constants it materialises come out in the other order. A macro whose
name ends in `_EXTENDED` means precisely that.

**The operand section is part of the operation.** Same instruction, different
clobbers, different register allocation. Four names stay defined per unit for
that reason and `include/README.md` tabulates them; a port that collapses them
into one function will drift on those call sites.

## The operations

| operation | instruction shape | macros |
| --- | --- | --- |
| register copy | `vmove.xyzw` | 17 |
| matrix bank copy | 4x `vmove.xyzw` | 10 |
| quad load / store | `lqc2` / `sqc2` | 28 |
| matrix load / store | 4x `lqc2` / `sqc2` | 10 |
| lane add / subtract / multiply | `vadd`/`vsub`/`vmul`, `.xyzw` and `.xyz` | 17 |
| mat4 x vec4 | `vmulax; vmadday; vmaddaz; vmaddw` | 5, plus 4 that are four of these |
| 3x3 x vec3 | `vmulax.xyz; vmadday.xyz; vmaddz.xyz` | 5 |
| cross product | `vopmula.xyz; vopmsub.xyz` | 2 |
| dot3 | `vmul.xyz; vaddy.x; vaddz.x` | 2 |
| length3 | dot3 then `vsqrt` | 2 |
| normalize3 | `vmul.xyz`; ACC sum; `vrsqrt`; `vmulq.xyz` | 2 |
| perspective divide | `vdiv; vmove.w; vmulq.xyzw` | 1 |
| linear interpolate | `vsubx.w; vmulax; vmaddw` | 4 |
| scale by a scalar | `mfc1; qmtc2; vmulx.xyzw` | 6 |
| identity matrix | `vsub; vmr32; vmove; vaddw.x; vmr32` | 3 |
| 4x4 transpose | `qmfc2` x4; `pextlw`/`pextuw`/`pcpyld`/`pcpyud`; `qmtc2` x4 | 2 |
| matrix inverse | transpose, `vdiv` x3, `vmsuba` chain | 2 |
| RGBA word to float lanes | `pextlb; pextlh; qmtc2; vitof0` | 6 |
| float lanes to RGBA word | `vmulx; vftoi0; qmfc2; ppach; ppacb` | 4 |
| s16 fixed-12 to lanes | `ldr/ldl; pextlh; psraw; vitof12` | 2 |
| unaligned vec3 load | `ldr; ldl; lw; pcpyld; qmtc2` | 5 |
| unaligned vec3 store | `lq; pcpyud; sdr; sdl; sw` | 3 |
| clip test and flag read | `vclipw.xyz`, `cfc2 $vi18` | 6 |
| quaternion product | `vopmula`/`vopmsub` plus an ACC chain on w | 1 |
| quaternion dot (4 lanes) | `vaddw.xyz vf1`; `vmul`; ACC chain; `qmfc2` | 3 |
| quaternion inverse | squared norm, `vdiv`, conjugate | 1 |
| quaternion normalize | squared norm, `vrsqrt`, `vmulq` | 1 |
| quaternion to matrix | 26 instructions, writes vf28-vf30 | 1 |
| weighted sum of two quaternions | `vmulax ACC, vf10, vf2x`; `vmaddx vf10, vf12, vf3x` | 2 |

## Where the obvious equivalent is wrong

Each of these is silent: the code still runs and the numbers are close.

**The accumulator chain is four roundings, not a dot product.** `vmulax;
vmadday; vmaddaz; vmaddw` rounds and clamps at every step. Writing
`m0*x + m1*y + m2*z + m3*w` lets a host compiler contract it into fused
multiply-adds and round once. The step order is part of the result.

**Perspective divide leaves w as 1/w, not 1.** `vmove.w vf10, vf0` sets w to one
*before* `vmulq.xyzw` scales all four lanes by `Q`, so the surviving w is `Q`.

**`SDF_VU0_APPLYMATRIX_XYZ` overwrites w.** Its last instruction is `vmaddz.xyzw`, which
writes all four lanes, so w becomes the accumulator chain's w. It is not a 3x3
rotate that leaves w alone.

**Normalize uses a reciprocal, not a divide.** `vrsqrt Q, vf0w, vf2w` followed by
`vmulq.xyz` is one reciprocal and a multiply, which does not round like
`v / sqrtf(len2)`. The squared length is also accumulated through `ACC.w` using
`vf0.w` as a multiply by one, a different order from `x*x + y*y + z*z`.

**The scratch clobbers are observable.** `SDF_VU0_INNERPRODUCT`, `SDF_VU0_GETLENGTH`
and `SDF_VU0_NORMALIZE` destroy `vf2`; `SDF_VU0_INTERPOLATE` destroys `vf3`;
`SDF_VU0_GETLENGTH` and `VU0_CLIP_RATIO_*` leave `Q` set. `vf2` is a carrier
elsewhere, so an equivalent that does not clobber it diverges.

**The clip register is a shift register, four results deep.** `vclipw` does
`clip = ((clip << 6) | bits) & 0xFFFFFF`. `code_002C6F38.c` reads the previous
result as `clipFlags >> 6` while reading the latest as `clipFlags & 0x3F`. The
`vnop` padding in `SDF_VU0_CLIPTESTWAIT` is hazard management, so no delay
model is needed, but the four-deep history is.

**`VU0_CLIP_TEST_BOX_CORNERS` is two four-corner windows combined with AND.** It
issues eight `vclipw` and reads `$vi18` twice, so each read sees one window:
corners 1-4, then 5-8. The AND asks whether every corner falls outside the same
plane. "Any corner outside" is a different test.

**An unaligned vec3 load fills w with a sign extension.** `lw` sign-extends the z
word into 64 bits and `pcpyld` puts that in the upper half, so w is `0x00000000`
or `0xFFFFFFFF` according to the sign of z's bit pattern. Not zero, not garbage.

**`vftoi0` and `vftoi4` saturate and truncate**, they do not wrap, and `vitof0`
reinterprets a lane's bits as a signed integer. The RGBA pack path depends on
this at both the 0 and 255 ends.

**`SDF_VU0_LOADIDENTITYVECTOR` is `vmove.xyzw vf10, vf0`**, so xyz go to zero and w to one.

## VU0 floats are not IEEE 754

No infinities, no NaN, denormals flush to zero, results saturate at
`0x7F7FFFFF`, multiply truncates rather than rounding to nearest, and the adder
aligns operands keeping one bit below the significand. The divider adds three
cases of its own: divide by zero answers `±0x7F7FFFFF` with the sign of the
operands exclusive-ored, `vsqrt` of a negative drops the sign rather than
failing, and `vrsqrt` by zero answers `0x7F7FFFFF` with the numerator's sign.

Scalar COP1 is the same family, which is why `fpu.h` exists: `sqrt.s` and
`rsqrt.s` are written out because cc1 2.96 will not emit them from `sqrtf`.

## Instructions with no C operator

These are what the assembly is for. All of them are a few lines of portable
code, and none needs 128-bit arithmetic:

`pextlb`, `pextlh`, `pextlw`, `pextuw`, `pcpyld`, `pcpyud`, `ppach`, `ppacb`,
`pexew`, `prot3w` (lane shuffles), `plzcw` (leading sign bits minus one),
`paddw`, `psubw`, `pand`, `por` (non-saturating 128-bit lanes), `psraw`,
`vminix` (minimum against a broadcast lane), `vmr32` (lane rotate), `vabs`,
`vclipw`, `vitof12`, `mult1`, `div1`, `divu1` (pipeline 1), and
`lwl`/`lwr`/`ldl`/`ldr`/`sdl`/`sdr` (unaligned, little-endian).

`div1` by zero needs writing out, since the EE answers ±1 and does not trap
where C has undefined behaviour.

## What is deliberately not a macro

Some operations are a whole function whose body is one asm block: `Mult` (the
quaternion product), `ToMatrix`, `SDF_QUAT_INNERPRODUCT`, `sdf_2C04B8` (the
inverse), `sdf_2C0510`, `FromRotation` and `FromRotation_B` in
`code_002BD340.c`, and the colour lerp in `model/mdlManager.c`. The function
already names the operation and is called from several places, so wrapping its
single-use body in a single-use macro would add a layer without adding a name.
A port translates those at the function boundary.

The macros exist where the same sequence is written out more than once, or
where it sits inside C control flow. `Slerp` and `Lerp` were the case for both:
between them they spelled out the same four operations, which is why
`VU0_QUAT_DOT_VF10_VF11`, `VU0_NEGATE_VF11_TO_VF12`,
`VU0_SET_WEIGHTS_VF2X_VF3X` and `VU0_WEIGHTED_SUM_VF10_VF12` are named.

Three things stay as raw asm because they are not instructions at all: the
`fptodp = sdfFloatToULong` style assembler symbol aliases, the `__asm__("")`
scheduling barriers, and `__asm__("nop\nnop\nnop\nnop\n")` delay padding.

## The quaternion functions

`code_002BD340.c` holds the engine's quaternion routines, named as the engine
names them: `Mult`, `Inverse`, `Normalize`, `SDF_QUAT_INNERPRODUCT`, `Slerp`,
`Lerp`, `ToMatrix`, `FromRotation`, `FromRotation_B`, `EulerAngleAxisY`. Three
of those were placeholders or wrong until the HD metadata settled them; the
proof and the call-site trap are in [IDB corrections](idb-corrections.md).

One is still open. `sdf_2C0510` reduces vf10 to a single float, which by
elimination would be `Length` -- the only scalar reducer left in HD's `sdfQuat`
list -- except that its accumulator chain starts `vmulay.x ACC, vf2, vf2y`
where the dot product uses `vadday.x`. It therefore computes
`(x*x)*(y*y) + z*z + w*w`, which is neither a length nor a dot, and HD's
`sdfQuat$$Length` is an empty stub. It keeps its address for a name until
something better than elimination is available.
