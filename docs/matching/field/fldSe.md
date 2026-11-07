# Matching notes: field/fldSe

## fldBgmBitChk (field/fldSe, 0x7C)

31 of 31 words match at 124 bytes with the now-matched `fldBgmPlay` C
provider visible. The body and interfaces are unchanged. No artificial
declaration, visibility shim or provider demotion is used.

### Historical dependency qualification

The earlier isolated draft had 30 of 31 words match at 124 bytes.
The first event-result branch at +0x2C lacks native's annul bit. The area-0x22
gate, opposite event-bit tests and two fade-out/play paths are preserved.
`fldGb` is a scalar in `.data`; `gFldAreaBgm` is a signed halfword in `.data`.
The actual event-bit provider takes and returns `s32`.

Current-unit isolation identifies a concrete dependency: making only the real
`fldBgmPlay` C definition visible closes all 31 words. Making each of the
other three preceding guarded definitions visible individually leaves 30.
That observation required playback to qualify before this caller could be promoted;
the reconstruction below now meets that dependency.

Paired qualified reorg traces preserve normal, dumped and instrumented assembly
and all intermediate representations. First-branch UID59 retains probability
3999 and `mostly_true_jump` returns zero in both controls. With the playback
callee opaque, `can_throw` stops the fallthrough liveness scan at call UID74;
the scan reports only the stack pointer live and accepts return-address
restore UID174 with flags 0x11, without annulling. With the C definition
visible, the scan reaches the return-address use and reports both stack and
return-address registers live; the restore is selected with annulment. This
qualifies a callee-visibility effect rather than a probability change. This trace describes the earlier visibility controls, not a new trace of the
final playback source.

## fldBgmPlay (field/fldSe, 0x2D0)

180 of 180 words match at 720 bytes. The initial bank, the area-14 bank
refreshed after its callback, and the final event-selected bank have separate
ordinary scalar lifetimes. Those three real snapshots preserve all original
reads and stores; no cached bank or area is carried across a callback in place
of a required reload.

Two localized reconstruction aids remain. The initial bank is published to
`fldSeSmgId`, then a read/write live operand retains that same initialized
bank for its actual track-sum consumer. Native performs the store before the
sum (the sum occupies the area branch delay slot). The empty template emits
no instruction and is not a memory barrier; the dependency is only between
these real uses of the bank value. A block-local signed `specialArea = 14`
bound to `$4` supplies the actual area comparison. It dies at that comparison,
before `evtBitCheck` and all later calls. Neither aid adds an observation,
artificial consumer or call-crossing fixed-register value. These are local
compiler reconstruction aids, not original-source claims.

The original shared-bank draft gives 161/180. Splitting the callback-refreshed
bank alone gives 168/180; separating the final selected bank as well gives
175/180. A separate initial sequence snapshot is inert. The scoped comparison
binding alone gives 178/180, leaving only the bank-store/track-add swap.
The bank live operand without that binding gives 173/180; together they are
exact. Function-entry comparison-constant controls were rejected rather than
retained, because their lifetimes cross calls. No failed control is part of
the final source. The retained fallback gate, arithmetic and callbacks are
described below.

### Earlier ordinary-source reconstruction

161 of 180 words match at the native 720-byte size. The older nested playback
gate emitted 692 bytes with 142 matching words in the native overlap. Returning
early when `gFldStart == 1` leaves the actual playback call at the function
tail, recovering native's sibling call and separate epilogue. The 19 residual
words concern caller-saved register choices and scheduling in the initial
bank lookup, the area-14 refresh and the final event-table bank lookup.

Native reads the start gate immediately after direct track resolution. Fallback
paths reload it after their event callbacks; the shared late source test
preserves both cases. An unconditional pre-fallback snapshot would not model
those reads. The decimal-area-20 floor routing remains distinct from the later
hexadecimal area 0x20 event-table case. Both event-table arms use `floor + 1`,
update the cached event result, then refresh the current bank and track.

Independent native/provider review confirms the initial floor skip, area bounds,
area-14 stop/reset path, special floors, event-table reloads and final audio
updates. Reading the just-published bank through the global rather than its
local leaves the same 19 differences. These older controls did not qualify a match; the localized final reconstruction
above resolves the residual without changing these native/provider contracts.
## fldBgmContinue (field/fldSe, 0x294)

165 of 165 words match at 660 bytes with ordinary C. No target-specific
register binding, empty compiler operand, volatile view or synthetic
computation is retained.

Initial and post-callback bank snapshots feed their track sums before being
copied into the current bank. The final event-selected bank has its own local.
The sums explicitly retain native 32-bit wrapping. The area-14 path publishes
the initial bank before its bit check and refreshes the bank and sequence
unconditionally after the false/true join, including after an optional stop.
The decimal-area-20 path publishes the ordinary bank only on its non-special
floor route, before the optional event check; rejection leaves that callback's
observations intact. Event-table paths re-read area and floor after their
callback. An early start-gate return leaves the resume operation at the native
function tail.

An intermediate constrained form was exact, but removing all its constraints
at once from the corrected ordinary source also matched. Individual constraint
ablations regressed; that did not make the constraints necessary. The final
ordinary source and the unchanged owner independently reproduce complete
compiler output across production, SKIP_ASM and retained contexts. All
non-target code and allocated-data contracts are preserved.

Fresh integration on main through cmpRecover passes six complete guarded/final
plain/stock-dump assembly and entire-ELF comparisons in production, SKIP_ASM
and retained modes. Exactly one equal-length canonical source filename is
normalized. All strict builds are clean. Complete owner: 26 match, 0 differ.
All 26/25/26 non-target peers and allocated data remain exact. Production and
retained relocation ordering is unchanged. SKIP_ASM changes global HI16
grouping, but ordered HI16/LO16 resolution gives identical targets and peer
relocation multisets. Cross-mode target contracts agree after resolving the
production-owned sdata symbols and their named external views.

Optimized and ASan/UBSan full-state/callback fixtures each pass 337,600 cases.
A fresh independent raw-retail interpreter comparison passes 20,000 cases,
with every conditional branch exercised both ways and 164/165 native words
reached; only the unreachable alignment nop is excluded. These fixtures use
provider hooks and valid modeled indices, not full hardware execution or a
proof of malformed-index safety. Leak detection is disabled in the allocation-
free sanitizer fixture.

Full retail checksum/byte comparison, generated-source validation, all 505
unittest cases without skips plus 818 layout assertions, frozen EE/seven-IRX
checks and reconciled production reports pass. The recovery and birth-mask
owners remain 391/0 and 196/0. Only this function gains credit: game totals
become 8,658 functions and 1,504,872 bytes. A subsequent build has no work.
