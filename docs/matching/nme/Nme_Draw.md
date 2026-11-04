# Nme_Draw matching findings

## nmeMoveCursorBy (nme/Nme_Draw, 0x10C)

2 of 67 words match in the retained guarded draft: 220 emitted bytes
against 268 native bytes, with 48 uncovered bytes. The body is unchanged.

A qualified whole-owner study on 2026-11-03 found that ordinary byte-buffer
comparisons retain the three template stores and the two character stores
without the guarded draft's volatile word temporaries or volatile byte buffer.
The initial template values still have no demonstrated native consumer;
compiler retention alone does not authorize dead initialization.

Native extent is 268 bytes, excluding four bytes of alignment. Existing guarded
source emits 220 bytes (2 matching/53 differing prefix words, 48 uncovered bytes).
An ordinary buffer and established nonvolatile table declaration emit 196 bytes
(1 matching/48 differing prefix words, 72 uncovered bytes). A single do-loop
control also emits 196 bytes and does not restore native duplicated entry/latch
updates. These are negative matching results, not improvements from shorter code.
All 20 exact peers remain exact; owned .data/.bss are empty.

A separate declaration-only control changes D_005516B8[3] to an incomplete
external array, recovering HI16/LO16 rather than GPREL16 addressing. It emits
204/268 bytes (1 matching/50 differing prefix words, 64 uncovered bytes).
The data owner supplies three accessed bytes plus padding; padding does not
prove a four-byte C object, and this control does not recover historical spelling.

Complete plain/-da output parity holds for all controls: assembly, allocated
sections, non-file symbols and ordered relocations. Stock dumps retain both advance
blocks through .22.ce2; the initial block is absent by .25.sched2. This brackets
the transformation, not its exact internal decision. .09.loop reports hoisting
the table address and 128 constant, while the pointer load remains in the loop.
No GDB-hook decision evidence or new exact match is claimed.

Do not repeat these buffer, natural do-loop, or incomplete-template controls
without new native/provider/compiler evidence. Keep the assembly fallback;
initialization semantics/policy and substantial loop/addressing residuals remain.
