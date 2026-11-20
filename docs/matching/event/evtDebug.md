# event/evtDebug matching notes

## evtSceneDispRows data (event/evtDebug, 0xCC)

The scene viewer copies 17 records of 12 bytes from `evtSceneDispRows` at
`0x0053B348`: a name pointer, a signed word type, and two zero halfwords.
The final record names the existing empty string. The C object is exactly
204 bytes; four natural alignment bytes before `D_0053B418` are unchanged
and are not part of the table. All 17 pointers name the existing strings,
including thirteen eight-byte objects owned by `evtMoon`.

Production `evtSceneDispProcess` still uses its original `INCLUDE_ASM` body.
Complete owner text, all 504 owned read-only bytes, and the linked retail
image match. In the retained draft, the sole prologue HI/LO pair changes
from external symbol relocations to a section-relative addend for the same
table. The resolved pair, all other instructions, and the other relocations
are unchanged. This is data ownership only, not a function match.

The production report's 504-byte read-only section fuzzy score rises from
12.698413% to 96.32653%. The aggregate matched data, function count, and
matched code bytes are unchanged. A forced configure leaves sources
unchanged and its stale generated-line control still fails as intended.
