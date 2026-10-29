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
