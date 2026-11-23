# Matching notes: interface/frFont

## frPreCalcStrWidth (interface/frFont, 0xF8)

Fifty-six of sixty-two words match, at the exact 248-byte size. The
reconstruction is structurally right: the stack `frTextHandle_t`, the
two-byte lead-byte combine, the `code - 0x8080` index formula and every
register but three are retail's.

The six remaining words are two separate things:

- Two words: the two byte stores that seed the stack glyph come out
  `sharedFlags` (`+0x17`) then `fontIndex` (`+0x15`), where retail has
  `+0x15` first. All six source orders of the three seeding statements
  (`fontIndex`, `sharedFlags`, `advance = 0`) give the same pair order, so
  this is not the reverse-source-order store rule; three of the six are 8
  words out and three are 9.
- Four words: the `code < 0x80` flag lands in `$2` where retail uses `$4`,
  which costs the two `sltiu`s and the two `bnez`es that read it. The draft independently allocates the flag and the `addu text, i`
  address temp to the same hard register at different times; the qualified
  trace below corrects the earlier coalescing explanation.
The previous early return for `length == 0` added a third residual: a branch
annulment and duplicated epilogue load. Guarding the conversion do/while with
`if (length != 0)` and keeping one common return closes those two words,
without converting the still-assembly `frGetXShiftLev` callee. Empty strings
still perform the glyph setup and provider queries, then return zero.


Ruled out: the two arms of the index formula written with the store hoisted
into each arm (36 of 62 differ, 8 bytes over), the store moved below both
tests (21 of 62), and `shiftIndex = code - 0x20;` assigned above the test as
a default (8 of 62, identical). Inverting the index test to
`if (code < 0x80) { ... } else { ... }` is what took it from 32 differing
words and one word over to 8 at the exact size: the single-instruction arm
then gets absorbed into the branch delay slot instead of needing a `b` over
it.

Confirmed on the way: the parameters are `s32`, not `u8`/`s8`; retail masks
only at the `frGetDefaultFontWidth` call and stores `$5` raw. The height call
re-reads `glyph.renderValueOrSetupOrShade.setupBytes.fontIndex` from the
stack rather than reusing the parameter. One local serves both the
`code << 8` lead-byte temp and the shift index, which is what puts them both
in `$5`. `frGetXShiftLev` takes a third argument it never reads: it
overwrites `$6` with 1 as its second instruction, and the caller still
computes `i == 0` into `$6`.

### Qualified predicate allocation and seed-store scheduling

An unchanged complete-owner allocation/scheduler observation qualifies against
ordinary, plain, dump and fresh observed output: 5,565 frozen inputs and 28
artifacts agree byte-for-byte, with all four pass gates, normal completion and
owned loopback listener/process cleanup verified. The case records 1,775 events
and 35 exercised guarded hooks. This is exact-case observer qualification, not
a blanket neutrality claim.

The address pseudo98 receives hard2 in local allocation. The real predicate
pseudo116 is still unallocated before global allocation; it is not coalesced
with that address. Its local combine attempts return false. Global allocation
records zero calls crossed, no preferences and conflicts with hard3/28/29;
hard0/1 are blocked and hard2 is the first eligible free candidate. Hard4 is
not blocked, but the search stops at hard2. This is an independent register
choice, not a copy-coalescing dependency.

For the setup stores, prereload scheduling compares Mode at glyph+0x17 and
Style at glyph+0x15 with equal priority6 but weights -1 and0. The register-weight
tier selects Mode first. Postreload priorities and weights tie, and LUID retains
that order. Source-order permutations already ruled out above remain closed.

One bounded control made both real code-range comparisons explicit through a
local predicate bound to hard4. Each value is initialized before its real branch
consumer and dies before the glyph-provider call; no extra consumer or scheduling
constraint was added. It emits two extra moves, still computing comparisons in
hard2, and regresses to 256 bytes against native248 (25 matching prefix words).
All 77 exact peers and the independent setup-store residual are preserved.
Ordinary/plain/dump complete outputs agree for this control and three strict
implicit-declaration compile contexts pass. It is rejected; no source is retained.
Do not repeat binding, operand or setup-order variants without new evidence.

## frAnalyzeTag (interface/frFont, 0x518)

326 of 326 words match at 1,304 bytes. The tag parser keeps the signed
cursor, actual text pointer, option, and low-nibble argument count in
their native phases. The four name tags read the glyph byte before
publishing the new cursor, create the handle, then use the fresh handle
and the signed-byte name selector. The narrow register bindings and
input operands apply to these genuine values at their real consumers;
the callback state and addressing are unchanged.

The declaration of the actual `frFindTagCode(u16)` provider is
uniformly pure here. The provider is a bounded, read-only scan of 416
ordinary halfwords, returning the first index or -1, with no writes,
callbacks or alternate input. It can depend on table contents: this is
not a const attribute. The unchanged provider and its caller ABI are
exact. The truthful declaration preserves the two native empty branch
delay slots for the tag cases. In F118, reading the first actual key
through an ordinary pointer to its halfword retains the separate read
for the secondary lookup. Neither the key nor table is made volatile;
both glyph emissions and their callback order are unchanged.

## frAnalyzeMessage (interface/frFont, 0x200)

128 of 128 words match at 512 bytes. Making the independently exact tag
body visible reproduces the native tag-continue branch. The message
parser keeps the native header skip and request-bit clear, then consumes
and advances through the actual encoded text. It reloads the text base
after a tag that continues, a newline callback, or the glyph sequence
that can affect the context. The second byte of a two-byte glyph comes
from the post-handle callback context.

The loop preserves the two-byte split and remapping, native temporary
encoding, setter order and final mode transition. The ordinary request snapshot
keeps the three bit clears together, and the localized text/context
operand preserves the actual cursor setup. They add no new memory
operation or promise about a callback.
The input domain is a valid game-encoded message, including any bytes
required by its tags. The two independently exact functions and all 79
previous owner peers are validated together; the complete owner is 81/0.

## Font load-state slots

The nine native font slots each have a writable load-state word initialized
to zero. Loading tests and sets a slot; deletion clears it, including the
explicitly used temporary slot 8. The following four zero bytes are alignment.

## Font-dump packet storage

The debug font renderer uses frame-indexed pairs of draw tags, draw
environment packets, texture tags, and font texture packets. The two
packet strides are 0x80 and 0x110 bytes; each draw-tag stride is 0x20.
The separate aligned definitions own 928 native zero-fill bytes at
`0x56B690..0x56BA30`. The raw padding before the first tag and the
font-system object at the second boundary stay in their original ranges.
The renderer retains its assembly fallback and is not promoted here.
