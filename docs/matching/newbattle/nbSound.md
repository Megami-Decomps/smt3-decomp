# Matching notes: newbattle/nbSound

## nbLoadMotionByIndex (newbattle/nbSound, 0x78)

The target at `0x0028D100` has a native extent of 120 bytes / 30 words, ending
at `0x0028D178`. Promoting the existing ordinary draft in the complete
`src/smt3/newbattle/nbSound.c` owner emits 112 bytes / 28 words: 16 of 28 words match,
12 differ, and eight native bytes are uncovered.

Both return-arm arithmetic tails survive through stock `25.sched2`; by
`27.jump2`, their common addition of 4096 and left shift have merged. This is an
observed pass boundary, not an explanation of an internal comparator or evidence
of historical source spelling.

A diagnostic empty read/write operand (`FAKEMATCH_ASM_LIVE`) on the initialized
mapped id immediately before its true-arm arithmetic emits 120 bytes / 30 words,
but 15 words differ. In `20.greg`, the reported allocation order changes from
pseudo registers 87,86 to 86,87: mapped id moves from `$17` to `$16`, and motion
moves oppositely. The common tail still merges at `27.jump2`. Recovering native
size is therefore not an exact reconstruction.

For each form, the target fragment is identical across production, complete-owner
`SKIP_ASM`, and retained-draft contexts at every available stock dumped pass.
There is no demonstrated stripped-source context discrepancy. Complete emitted
assembly and ELF parity between plain and dump builds was verified in all three
modes with stable inputs, normalizing only the single equal-length generated
source path. The diagnostic preserves 25/24/25 peers respectively and all owned
data. Both forms pass strict implicit-declaration, uninitialized and warning
checks.

The mapped id comes from the existing unsigned-halfword table. Motion is the
sign-extended byte returned by `nbGetMotionTable`; negative motion returns zero.
For nonnegative motion, both paths compute the same unsigned-shift result.
The diagnostic changes no call target, memory width, provider type or result
contract, and introduces no dummy consumer. It is not proposed for landing.

Keep the ordinary draft and assembly fallback. A useful next investigation must
explain how the two real tails remain distinct through `27.jump2` without changing
behavior. Arithmetic spelling probes, the tested live-id operand and an
exact-signature opaque provider declaration did not solve this case.
