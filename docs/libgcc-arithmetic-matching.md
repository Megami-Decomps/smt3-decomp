# Double arithmetic

`libc_2D1590` (604 bytes) and `dpmul` (644) match all 1,248 native instruction
bytes. The unpacked fraction is an unsigned 64-bit magnitude; mixed-sign
subtraction uses a separate signed difference. All previously matched
functions retain their bytes with that field type.

## Addition and subtraction parts

The helper returns a read-only view of either input, the output record, or the
existing NaN record. Both-zero input copies the real 24-byte record and ANDs
the signs. Finite alignment increments the smaller exponent while shifting its
fraction with a sticky low bit. Writing those exponent updates as loops lets
the compiler strength-reduce the first loop into the native decrementing count;
precomputing the count instead retained an extra CSE value.

Mixed signs select a signed difference, store its magnitude/sign, and normalize
the output fields. Equal signs add magnitudes. The final right normalization
keeps the sticky bit. Copies use the typed unpacked record.

## Multiplication parts

The private `dfMultiplyParts` primitive follows the unpacked-record pipeline of
[the historical fp-bit runtime](https://github.com/gcc-mirror/gcc/blob/releases/gcc-2.95/gcc/config/fp-bit.c).
The wrapper unpacks both doubles and packs the selected record. A zero operand
returns its own record with the combined sign; infinity times zero selects the
existing NaN record. Sign combination uses boolean inequality.

Four integer products form the 128-bit fraction. Cross-product overflow adds
`1 << 32` to the upper accumulator, and lower-sum overflow adds one. Each
cross-product limb remains explicitly bounded to 32 bits. The lower half shifts
right only when the upper half's low bit is set. This conditional update is
present in the native loop. Exponent updates precede the
normalization operations. The shifted cross-product has its own local.

A qualified `cc1probe.sh --alloc` trace of pinned cc1
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`
passes dumped/plain and hooked/unhooked output parity. In `rtl.13.life`, UID 506
defines the `0xFFFFFFFF` mask as pseudo 284. UIDs 507/512 use it for input limbs;
UID 553 uses it after all four integer calls. Local allocation assigns its
quantity to `$21` with four calls crossed. Removing only the upper cross-limb
mask emits 628 bytes instead of 644; the separately qualified control passes
hooked/unhooked parity. Reproduce with `tools/cc1probe.sh
src/smt3/sdk/libgcc.c --as-unit src/smt3/sdk/libgcc.c --func dpmul --alloc
--out <output>`.

## Integer product provider and data ownership

The compiler's four natural integer multiplications call `__muldi3`, now
curated at `0x002D3398`. Its low-word unsigned product and two cross terms
produce the low 64 bits of the complete integer product. The name is inferred
from that contract and the matching callers; the provider remains assembly
backed and receives no C matching credit. The curated override protects future
symbol exports; the IDB correction is pending.

The NaN record is referenced through the inline primitive. `include_rodata.py`
follows private helper calls to retain its existing native block and padding.
Regression tests cover that reference, unrelated literal ownership and unused
extern declarations. The complete merged libgcc owner remains exact.
