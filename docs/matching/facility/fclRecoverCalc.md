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

## rcvDrawPanelAt (facility/fclRecoverCalc, 0x1AC)

107 of 107 words match at 428 bytes. An ordinary per-iteration `s32 price`
snapshot loads `rcvDrawWork->recoverPrices[i]` before comparing it with
`DDS3_GBWK->maka`. Both fields are signed 32-bit values. This expresses
retail's price-before-money load order and resolves the previous five-word
load/register-order residual. No declaration, helper, compiler constraint,
volatile qualifier or provider change is involved.

The price has a genuine comparison consumer. There is no intervening call or
write between the two operand loads, and the snapshot stays inside the loop.
Draw callbacks, subsequent cursor and row-spacing reloads, the loop bound,
and end-of-function callbacks are unchanged. This is a measured source/output
relationship, not a claim about an internal compiler pass or original source
spelling.

Independent semantic review and canonical complete-owner checking pass
22 match, 0 differ, with owned data exact.

Combined local gates pass the retail checksum, 26 tooling test files / 427
cases, all 147 guarded owners / 537 drafts with stable source/compiler inputs,
seven IRX owners, and reconciled production reports.

## rcvFullHeal (facility/fclRecoverCalc, 160 bytes)

All 40 of 40 words match. The initialized signed result mask is bound locally
to its native callee-saved register, and the MP comparison sets its genuine
bit with `result |= 1`. On that path the incoming mask is only zero or one,
so this is equivalent to assigning one. The later status tests OR mask value 2 for
0x20, 0x40 or 0x100 and mask value 4 for 0x800; the result remains in 0..7.

HP and MP comparisons retain their unsigned types and actual unsigned
providers. The HP halfword is read before the maximum-HP call, MP after that
call and before the maximum-MP call, and the status halfword after both calls.
The mask is initialized before its real lifetime spans the MP call. That
provider saves and restores the selected callee-saved register, and this
function preserves its caller's value as well. No type, layout, callback,
alias, volatile or capacity promise changes.

Qualified ee-gcc 2.96 traces show that the localized binding and real bit-set
update retain five branches through combine; later conditional conversion
creates the five native XOR-zero/select sequences. The unconstrained form
converts earlier and loses the XORs during common-subexpression elimination.
Removing only the binding emits 132 bytes; replacing the MP bit-set with an
assignment emits 156. Both changes are needed. The constraint is a narrow
reconstruction aid, not evidence of the historical source spelling.

Independent semantic, provider-ABI and compiler-evidence review passes.
The public complete-owner checker reports 23 match, 0 differ. Baseline, final
and both minimal ablations pass all twelve three-mode plain/dump complete
assembly and ELF-file comparisons. Normalization is confined to temporary
source-filename metadata, never code or data. All 23 production, 22 SKIP_ASM
and 23 retained-C peers and their resolved relocation targets are preserved,
as are all allocated data. The production text and relocation entries are
byte-identical to the assembly-backed baseline; among production symbols,
only local compiler label suffixes change. Earlier stock-recovery and price-display matches remain exact.

Final source and both controls also pass strict implicit-declaration checks in
all three modes, with identical output to their non-strict builds. The baseline
passes strict retained-C checks; its production and SKIP_ASM contexts retain
one inherited implicit target declaration at the caller because the definition
is hidden there. Promotion supplies the definition without adding declarations.

Full retail, generated-source, regression, guarded-source and report gates
pass. Production provenance verifies a unique compiler-owned linked 160-byte
span matching retail. Game coverage gains one function and 160 bytes, with the
same increase in the all-unit report.
