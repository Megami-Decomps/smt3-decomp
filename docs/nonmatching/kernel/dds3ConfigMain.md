# Non-matching drafts: kernel/dds3ConfigMain

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## dds3UpdateConfig (kernel/dds3ConfigMain, 0x1EC)

30 of 121 words match; the draft emits 476 bytes against retail's 492.

The structure is retail's. What differs is one load: retail reloads `GBWK[0]`
inside the arm that sets bit 4, and the delay slot of each `beql` into that arm
carries the `lbu`. The draft's entry test `GBWK[0] & 0x40` leaves the byte in a
register that CSE then reuses in the arm, which drops the `lbu`, shortens the
arm by a word and moves four `.p2align` padding `nop`s elsewhere. Marking just
the entry read `volatile` reproduces retail's reload and takes the match from
30 to 30 matching words at 484 bytes, so the reload is confirmed as the cause,
but a `volatile` there is not what the original said. The remaining difference
after that is the register pair: retail puts the row index in `$16` and the
button mask in `$17`, the draft the other way round, and no declaration or
assignment order tried flips it.

Pinned down on the way:

- The process-id parameter is unused; `cmpGetPressedButton(0xF3)` is the first
  thing the function does and `playSE` is initialized in its delay slot.
- The row index is `*(s16 *)(GBWK + 0x3E) + GBWK[0x40]`, computed before the
  `GBWK[0] & 0x40` early return, whose value is -1.
- `DDS3_GBWK + 0x11AB8` is rematerialized in each arm; a local for it costs a
  callee-saved register and 12 bytes.
- `GBWK` is used as the global throughout the switch, not copied to a local.
- The config-flag arms share one tail: case 0x40 proceeds when the flag reads
  0, case 0x80 when it reads nonzero, and both then skip row 3. A `goto` into
  the 0x80 arm's tail from the 0x40 arm gives retail's single shared block.
