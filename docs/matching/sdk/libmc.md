# sdk/libmc

Nineteen of the library's twenty-one `sceMc*` entry points match. Every one
follows the same template, so the two remaining drafts differ only in their own
detail, not in the frame:

    if (PollSema(D_0052BB24) < 0) return -200;
    if (D_00599740.bound == 0) { SignalSema(D_0052BB24); return -100; }
    <fill the request at D_005997C0 or D_005997F0>
    ret = sceSifCallRpc(&D_00599740, <fno>, <mode>, &req, <size>, &D_0059AD00, 4, ...);
    if (ret == 0) D_0052BB20 = <fno>; else SignalSema(D_0052BB24);
    return ret;

Three details the template needs, each measured:

- **`SignalSema` is written out in both branches**, never hoisted above the
  `if`. `sceMcGetSlotMax` is 12 bytes shorter when it is hoisted.
- **The pointer-to-flag conversions are explicit `if`/`else`.** Writing
  `req.w[5] = type != NULL` costs `sceMcGetInfo` 52 bytes, because retail
  branches per flag.
- **The entry-info block is 4-byte aligned**, so the 0x40-byte copy in
  `sceMcSetFileInfo` is `ldl`/`ldr` pairs. Giving the struct `u64` members
  makes it 8-aligned and the copy becomes `ld`/`sd`, 76 bytes short.

`sceMcUdCheckNewCard`'s format string stays an extern rather than an inline
literal: the rodata chunk it opens also carries the float constants the next
unit needs, and inlining the literal orphans them, which shifts the image and
overflows a `$gp`-relative literal reference in a game unit.

## sceMcWrite

380 emitted and native instruction bytes: 88 of 95 words match and seven
differ, first at +0x6C. The 384-byte address span includes four alignment bytes.
The leading-byte arithmetic is right, and it
has to be written as two separate subtractions:

    u32 top = ((u32)buf - 1) & ~15;
    u32 bot = (u32)buf - 16;
    s32 head = top - bot;

Folding it into one expression makes gcc compute `((buf-1) & ~15) + 16 - buf`
instead, which costs three instructions. The inline payload must also be a
named member (`req.data[i]`), not `((u8 *)&req)[32 + i]`: the cast form hoists
`base + 32` out of the loop, where retail keeps the base and uses 32 as the
store displacement.

The write-specific view records descriptor at +0, signed bulk size at +0xC,
unsigned inline size at +0x14, bulk-buffer pointer at +0x18 and sixteen prefix
bytes at +0x20. The remaining words stay untouched. The original signed count
selects the short path at count < 17, including negative counts; the copy loop
uses the unsigned inline size. Preserve that native behavior. The source buffer
and forwarded bulk pointer are read-only.

Typed fields retain the same seven-word residual as the generic request view:
branch-local request-base selection and independent stores differ. Native long
writes store bulk buffer, bulk size, then inline size; the compiler schedules
inline size first. A cached request pointer shortens the function to 372 bytes
and hoists the inline-data address, so the preferred source keeps direct global
accesses. No field qualifiers or artificial dependencies constrain the stores.

## sceMcSync

224 bytes, retail's size, with every instruction but two in place. The loop

    while (1) { done = 0; if (sceSifCheckStatRpc(&cd) == 0) break; mcDelayThread(60); }

puts `done = 0` in the branch delay slot where retail keeps a `nop`. Rewriting
it as `while (check() != 0) delay(60); done = 0;` is 12 bytes short, so the
`done = 0`-per-iteration shape is the right one and only its scheduling is
wrong.

## sceMcSetFileInfo (sdk/libmc, 0x1D8)

Matched: all 472 native instruction bytes. The 64-byte entry-info copy keeps
its truthful 4-byte alignment and native unaligned doubleword operations.

The incoming flags have a real lifetime from function entry, across the
semaphore poll and validation branches, to the masked request-field store.
A local binding retains that value in native callee-saved `$17`, separately
from the request cursor in `$16`. The ordinary draft coalesces those roles,
emits only 464 bytes and uses a 0x90-byte frame; this constraint restores the
native 0xA0-byte frame and complete instruction sequence. It is a localized
compiler reconstruction aid, not a claim about historical source spelling.
There is no empty asm operand, new memory access, helper or ABI change.

Ablating only the binding restores the 464-byte residual. Paired plain and
RTL-dump EE 2.9 builds have identical assembly, complete-owner text and
relocations for both baseline and final source. The final owner checks
26 functions exact with no differing owned data. The early -200, -100 and
-210 results, name validation, `flags & 7`, info copy, name termination,
cache flush, RPC arguments and semaphore release behavior are unchanged.
