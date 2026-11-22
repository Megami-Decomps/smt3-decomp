# Nme_Draw matching findings

## nmeMoveCursorBy (nme/Nme_Draw, 268 bytes)

All 67 retail instruction words match, excluding the separate trailing
alignment. The cursor advances by the signed input byte, publishes the
low halfword, and compares the signed cursor against the visible range.
The negative path clears the cursor; the upper clamp rereads the two
range bytes. A blank glyph is the pair of 0x80 bytes. The ordinary
backward branch to the preceding zero-store block lets the R5900 compiler
emit the five native short-loop padding words without inline instructions.
Rows use the existing 32-byte stride and are read only inside the active
range. In the 220-row input mode, a six-row viewport leaves at most row 213
reachable by the scan.

The private stack buffer retains the native three-byte template copy and
two glyph stores. This is the same live copy shape as nmeDrawCursorAt
in the adjoining name-entry draw code: there, the two glyph bytes
replace the first two bytes and the third terminates the text buffer.
The scanner does not consume the initial values or pass the buffer
anywhere. Removing the private scaffold preserves navigation behavior
but loses the native copy trace. This reconstruction neither introduces
a terminator consumer nor asserts an original shared macro.

The localized compiler constraints preserve the real high-byte phase
of the signed step and the independent row-byte comparison values.
They do not emit assembly instructions. The whole-owner comparison
preserves all 20 existing C functions and owned data.
