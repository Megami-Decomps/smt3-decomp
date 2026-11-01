# Matching notes: result/rstdraw

## rstDraw (result/rstdraw, 0x1D0)

116 of 116 words match at 464 bytes. A scoped ordinary-C snapshot of the
initial process work supplies the frame color on the non-termination path.
The global is still assigned before dispatch and reloaded after calls. The
early termination call, 25-entry phase switch, all signed-byte row arguments,
and final effect-slot call are preserved.

After each encyclopedia reset call, a new row-work snapshot reads the global
and supplies that arm's row byte. These two real initialized pointers use a
localized $2 binding, ending before the status-row call. No constrained live
value crosses a call; both reset declarations retain their actual s32-return
interfaces. The bindings are target-specific reconstruction aids, not evidence
of original spelling. No empty asm, artificial consumer or provider change is
used.

Ablation keeps the scoped entry snapshot and removes all bindings: 113/116
words at the same size, differing only in two post-reset reload destinations
and their shared byte-load base. Binding the initial frame snapshot to $3 is
unnecessary and was removed. The two row bindings close all three residual
words. Keeping only the case-17 binding gives 72/116 words and only the
case-20 binding gives 71/116; both expand to 472 bytes and split the shared
tail. Both post-reset constraints are therefore needed together. Keeping only the case-17/18 binding gives 72/116 matching words;
keeping only the case-20 binding gives 71/116. Both emit 472 bytes and lose
the native shared row-load tail, so neither is retained.
Complete diagnostic owner checking gives 2 match / 0 differ including
owned data. Final plain and stock-dump builds have identical allocated and
relocation sections and normalized assembly. Independent current-tree and
retail qualification remain the publication gate.

The phase switch covers 0 through 24. Cases 2, 4, 9, 14, 15, 16, 19 and 23
fall through; 0 shares with 24, 7 with 8, 11 with 12 and 13, and 17 with 18.
The work's frame color input is the word at +0x2310; row flags remain s8.
