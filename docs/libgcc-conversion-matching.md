# Integer/double runtime conversions

`litodp` (184 bytes), `dptoli` (156) and `dptoul` (160) match all 500 native
instruction bytes. Their scalar interfaces are signed 32-bit to double, double
to signed 32-bit, and double to unsigned 32-bit respectively. The signed
conversion saturates at exponent 31; the unsigned conversion rejects negative
values and saturates at exponent 32. Both return zero for zero and NaN classes.

The inline zero/infinity predicates retain the class-test boundary present in
[GCC's historical fp-bit runtime](https://github.com/gcc-mirror/gcc/blob/releases/gcc-2.95/gcc/config/fp-bit.c).
Direct comparisons instead change the native XOR tests. Integer normalization
updates the unpacked fraction and exponent in an ordinary while loop; the
minimum signed input returns its exact double constant before negation.
The generic unsigned fraction-shift branch remains present in native code even
though the preceding 32-bit saturation bound makes its left-shift arm unreachable.

The same unit now declares the double returns of `__pack_df`, `__make_dp`,
`fptodp`, `dpadd` and `dpsub` with their floating types. All existing matched
functions remain exact. Complete libgcc owner: 23 match / 0 differ.


## Native unpacking and comparison

`sdfUnpackFloat` (92 bytes), `sdfUnpackDouble` (188) and `__fpcmp_parts_df`
(288) add 568 exact instruction bytes. Both unpackers store the sign and classify
exponent-zero inputs as zero without clearing the remaining output fields.
The float helper treats every nonzero exponent as a normal value; the double
helper separately preserves infinity and NaN classes and payloads. Its binary64
view uses the native 52/11/1-bit fields, rather than rebuilding them from two
32-bit word loads.

Comparison returns a signed 32-bit result, including one for unordered inputs.
Signed zeros compare equal. Finite values use signed exponent and unsigned
fraction comparisons, with magnitude order reversed for negative values.
Returning directly from each magnitude comparison preserves native control
flow; collecting the order first emitted a different, shorter body. The existing
`dpcmp` wrapper retains all 76 matching bytes with its corrected signed return.
Complete libgcc owner: 26 match / 0 differ.
