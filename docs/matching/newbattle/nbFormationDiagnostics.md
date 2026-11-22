# Battle formation diagnostics

The party and enemy formation handlers retain their native assembly bodies.
The fourteen named diagnostic strings now come from aligned C arrays. Each
object contains its text and terminating zero only; alignment between the
objects and the subsequent switch tables is not part of the strings.
