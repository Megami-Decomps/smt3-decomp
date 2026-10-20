# sdk/libpad

Both drafts fail the same way, and it is the same cause: ee-gcc 2.9 gives
retail `$s0` to the slot index and `$s1` to the pointer it loads from
`D_005989D0`, and both drafts get that pair the other way round. Everything
that does not name a saved register matches, the frame sizes agree
(0x150 and 0xB0), and the retail spans each carry a trailing `nop` of
alignment fill.

The index arithmetic itself is right, including that the compiler splits it
across both integer multipliers: `port * 0x70` on `mult1` and `slot * 0x1C`
on `mult`, computed once at entry and again after the RPC, because the base
is not live across the call.

## _send_to_iop

28 of 69 words differ; 276 bytes against retail's 280, the last of which is
alignment fill.

Ruled out, all giving the same 28 words:

- every order of the four locals, including declaring `dma` first and
  assigning `c` after the `dmaId` check (which is worse, 46 words, because `c`
  stops being live across the call);
- writing the DMA destination as `iopAddr + ((seq & 1) << 5)` or with the
  operands the other way round (28 against 29 words, so the shift-first form
  is the one kept here);
- `s32` against `u32` for `id` and `seq`.

## scePadPortOpen

120 words against retail's 122. The same saved-register swap, plus the
double-buffer initialisation loop: retail keeps the buffer pointer and the
counter in registers the draft spends differently, so the loop body is a
register short and reloads.
