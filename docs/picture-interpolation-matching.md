# Picture interpolation matching

`evtPictureProcess1` matches all 552 game EE instruction bytes. The complete
picture owner has 23 matching functions, including data, and no guarded draft.
Both the disabled flag and null-texture paths return zero before advancing
counters. The former null-texture bare return was compiler-inert but did not
express that integer callback contract.

Position and alpha interpolate with signed integer differences, products and
divisions; zoom converts its signed endpoint difference to binary32 before
multiplication and division. Only positive counters advance, alpha clamps to
0..128, and the completed 0x30-byte priority row is added after all field reads.

The old assertion that the position counter must be stored last applied only
to the former allocation. Two ordinary working scalars close its 33-word
residual and permit the native earlier store:

- One signed integer endpoint-difference local is reused for X, Y and alpha.
  The position counter is stored before interpolation. This preserves native
  arithmetic interleaving while giving the differences a common real working
  value; no extra operation or dummy use is introduced.
- One float local holds the zoom endpoint difference, then is overwritten by
  the horizontal and vertical packet-size differences. The phases consume
  distinct values through the same scratch scalar; this does not equate those
  three mathematical differences.

The integer change alone leaves eight zoom words different. The float change
alone leaves 25 integer words different. Together all 138 words match. Qualified
stock compiler traces of the old and final actual-owner forms pass normal,
dumped and hooked assembly and every RTL parity check. Final allocation places
the integer working pseudo 89 in `$8` (six references, live length 7), the float
working pseudo 90 in `$f7` (eight references, live length 20), work pointer 85 in
`$9`, and texture pointer 86 in `$10`. The position/alpha/zoom counters receive
`$2`, `$5` and `$6` respectively. These are compiler observations, not register
constraints. Independent native/provider review confirms the phase boundaries,
field types and absence of intervening callbacks.
