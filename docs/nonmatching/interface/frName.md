# Non-matching drafts: interface/frName

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

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

82 of 86 words match at the exact 344-byte size. Four setup instructions at
`+0x68` through `+0x74` differ: retail places the dynamic text-buffer address
copy in the empty-name branch delay slot, then initializes the input stride
and output cursor; the draft copies the buffer address before the branch and
puts the output-cursor copy in its slot. All conversion, provider-call and
stack-restoration instructions after that window are exact.

The VLA holds twice the current character count plus the terminator, matching
the native aligned dynamic allocation. Each input pair emits its second byte;
a nonzero first byte is emitted before it. Count and global-work reloads are
preserved, and the output is NUL terminated. Mode 1 surrounds
`frLinkFontString(text, style, firstOption, 0, chain)`; font spacing is then
set to 4. The fourth input is retained as a full scalar until the link
provider's signed-byte conversion. Input chain and returned font are distinct
locals, which closes the initial register rotation (15 differing words).

Ruled out: a signed-byte fourth input (72 differences, four bytes over),
reusing the input-chain local for the result (15 differences), and an explicit
nonempty guard around a for loop (two tests remain, eight bytes over). A
guarded do/while restores the branch-slot setup but adds a global-pointer
copy at the loop bound and is four bytes over. Changing the increment to the
condition and initializing the loop counter first does not remove that copy.
The next useful question is the native loop-header/address lifetime, not the
already audited pair conversion or font API.
