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

## fldEveHit_AddEveHitItemBox (field/fldEveHit, 0x1F8)

126 of 126 words match at 504 bytes in ordinary C. Keep the real
`fldEveHit_AddEveData` return in a separate `eventIndex` local while grouping
the distinct sphere, put and hit-field stores before their next callback.
Initialize a real `s32 hitKind = 2` before incrementing the count and addressing
the returned hit-table slot; its consumer remains the signed-halfword store.
Neither phase uses a constraint, register binding, volatile access, dummy
consumer, invented type or changed compiler flags. Separate event and hit
indices are equally exact to the initial shared-slot diagnostic and make
ownership clearer. Removing the event-initialization changes leaves 104 of
126 words matching; removing only the hit-kind local leaves 116 matching.
These are paired output observations, not recovered original spelling or an
unmeasured compiler-internal mechanism.

Independent native/provider review preserves the count snapshot before the
first allocator, distinct event and point ids, six-call order, full transform
initialization before the second allocator, callback-sensitive reloads, and
the post-registration count increment. No store moves across a callback.
Final halfwords retain the item index, kind 2 and NPC index -1. Existing valid
record/index and alias contracts remain necessary; no corrupt-object or
portable-layout guarantee is inferred from matching.

Complete owner: 23 match, 0 differ. Independent review resolves all 504 target
bytes, including the three GP-relative count accesses. Eighteen development
plain/stock-dump whole-assembly and entire-ELF parity pairs cover six sources
in three modes. A fresh integrated guarded/final comparison adds six passing
pairs in production, SKIP_ASM and retained-C modes, with all six strict builds
clean and 133 collected input hashes stable. All 27/22/27 peers and owned
data remain unchanged after resolving relocation targets. No hooked compiler
trace is claimed.

Integration on the previously published status-effect match preserves that
owner at 390/0. Full retail checksum and byte comparison, generated-source
validation, 505 unittest cases without skips plus 818 layout assertions,
frozen guarded EE/IRX sweeps, and reconciled game/full-binary reports pass.
Configure adds no source imports. The target is C-owned in the production
report at 504 bytes; game coverage increases by one function and 504 bytes,
to 8,652 functions and 1,502,568 bytes. The subsequent build has no work.

## fldStatusSet (field/fldEveHit, 0xF8)

62 of 62 words match at 248 bytes. The final body captures the complete work
object once and walks its 16 actual 0xEC-byte records using a record offset and
a separate status-biased address. Both byte-pointer walks are anchored to the
complete declared object, including the final unused advance. The native u32
flag, u16 HP and u16 status widths and alignment are preserved. The ordered
status tests map 0x40, 0x100, 0x20 and 0x8000 to summary bits 1, 4, 8 and 0x10.

One empty read/write operand follows the actual 0x100 status snapshot and uses
the initialized status address. Later real reads and loop advances consume
that address. It is dead before the provider call, with no fixed register,
volatile alias, artificial consumer or emitted instruction. It is not a general
memory barrier. Removing only that operand emits 244 bytes with 19/61 overlap
words matching and 4 native bytes uncovered. Development proved the earlier
binding and extra operand unnecessary and removed both. This supports the
remaining local dependency without an unobserved compiler-pass claim.

The provider argument is the canonical &work->unitwork[0] union view at +0xA54.
The real datCheckDying provider computes hp * 100 / maxhp < 25 from u16 fields;
nonzero maxhp remains its valid-domain requirement. The captured pre-call work
object supplies the argument. A nonzero return, including conservative negative
hook results, causes a fresh signed word50 read/OR/write, preserving callback
changes. Redirection of DDS3_GBWK does not retroactively change the argument.

The current activated original draft emits 236 bytes: 7 of 59 overlap words
match, with 12 native bytes uncovered. This is distinct from the older exact-
size 36/62 experiment recorded below. Final20 uses the reviewed canonical union
argument, not a superseded earlier candidate. The complete owner is 24 match,
0 differ. Guarded, original, final and ordinary-ablation owners pass twelve
fresh complete assembly/entire-ELF plain-versus-stock -da checks, after only one
equal-length temporary source filename normalization. All strict builds pass.

All 27/23/27 peers preserve resolved instruction/relocation contracts. An
independent audit also preserves owned bytes, section sizes/alignments/flags,
NOBITS, COMMON and non-target symbol locations, ordered relocations and paired
HI16 effective targets, including R_MIPS_LITERAL and the lit4 pool.

The exact final body passes 1,114,112 optimized and 1,114,112 ASan/UBSan host
comparisons. A separate interpreter freshly executes the exact 248 raw retail
bytes against that body for 327,680 cases under each build. All 62 instructions
and both outcomes of all eight conditionals are covered. Both runs produce
the same signature and cover all 16 slots, every u16 status/HP/low flag value,
valid provider arithmetic, callback writes, global-pointer redirection and
caller-register clobbering. Complete modeled objects, callback entry, and native
read widths/counts/addresses are checked. The host layout asserts the actual
complete-work size and accessed offsets. These are bounded modeled-provider
fixtures, not full game execution or guarantees for invalid maxhp/indexes.
LeakSanitizer alone is disabled for executor compatibility.

The linked retail image is byte-identical. Generated-source negative controls,
505 tooling tests with no optional skips, 818 layout assertions, frozen EE/IRX
drafts, inline-asm policy and primary/audit report-scope checks pass. Complete
facility 197/0, recovery 391/0, music 27/0 and Flow10 33/0 owners remain intact.

### Historical draft experiments

This earlier experiment had 36 of 62 words matching at 248 bytes. It is
not the current activated-original baseline, which emits 236 bytes with
7 of 59 overlapping words matching. The observations below are historical.

Retail carries two induction variables where the draft has one: an integer
offset added to `DDS3_GBWK` serving `hp` and the first `badstatus` read, and
a pointer biased at the `badstatus` field serving the flag at `-0xE`. The
body never uses the loop index.

Experiments that did not match at that stage: the typed `akuma[i]` form throughout; a `fldAnahoriAkuma_t *`
local at three scopes; all eight cast orders of `(u8 *)base + ofs` against
`(u8 *)ofs + (u32)base`, which gcc folds together here; a `u16 *` walking the
`badstatus` field with the flag at `p[-0xE]`; and an explicit second pointer
induction.

This draft proved `fldAnahoriAkuma_t.badstatus` at `+0x12`: retail tests that
`u16` against 0x40, 0x100, 0x20 and 0x8000, and the early draft passed
`&akuma[0].flag` to `datCheckDying`. That native address makes the record
`datUnitWork_t` shifted by four, placing `badstatus` at `0x0E + 4`. The final
code uses the canonical `unitwork[0]` union view of the same address.
