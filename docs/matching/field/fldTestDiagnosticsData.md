# Field test diagnostic messages

Four eight-byte diagnostic slots are used as printf formats by the field
test and event code. Their first native address is 0x54EAC0. The live
Dante sound index and three following unit values keep the same C types,
initial values, and retail addresses.

This naturally aligned block has a separate small-data owner. Its four
zero bytes between the sound index and its final format are kept by the
generated alignment pad. Earlier fldTest small data is unchanged. Production and guarded
objects keep their opcode/register fields and relocation kinds; references
to the moved objects now use their names instead of offsets relative to
the old owner. All production references resolve to identical instructions
and retail addresses.
