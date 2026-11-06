# Matching notes: fclRagUpdate

## ragCheckHeartsChoice (fclRagUpdate, 0x80)

32 of 32 words match at 128 bytes with ordinary C. Retain the nested positive
and less-than-four selector checks, put the table return in the `else` arm of
`index != 1`, and leave the zero return after the whole conditional. This real
common-zero exit preserves the extra table-arm branch and native duplicated
saved-register restore. The old draft returned zero inside the non-table arm
and fell through from its final table return, emitting 116 bytes instead.

Qualified complete-owner stock RTL shows the table-arm jump before the trailing
zero block at jump2; the delay-branch output fills that jump with the saved s0
restore and targets the shared ra restore. Plain/dump assembly and literal
entire-object parity passed for production, SKIP_ASM and retained-draft modes.
These observations establish the emitted control flow, not historical source
spelling or a debugger-observed internal compiler decision.

The provider is still called once before dispatch. Selector one returns the
unsigned table item at the signed flag's 14-byte record; selectors two and
three return the existing preview unit's unsigned ID at work+0x54; other signed
selectors return zero. Types, calls, load widths, peer functions and owned data
are unchanged. No register constraints, provider-opacity aliases or extra bound
assumptions are used. Complete-owner checking gives 20 match, 0 differ.

Earlier failed controls remain distinct: index==1 with a goto to trailing table,
a common result variable, goto done from all arms, and multiply reassociation.
A trailing-table one-pass block was also neutral. The common-zero conditional
above supplies the missing branch without any of those changes.


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
