# Result lookup row

`func_002229E8` reads an unsigned halfword at row offset 0 and
advances by four bytes. Its unsigned counter starts at zero and becomes
one before the backedge test, so it reads at most one row. The halfword
at offset 2 is not read by this function. The row occupies
`0x339638..0x33963C`; the following four bytes stay raw.

The definition has exactly one row. Its explicit `.data` section
preserves the retail placement: removing the attribute moves all four
bytes from `.data` to `.sdata` with the stock compiler. The consumer's
incomplete external array declaration is compatible with the one-row
definition. Changing only that declaration to `[1]` makes stock EE GCC
classify the symbol as small data in `rstcalc.c`, replacing the native
absolute `R_MIPS_HI16`/`R_MIPS_LO16` pair with `R_MIPS_GPREL16`.
Both normal and `SKIP_ASM` builds showed the relocation change.
