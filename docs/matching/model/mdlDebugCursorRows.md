# Model editor cursor rows

The PIB track marker editor selects rows 0 through 22: seven scalar fields
and four channels for each of four colors. The directional-light editor
selects rows 0 through 14, and the soften/bias editor selects only rows 0
through 2. Each table uses the same unsigned halfword loads as before.
The two zero bytes immediately after each table stay outside the C object.
