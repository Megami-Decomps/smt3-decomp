# Battle field and system editor menus

The submenu records at 0x31CC40 and 0x31CF48 each own one complete 0x40-byte
editor descriptor. The existing 17 field and 14 system item rows have
independent owners. Each menu holds four relocated pointers, with no
other initialized bytes after the callback and parameter slots.

The field enter callback always returns NULL after saving the current
light state; its close callback ignores the dispatcher context. The
system enter callback returns the opaque value received from the
dispatcher unchanged, and its update callback ignores the context.
Use the dispatcher argument and return types without casts.

## Effect editor menu flag

The writable SYSTEM label occupies seven bytes including its NUL, at
`0x54F1D8`. The editor's separate byte flag follows at `0x54F1DF`:
the native code clears and toggles that byte and tests it to choose whether
to draw the debug menu. The C definitions give the label and flag their own
extents; they retain their native small-data layout. The drawable label still
ends at its original NUL even when the flag is set. This is a correction to
the ownership of an initialized byte already covered by C, not new matching
credit.
