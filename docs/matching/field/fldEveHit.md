# Matching notes: field/fldEveHit

## fldEveHit_AddEveHit (field/fldEveHit, 0x1868)

`fldEveHit_AddEveHit` remains assembly-backed in `src/smt3/field/fldEveHit.c`. These findings narrow the next source experiment; they do not promote a candidate or award matching credit.

### Qualified observations

The complete-owner baseline enables only the existing guarded C body. Its SHA-256 is `3e57568d049eb1410ea7829b5724b45e040bc818535124b9347f10598751e9b8`. The pinned cc1 SHA-256 is `d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.

Normal, diagnostic (`-da`), and two read-only compiler-observation runs agree on all four allocated sections, 177 non-file symbols, both ordered relocation sections, and whole assembly except `.file` metadata. Both traced runs also reproduce all 22 diagnostic RTL dumps after normalization of generated source-directory names. The observer changes neither compiler instructions nor target flags. These qualifications apply to these exact inputs, not arbitrary future instrumentation.

#### The current allocation trace does not show an equal-priority tie

The shared `index * 4` value is pseudo 117, defined at UID 102, with 17 references, a 1,285-instruction lifetime and 50 calls crossed. Its observed priority is 529. First-case normal pointers 242/244/246/248 have priorities 327/305/303/301; the actual 117-versus-242 comparison is **529 versus 327**, not a tie. Four observed global `find_reg` attempts leave 117 unassigned. The first-case pointers also fail allocation.

Box-case pointers 2455 (`&nrm[0]`) and 2454 (`&triangle[0]`) receive saved registers 21 and 22 at priorities 550 and 549 before 117 is attempted. Later normal pointers receive registers 23 and 30. This corrects the earlier tie-based explanation for the qualified current baseline; it does not reconstruct a different historical run or prove a complete register-conflict explanation from priority alone.

#### GCSE removes the copy's consumers; loop cleanup removes the dead definition

PRE creates UID 9099, `reg3602 = reg117`. GCSE CPROP pass 2 explicitly replaces 3602 with 117 in **22 consumer instructions**. The final GCSE listing therefore retains only the copy definition, not a useful independent lifetime. Counting occurrences across intermediate diagnostics and multiple RTL listings misidentifies live uses.

The subsequent trace follows that same instruction to `delete_trivially_dead_insns` between loop passes 1 and 2. Thus loop cleanup deletes an already-dead copy; it is not where the consumer lifetime is lost. Function mapping accounts for four non-emitting compilation invocations before the 23 emitted functions.

### Controls and remaining property

Native size is 6,248 bytes. Every tested complete owner retains its 22 previously matching peers; the target still differs.

| Control | Emitted bytes | Result |
| --- | ---: | --- |
| Existing C baseline | 6,236 | Shared scaled index spills |
| Capture table-entry pointer once | 4,872 | Substantially different code |
| `LIVE(index)` after resource publication | 6,244 | Better raw overlap, wrong 0x1F0 frame |
| `LIVE(index)` after box transformed-center store | 6,224 | Native 0x1E0 frame and early saved-index pattern |
| Plane-center boundary only | 6,240 | Still not exact |
| Both center boundaries | 6,272 | Extra splitting/reloads; not a fix |

The box-boundary control adds only `FAKEMATCH_ASM_LIVE(index)` on the real initialized index, preserving later consumers and callback reloads; it binds no register and claims no memory barrier. Its SHA-256 is `10c5892eff8524de47ed2d6b6fa5c1c6f185b245d53a1a4c5e6e37b33126ebe4`, with separately verified normal/diagnostic parity.

Native code recomputes `index * 4` at `0x13DC04`, saves it at `sp+0x15C` at `0x13DC0C`, then reuses saved register 21 for a box-normal pointer at `0x13DC2C`. The box-boundary control recovers the early spill/reload/saved-register pattern without changing allocator policy.

Its net 24-byte deficit consists of two `lui`, two `sll`, one `addu`, and one `subu`. At the plane center-store → corner-copy boundary (`0x13CDBC–0x13CDE0`), native reconstructs the table address; the candidate instead reuses the earlier `index * 304` and adjusts the table-plus-0x80 base. Equal net counts of other instruction families do not establish operand, scheduling, or semantic equivalence.

**Remaining question:** obtain the native table-address reconstruction while preserving the existing scaled-index lifetime. Another unscaled-index boundary constraint is not an established solution: the combined control introduces new reloads where native retains saved register 21. Operand-level, scheduling and tail-address differences remain; mnemonic-family alignment is not a matching percentage.
