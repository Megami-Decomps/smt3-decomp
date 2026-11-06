# Matching notes: newbattle/NbNegoFlowQuestion

## Flow7_20 (newbattle/NbNegoFlowQuestion, 0x130)

76 of 76 words match at the native 304-byte size. Two localized empty
operand constraints preserve the separate low/high complement paths. The
rest is ordinary C with shared forward selection paths; no register binding,
helper, declaration, layout or compiler-flag change is needed.

The table remains an array of seven-entry rows of `{u16 type; u8 lo; u8 hi;}`.
After both candidate-registration calls, native code reads type, high and low.
Type zero compares signed `datGetParam` results, type one compares unsigned
level fields, and the default uses 50. A zero selection uses the selected
byte directly; a nonzero selection uses 100 minus that byte. The final random
comparison remains signed. Table/index validity is the existing caller domain,
not a new bound asserted by this reconstruction.

Each low/high path first initializes the real rate from its byte. Only inside
the nonzero-selection arm, `FAKEMATCH_ASM_LIVE` constrains that same initialized
byte-derived scalar immediately before its real subtraction. The empty `+r`
template neither changes the value nor emits an instruction. Neither operand
crosses a call within its constrained lifetime. This is a target-specific
compiler reconstruction aid, not evidence of historical source spelling.

Removing only those two constraints gives a 296-byte ordinary control. Its
first CSE pass substitutes the selected-rate pseudo into both subtractions;
the later jump pass merges their complement blocks. With the constraints,
first CSE retains the distinct low/high operands, both subtractions survive,
and the final code places each native rate copy in its selection branch slot.
These are qualified RTL/output observations, not a hooked comparator trace.
The original duplicated-arm draft emits 316 bytes. Earlier selector, goto and
partial-initializer controls remain rejected; they did not supply this live
operand boundary.

Independent review verifies unsigned table loads, signed arithmetic and
comparison, provider/callback ABI, and the native callback-sensitive accesses:
enemy work after the first parameter call, selection after the parameter
calls, and candidate IDs after the random call. A host harness checks
23,592,960 byte/rate/comparison cases and callback reload scenarios. It is a
focused semantic check, not EE execution or a whole-program range proof.

All twelve complete plain/dump assembly and literal entire-ELF pairs pass for
baseline, paired ordinary control, candidate and guarded candidate in three
build modes. Only each observed equal-length scratch source pathname is
normalized. The 659-file explicit source/tool/runtime closure is stable,
including configured QEMU and the actual second loader. It is not a complete
operating-system snapshot. Independent audit finds the candidate's complete
allocated sections and relocation records identical to native guarded
production, with all ten peers preserved. Both whole-owner checkers report
11 match, 0 differ.

Early independent checks reported three padding diagnostics in unchanged
peer jump tables while a separate baseline regeneration was active. After
regeneration completed, serial checks in both minimal and ambient environments
passed. The temporal overlap is a plausible explanation, not a proved cause;
the initial failures are retained privately rather than discarded. Final
canonical owner, retail checksum, generated-source, tooling, retained-draft,
IRX and production-report gates accompany publication.
