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
