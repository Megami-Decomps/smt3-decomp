# Arithmetic


ee-gcc 2.96 does not turn a division by a constant into a multiply: it loads
the divisor into a register and emits `div` plus the divide-by-zero trap, the
`beql`/`break 0, 7` pair. It does synthesise constant multiplies out of
shifts and adds, so `n * 100 / 0xB8` is a shift chain followed by a real
`div`.
