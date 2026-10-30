# Matching notes: fclRagUpdate

## ragCheckHeartsChoice (fclRagUpdate, 0x80)

22 of 29 words match, 116 emitted bytes against retail's 128. Returns a hearts
choice: a table entry for index 1, the rag work's `+0x54` halfword for 2 and 3,
and 0 otherwise.

The body is settled. `ragGetHeartsFlag`'s result has to land in `$4`, the
`index == 1` test has to be written as `if (index != 1) { ... }` with the table
arm after it so the `beq` reaches the table arm as retail's forward branch
does, and the two range tests have to be **nested** `if (index > 0) { if (index
< 4) ... } return 0;` rather than two sibling early returns: as siblings
ee-gcc folds them into an `addiu -1`/`sltiu 3` range test and loses retail's
`blez` and `slti 4` pair, which is 8 of 30 words match.

The seven remaining words are the exit layout. Retail restores `$16` twice,
once in the delay slot of the `b` that leaves the table arm and once at the
shared `L830`, and lets `jump2` merge only from the `ld $31` on; this draft
gets one `ld $16` and three words fewer, so every branch displacement in the
function is off by three.

Ruled out: `index == 1` with a `goto` to a trailing table arm (8 of 30 match);
a `u16 ret` local assigned in each arm with one trailing `return` (8 of 30);
the same with explicit `goto done;` out of each arm (15 of 29); and
`(flag * 7) * 2` in place of `flag * 14` at the subscript.

## ragUpdateSeqConfirm (facility/fclRagUpdate, 0x208)

130 of 130 words match at 520 bytes with ordinary C. A shared completed return
and late return-selection call reproduce genuine native joins. The actual flag
switch and direct existing unitwork fields preserve the confirmation paths;
all register constraints from intermediate diagnostics were discarded.

The existing DDS3 work union's unit prefix and 236-byte stride establish that
its typed hp/maxhp/mp/maxmp fields address the same live unit bytes. Exact output
preserves the separate unit pointer passed to registration, post-callback global
reloads and halfword accesses. Three real requirements are traversed; negating
the signed-byte count after integer promotion cannot overflow. Calls and final
process-result publication retain their native ordering.

The flag provider currently normalizes its result to zero or one, so the explicit
no-op case two is not claimed reachable. The inherited valid selector, stock and
table contracts remain; an invalid kind still has the native NULL-requirement
risk and is not newly declared safe. No provider purity, pointer bound or extra
reachability assumption is introduced.

Independent complete-owner checking gives 19 match, 0 differ with normal data
checks. Qualified diagnostic plain/stock-dump complete-owner assembly, text and
relocation parity is output evidence, not an internal debugger trace. Final-source
checks pass the retail checksum, all 149 guarded owners / 591 drafts, seven IRX
owners, 26 tooling files / 427 cases, shell mocks and documentation checks.
Source commit `87ba2423024040605637c92fd918c5adacb1b9b4` passed exact-source hosted CI
[40988954268](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40988954268),
including build and deploy.
