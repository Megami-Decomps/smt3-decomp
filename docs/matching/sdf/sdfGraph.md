# Matching notes: sdf/sdfGraph

## sdfGrSetFrameSetMode (sdf/sdfGraph, 0x58)

22 of 22 words match at the native 88-byte size. Complete owner checking gives
15 match, 0 differ, with all peers and owned data preserved.

The ordinary retained reconstruction had eleven differing words at the same
size. Native holds the row in register 2, the four width/height/pixel values
in 3 through 6, the depth format in 9 and the destination in 10. The ordinary
form reused the dying row register for the last field, changing allocation
and the dependent byte-store order. Previously measured parameter, read and
store permutations did not close that difference and are not used here.

Three localized bindings retain only the real row pointer, destination pointer
and depth-format scalar. Two read-only empty operands reference the initialized
depth format, immediately after its load and before its actual destination
store. Neither follows its consumer, invents a row-pointer use, or extends a
value across a call. There are no calls in the function. The remaining four
fields have ordinary locals with their original signed or unsigned halfword
types; format publication narrows through the existing byte fields.

These are target-specific allocation and scheduling aids, not historical source
spelling or a general memory barrier. All five loads, five field stores and
optional three handle clears retain native widths and ordering. No extra access,
arithmetic, write, bound, qualifier, provider or ABI change is introduced.

Removing the row, destination or depth-format binding independently produces
12, 9 or 10 differing words. Removing the first read operand produces five;
removing the second produces eight. Bindings on width, height, buffer height
and pixel format were unnecessary and removed. These paired results establish
source/output dependence, not an unobserved internal compiler decision.

The existing valid-row and correctly aligned typed-object requirements remain.
Native indexing is unchecked and retains its 32-bit multiplication/addition
behavior; invalid modes do not gain a defined-C safety guarantee. Incompatible
overlapping source/destination objects are not invented as valid inputs. Opaque
handle values may alias each other or either object because no pointee is
accessed or freed; clear requests only replace the pointer values.

Independent native, ABI/layout and semantic review passes. An extracted-body
oracle passes 2,621,440 cases each under optimized and ASan/UBSan builds: every
halfword pattern, all four valid modes, varied clear flags, aliased opaque
handles, source immutability and whole-object canaries. Host pointer sizes are
not used as evidence for the EE layout; native output supplies that evidence.

Nine complete production, SKIP_ASM and retained-C plain/stock-dump/strict
triples preserve whole assembly, allocated sections, symbols and relocations.
Resolved peer contracts and all owned data are unchanged in every mode; the
production target equals the native fallback. Inputs remain unchanged across
post-generation qualification. Full retail checksum and repository regression
gates remain required for publication.
