# Scalar declaration order and reload spill slots

`effPCPDANTE_Damage_Run` keeps a 48-byte placement block at stack offset zero,
then four spilled scalars: the formation index, target bits, compact target slot
and total duration. Declare the scan state before the timing snapshots. This
reproduces their native homes at `0x30`, `0x34`, `0x38` and `0x3C` without an
artificial aggregate, padding or volatile storage.

The qualified `cc1probe --alloc --reload` comparison passed hooked/unhooked
assembly and RTL equality and dumps/plain assembly equality (`verify: OK`).
With timing snapshots declared first, total duration is pseudo 85 and the
formation index is pseudo 92. Reload's `alter_reg` assigns them `0x30` and
`0x3C`; those two swapped homes leave eight of 180 words different at the exact
720-byte size. Declaring `i`, `targetBits` and `slot` before `total` gives them
pseudos 85, 86, 87 and 88 respectively. The trace assigns those four pseudos
successive slots `0x30`, `0x34`, `0x38`, `0x3C`, and all 180 words match.
The counters and duration are independently used values; no variable is kept
only to tie registers.

Reproduce with `tools/cc1probe.sh <owner.c> --as-unit
src/smt3/effect/effPCPBoss.c --func effPCPDANTE_Damage_Run --alloc --reload
--out <output>`. The comparison variant declares and initializes the timing
snapshots first, followed by the unit and target-bit snapshots, `slot`, and
`i`. The formation index still initializes in the `for` statement in both.

Two ordinary source distinctions matter before investigating spill slots:

- Index the three parallel arrays by the compact target slot. Advancing three
  independent pointers instead shortens the native loop substantially.
- Compute `total - frame` only in the fade-out arm. An unconditional calculation
  before the fade-in test preserves the result but changes branch scheduling.

The timing snapshots precede the main-data and target-bit callbacks. The compact
slot advances only for active selected formations; the formation index advances
for all 32 slots. Keep the shared placement block alive across the loop and
reload a cut's counter after its draw callback.
