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

