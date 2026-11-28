# Battle formation diagnostics

The [party formation handler](nbFormation.md) is matched C; the enemy
formation handler retains its native assembly body.
The fourteen named diagnostic strings now come from aligned C arrays. Each
object contains its text and terminating zero only; alignment between the
objects and the subsequent switch tables is not part of the strings.
