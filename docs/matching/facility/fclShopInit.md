# Matching notes: facility/fclShopInit

## Shop item unlock rows

The pricing lookup uses seven six-byte category rows. A signed negative flag
selects the first state byte; the one event-gated row switches from price row
2 to row 7 when event bit 0x930 is set. The six bytes before the following
price table are separate alignment, not a partial eighth record. Compiled
production references the table base and encodes the flag field offset.
