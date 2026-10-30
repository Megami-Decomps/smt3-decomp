# Matching notes: facility/fclShopUpdate

## shpUpdateSeqConfirm (facility/fclShopUpdate, 0xFC)

63 of 63 words match at 252 bytes. The complete owner has 16 matching
functions and no differences, including its owned data.
Source commit: `237c886c51c21ed0ecb8182fc42182180a139c2f`.
Exact-source [CI run 40970963002](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40970963002)
passed build and deployment.

The initialized `scriptState` value 2 is consumed by both the process-status
comparison and the final state-byte publication. The switch includes the
explicit no-op case 2 used by the matched `trmUpdateSeqConfirm` and
`rcvUpdateSeqDispel` family. This recovers retail's signed dispatch tree;
it does not assert that the current `fclGetFlag` provider can return 2.

Case 0 takes one work snapshot for `paymentRemaining` and `paymentStep`.
The signed quotient is divided by 20, with only a zero quotient replaced
by 1. Negative quotients remain negative; this is not a minimum-one clamp.
The final state-byte store independently reloads the global work pointer.
Case 1 starts script entry 2 and publishes state 5 for mode zero, or starts
entry 4 and publishes state 6 for every other mode. Each publication reloads
the current work pointer after its script call. Its final script-state
store again uses an independent global reload after the first byte store.

Ordinary C reaches 58 of 63 words matching, with only the post-call store
pointer registers differing. A qualified local-allocation observation
selects first-free homes `$3`, `$3`, and `$2`, whereas retail uses `$4`,
`$4`, and `$3`. The observation has 1,079 events, with all six artifact
parity checks passing, plus same-path plain/dump assembly and object
agreement and agreement with the checker target. There is no hidden
call-crossing value that explains those register choices.

Two localized register bindings, each with an empty read/write operand
constraint, preserve the native homes of the actual initialized store
destinations. The first pointer is
assigned after either script call, then consumed at the common state-byte
store; no value of it is live across a call. Placing its constraint at this
common publication point preserves retail's merged store, unlike separate
constraints inside each arm. The second pointer is loaded after that store
and consumed immediately. The actual output retains `lw $4`, the common
`sb` at offset 1, an independent `lw $3`, and `sb` at offset 4. An empty
constraint is not assumed to be a memory barrier: the native loads, stores,
widths and ordering were checked in the object. This is a target-specific
allocation aid, not evidence of the original source spelling.

## shpGetItemFromItemBox (facility/fclShopUpdate, 0x148)

82 of 82 words match at 328 bytes. An initialized capacity of 8 is shared by
the real candidate-list VLA and its collection loop. Retail saves the stack
and frame pointer, dynamically allocates another 32 bytes for the eight real
four-byte candidate records, and restores the saved stack on both return paths.
This directly supports the dynamic aggregate; it is not unused frame padding
or proof of the original source spelling. The static-array draft emits 292
bytes. No constraints, aliases, volatile accesses or provider changes are added.

The eight-iteration loop appends at most once per iteration, establishing
count 0..8 and written indices 0..7. The second loop reads only initialized
candidate records. Eight unsigned-byte weights bound total and cumulative
weight by 2040, so accumulation and the nonzero-total remainder conversion
cannot overflow. Zero total is not assigned a fabricated guarantee: C modulo
zero remains an inherited undefined path, with retail's divide trap retained
in the exact output. The existing valid table-row contract is unchanged.

The item snapshot used for filtering remains before the slot/heart callbacks;
the actual four-byte record copy remains after them. No stale pre-call copy,
purity assumption or observation collapse is introduced. Signed slot narrowing,
record order, one RNG call after collection, weighted strict-less-than selection
and the fallback are preserved. The unchanged `dds3GetRandA(void *)` provider
receives `NULL`, selecting the mutable global RNG state.

Canonical complete-owner plain/dump assembly, text and relocations have qualified
parity. Initial RTL distinguishes real dynamic extent calculation and stack
allocation from the static-array control; this is stock-output evidence, not
a debugger-hook claim. Independent final-source checking gives 17 match,
0 differ with normal data checks, including the previously published
`shpUpdateSeqConfirm`. Final gates pass the retail checksum, 149 guarded owners /
594 drafts, seven IRX owners, 26 tooling test files / 427 cases, shell mocks
and documentation checks.
Source commit `6de3a0947aaf3cf6dc88ea98dc33709dea102b05` passed exact-source hosted CI
[40986716570](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40986716570),
including build and deploy.
