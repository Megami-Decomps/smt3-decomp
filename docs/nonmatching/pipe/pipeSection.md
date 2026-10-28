# Non-matching drafts: pipe/pipeSection

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

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
