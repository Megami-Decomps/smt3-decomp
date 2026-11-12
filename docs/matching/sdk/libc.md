
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

## malloc_extend_top

Heap extension matches all 600 native instruction bytes: 150 of 150 words
match. This contributes 600 SDK EE bytes and zero game-code bytes. The
production build selects C; the four remaining libc drafts keep their guards.

The request adds the 64-bit top pad to the zero-extended 32-bit need and 16,
then narrows to a signed break increment. A known break base enables page
rounding. A failed first request, or a backward break with a real old top,
returns immediately. Contiguous growth keeps the unflagged size sum distinct
from the header value with its previous-in-use bit set.

Noncontiguous growth charges the first request, then the gap from the old
top end when the base was already known. The second request includes the
front alignment adjustment and a page complement, including a full page
for zero remainder. Its complement stays 64-bit until the signed narrowing.
Failure preserves earlier arena/base updates but skips new-top, fencepost
and maximum-memory updates. Success charges the entire second increment.

The localized constraints name real initialized values with actual consumers:
the contiguous unflagged sum uses `$4` and its flagged store header uses `$2`.
The second break result and the later new-top size have disjoint `$4` live
ranges; neither crosses a provider call. The new header computation and its
existing operand constraint stay after arena accounting and top publication,
at the header store's consumer phase. The small-old-top path uses `$3` for
the actual new-top pointer whose size is poisoned to one, then returns.
No dummy use, artificial live range or extra instruction template is needed.

For an old top of at least 16 bytes, `(oldSize - 12) & ~15` forms its usable
remainder, preserving the previous-in-use bit. The typed twelve-byte fencepost
view leaves its first word untouched and writes five at offsets +4/+8.
Only a usable remainder of at least 16 is freed. Maximum counters compare
the sign-extended 32-bit arena word as unsigned 64-bit and store that value.
The provider ABI, real heap accesses and existing valid-EE-heap/accounting
assumptions are unchanged; matching does not establish original source spelling.

The earlier overlapping-header draft emitted 588 bytes; the truthful fencepost
view emitted 584. Restoring the actual arithmetic widths, value lifetimes and
header-store phase reaches 600 without changing peers. Complete qualification
checks the owner and owned data, all 100 peer functions, the full retail image,
and literal plain/stock-dump assembly and raw ELF equality separately in
production, SKIP_ASM, retained-C and retained-C plus FIX_BUGS contexts.

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

116 bytes / 29 words match in ordinary C. A one-pass block keeps stream
attachment and initialization in one phase, transferring the shape already
used by the matched `__swsetup` in this owner. Removing the block leaves the
same 116-byte size but only 8 of 29 words matching: the saved format and
argument-list registers exchange, and stream-data/branch scheduling changes.
No register constraints, storage qualifiers or ABI changes are needed.

The wrapper installs `_impure_ptr` only when the stream has no reentrancy
record, calls `__sinit` only when that record is uninitialized, and forwards
the fresh stream record after the call. The one-pass block does not cache the
record across initialization. The independently framed formatter core at
0x002D6678 remains assembly-backed with its literal and dispatch-table ownership.

## _d2b

388 bytes / 97 words match. The existing binary64-to-bigint reconstruction
preserves the actual `(_reent *, f64, s32 *, s32 *)` interface, representation
union, allocation, limb normalization and exponent/bit-count outputs.

The remaining four words were the raw low-word extraction, zero test and
stack copy: ordinary C inherited GPR16 from the input rather than native
GPR5. A scoped, initialized `u32` snapshot bound to `$5`, consumed by one
nonvolatile empty `+r` operand, gives the native allocation. Its only real
consumers are the zero test and the addressable low-word initialization; it
is dead before `_lo0bits` can clobber caller-saved `$5`. Normalization uses
the separate addressable local exactly as before. No call, memory access,
numeric conversion, layout, ABI or compiler flags change.

This is a target-specific compiler allocation aid, not a claim about
historical source spelling. Ordinary C, binding without the operand and a
generic operand without the binding each retain the same four-word
residual (93/97); both pieces of the localized constraint are necessary.
No volatile qualifier, extra arithmetic or artificial use is introduced.
Plain and `-da` builds agree in target bytes, full text, relocations and
normalized assembly. The complete owner has 76 matching functions and no
code or data differences. Earlier object-context, in-place union and
disjoint scratch-reuse controls remain superseded negative diagnostics.

## func_002D6678

The 5744-byte formatted-output core is independently bounded by its entry,
frame and direct reentrant callers; see [SDK matching](../../sdk-matching.md).
Its guarded reconstruction emits 5236 bytes: 48 of 1309 emitted words match,
1261 differ, and 508 native bytes are uncovered. The frame is 0x2A0 rather
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
0x1D8, 0x1DC and 0x1E0. At buffer-placement revision `b7759264`, this added two matching words with
unchanged body and frame sizes. Before/after actual-owner allocation probes pass hooked/unhooked
and plain-output parity; the target also emits identical assembly with and
without `SKIP_ASM`. Reproduce with `tools/cc1probe.sh src/smt3/sdk/libc.c
--func func_002D6678 --alloc --no-dumps --cflag -DNON_MATCHING --maxlines 2500
--out <output>`. The remaining frame gap is not an aggregate-size mismatch;
no local has been resized or added to force it.

Converted output now has one `text` cursor. Integer conversion backfills
from the end of `numberBuffer`; strings and special values select their own
read-only text; finite floats select the `cvt` result. These alternatives do
not need their pointers concurrently. Their physical buffers remain distinct,
and writes through the cursor are confined to integer backfill. The unused
conversion-entry text initialization is removed. Literal scanning retains its
separate const-qualified pointer; sharing that role was code-identical and is
not retained.

Qualified actual-owner allocation/RTL probes pass hooked/unhooked, dump/plain
and target `SKIP_ASM`/full-guard assembly parity. Before consolidation, UID1122
receives the `cvt` result in pseudo122: 16 references, live length1112, 41 calls
crossed, no available register, and a spill at stack+0x218. Afterward UID1119
receives it in pseudo92, also used by integer backfill (UID1415): 87 references,
live length478, 19 calls crossed, assigned GPR21. This recovers the native
formatted-cursor role without a register constraint or an artificial use.
Reproduce with `tools/cc1probe.sh src/smt3/sdk/libc.c --func func_002D6678
--alloc --cflag -DNON_MATCHING --maxlines 10000 --out <output>`.

The cursor change adds eight matching words and removes 72 emitted bytes;
72 more native bytes remain uncovered. Frame size stays 0x2A0. Control-flow
layout, register allocation and scalar spill lifetimes remain broad frontiers. The source does not attempt to
fill missing bytes or force the native frame. A new compiler-property
premise is required before allocation or control-flow tuning.

## _dtoa_r

The seven-argument conversion emits 4848 instruction bytes against 4708 native:
109 of 1177 native words match and1068 differ, with140 excess bytes. Its 0xC0
frame, allocation, block layout and source data ownership remain open. Normal
builds retain assembly; no exact credit is claimed.

The result cache uses the allocation header itself as character storage; entry
restores `k` and `maxwds` before recycling it. Full 52-bit fraction tests preserve
special values. Subnormal normalization uses bounded 32-bit shifts. Native mode
limits, quick-path rollback, small-integer rounding, exact bigint margins and
cleanup are preserved. Independent native reviews covered both conversion halves.

The existing binary64 union now has a genuine64-bit view. Typed readonly scalar
declarations reproduce native absolute accesses and remove the initial
whole-unit/standalone discrepancy. The positive-limit table access uses
`limit-1`; the large-power table includes its separately labeled last scalar.
No compiler flags changed. An unguarded trial still duplicates the original
mode-table import and places its zero string in small data; resolve those
ownership issues at promotion while preserving the default assembly context.

A host diagnostic completed 124117 conversions over 2420 boundary/random input
patterns against independent decimal rounding and binary64 round trips. It
checks modes 2/3 against quick-disabled modes 6/7 and permits the native shorter-output
variation in modes 4/5. Cache reuse and allocation balance passed. This checks tested
numerical behavior; EE instruction/data matching remains open.

The unchecked mode 5/9 corner remains: input 0.1 with digits -3 derives power-5
argument -4 at 0x002D8FB0. Only mode 3 clears boundary-aware setup. The observed
`cvt` caller uses modes 2/3; the host diagnostic detects this negative exponent and
excludes it from terminating numeric tests. No defensive clamp was introduced.

Moving shared quick rounding beside the integer path saves 16 bytes. In the
initial unguarded owner, sharing the decoded exponent, buffer capacity and
quick error count is compiler-inert; separate roles remain preferred. Both
allocation probes pass output parity with pinned cc1
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`,
using `--func _dtoa_r --alloc --no-dumps --maxlines 8000`. Do not repeat that
control unchanged.


## _b2d (sdk/libc, 0x198)

102 of 102 words match at 408 bytes in ordinary C. This owner uses the
configured default ee-gcc 2.96, not the older Sony SDK compiler.

The optional lower limb in the `leading < 11` branch is selected with
`below = first < last ? *--last : 0`. It performs the same conditional
pointer decrement and unsigned limb read as the zero-initialization plus
`if` control. The false arm neither decrements nor reads. Actual compiler
dumps show the two-arm join preserves the second `11 - leading` calculation
through CSE; the control reuses the earlier shift distance and leaves fourteen
words different at the same size. This ordinary control-flow shape restores
the native final join without a constraint, extra consumer or artificial
lifetime. It does not establish historical source spelling or a hooked
internal CSE decision.

The existing bigint contract supplies a populated nonempty limb allocation,
with `1 <= wds <= maxwds`; normalized magnitude interpretation also requires
a nonzero top limb. The allocator's trailing `x[1]` uses the configured old
compiler's overallocated-storage convention, not portable ISO array bounds.
Subsequent decrements occur only while another limb remains. All variable
shifts operate on unsigned 32-bit values with distances below 32, and the
zero-distance path is separate. No bounds check, sign handling, rounding or
invalid-input repair is added.

Both union words are written before returning the target little-endian
binary64 value. This uses the existing GNU union representation contract.
For normalized magnitudes, the result keeps the leading one and next 52 bits,
truncating lower bits, with the same exponent output. An independent integer
oracle passed 256,000 cases covering all 32 leading-zero counts and one to
eight limbs; this supplements native matching rather than claiming target
execution. The actual `_ratio` caller's pointer, exponent-output and return
contracts agree with the provider.

The existing truthful `_close_r(_reent *, s32)` declaration is now visible
before unconditional `__sclose`. Declaration-only comparisons preserve full
outputs in all three modes. This repairs an inherited strict implicit-call
failure, not a demonstrated runtime bug. Candidate production, `SKIP_ASM`
and retained-C strict builds pass; candidate/control ordinary-versus-dump
comparisons cover complete assembly, ELF and allocated-section headers/data,
non-file symbols and all relocations. Independently checked native bytes
resolve the actual `_hi0bits` relocation without masking. Both complete-owner
checkers pass 78 functions with no owned-data difference.
All other emitted functions are preserved in each mode (100 production,
77 `SKIP_ASM`, 82 retained-C), including resolved code-relative relocations;
complete owned data, layout and data-symbol contracts are unchanged.


## strcasecmp

The reconstruction matches all 216 native bytes with ordinary GNU C and
scoped, once-evaluated byte snapshots. The loop stops on the original left
NUL, compares folded signed bytes, and advances both input cursors when equal.
The exit reloads unsigned bytes, folds each independently and subtracts.
No helper, binding, compiler flag, ABI or storage extent changes are needed.

Retail has a signed-byte classification bug. Its 257-byte `_ctype_` object
spans `[0x0054B550, 0x0054B651)`, including the EOF entry. Loop bytes 0x80 through
0xFE read preceding addresses in the real 192-byte CFI dispatch table
`jtbl_0054B490`; 0xFF selects the actual EOF entry. The final unsigned lookups
stay in bounds. Integer EE 32-bit address arithmetic followed by an unsigned
character read preserves those actual mapped bytes without inventing a larger
ctype array or forming an out-of-array pointer expression.

The actual preceding table byte classifies 0xE0 as uppercase. Its signed value
-32 folds to zero, so a left 0xE0 can compare equal to a right NUL and advance
past that terminator. Left `{0xE0, 'A', 0}` and right `{0, 'B', 0}` return -1.
Input cursor advancement also uses integer EE addresses, preserving traversal
between adjacent readable mapped objects. This deliberately relies on the
EE implementation's numeric-address mapping, not portable ISO C provenance,
an ASCII-only precondition, or an ordinary string-length safety guarantee.
No guard repairs native faults or malformed-input behavior.

Each statement expression captures a real signed or unsigned byte in its own
initialized signed 32-bit local. It introduces no dummy consumer or artificial
lifetime. Qualified compiler traces retain native peeled/repeated loop roles:
peeled signed byte pseudo 136 selects register 7 and folded result 137 selects
register 6; repeated signed byte 107 selects register 6 and plus-32 result 124
selects register 7. Exit snapshots 109/116 select registers 5/2. These distinct
actual roles prevent the late tail merge seen in an outer-local control.
A minimal lexical-scope ablation keeps the statement-expression shape but
reuses outer left/right snapshots across the signed and unsigned phases; it
emits 228 bytes with different allocation. Complete plain/dump output parity
qualifies both forms. This is evidence about the configured compiler, not the
historical source spelling. An earlier separate-fold-local control also changes
retained `_d2b` and is not an owner-safe alternative.

Independent review verifies all 54 instruction words after resolving the 12
real HI/LO relocation records. Both owner checkers report 79 match, 0 differ.
Baseline/final complete assembly and literal entire-ELF plain/dump comparisons
pass in production, `SKIP_ASM` and retained-C modes, with per-invocation hashes.
All 100/78/83 peers, allocated data, NOBITS layouts, section metadata and
974/454/806 non-target symbol contracts are preserved. Compiler-local label
renamings are checked by their actual locations and resolved targets. All
reused low relocations are separately audited, including the native allocator
high-register paths; production peers also retain identical raw instructions
and relocation tuples without masks.

An independent decoder of the 54 retail instructions, including branch-likely
and delay slots, agrees with a source-expression model on all 65,536 byte
pairs, 10,000 backing-buffer cases and four corner cases. This is a focused
instruction decoder, not a general EE emulator or hardware execution claim.
The full linked 48-entry CFI dispatch table, all resolved destinations and
adjacent ctype bytes remain identical to retail. Full retail checksum,
generated-source, 569 regression-test, guarded-source and report gates pass,
with a unique compiler-owned production span. SDK coverage gains one function
and 216 bytes; game coverage is unchanged.


## _calloc_r

The ordinary C reconstruction matches all 180 instruction bytes (45 words)
under the configured ee-gcc 2.96, with four following alignment bytes.
Unsigned 32-bit multiplication retains the native modular request size.
A failed allocation returns null before reading its header. A successful
allocation clears the chunk size with its low two flag bits removed, less
one four-byte word. Small chunks use the native unrolled word stores; larger
chunks call the existing memset provider. No overflow check or allocator bug
repair is introduced.

This is allocator-internal storage: while PREV_INUSE is set, the allocated
chunk may borrow the successor's previousSize word. Clearing stops before
the successor's size word. It is not a claim that a caller's requested-size
C object generally extends into another object. Native word widths, null
handling, arithmetic and store ordering were independently reviewed. The
final three stores execute at cursor offsets 0, 8 and 4, even though their
ordinary C expressions are sequential.

The small-arm cursor scope and genuine do/while(0) follow the historical
MALLOC_ZERO structure in [newlib 1.9.0 mallocr.c](https://github.com/mirror/newlib-cygwin/blob/newlib-1_9_0/newlib/libc/stdlib/mallocr.c)
(blob c2b2fb91f2b33f2ee32eac6c341d966cac234f47). This corroborates the source
shape, not the SDK's exact newlib release or original source spelling.
The archive map's 296/callocr.o attribution is name-only evidence, not a
proven native translation-unit boundary. No boundary, compiler flag, ABI,
constraint, declaration or malloc draft changes accompany this promotion.

Discovery consumed four compiler/assembler pairs: baseline, two ordinary
nonmatching controls, and the exact historical-macro control. Fresh complete
owner and independent --source checks each pass 81 functions with no
code or owned-data difference. Production, SKIP_ASM, retained and retained
plus FIX_BUGS plain/dump routes have literally identical complete assembly
and raw objects within each context. All 100/80/84/84 peer contracts,
allocated data and ordered relocations are preserved. Production retains
all 33,376 text bytes and 196 padding bytes unchanged, including the existing
600-byte malloc_extend_top. C-only modes add only the target and its alignment.
The production peer comparison reuses the verified discovery baseline object;
the other baseline contexts were freshly compiled.

All 101 linked owner function spans, complete owner text and 1,424 rodata
bytes match retail. The complete 4,551,696-byte image is byte-identical to
its reference. Fresh generated-source and stale-line controls, all 31 normal
regression files (532 methods), 125 optimized checker methods, strict retained
contexts and seven IRX checks pass. Two setup failures remain recorded: a
premature owner check before Ninja's compatibility prerequisites (two compiler/
assembler attempts), and a baseline production attempt whose now-promoted
assembly fallback was omitted (one pair). Neither is a passing gate.
Nine inherited unrelated owner failures were not rerun; this is not a global
owner-suite pass. Prospective credit is one SDK/runtime function and 180 bytes,
with no game-code gain, after publication and exact-commit CI.
