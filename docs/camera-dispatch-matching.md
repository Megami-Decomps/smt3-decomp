# Field camera dispatcher and chase mode

Two functions close together in `field/fldCamera`: `fldCamUpdateChase`
(`0x0012ED88`, 592 bytes) and `fldCamMain` (`0x0012F718`, 1,260 bytes), plus
Main's six-entry jump table. Main is reconstructed from SMT3 native control
flow, with the HD reference `smt-ret/smt3-reconstructed-sources` snapshot
`cdcd3e5874d72e63afae6e771e3b2d8f85cf66b1` corroborating its `void *` return
and mode-dispatch skeleton. Chase refines tge-was-taken's existing published
draft (`76257a2f0`), not a newly discovered donor body. Shared camera-family
DDS provenance remains documented in [normal-camera-matching.md](normal-camera-matching.md).

## Real Chase storage and geometry

Both 16-byte local vectors are passed to actual vector-record providers and
copied with the existing aligned quadword primitive. They now use the existing
`sdfVector4_t`, rather than a union of `u128` and four floats cast to that
record. This is the actual camera/pose API representation, not a new alias
type or an opaque storage trick. Native `memset` calls and homogeneous W = 1
remain; the final camera callback still precedes the persistent vector copies.

This provider-storage correction reduces the old 28-word residual to nine.
Output-parity-qualified ordinary RTL explains the scheduling change: in
`17.sched`, eye XYZ stores 234/253/272 have alias set 0 for the union and
alias set 1 for the vector record. The actual camera-handle load, instruction
287 from control +0x78 (alias set 10), depends on all three stores in the
union control but not in the truthful vector version. Its earlier placement
is exactly the native `lw` at `0x0012EEF4`, not an added pointer read.

The remaining nine register operands close when the target is first built
from player X, player Y minus 200 and player Z, then smoothed toward the prior
target by 0.0625. This is the same real construction-then-smoothing shape as
DirLock; no component-order sweep is involved. Input still uses the native
unsigned stick bytes `[0][0][0]` and `[0][0][1]`, subtracts 0x80, uses 0.7 and
negative 2.5 scaling, and constructs the 256-unit eye position. The legacy
28-word draft entry is retired from `docs/matching/field/fldDraw.md`, where
it remained after the owner split; unrelated entries are preserved.

## Main's native behavior

Main returns NULL on every path, including missing camera or unit. It keeps
all player visibility calls and the repeated event-camera query. Event camera
ownership sets the real first-person reset field +0xF8 before the elevator
update and player visibility cleanup. Areas 18/28/30 select row 1; areas below
10 use the default camera, while stick direction flags are read only for
areas at least 11 with encounter flag 0x40.

The four directional flags start at zero and are conditionally set to one
when the corresponding native threshold is crossed. This ordinary flag form
reproduces native `sltiu ...,1`; direct boolean assignments instead gave two
`xori ...,1` words. Every flag has a real mode-0 consumer, and no additional
read or lifetime is introduced.

FOV selection retains the NPC 50/40-degree cases, the 60-degree first-person
case, and both real row-FOV paths. The expressions use binary32 3.14/180 as
retail does; no address-bound constant aliases are added. The existing shared
`elvstart_label` string remains externally referenced. Modes 0, 3 and 1 retain
their respective fade selectors 1/2, 3/4 and 5/6, native NPC-state reloads after
`fldPlayerOff`, and the wordCC/reset gates. Mode 0 chooses chase or the normal/
first-person camera; mode 3 uses DirLock; mode 1 calls the actual event-target
camera; mode 4 interpolates follow; mode 5 hides the player and updates NPC
camera. Mode 2 and out-of-range values do nothing. Nonzero mode resets the
existing look-around counter and float coordinates. No branch is collapsed
across a callback that can change the tested global state.

All provider declarations follow their real source/native contracts: player
visibility helpers are void, FOV receives a camera basic and binary32 angle,
and `dds3FadeIn` receives four signed 32-bit arguments. Existing field, pad,
camera and callback records are unchanged.

## Genuine same-owner visibility

With Chase still assembly-backed, Main is 314/315 words: native `beql` at
`0x0012FA14` is an ordinary `beq`. Exposing the complete existing Chase draft
in a private control makes Main exact while Chase remains nonmatching. No
stub, no-throw annotation, reordered function or boundary change was used.
The final patch lands only the fully reconstructed and exact pair.

The opaque/visible controls agree in Main's pass-28 branch topology and donor
(after ignoring unrelated label-number/source-path metadata). Call 710 gains
`REG_EH_REGION 0` only when the actual Chase body is visible. In `29.dbr`,
jump 699 gains `/u` annulment with the same target donor; this is the sole text
word that changes in Main. These are output-parity-qualified ordinary dumps,
not a hooked GDB/cc1probe record or proof about a different translation unit.

## Qualification

The final whole camera owner passes **22 matches, 0 differences**. Both
remaining reset drafts compile. The full retail image, including the jump
table and literal pools, passes SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`.

Plain and `-da` complete normalized assembly agree (only the first randomized
`.file` line removed), SHA-256
`0409222a62b9820e33644864ef1c7debd33f463cd252ac5a9302f7fe5b25542f`.
The union, typed-vector and opaque-callee controls each independently have
plain/dump parity. Target-only RTL and their comparison receipts are retained
outside the repository. No new headers, macros, dummy consumers, volatile
qualifiers, aliases, compiler flags, compiler patches or register pinning.
