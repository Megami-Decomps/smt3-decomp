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

189 of 211 instruction words match at the native 844-byte size. Production
remains assembly-backed. The complete 101-entry action table and its target
layout are reproduced by the C switch, including the explicit no-action case.

The body preserves the two lifetime windows, flag-dependent counter clearing,
two independent player-follow paths and all action callbacks. The second
player-follow path reads the current ID after the first callback. Deactivation
from an action still reaches model positioning; lifetime expiration returns
earlier. The zeroed temporary is the real 32-byte object passed to construction
and insertion. Constructor and insertion declarations match their providers.

Separate entry/dispatch action snapshots reproduce the initial byte registers.
Capturing the object offset before the global counter reproduces the lifetime
comparison operands. A signed halfword snapshot of each decremented follow
count reproduces the native extent. All 22 remaining differences occur in the
two player-follow blocks: the global offset and player-pointer registers,
counter narrowing, and load/store scheduling differ. Native stores the counter
before loading the player's Y coordinate and carries the player pointer in a
separate register. A raw word count snapshot loses two instructions per block;
checking the pre-decrement before copying Y gives 188 matching words instead of
189. Capturing the stored pre-decrement result after X also loses two words per
block; an unsigned halfword snapshot does not close the gap.

A qualified pinned-compiler `--alloc --no-dumps` trace identifies the current
allocation in the first follow block (local-alloc block 11). Call-result pseudo
116 has the `$2` suggestion and is assigned `$2`, retaining the player pointer
through the count load. Offset pseudo 117 has live interval `[6,16]` and no
suggestion, and receives `$4`. Halfword count pseudo 121 has interval `[10,28]`
and receives `$3`. The suggested call-result quantity is allocated before the
unsuggested quantities; the count's priority is 15000 versus the offset's 2000.
This accounts for the current pointer/count register arrangement. It does not
establish an ordinary source change that produces the native copy into `$4`.
Hooked/unhooked assembly and normal-output parity passed without RTL dumps
using compiler SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.


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
