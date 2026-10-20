# sdk/libmc

Eighteen of the library's twenty-one `sceMc*` entry points match. Every one
follows the same template, so the three drafts below differ only in their own
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

380 bytes against retail's 384. The leading-byte arithmetic is right, and it
has to be written as two separate subtractions:

    u32 top = ((u32)buf - 1) & ~15;
    u32 bot = (u32)buf - 16;
    s32 head = top - bot;

Folding it into one expression makes gcc compute `((buf-1) & ~15) + 16 - buf`
instead, which costs three instructions. The inline payload must also be a
named member (`req.data[i]`), not `((u8 *)&req)[32 + i]`: the cast form hoists
`base + 32` out of the loop, where retail keeps the base and uses 32 as the
store displacement.

What is left is the store order inside the two size branches (retail writes
`w[6]`, `w[3]`, `w[5]`; the draft emits `w[5]`, `w[6]`, `w[3]` whatever order
the source uses) and the saved register holding the descriptor.

## sceMcSync

224 bytes, retail's size, with every instruction but two in place. The loop

    while (1) { done = 0; if (sceSifCheckStatRpc(&cd) == 0) break; mcDelayThread(60); }

puts `done = 0` in the branch delay slot where retail keeps a `nop`. Rewriting
it as `while (check() != 0) delay(60); done = 0;` is 12 bytes short, so the
`done = 0`-per-iteration shape is the right one and only its scheduling is
wrong.

## sceMcSetFileInfo

464 bytes against retail's 472. Retail spends one more saved register, so its
frame is 0xA0 where the draft needs 0x90; the `ldl`/`ldr` copy and the request
stores are otherwise identical.
