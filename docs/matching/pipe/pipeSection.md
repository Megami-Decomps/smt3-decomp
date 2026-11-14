# Matching notes: pipe/pipeSection

## ActionZMotion (pipe/pipeSection, 0x140)

74 of 80 words match at the native 320-byte size. Production remains
assembly-backed.

The draft snapshots the original count, clamps only its upper blend limit,
interpolates the two position components, and decrements a positive original
count. Negative input bypasses both interpolation and the post-decrement zero
clamp. The scale window uses the clamped pre-decrement count and inclusive
endpoints. Exact binary32 literals preserve the non-integer boundary and the
slightly sub-0.2 additive term.

All six residual words concern the order of independent complementary/count
float conversions and their first products. Swapping cast statements, separating
the blend and scale locals, or naming the weighted data component leaves the
same result. A one-pass conversion block also changes constant hoisting and
was discarded.

The single post-call local reset follows the matching-rule exception and the
matched ObjectDraw pattern in this unit. Qualified candidate/control traces
show reset187 after sibling selection and absent in life13; removing it gives
a tail call and twelve extra bytes. Hooked/unhooked and dump/plain outputs
agree. No extra loop block or dummy copies are retained.

## EmTask (pipe/pipeSection, 0x34C)

211 of 211 words match at the native 844-byte size in ordinary C. No register
binding, empty operand, new asm macro or changed interface is retained.
The complete 101-entry action table and target layout are reproduced.

One ordinary `tagPipeObject *player` serves the two independent follow phases.
Each phase assigns the actual `objMyData()` result before reading X and Y;
the first value is dead before the second call. The intervening object-ID
observation remains a reload after the first callback. Offset and count remain
separate block-local values, so their lifetimes are not merged with each other
or across the two phases.

The countdown is observed as its unsigned 16-bit pattern, then decremented in
`s32` (range -1 through 65534, without overflow). The store and signed-halfword
zero test narrow that result using the verified EE low-halfword behavior.
The result therefore matches native for all 65536 input patterns, including
zero wrapping to 0xffff, 0x8000 becoming 0x7fff and 0xffff becoming 0xfffe.
This does not claim portable signed narrowing outside that target. Offset
publication, X copy, count publication, Y observation and deactivation retain
the native order; no callback, additional read or artificial consumer is added.

The old separate-pointer/signed-count form gives 189/211 at 844 bytes. Shared pointer,
offset and count locals with the old narrow arithmetic give 852 bytes. Explicit
wide decrement and narrowed zero test with separate ordinary pointers gives
836 bytes; a shared player pointer with separate offset/count phases is exact.
Sharing all three wide-phase locals instead leaves 199/211 at 844 bytes. Diagnostic
bindings of the actual pointers also reached exact output with the corrected
counter expression, but were reduced away and are not retained. These are
source/output dependencies, not original-source or internal-pass claims.

The body preserves both lifetime windows, flag-dependent clearing and all
action callbacks. Action deactivation still reaches model positioning, while
lifetime expiration returns earlier. Construction/insertion receive the real
zeroed 32-byte temporary, with their truthful provider interfaces unchanged.

### Historical allocation observation

The earlier 189/211 draft's qualified no-dumps trace assigned the first
call-result pseudo 116 to its suggested `$2`, before offset 117 and count 121.
That explained the old arrangement but did not establish a permanent source
boundary. The exact ordinary reuse form above is newly measured; no debugger
trace of its internal allocation is claimed.


## MyAction (pipe/pipeSection, 0x21C)

135 of 135 words match at the native 540-byte size in ordinary C.
Action zero and the default path reach the shared offset-window test and
`ActionKey`. Production, SKIP_ASM and retained-draft contexts reproduce the
same native target, with the complete production owner and owned data exact.

The count snapshot is explicit on each predecessor: when status is nonzero,
the signed comparison and unsigned halfword snapshot precede `pipeEffSet`;
the callback path refreshes that snapshot, as does the status-zero path.
These ordinary accesses reproduce both native loads before the effect call
and the join load after it. Separate attack switch cases retain the two native
mode branches. No volatile qualifier, artificial dependency, register binding
or opaque callee declaration is needed.

The paths preserve wrapped halfword countdown ranges and signed low-count
tests. After resetting the byte attack mode, that path refreshes the count before
testing zero, matching the native reload; the other path retains its snapshot.
The shared limit retains integer subtraction, both integer-to-single
conversions, the single-precision addition of 256.0f and ordered comparison.
All provider declarations retain their actual types.

One reset of the consumed count local after `ActionKey` follows the matched
same-owner `ObjectDraw` regular-call precedent. The reset-only ablation emits
532 bytes and a sibling jump instead of retail's `jal` plus epilogue. This is
the matching-rule tail-call exception, not a claim about original spelling.

The earlier 129/135 claim described a different historical draft, not the
retained source at feba589: that source used volatile count/attack accesses,
merged the two attack modes and had no post-call reset. Its active C body is
516 bytes. Removing only the false qualifiers and adding the qualified reset
while keeping the merged attack test yields 508 bytes; the separate switch
cases and predecessor snapshots above yield the exact 540-byte target.
Frozen ordinary-wrapper and direct plain/dump compilations agree across all
three contexts, including complete assembly and literal whole object files
with only the unique, equal-length scratch source path normalized between
wrapper and direct runs. These measurements supersede the old six-word
scheduling-blocker claim.

## ActionKey (pipe/pipeSection, 0x5EC)

379 of 379 words match at 1516 bytes in ordinary C. Flag 0x400 has its own
normal-coefficient arm; otherwise dispatch uses the low status byte's high
nibble. Unsigned-byte access to the real u16 object, followed by s64 promotion
and shift, preserves the native extraction; modes 3..15 keep five positive
zeros. The emitted speculative byte read in the override delay slot is exact
without volatile. The real attackAction=1 scalar feeds the status callback
and later comparison; callback-sensitive action/count/flag reads remain fresh.
No constraints, bindings, invented consumers, ABI or data changes are used.

The neutral f32 scalar supplies zero to real default-coefficient consumers,
then is redefined by vecNormalize for actual length comparisons and vecScale.
Qualified shared/split complete-owner compiler observations establish this
ordinary phase-reuse effect. In shared expansion, pseudo93's zero definition
UID42 and coefficient copy UID45 survive CSE03. Splitting only initialScalar
makes UID42 define coefficient87 directly and removes UID45 at that pass;
the control is 1508 rather than 1516 bytes, with all 29 peers unchanged.
The shared initial value dies before the optional status callback, and the
normalization result is a fresh definition, not an artificial call-spanning
zero. Global allocation maps shared pseudo93 to f0. Both sources separately
pass plain/dump normalized assembly, allocated-section contents/contracts,
relocation contracts and function-inventory parity, independently rerun.
This demonstrates observed grouping/CSE behavior, not original spelling or
an unobserved allocator decision.

Native ordered float branches remain explicit rather than fabs/min/max
shortcuts. Each clamp publishes its actual final rate; each scoped damped
value feeds both the real store and continuing velocity. Final X clamping
stores positive zero then reloads the field, while the no-clamp path retains
its genuine cached velocity. No intervening call or alias-writing operation
invalidates that cache. Y remains an independent load. Normalization mutates
the vector before the fresh flag-0x800 check, and position updates follow
scaling. Actual vector provider contracts are unchanged.

Bounded models pass 131,072 coefficient-dispatch cases (all status halfwords
and both override choices), plus 40,288 damping/cache and 80,576 rate-clamp
cases over 10,072 selected/seeded binary32 inputs. These include signed zeros,
subnormals, threshold neighbors, extrema, infinities and NaNs. The models
accept both-NaN comparisons and do not establish NaN payloads, exception flags,
PS2-specific FPU behavior or universal floating-point correctness. Exact
native operations and access ordering remain the target evidence.

Independent complete-owner verification gives 30 match, 0 differ, preserving
all 29 peers and owned data. Retail checksum, 427 tests, 148 guarded owners /
553 drafts, seven IRX owners and report gates passed for source
`4ae367bfb3cbd56cb48a0a17e21ded7851769ca5`. Exact-source CI
[41067611880](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41067611880)
completed successfully, with every actual step passing and none skipped.

## pipeOffset (pipe/pipeSection, 0x108)

23 of 66 words match at the native 264-byte size. The guarded
reconstruction remains assembly-backed. Its ordinary signed and
unsigned offsetStepCount observations retain the native byte-load widths and
the intervening selected-ID store without a false volatile qualifier. The
explicit entry guard and advancing do-loop state the traversal phases directly;
the old generic while already compiled to that general loop shape, so this is
not evidence of a newly recovered compiler control-flow requirement.

The retained draft occupies the native 264 bytes but differs in 43 of 66
instruction words. The previous volatile draft occupied 272 bytes and differed
in 54 of the 66 native-window words. These are aligned-word comparisons, not
exact credit. All 31 other C functions and owner data remain exact under the
complete-owner check.

The full u32 descriptor-frame range is retained. Accumulated frame totals and
next position wrap in u32 before target signed interpretation; positive,
negative and zero signed-byte count paths preserve their low-byte stores,
steps -3/+3/0 and negative-position fallback to 10. This assumes the inherited
initialized head, valid reached links, matching provider rows and valid
descriptors, including for a reached sentinel node. It is not an invalid-input
recovery guarantee, a positive-frame restriction or an asynchronous-memory
model.

Native memory-access parity is still incomplete: the compiler caches the
previous frame instead of rereading it at 0x29D788. Qualified stock GCSE dumps
identify load UID75 as PRE-redundant, replacing it with register 134 supplied by
entry/later observations through UID242/243. Plain and dump-producing compiles
agree in assembly, allocated sections, relocation contracts and function sizes.
No debugger/hooked trace or original-source claim is made. Independent review
accepts this only as a NON_MATCHING draft; the native fallback must remain.

Final integrated qualification: complete owners 157/0 (Boss), 31/0 (Pipe)
and preserved peer StageLoad 16/0 pass. Canonical three-mode strict and
plain/dump object/assembly checks pass; retained Boss pointer and Pipe
limited-range comparison warnings remain. Fresh stable 140-owner/480-draft
qualification, 450 tests, 817 layout assertions, inline/shell checks, seven
IRX modules, retail SHA1/byte comparison/no-op and reconciled reports pass.
Game coverage is 8594 functions / 1485744 bytes, including the separate peer
StageLoad addition; this batch adds no exact matching credit.

### Descriptor provenance and bounded reload control

The provider `getPartsInfo` is a separate-owner 60-row lookup: it reads the
signed ID argument and table IDs and returns the first matching row or null.
Its exact owner has eight matches and no differences. It does not write the
frame, link or position state and has no callbacks; an opaque same-symbol
declaration cannot remove same-unit knowledge that this caller never had.
The initialized/reached-row preconditions above still apply.

A localized empty read/write operand on the actual `part` pointer immediately
before its advancing frame use restores that native previous-frame load in a
bounded control. The value is live only until the frame addition, before the
next lookup replaces it. Nevertheless this control regresses from 23/66 to
20/66 words at 264 bytes and is not retained. Recovering the reload alone is
not sufficient: entry ordering, position allocation and tail selection still
differ. This is measured source/output evidence, not a claim that the control
reproduces every native access or proves a particular internal compiler pass.
Both forms passed three-mode complete-owner plain/dump assembly and object
parity; production and SKIP_ASM outputs were unchanged by the guarded control.
Further work should target the remaining allocation/branch structure with
native evidence, not posit provider writes or repeat blanket volatility.

### Localized negative-position fallback constraint

The retained guarded draft now reaches 30/66 aligned words at the native
264-byte size (previously 23/66). Its negative-position fallback assigns the
real value 10 to `next`, constrains only that live store value, and publishes
it to `offsetCount`. The empty operand emits no instruction, has no fabricated
consumer and ends at that store. This recovers the native branch/store tail
rather than the previous conditional-move tail; it is a reconstruction aid,
not evidence of original spelling. The positive store still uses a different
register, and the previous-frame reload and other entry/loop differences
remain. This is not an exact match or full native-access parity.

An ordinary explicit branch without the operand produces 260 bytes and 21/66
native-window words; moving the operand to the positive path instead produces
268 bytes and 22/66. Neither control is retained. The selected change leaves
all assembly outside `pipeOffset` equal modulo local-label numbering in the
guarded owner and leaves normal/SKIP_ASM complete assembly and objects
unchanged. Both source forms pass all three modes' plain/dump assembly and
object parity. A bounded 100,005-case integer model supplements the direct
fallback-value review.
