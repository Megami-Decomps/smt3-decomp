# Matching notes: facility/fclRecoverCalc

## rcvCreateLocalStock (facility/fclRecoverCalc, 0x25C)

151 of 151 words match at 604 bytes. Ordinary direct field expressions through
an existing union replace the draft's local unit pointer. Flag and bad-status
tests use `DDS3_GBWK->akuma[slot]`; calls still receive genuine `unitwork`
pointers. The original signed stock-order conversion is retained. No new type,
shifted object, alias or compiler constraint is introduced.

The declared union overlays `akuma[16]` with the four-byte prefix followed by
`unitwork[16]`. Both records have stride 0xEC. The akuma flag and bad-status
fields are at +4 and +0x12, corresponding to the unitwork fields at +0 and +0xE
after that prefix. Thus both views address the same global bytes, at +0xA54 and
+0xA62 plus the slot stride. Target-compiler layout assertions, including the
last array element, verify the existing layout. Native access widths and
addresses also agree; this is not a fabricated object starting at global+4.

The direct expressions naturally recover retail's shared address calculation.
The local-pointer draft emits 588 bytes; direct fields with an explicit global
snapshot emit 612, and a loop-condition snapshot emits 608. Removing the
unneeded source snapshot gives the exact 604-byte output. These are paired
output observations, not an internal allocator trace or evidence of historical
source spelling. Complete-owner plain and stock-dump assembly, text and
relocations have qualified parity for the diagnostic baseline and exact source.

The party and reserve passes, signed mask, dead exclusion and healing test are
unchanged. Native fresh DDS3 global loads after heal and price callbacks, and
fresh recovery-state loads for prices and cursor setup, remain intact. The
existing valid stock-index, output-capacity, global-pointer and object-lifetime
contracts still apply. No arbitrary negative slot, oversized stock count or
hostile callback mutation is newly asserted safe. Original signed-byte counters
and final byte narrowing remain; provider declarations are unchanged.

Independent final-source owner checking gives 21 match, 0 differ with normal
owned-data checks. Final-source gates pass the retail checksum, all 149 guarded
owners / 589 drafts, seven IRX owners, 26 tooling test files / 427 cases, shell
mocks and documentation checks. Source commit `76d2379f33b8ca616657414d24bf8510f3f6c82d` passed exact-source
hosted CI [40990612021](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40990612021),
including build and deploy.
