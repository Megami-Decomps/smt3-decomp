
## _free_r

The draft emits all 756 native instruction bytes: 86 of 189 words match and
103 differ, first at+0x38. Allocation and coalescing-path scheduling remain
unresolved. Normal builds retain assembly; no exact credit is claimed.

The chunk header precedes the user pointer by 8 bytes. Its previous size and
size are 32-bit words; free-list forward/backward links occupy +8/+0xC. The
current size clears only bit 0, while the successor size clears both low bits.
Backward coalescing consumes the previous-size word only when bit 0 is clear.
Forward coalescing checks the in-use flag in the chunk after the successor.
The top merge publishes the combined chunk and invokes the matched trim
helper only when its zero-extended size reaches the 64-bit threshold.

The arena is 0x408 bytes, with bitmap +4, top +8 and 127 pairs of bin links at
+0x10. Bin sentinels are overlapping chunk-header views whose links reside
in those pairs; their address is arena+index*8. Last-remainder sentinel is
arena+8. The integer address expressions derive those actual header positions
inside the arena, without pointers before the arena or synthetic padding.
Both last-remainder merge cases preserve list membership and bypass insertion.

Small sizes below 0x200 use size>>3. Large sizes use size>>9 to select the
native thresholds 5,0x15,0x55,0x155,0x555 and the corresponding shifts/offsets;
the otherwise unreachable zero case remains explicit. Bitmap shifts use a
64-bit one before narrowing. Empty large bins publish the bitmap; populated
bins traverse forward links in descending masked-size order.

Actual link values replace a long-lived cached sentinel; branch-local link
stores before the small-bin bitmap update recover native size and 86 matching
words. A cached arena pointer shortened the body to 716 bytes; combining the
large-bin anchor and predecessor locals shortened it to 748 bytes. Both were
removed. No declaration permutations, register constraints or dummy uses.

A qualified `cc1probe.sh` observation (`--func _free_r --alloc --no-dumps
--cflag -DSKIP_ASM`) preserves plain/hooked/unhooked output parity. Global
allocation assigns size pseudo 91 (27 references, 120 live instructions,
priority 9000) to GPR5, successor pseudo 87 (8/33, priority 7272) to GPR8, and
chunk pseudo 86 (27/153, priority 7058) to GPR9. Native keeps the chunk in GPR9
but uses size GPR8 and successor GPR5. A new ordinary lifetime/control premise
is needed; types and output semantics do not justify pinning that pair.

## gcc2_compiled_

Heap extension emits 584 of 600 native bytes, with 76 of 146 emitted words
matching and 70 differing. Sixteen bytes remain uncovered; first difference
is an exit displacement at +0xA8. The prologue and initial request sequence
match. The second request simplifies to a 32-bit subtract instead of native
64-bit subtraction followed by sign narrowing; later temporaries and the
small-old-top exit also differ. Assembly remains active, with no exact credit.

The request adds the 64-bit top pad to the zero-extended 32-bit need and 16,
then narrows to a signed break increment. A known break base enables page
rounding. A failed first request, or a backward break with a real old top,
returns immediately. Contiguous growth extends the fresh top header.
Noncontiguous growth first charges the request, then charges the gap from
the old top end when the base was already known; it does not replace that
accounting with an assumed break-minus-base identity.

The new header aligns its user pointer to 16 bytes. The second request adds
the front adjustment and a page complement, including a full page for zero
remainder. Failure leaves earlier arena/base updates intact but skips new
top, fencepost and maximum-memory updates. Success uses the entire second
increment in both the arena charge and the new-top size.

A real old top smaller than 16 poisons the NEW top size to one and returns.
Otherwise its usable size is `(oldSize - 12) & ~15`, its previous-in-use bit
is preserved, and two fencepost size words at offsets +4/+8 receive five.
Only a usable old remainder of at least 16 is freed. The typed twelve-byte
fencepost view leaves its first word untouched and avoids treating the
second size word as a free-list pointer. Maximum counters compare the
sign-extended 32-bit arena word as unsigned 64-bit and store that same value.

The initial overlapping-header spelling emits 588 bytes; the truthful
fencepost record emits 584. Explicit signed/unsigned long page constants
and physical field order do not retain the missing conversions. The actual
`_sbrk_r` prototype is supplied before this earlier helper; it is compiler-inert.
No extra pointer copy or dummy width operation was added.

## gcc2_compiled__1

The dtoa quotient/remainder helper at 0x002D8148 has a native-sized draft:
556 instruction bytes, 74 of 139 words match and 65 differ. Normal builds
retain assembly; no exact matching credit is claimed. Both `_dtoa_r` callers
add the integer return to ASCII zero and store an output digit.

Existing `_Bigint` records supply the signed limb counts and unsigned 32-bit
limbs. A shorter dividend returns zero. Otherwise the top limbs estimate an
unsigned quotient using divisor-top plus one; native division retains its
zero-divisor trap. The first loop subtracts that multiple with 16-bit products,
carry and signed borrow shifts. After trimming, the matched `__mcmp` decides
whether one additional subtraction and quotient increment are needed. There
is no decrement/carry-back correction or digit clamp. The callers provide the
normalized digit-producing ratio; arbitrary bigint division is not the API.

The typed limb array is updated with two halfword stores, as in the matched
bigint subtraction helper. The first top-limb test compiles to the native
cached pre-loop word; a nonzero estimate already implies that word is nonzero.
The post-correction trim uses a fresh word load. Independent native and caller
review confirmed both loops, correction direction and count updates.

The parity-qualified pinned cc1
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`
trace uses `tools/cc1probe.sh src/smt3/sdk/libc.c --func gcc2_compiled__1
--cflag -DNON_MATCHING --alloc --sched --no-dumps --maxlines 3000 --out <output>`.
Global allocation gives the high difference pseudo 98 (12 references, live
length 10) GPR6, low difference pseudo 97 (12/12) GPR3, and product-high
pseudo 99 (12/22) GPR4. Native uses GPR5 for the high difference and GPR8 for
the product high; the cursor, borrow and cached-word registers consequently
also differ. Control-flow shape and function extent already agree.

Signed quotient/borrow spelling, direct limb reads versus a captured word,
and deferring the carry assignment do not resolve this allocation. Separate
phase locals leave 72 differing words; a repeated inline subtraction helper
leaves 90. Sharing the carry and low difference leaves 60, still nonmatching;
that control is not retained. A new ordinary lifetime or source-context premise
is needed before another attempt. No register constraints or artificial work.

## vfprintf

The wrapper emits its native 116 instruction bytes: 8 of 29 words match
and 21 differ, first at +0x10. The format and argument-list saved registers
are exchanged; stream-data reload, initialization argument and branch-delay
scheduling also differ. The independently framed core at 0x002D6678 stays
assembly-backed with its complete literal and dispatch-table ownership.

The wrapper installs `_impure_ptr` only when the stream has no reentrancy
record, calls `__sinit` only when that record is uninitialized, and forwards
the fresh stream record after the call. A cached-record/explicit reload
control emits 112 bytes and was discarded. No storage qualifiers, dummy
references or false argument types constrain allocation.

## _d2b

The binary64-to-bigint reconstruction emits all388 native instruction bytes:
93 of97 words match and four differ, at+0x84,+0x88,+0x8C,+0x98. These are the
low-word extraction, zero test and stack store using GPR16 instead of native
GPR5. Normal builds retain assembly; no exact matching credit is claimed.

The existing binary64 word union and `_Bigint` layout preserve the actual
`(_reent *, f64, s32 *, s32 *)` interface. Allocation precedes representation
extraction. The high mantissa snapshot precedes sign clearing and exponent
extraction; normal values restore the implicit bit. A detached unsigned low
word is normalized through `_lo0bits`, then joined with the high mantissa.
The low-zero path normalizes the high mantissa and adds32 to the shift count.
Both paths set the real limb count and cache it for subnormal bit counting.
The upper payload store occurs even when zero. Exponent and significant-bit
outputs follow the normal/subnormal native branches without new special cases.

Qualified actual-owner allocation/scheduler traces use pinned cc1
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`,
with hooked/unhooked and plain-output parity. In the unguarded control, UID86
truncates input pseudo88 into low-word pseudo113. Global allocation gives
pseudo113 three references, live length4, conflicts with GPR4 and a GPR16
preference inherited from the input; it receives GPR16. The later trailing-bit
pseudo93 has eight references, live length30, no preference and receives GPR5.

Reusing `trailingBits` first for the signed raw low word is semantically sound
but does not supply native allocation. The qualified control puts UID86 and
the later helper return in pseudo93 (11 references, live length34), inherits
GPR16 and assigns it there. The result grows to392 bytes with70 differing
native words and four excess bytes. Binding the mutable `lowWord` in the
conditional instead leaves388 bytes and five differing words: GPR2 receives
the raw word and its store moves into the branch delay slot. Sharing the bit
count with that bound value is inert. These controls are not retained.

Reproduce the baseline with `tools/cc1probe.sh src/smt3/sdk/libc.c --func _d2b
--alloc --sched --no-dumps --maxlines 3000 --out <output>` after removing only
its guard; the guarded owner needs `--cflag -DNON_MATCHING`. Earlier in-place
union normalization and isolated original source-object context also failed.
A new grounded allocation or source-lifetime premise is needed; scalar reuse
alone does not defeat the inherited preference. No register constraints,
extra arithmetic or artificial lifetime extensions were added.


## func_002D6678

The 5744-byte formatted-output core is independently bounded by its entry,
frame and direct reentrant callers; see [SDK matching](../../sdk-matching.md).
Its guarded reconstruction emits 5308 bytes: 40 of 1327 emitted words match,
1287 differ, and 436 native bytes are uncovered. The frame is 0x2A0 rather
than 0x290. Assembly remains active and this adds no exact matching credit.

The reconstruction uses the native eight-entry output-vector batches and
348-byte digit workspace. Text scanning always calls `_mbtowc_r` with global
reentrancy state and the decoder limit, then parses the conversion byte.
Integer arguments consume eight-byte slots; a single `l` selects 64-bit
loads, while `ll` and `q` set a different flag which these integer paths do
not consult. The pointer conversion sign-extends its 32-bit pointer before
hexadecimal conversion. Stream setup tests `_bf._base`, not `_p`.

Finite floating output uses the existing `cvt` and `exponent` providers.
Separate vectors preserve the native mixture of locale decimal points and
literal periods. Scientific output retains its zero-length suffix vector
when applicable: it still occupies a batch slot. Integer precision is
represented by zero-padding vectors, including the native alternate-octal
ordering and empty numeral at value zero with precision zero.

Cumulative output is updated after a successful literal batch and once per
completed conversion, before its final flush. An earlier full-batch failure
therefore excludes the unfinished conversion from that count. The common
exit selects -1 only for the stream error flag; setup failure returns -1
directly. Empty conversion batches reset their vector cursor even when
there are no pending bytes to flush.

A single switch covers both flags and conversions, as in the native dispatch
table. The earlier two-stage parser emitted 5596 bytes with 1359 differing
words but did not reproduce that structure; it is not retained. The decoder
limit has its observed ordinary-data storage annotation, avoiding an inferred
small-data reference.

Numeric cases now converge directly on a shared integer-formatting label.
The internal selectors are octal 0, decimal 1 and hexadecimal 2, rather than
radix values. A decimal/octal/hexadecimal comparison chain retains the native
invalid-base diagnostic once; an ordinary switch emitted two diagnostic
address paths and an extra range comparison. Zero value with precision zero
bypasses this dispatch and remains an empty numeral. Common precision padding
is derived directly from the precision field and converted-text length.

The previous 5488-byte body matched 7 emitted words and had a 0x2C0 frame.
The numeric-dispatch revision matched 31 more words and reduced the frame gap
from 48 to 16 bytes, while leaving 180 more native bytes uncovered. A selector switch
with the old conditional convergence emitted 5456 bytes; direct convergence
with that switch emitted 5316 bytes. These controls are not retained.

At numeric-dispatch revision `a20eb528`, a qualified actual-owner jump trace
used pinned cc1
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`:
`tools/cc1probe.sh src/smt3/sdk/libc.c --func func_002D6678 --jump --no-dumps
--cflag -DNON_MATCHING --maxlines 1200 --out <output>`.
Hooked/unhooked and plain-output parity passed. The target assembly is also
identical with and without `SKIP_ASM`. Late `jump_optimize_1` merges parser
character-load tails: UID4270/4274/4278 enter the shared tail at UID4193.
This confirms that revision's tail sharing, not the origin of the remaining gap.

Declaration order now follows the observed live buffer layout. Sizes and
uses are unchanged. Offsets below are relative to the post-prologue stack
pointer; the overall frame remains 16 bytes larger than native.

| Object | Previous C | Native and current C |
|---|---:|---:|
| Exponent buffer | 0x1C0 | 0x000 |
| Output descriptor | 0x000 | 0x010 |
| Eight vectors | 0x010 | 0x020 |
| Digit workspace | 0x050 | 0x060 |
| Two-byte prefix | 0x1B0 | 0x1C0 |
| Sign byte | 0x1AC | 0x1D1 |

The float sign, wide-character output, multibyte state, decimal position and
digit-length outputs retain their already-correct offsets 0x1D0, 0x1D4,
0x1D8, 0x1DC and 0x1E0. This adds two matching words with unchanged body and
frame sizes. Before/after actual-owner allocation probes pass hooked/unhooked
and plain-output parity; the target also emits identical assembly with and
without `SKIP_ASM`. Reproduce with `tools/cc1probe.sh src/smt3/sdk/libc.c
--func func_002D6678 --alloc --no-dumps --cflag -DNON_MATCHING --maxlines 2500
--out <output>`. The remaining frame gap is not an aggregate-size mismatch;
no local has been resized or added to force it.

Control-flow layout, register allocation and scalar spill lifetimes remain
broad frontiers. The source does not attempt to
fill missing bytes or force the native frame. A new compiler-property
premise is required before allocation or control-flow tuning.
