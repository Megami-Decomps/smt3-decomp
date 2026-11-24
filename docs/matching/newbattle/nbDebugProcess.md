# Matching notes: newbattle/nbDebugProcess

## nbDebugProcess1 (newbattle/nbDebugProcess, 0x2A8)

170 of 170 words match at 680 bytes. Native state dispatch retains distinct
menu/HP/MP/status outcomes and the ordered inactive/active motion callbacks.
A real shared menu continuation sets nextseq and advance, without an artificial
consumer. Invalid menu selections do not advance; all non-early paths call
SetNextSeq only when advance is set, then return NULL. State 3 checks the
required side from status bit 8 before any order callback. After nbSetOrder
and every nbSetNextOrder, it reloads d->data, active party and both form-index
observations, retaining callback mutation behavior until the native match.

Two empty operands act on real initialized values: a pointer-valued -1
return and the side-comparison value one. Only the latter has a fixed `$2`
binding, consumed before the next call; no caller-clobbered live value spans
a call. The former preserves a distinct native early-return epilogue;
removing it leaves two differing words at +0x220/+0x224. Removing the side
binding while retaining generic operands changes allocation/size. Reduction
removed two other bindings and an unnecessary side operand without losing
exactness. Neither constraint emits an instruction or asserts a memory
barrier. They are SMT3 reconstruction aids, not original-source evidence.

SelectDevil's context formal is a recovered ABI fact, not a fake dependency:
the recorded prototype takes nbDebugProcessData_t *d, and the native caller
at 0x2851E0 explicitly supplies d in the call delay slot. The provider discards
incoming a0 before use at 0x284B3C. Reference and retail-word audits find one
direct call and no address-taken site. The exact prototype, guarded definition
and call agree; no opaque alias, wrapper or extra-argument cast is used.
Paired canonical NON_MATCHING builds preserve the provider's 552-byte draft
text and all 29 relocations; normal provider assembly is unchanged. The
provider remained **NON_MATCHING** at that landing, with its then-current
552-versus-536-byte mismatch. Only nbDebugProcess1 received matching credit.

Independent complete-owner verification gives 15 match, 0 differ, preserving
all 14 compiled peers and both owned tables. Integrated retail checksum and
full regression, guarded EE/IRX and report gates passed for source commit
`a831913c94e1ade4034aa302f27a59cb8df6378f`. Exact-source hosted CI
[41038826142](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41038826142)
completed successfully, including all build/deploy steps and actual retail verification.


## SelectDevil (newbattle/nbDebugProcess, 0x218)

134 of 134 words match at the native 536-byte size. The routine draws ten
rows, reloads the page base between the two text calls, and checks the right
page control before reading the left one. The active name-table row uses the
pre-increment row and a fresh page-base read after linking the index tag.
Early exits and the native page limit of 374 are unchanged.

The index-column coordinate, name-table pointer, tag, and name-column
coordinate are initialized values used by the real drawing calls. Empty
operands and localized EE register bindings retain their distinct native
homes without emitting instructions or fencing ordinary RAM. The only
caller-saved binding holds the previous page during arithmetic with no
intervening call. The constraints explain this compiler output, not the
original source spelling.

The full owner matches 17 functions. The ordinary and verbose compiler
outputs retain the same allocated sections, ordered relocations, and symbol
records. An extracted-source fixture checks 13,824 callback traces at each
of two host optimization levels; the linked retail comparison remains the
instruction and layout check.

## SelectMotData (newbattle/nbDebugProcess, 0x434)

269 of 269 words match at 1,076 bytes. The four-row editor retains the
pre-allocation label snapshot, callback-sensitive cursor and field reads,
increment-before-decrement behavior and signed byte/halfword conditions.
Editing uses the selected motion snapshot; committing reloads the current
process data, form and motion selection after callbacks. The inherited
valid display type domain is 0..3 and the selected motion domain is 0..25;
this reconstruction does not add bounds that retail lacks.

A pixel-space y induction starts at 370 and advances by 12. Each print uses
`(y << 3) + 0x7900`, producing the native four GS coordinates. Qualified stock
loop output reduces the four affine expressions to separate induction
registers. One empty operand keeps the real `50 * 16` x offset at its four
print consumers. Each signed halfword decrement condition has a block-local
initialized `$2` binding: native `lh`, `blez`, then the unsigned load in its
delay slot. That value dies before the decrement overwrites `$2`, with no
intervening call. These localized constraints emit no instructions and make
no memory-barrier or original-source-spelling claim.

The unsigned byte/halfword representations use promoted `s32` arithmetic
and narrow only at the native stores; the record fields remain `u8`, `s8`
and `s16`. Ordinary RAM accesses replace inherited volatile casts. Local
`sdfDevPrintf` and `frwGetSmallFontFRQ` declarations now agree with their
canonical provider types, including `frFontHandle_t *`; no provider, shared
header, object layout, compiler flag or unit boundary changes.

Complete-owner checks give 16 match, 0 differ with all 15 compiled peers and
owned data preserved. Separate production, SKIP_ASM and strict retained-C
contexts pass wrapper/plain/stock-dump whole-assembly and raw-object parity
at the resolved compiler paths, without normalization. The retained owner
has 16 matching and four still-differing bodies; only SelectMotData is
promoted. Ordered relocations retain the original providers and ordering.
Independent semantic review and an extracted-source host fixture cover
526,890 cases, with seven behavior-changing negative controls rejected.
The host fixture establishes logical behavior, not target allocation.

### nbDebugStockSelect (retained draft, native 628 bytes)

The retained ordinary reconstruction now emits 624 bytes and matches 60 of
157 native words, versus 632 bytes and nine matching words in the inherited
draft. It remains guarded with its assembly fallback; no matching credit is
added.

The native row loop keeps the current status/empty-label pixel Y and font Y
separate from the next row Y. Expressing those real coordinate phases, with
the consumed loop index and Y initialized before drawing, improves allocation.
The canonical `unitwork` overlay addresses global + 0xA54 + 0xEC * index,
identically to the former `akuma` field view; it directly names the flag,
unsigned-halfword ID and status fields used by retail. No provider signature,
layout, bound or call is changed.

The stock base is ordinary RAM. Its inherited volatile casts had no demonstrated
hardware or asynchronous-writer justification and are removed. The source
still reloads it across row drawing calls, and keeps the preselected unit
pointer while observing flags after `DispBadStatus`. No new constraint or
compiler promise is used. Remaining differences concern string-base allocation and argument scheduling.
The ordinary draft retains the unconditional stock-base reload, including the
last iteration, in the loop branch delay slot.

Extracted-body host tests compare a native byte-offset/event-order oracle in
92,160 scenarios each under optimization and address/undefined-behavior
sanitizers. They cover both occupancy paths, all menu outcomes -2 through 2,
valid base/cursor combinations, varying status/IDs, and callbacks that change
the global-work backing, selected base/cursor and unit fields. Entire event
streams and resulting state agree. These are not EE execution tests; valid
stock and name indices remain caller preconditions.

All 16 existing exact owner functions remain exact. The complete guarded owner
passes, and full raw assembly/whole objects from plain versus diagnostic
compiler runs agree in production, SKIP_ASM and retained-C contexts.


## SelectMot (newbattle/nbDebugProcess, 0x27C)

159 of 159 words match at 636 bytes, beginning at 0x002848B8. The ten-row selector keeps the native
index-printer and font coordinates separate. Each row reads the current
page index for the index print and again after the print and tag-link
callbacks for the label. Highlighting refreshes the motion cursor after each
font request's submission and cleanup. Registering the completed tag
precedes the active check; selecting a motion then reads current process,
form, page and cursor. The signed byte at pad offset 0x20, page-right cap,
and subsequent page-left clamp follow the original menu dispatch.

The same-owner printer precedent supplies the raw column/Y phase. Paired
empty constraints on the actual initial label table and cursor and on the
printer's real operands preserve the native schedules. The initialized raw
font X remains live at the real shift into the font request. The compiler
prints a loop alignment directive next to the empty initial-cursor region;
it is compiler-owned alignment, not source assembly. The color and request
reuse the saved register in disjoint phases, with the request assigned only
after the font call has consumed the color. No new alias, provider ABI,
compiler flag, or unit boundary is used.

All 18 owner functions and linked retail bytes are exact. The behavioral
fixture covers 262,144 callback and state scenarios at each of two host
optimization levels, including a negative control with the pad sign reversed.
