# Non-matching drafts: facility/fclShopCalc

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## shop_Process (facility/fclShopCalc, 256 bytes)

37 of 64 words match at the exact 256-byte size. The prologue is exact: the
`shopProcWork = dds3GetProcessData(id)` store in the `fclChkDbgProcess` delay
slot, the `started` guard around `shopStartEvent(1)`, the `fclGetFlag(1)` exit,
and at the other end the shared `step = 0` tail, the `flags & 0x10` test and the
`cmpUpdateTerminate` exit returning -1.

The difference is the dispatch on `state`. Retail is a `beql` to the `state == 0`
body with the `step` load in its delay slot, then `bltz` to the default, then a
`beql` for `state == 8`, then `b` to the default; the draft's two-case switch
emits `beq`, `beql`, `b` and no `bltz`. `emit_case_nodes` drops the
`index < low` test to the default when the node's right subtree is a single
value, so retail's switch had a third node, and the `bltz` reaching the same
block as the default means that extra case's body is empty.

Ruled out: an empty `case -1:` or `case -2:` ahead of the others (42 of 64, one
word over), an empty `case 9:` after them (32 of 64, one word over), and an
`if (state == 0) ... else if (state >= 0) { if (state == 8) ... }` chain, which
puts both bodies inline and is 43 of 63.
