# Matching notes: newbattle/nbSkillError

## nbCheckBattleSkillError3 (newbattle/nbSkillError, 0x184)

97 of 97 words match at 388 bytes in ordinary C. The real result accumulator
starts at zero, becomes 3 only when CheckHojo returns zero, and supplies the
common switch return. Nonzero-unit default paths genuinely consume its initial
zero after collection callbacks; this is not a dead initializer or invented
lifetime. An explicit zero-unit branch returns zero independently of partyCount.
The later native call-delay zero reset is compiler-generated, not an inserted
source reset or a dead-code exception.

Both existing 32-pointer arrays correspond to real native stack regions at
sp+0 and sp+0x80. Only selection bits 0..15 are scanned, so either independently
filtered array receives at most 16 entries. For every selected bit, the unit
getter precedes the party getter even when the unit is NULL. The signed sparse
switch protects table access and admits only 0x34..0x36, 0x40..0x43, 0xCB..0xCE,
0x109 and 0x114. Providers, declarations, layouts and data remain unchanged;
no constraint, asm, volatile, synthetic aggregate or fake consumer is introduced.

The promoted original early-return control is 384 rather than 388 bytes.
A genuine accumulator with the old positive-unit wrapper reaches 388 bytes
but leaves 16 words different; the explicit zero-unit edge completes the match.
Final expansion shows result pseudo91 initialized before collection, with
actual default consumers and common return; global allocation maps it to
hard30. CheckHojo feeds the actual conditional selection. These are observed
compiler facts, not original spelling or an inferred internal comparator rule.

Independent review reran final and accumulator-wrapper-control complete-owner
plain/dump parity: normalized assembly, allocated-section contracts,
relocations and function inventories agree. The worker additionally qualified
the original early-return baseline; that later baseline comparison is not
claimed as an independent-review rerun. Bounded review checks pass 3,342,642
signed-branch/result cases. A separate model checks all 65,536 selection masks
with seven representative NULL-presence patterns for order, capacities and
high-bit irrelevance. Neither model proves arbitrary callback/memory execution
or the full signed32 domain.

Independent complete-owner verification gives 16 match, 0 differ, preserving
all 15 peers and owned data. Retail checksum, 427 tests across 26 tooling files,
147 guarded owners / 546 drafts, seven IRX owners and report gates passed for
source `c41e8038dfbf0fb6f21e0120b4dbc3dbd2d2ecb1`. Exact-source CI
[41080593409](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41080593409)
attempt 1 completed successfully, with every actual step passing and none skipped.
