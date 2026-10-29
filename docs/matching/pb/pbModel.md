# pb/pbModel residuals

## pb_draw_mapbase_Process (pb/pbModel, 0x214)

126 of 133 words match at the native 532-byte size. The only remaining
instruction differences are the panel and status registers at offsets 0x48,
0x4C, 0x50, 0x54, 0x5C, 0x70 and 0x74. Native uses register 4 for the panel
and 5 for its status; the draft uses 2 and 4. The two tile passes, calls,
callback results, accumulator updates and ordering-table registration match.
The assembly fallback remains active.

The map contains 20 by 20 unsigned halfwords at work offset 0x1C. Its draw
counter is at 0x36C. Each tile first draws unless bit 0x800 is set, then draws
its second face independently when its upper three bits equal one. The second
predicate reloads the map after the first draw. Each draw also reloads the
global work pointer before updating its counter.

The initial draw mask is 0x1F. When the material work flag is zero, its selector
chooses one of the two copied callbacks; otherwise the previous mask survives.
Callback coordinates are signed halfwords, while the block renderer accepts
wide coordinates and masks its draw argument to eight bits inside the provider.
Declaring that argument narrow adds two call-site mask instructions. The
callback table keeps its existing data owner.

A copied two-pointer struct closes the panel/status register differences but
moves thirteen prologue words, including the template-copy temporaries. Explicit
aggregate assignment leaves that residual unchanged. A named status local and
moving the table declaration leave the preferred seven-word result unchanged.
No artificial register constraint or pointer offset is used.
