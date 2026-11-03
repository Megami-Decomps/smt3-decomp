# Matching notes: newbattle/nbTest

## nbSetBattleDummy (newbattle/nbTest, 0x2B4)

Actual caller extent blocker.

No source promotion or guarded-source cleanup is retained; the existing guarded draft and production assembly fallback remain unchanged.

Native TestProcess8 calls nbSetBattleDummy at 0x28C708 with devil-list address 0x526AC0 and skill-list address 0x526AD0. The initialized devil list is 194, 192, 193, -1. At the native 30-word row stride, the third skill row begins at 0x526BC0 and contains 23 nonnegative words before its signed sentinel. Native datSetDevilWork uses the table at 0x333CF8 with 102-byte records, 24 entries starting at offset 6; the initialized ID 193 record supplies five initial skills. If these values reach the call unchanged, appending that row produces 28 skills against the real skill[24] field, reaching the following command/index/target fields.

This is concrete native-call-edge and initial-image evidence, not a demonstrated gameplay overflow. Runtime reachability of TestProcess8 and prior mutations of the lists or provider table remain unresolved. The existing C array indexing therefore lacks a complete semantic justification for this native input path; neither volatile qualification nor its removal resolves it. Do not add an invented clamp or enlarge the array to conceal the discrepancy.

The next investigation must establish TestProcess8 dispatch/reachability and relevant data writers, then determine the intended native object/row extent and any cross-field writes before changing the C representation. A qualifier-only private diagnostic preserves the measured native reloads but remains nonexact (676 bytes versus 692 native bytes). Equal size or word scores do not establish identical memory-access output. No matching credit or universal semantic qualification is claimed.
