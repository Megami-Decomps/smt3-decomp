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

129 of 135 instruction words match at the native 540-byte size. Production
retains assembly. Action zero still reaches the shared offset-window test
and `ActionKey`; the older reconstruction incorrectly returned early.

The action-specific paths preserve wrapped halfword countdown ranges,
separate attack modes one and two, signed low-count tests, and callback-visible
count reloads. The post-effect snapshot is an unsigned halfword; its signed
interpretation is used only for the low-count clamp. Resetting the byte attack
mode can alias the object, so that path refreshes the snapshot before testing
zero. The other path retains the existing snapshot as native does.

The six residual words are at +0x98, +0x9C, +0xAC, +0xB0, +0xB4 and +0xB8.
Native places the post-effect count load before the attack-mode load; the
draft schedules the mode load into the preceding branch and the count load
into the first mode-test delay slot. Qualified compiler output parity confirms
that ordering. No volatile qualifier or artificial dependency constrains it.

The consumed counter reset after `ActionKey` follows the existing same-owner
`ObjectDraw` and `ActionZMotion` regular-call precedent. Qualified traces retain
reset UID422 through sibling selection, remove it in life analysis, and keep
call UID418 regular. Removing only the reset emits 532 bytes with a sibling
call. Both controls pass normal/dumped/hooked assembly and all RTL parity.

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
