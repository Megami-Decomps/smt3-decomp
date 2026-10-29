# Arithmetic


ee-gcc 2.96 does not turn a division by a constant into a multiply: it loads
the divisor into a register and emits `div` plus the divide-by-zero trap, the
`beql`/`break 0, 7` pair. It does synthesise constant multiplies out of
shifts and adds, so `n * 100 / 0xB8` is a shift chain followed by a real
`div`.

A register-packing `|` chain of shifted parameters is associated into a tree,
not a left-to-right run, and the tree gcc builds depends on the source order.
`nbPnl_rsetTest` packs eight arguments into the GS `TEST` word; written in
ascending field order it is 40 of 69 words out with every callee-saved
register holding the wrong parameter, and written in **descending** order,
`(ztst << 17) | 0x10000 | (datm << 15) | ... | (atst << 1) | ate`, it is
exact. Hand-parenthesising the pairs gcc actually emitted does not reach it,
so sweep the order rather than the parentheses.

A read-modify-write of a narrow field into an `s32` local loads straight into
the result register only when the load and the add are separate statements.
`v = work->hp; v += delta;` gives retail's `lhu $6, 6($10)` followed by
`addu $6, $6, $8`; `v = work->hp + delta;` loads into a second register and
adds out of place, which costs `SelectHpMp` nine words across its two arms,
two of them in the `movz`/`movn` clamp that follows.

`(x + K) * 2**n` and `(x + K) << n` are not the same source. fold distributes
the multiply over the constant addend and emits `sll` plus `addiu K << n`; the
shift is left alone and gives retail's `addiu rd, x, K` followed by `sll`.
`campCheckStockFree` was two words out until `(x + 0xCE) * 16` became
`(x + 0xCE) << 4`. A non-constant addend never distributes, so `(y + off) * 8`
beside it is already right.

A wide constant that retail materialises in the entry block but only uses
inside an `if` is the initialiser of the local the `if` updates:
`s32 color = 0x80808000;` then `color = alpha / 8 | color;` inside the branch.
Computing the whole value inside the branch leaves the `lui`/`ori` there and
costs `campDrawFade` 20 of its 58 words.


## Commutative operand order follows the source only between named locals

`and rd, rs, rt` with two register operands keeps the source's order when both
sides are plain locals, and swaps when one side is a sub-expression: `fold`
ranks a `BIT_XOR_EXPR` above a `VAR_DECL` and puts it first. So
`pressed = (current ^ previous) & current` gives `and $13, $8, $2` (the local
first) while naming the intermediate,

    changed = current ^ previous;
    pressed = changed & current;

gives retail's `and $13, $2, $8`. That one word was the whole of `sdfPadWork`'s
remaining difference.


## A redundant `andi 0xFFFF` can reflect value width or repeated assignments

cc1 drops a 16-bit truncation whose operand it can already bound, so
`u16 w = (c << 8) | p[1];` with `c` a byte load emits the `or` alone. Retail's
extra `andi $4, $2, 0xFFFF` can come from a `nonzero_bits` failure: a pseudo with more than one assignment has no known
bit range, so everything computed from it has to be truncated. Assigning the
pair back to the byte variable is what does it:

    u32 c = src[i];
    if (c < 0x80) {
        buf[i] = c;
    } else {
        u32 v = src[i + 1] | (c << 8);

        c = (u16)v;

`frwGetSmallFontFRQ2` in `interface/frWrap` needs both halves of that: the
intermediate `v` so the `or` lands in the second byte's register, and the
truncation into the twice-assigned `c` so the `andi` survives. Writing the pair
into a fresh `u16` is two words short of retail, and `(u8 *)` indexing is what
gives `lbu` plus `sltiu` where a `u8` local folds the `< 0x80` test into a sign
test on `lb`.


A single wide local can also retain a narrowing instruction. `Flow2_2` in
`newbattle/NbNegoFlowFullMoon` loads the actual `u16` ID into a `u32` local,
uses `(u16)(devilId - 0x24)` for its range test, then compares `(u16)devilId`
with the individual IDs. The matched source has only one ID assignment.
Changing that local to `u16` removes retail's extra `andi` and differs by
12 of 51 words at the same 204-byte size.

Qualified `cc1probe --alloc --cse` runs on both forms pass hooked/unhooked
RTL and assembly, and dumped/plain assembly parity. In `.00.rtl`, the wide
form's UID 33 explicitly zero-extends the HI subregister of ID pseudo 86;
the narrow control compares pseudo 86 directly. CSE shares the wide form's
three ID narrowings through pseudo 93. Thus a repeated assignment is one
explanation for a redundant narrowing, not a requirement to infer from the
instruction alone. Preserve real data widths and explicit format conversions.


## A dead division leaves its zero-divide trap behind

A `beql rX, $0` followed by `break 0,7` with no `div` anywhere near it is the
trace of a division whose result is dead. ee-gcc 2.96 deletes the division but
keeps the trap, so the divisor is still tested and the `break` still stands.

Standalone probe through `tools/cc.sh`:

    int g(int x) { ra(0); a = x / 24; a = x * 2; ra(1); return a; }

emits `li 24`, `beqzl` and `break` and no `div` at all. So where retail shows a
bare trap, the source computed a quotient and then overwrote it before any use.

On `dds3TitleDrawMenu` the draft was two words short at +0xC4 for exactly this
reason, and writing the overwritten assignment before the four sprite calls was
worth 74 words. The canonical rules already permit dead code whose trace
survives in retail; this is the arithmetic shape that produces it.
