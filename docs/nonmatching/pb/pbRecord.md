# Non-matching drafts: pb/pbRecord

Conventions: [../../nonmatching.md](../../nonmatching.md).

## pbAddRecord_char (pb/pbRecord, 0x144)

73 of 81 words match, at the exact 324-byte size. Saves character positions
and display flags into the current record slot using the actual eight-byte
character snapshot. Allocates even for a zero character count and reads the
live global count after callbacks.

The remaining eight words, starting at +0xB8, concern the coordinate loads
and the record-position/slot-base address calculation after the position
callback. The final status-call, record reload and status-store sequence
matches. The callback result is assigned directly to the snapshot field;
its destination address is recomputed after the call.

A separate status local gave 66 of 81 matching words. Direct typed field
accesses and the provider call on the assignment right-hand side close seven
words. The generated code reloads the character input entry for the second
callback and reloads the record position, slots and snapshot after callbacks.

Ruled out: reversed coordinate stores; refreshed signed slot-index locals;
actual slot byte-offset expressions; and coordinate pairs represented by a
two-element array or two-field structure. The index forms retain the residual;
the coordinate aggregates lose four more matching words. No shifted owner
view, unused storage or artificial dependency is used.
