# pipe/pipeObject

## objOption

78 of 152 instruction words match at the native 608-byte size. The frame
and actual local extents match: one 32-byte object and a 16-byte way table.
The default build retains assembly.

The guarded reconstruction corrects two errors in the older draft: option
100 consumes all sixteen table bytes, with a byte counter from 15 through 0,
and reloads the parent ID and action for every construction. The exact
`objMake` and `objCopy2` definitions establish both call interfaces.

Status zero adds 128 to a 32-bit threshold, without byte wrapping. The signed
status switch preserves all three random masks and reaches native size.
An earlier unsigned-status variant emitted 600 bytes; a combined widened-
selector variant emitted 584 bytes. Neither is retained. The table cursor
is initialized with its copied buffer before dispatch. A separate loop
counter, a consumed-selector counter and explicit table-byte snapshot compile
identically. No source constraints, padding or extra observable accesses are
used.

Remaining questions are the initial table-copy temporary registers and byte
extension timing, the status-threshold copy and case-tail layout, and the
sixteen-iteration loop's sentinel hoisting and field-load scheduling. The
ordinary decrement-to-option-boundary loop already matches. Resolve those
specific differences before retrying declaration or loop-form changes.

A qualified current-owner comparison identifies a construction-loop blocker.
GCSE reuses the earlier ID/action dispatch reads for the first construction,
then inserts their callback-sensitive reloads on the backedge outside the
loop marker. The loop pass explicitly rejects that loop for multiple entry
points, leaving the 255 sentinel inside the emitted loop. Retail instead
initializes 255 once before the loop and reloads ID/action in every body,
including the first iteration. The separate option-boundary loop still
hoists all four 20/40/60/80 constants. This is a measured compiler
transformation, not evidence of historical source spelling.

Exact-ABI opacity for `objMake` is byte-inert. A localized binding on the
actual status threshold reaches 90 matching words out of 152 at 608 bytes but remains
nonmatching. An input-only operand on the real option regresses to 616
bytes. All three controls are rejected. Original, active and diagnostic
sources pass complete direct/wrapper and plain/selected-dump assembly and
entire-object parity in production, SKIP_ASM and retained configurations.
All 41 peers and owned data remain unchanged; the canonical guarded source
is unchanged and earns no new matching credit.

The next distinct question is a truthful acquisition/dispatch form that
retains the real ID/action reads within a single-entry construction loop.
Another counter spelling, sentinel register choice or `objMake` opacity
control does not address the measured entry topology. This evidence does
not justify volatile casts, dummy reads, fabricated memory effects or a
general barrier. The table-copy and status-tail residuals remain separate.


## objLoad

The ordinary C reconstruction matches all 552 native bytes (138 words). The
complete owner checks 41 match, 0 differ. No constraint, opaque declaration,
new flag or fabricated bound is used.

The source is an eight-byte spawn record from the actual sixteen-byte pipe
table's tabPtr field at +8, not a separate pointer object at the interior
label. The producer allocates count*9 bytes, copies count*8 record bytes and
places the loaded flags after them. The caller supplies a distinct zeroed
32-byte object. Its signed file count remains unchecked; neither this source
nor the matching claim asserts validated indices for malformed input.

One RNG call is added to the actual record address with unsigned 32-bit wrap.
Byte narrowing preserves the native decimal mode/place intermediates. Special
modes and zero places use remainder-nine selection; randomized center position
five retries with random += random >> 1 and no extra RNG call. Explicit center
five remains valid. The signed-word-to-float radius and native diagonal
constant preserve coordinate signs, including negative zero. The comparison
chains follow the already matched objMake sibling.

Natural id/offset/enabled/action/count statements reproduce the native memory
order. Missing metadata returns zero after partial initialization. Otherwise
width is copied, low-category actions 6/15/16 subtract 0x230 with 32-bit wrap,
and category bit 0x10 scales the source value by ten. ID and action reloads
across the metadata call and the final value reload remain intact; untouched
output fields stay untouched. This is a matching reconstruction, not evidence
of the original source spelling.

Independent review verifies actual producer/caller/provider contracts and all
552 resolved retail bytes in production, SKIP_ASM and retained configurations.
All 41/40/41 other functions, owned data, section and symbol contracts, and all
relocation targets pass. Production data is unchanged. In SKIP_ASM the new
four-byte diagonal literal shifts later references without changing values.
Retained C removes the old draft's non-native switch table and its measured
alignment padding; every remaining string, table and peer target is verified.

Baseline and final source pass six strict plain/dump pairs covering complete
assembly and literal entire ELF files, with per-invocation input hashes. An
independent host/native-derived reference checks 176,128 cases covering all byte
way/ID/action values, seed boundaries, wrapping, all output bytes and return
paths. This is not EE execution or an exhaustive RNG-state proof.

Full retail checksum, generated-source, 569 regression tests, guarded-source,
report and compiler-ownership gates pass. Game coverage gains one function and
552 bytes; SDK coverage is unchanged.
