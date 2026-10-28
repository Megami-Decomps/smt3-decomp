# Non-matching drafts: result/rstdraw

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## rstDraw (result/rstdraw, 0x1D0)

103 of 116 words match, at the exact 464-byte size.

Residual 5: retail keeps a plain `daddu $3, $4, $0` copy of the work pointer in
the entry block, so the pointer the early-return path stores to `rstDrawWork`
and the pointer the fall-through path reads `unk_2310` from are two live
registers. Every C spelling tried coalesces them, which frees one word and
makes the next jump target need an alignment `nop` instead; the three remaining
words are the register renumbering that follows.

Pinned down on the way:

- The work pointer is the global `rstDrawWork`, re-read after every call, not a
  local kept across them.
- The phase switch is a 25-entry jump table over cases 0 to 24 with 2, 4, 9,
  14, 15, 16, 19 and 23 falling to the default; 0 shares its arm with 24, 7
  with 8, 11 with 12 and 13, and 17 with 18.
- `rstDrawStatusRow`, `rstDrawStatusRow2` and `rstDrawStatusPanel` take `s8`
  arguments, which is what sign-extends `-rstDrawWork->unk_1B` at each call.
- `unk_2310` is a word just below `fadeColor`, and `unk_74` is `s8`.

Ruled out: a local for the pointer in either half of the function, an explicit
`work = rstDrawWork` copy before or after the early-return test, and reading
the flags through the call result instead of the global.
