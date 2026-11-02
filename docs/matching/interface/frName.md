# Matching notes: interface/frName

## frRestoreNameImage (interface/frName, 0xD8)

42 of 54 words match at the exact 216-byte size. The remaining 12 words are
count/counter setup: retail forms the name-count pointer before the counter
index, uses `$4` for the counter pointer and `$6` for the summed count, and
uses the assembler temporary for the counter array address. The draft swaps
these roles and two setup instructions. The release loop and diagnostic tail
are exact.

`pFont` is a four-by-sixteen table of `GsFont_t *`: its 0x100-byte symbol
extent, 0x40-byte target stride, `frSetNameData` writes and
`frCreateNameHandle` image-provider calls establish the view. Entries release
`font_mem` at offset 8 through the actual `fmRetFontMem` provider. The loop
uses the sum of both name-count bytes and skips null entries. It runs only
when a nonzero reference count is decremented to zero; a zero glyph count
still reaches the restoration diagnostic.

Ruled out: a separate `activeCount` local, a named counter pointer,
pre-decrement versus explicit decrement/store, and nested guards versus early
returns (all retain 12 differences). Capturing the name-count byte pointer
instead makes 30 words differ. Do not repeat these without a new compiler
property explaining the initial address and count allocation.

## frGetCNameFRQ (interface/frName, 0x158)

86 of 86 words match at the native 344-byte size in ordinary C. Output stores use `text[length]`, retaining the
conditional and unconditional length increments and final NUL store. The
complete owner checks six exact functions, with no code or data discrepancy.

The VLA holds twice the current character count plus the terminator. Each input
pair emits its second byte; a nonzero first byte is emitted before it. Count
and global-work reloads are preserved. Mode 1 surrounds
`frLinkFontString(text, style, firstOption, 0, chain)`; font spacing is then
set to 4. The fourth input remains a full scalar until the link provider's
signed-byte conversion; input chain and returned font are distinct locals.

The previous explicit output cursor was exact-sized with four setup words
wrong at `+0x68` through `+0x74`. Qualified compiler dumps establish the
reason: initial RTL defined both the VLA base and cursor before the loop;
the jump pass introduced the empty-name guard after both, and delay filling
put the cursor copy in its slot. Indexed stores instead let the loop pass
combine their address induction into a derived pointer initialized only in
the nonempty successor. The length counter remains live for the final NUL.
Delay filling then puts the buffer-base copy in the empty-name branch slot,
followed by stride and cursor setup in retail order. The original rotated
loop also preserves CSE forwarding from the loop-condition global-work
pointer to the first byte load, without an extra adjusted-pointer tail copy.

Plain and `-da -fsched-verbose=5` builds of the indexed owner had identical
allocated section bytes, non-file symbols, ordered relocations and normalized
assembly. This establishes the measured optimization, not historical source
spelling. No compiler constraint, explicit extra guard or global snapshot is
needed.

Historical negative controls: a signed-byte fourth input (72 differences,
four bytes over), input-chain/result reuse (15 differences), and a nonempty
guard around a for loop (two tests remain, eight bytes over). A guarded
do/while recovered the branch-slot setup but added a global-pointer copy at
the loop bound, four bytes over. Increment-in-condition and counter-first
variants did not remove it. An explicit global-work snapshot/refresh retained
the required reloads but emitted 352 bytes, with 30 of 86 words matching and
an early adjusted pointer plus extra tail copy. These are not alternatives
to the matched indexed-output form.

## frSetNameData

The guarded reconstruction emits 496 bytes against 488 native bytes, with
48 of 122 native instruction words matching. Normal builds retain assembly;
this adds no exact matching credit. The first 100 bytes match. Remaining
differences include the cleared-slot counter register and the bitmap-copy
source induction. The compiler advances the source pointer while establishing
the inline copy endpoint, with an extra move in each alignment arm; native
forms a temporary endpoint and advances the saved source after translation.

The parameters are signed bytes. Type zero clears old slots starting at zero;
other types start at the signed first-name count and clear the signed second
count. Clearing stores null pointers without releasing fonts. A positive
input glyph count allocates and copies exactly `0x120` bytes per glyph, calls
`frTranslateUserData(24, 24, copy, 1)`, stores the returned font in its target
slot, and writes the target to `GsFont_t.font_no`. The copy is nine 32-byte
chunks. Logging occurs even when the input count is nonpositive.

The allocator declaration is its real pointer-returning contract from
`sdf/sdfChip.c`. An initial draft without that declaration happened to emit
488 bytes; its implicit integer return is not a permissible matching lever.
Direct global-work reads in the two selection arms recover the native prefix.
A pre-tested versus post-tested positive countdown and using the memcpy result
for the allocated image are compiler-inert. Sharing the two loop counters
changes three register words but retains the eight-byte excess; the preferred
source keeps distinct counters. Advancing the incoming parameter directly
rather than a local cursor worsens allocation and retains the excess. A real
copy-induction/lifetime premise is needed before further trials.
