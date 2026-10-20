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

