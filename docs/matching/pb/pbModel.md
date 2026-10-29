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

## pbDrawMapPolyTransformed

The retained source emits 1060 bytes against 1056 native bytes. Its bounded
prefix has 29 matching words and 235 differing words; four emitted bytes
exceed the native extent. The frame is 0x570 versus native 0x560. Allocation
and scheduling differences remain broad; assembly remains active.

Native geometry uses two distinct 36-vector local arrays and the existing
44-byte request. Only five faces are emitted, six vertices per enabled face;
the signed point count is returned. Six-face template data corroborates the
array capacities. Each active face snapshots all four base components before
the inner conversion calls.

Selector 6 scales xyz in binary64 by 0.5, leaving w intact. Selector 1 applies
its binary32 height change before sequential binary64 x/z adjustments; each
assignment rounds back to float. Opposing mask bits retain both operations.
Four distinct readonly labels hold the double word 0x3FC9999999999999.
Mutable geometry/color declarations are preserved: read-only use in this
function does not establish global const storage.

Replacing only the normal-copy primitive with a plain-C quadword copy emits
1040 bytes: 8 prefix words match, 252 differ and 16 native bytes are uncovered.
That control was discarded. A separate indexed/literal reconstruction emits
1048 bytes with 10 of 262 words matching and unqualified readonly relocation
ownership; an explicit input-row offset is inert. Its cursor form emits
1064 bytes with 24 matching prefix words. Those duplicate bodies were retired;
the existing published source remains preferred, with no additional credit.
