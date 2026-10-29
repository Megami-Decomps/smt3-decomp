# Non-matching drafts: interface/frFont

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

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
  which costs the two `sltiu`s and the two `bnez`es that read it. Mine
  coalesces the flag with the `addu text, i` address temp; retail keeps them
  apart.
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

## frAnalyzeTag (interface/frFont, 0x518)

Forty-two of three hundred twenty-two words match at the head, and the draft
emits 1288 bytes against retail's 1304. The word count overstates the gap: a
normalised instruction diff (both listings with branch targets and `gp`
displacements masked) leaves only four real differences, and everything else
in the 280 is branch displacements cascading from the four-word shortfall.

What is confirmed and worth not rediscovering:

- The twenty-two case values and the body order. Bodies are laid out `0xF206`,
  `0xF202`, `0xF209`, `0xF207`, `0xF203`, `0xF20A`, `0xF20B`, `0xF20C`,
  `0xF20D`, `0xF20E`, `0xF10F`, `0xF110`, `0xF111`, `0xF104`, `0xF112`,
  `0xF113`, `0xF114`, `0xF115`, `0xF116`, `0xF117`, `0xF118`, default, and
  written in that order the compare tree comes out exactly.
- `0xF111` falls through into `0xF104`: both end in
  `gMessageRequestResult |= 1; return 1;` and retail shares the block.
- The prologue holds five things in locals, and all five are needed to reach
  the matching prologue: the `s32 *` to `ctx->encodedTextOffset`, the nibble
  `code & 0xF`, the offset *value*, `ctx->encodedText`, and
  `ctx->firstOption`. `code` is then reused for the tag, which is what makes
  the nibble get computed in the prologue before `$4` is overwritten. Reading
  the offset or the text pointer through `ctx` inside the arms instead costs a
  saved register and the whole body shifts.
- `gVarStr[text[pos] - 1]` is the `0xF203` guard: the `-1` folds into the
  symbol, which is why spimdisasm names it `D_0056BBCC`, four bytes below
  `gVarStr`.
- The three `frFindTagCode` arms want `s32 tagChar = <high>; if (... < 0) {
  tagChar = <low>; } frPushTagChar(ctx, tagChar);` declared inside the arm's
  own block. The ternary spelling gives `movz` where retail has `movn`, and
  the same local at function scope adds a fifth saved register and costs the
  whole prologue.
- The default arm writes `ctx->encodedTextOffset`, not `*offset`, and the
  reload has to land in `$3`: `ctx->encodedTextOffset = (argCount - 1) * 2 +
  ctx->encodedTextOffset;` gives that, where `+=` leaves it in `$2`.

The four residual differences:

- Two `nop`s in the compare tree. Retail leaves the delay slot of the
  `beq` against `0xF115` and the one against `0xF113` empty; the draft fills
  both from the fall-through `slt`. This is reorg's fall-through heuristic,
  not anything the source states.
- One more delay slot in the `0xF116` arm, retail `nop` against the draft's
  `lhu frMessageCode`.
- The four name arms (`0xF20A` to `0xF20D`) read the glyph byte *before*
  storing `pos + 2`, keeping it in `$5` and forming `nameIndex` in the
  `frCreateNewHandle` delay slot. The draft stores first and loads into `$16`
  in that delay slot instead. Four spellings were measured and every one that
  moves the read ahead of the store adds a fifth saved register and goes from
  280 of 322 to 311: a second local for the raw byte, one local decremented in
  place either side of the store, and reusing `pos` itself for the byte.

## frAnalyzeMessage (interface/frFont, 0x200)

One hundred twenty-one of one hundred twenty-eight words match at the exact
512-byte size, and that measurement is with `frAnalyzeTag` compiled as C
above it; with `frAnalyzeTag` back behind its own fallback it is eight,
because the `frAnalyzeTag(code, ctx) != 0` test then emits `bnel` where retail
has `bne`. That is the callee-visibility mechanism, measured here in both
directions, so this draft should be rechecked the moment `frAnalyzeTag`
matches rather than permuted.

Confirmed: `offset` is an `s32 *` local to `ctx->encodedTextOffset`, assigned
just before the loop, and without it the function is nine words short and one
saved register down. The second byte of a two-byte code has to be read with a
post-increment in one expression,
`code = (code << 8) | ctx->encodedText[(*offset)++];`, which took it from 20
differing words to 10. `encoded[0] = (u32)code >> 8` needs the unsigned cast:
`code` is `s32` everywhere else, because the `< 0x80` and `<= 0xC07F` tests
are `slti`/`slt`, but the byte extraction is `srl`. The `<= 0xC07F` and
`> 0xC07F` arms each write the three `encoded` stores and the
`frSetFontString` call out in full and let gcc cross-jump the tail; sharing
them after the merge is six words short.

The seven remaining words are two things:

- Five: `addiu $18, $17, 0x18` sits ahead of the last `gMessageRequestResult`
  mask and the `frEnableMode` call, where retail has it in the loop
  preheader's `b` delay slot. Assigning `offset` before `frEnableMode(1)`,
  after it, at its declaration, and in a `for` init clause all emit the same
  thing; only dropping the local moves it, and that costs far more.
- Two: `isFirst = 0` lands in the `frSetFontSpace` delay slot with the
  `gMessageSpaceX` load ahead of it, where retail has the order the other way
  round. Both source orders of the assignment and the call give the same
  output.
