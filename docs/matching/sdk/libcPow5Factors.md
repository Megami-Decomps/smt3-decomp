# Three power-of-five multipliers

The native `_pow5mult` tests the low two bits of the exponent and skips
the multiplication for zero. It reads the signed factors 5, 25 and
125 for remainders 1, 2 and 3. The C array now supplies all three
entries: the old two-entry declaration read across its end for
remainder 3. The factor 625 used to seed cached powers is unchanged.

The following zero word is not a fourth factor. Double alignment
before the next power-of-ten table preserves that word. The native
double-scaling references use that next table's base minus eight,
not an additional entry in the powers-of-five table.
