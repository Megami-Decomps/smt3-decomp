# Non-matching drafts: newbattle/nbUnitProcess

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## UpdateFormAnime (newbattle/nbUnitProcess, 0x260)

138 of 152 words match, at the exact 608-byte size. Production remains
assembly-backed.

The typed draft preserves the dying-motion gate, acceleration and progress
clamps, VU0 interpolation of the position, pose callback, movement-bit clearing
and HP scale update. It keeps pre-update progress separate from the published
blend value, and clears movement bit 0x20 from the captured animation status.
After the pose callback it reloads both status and progress; the no-callback
path retains the snapshots, as retail does.

The fourteen remaining words are concentrated in the interpolation block.
Retail copies animation status from $6 into $3 in the first branch delay slot,
and blend from $f1 into $f2 before subtracting the complementary weight. The
draft coalesces these values, changes the first branch to its annulled form,
and shifts the scale/vector-selection sequence. All other instructions match.
A separate movement local and a one-pass complementary-weight block did not
restore either copy. No dummy copies or register constraints are retained.
