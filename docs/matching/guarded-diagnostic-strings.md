# Diagnostics in retained assembly

The Dante stance and restore diagnostics, and the unit-movement form index
and rotation diagnostics, are standalone strings. Their assembly callers
still use the same addresses and argument values. Only the four strings,
including their terminators, move to C. The 22 intervening and trailing
padding bytes stay outside the C string objects.

The display-mode error and four image-store timeout diagnostics in
`libgraph` likewise use standalone strings. The retained functions still
pass the same format pointers and arguments. The five strings keep their
native placement with ordinary declarations, including the original CRLF
where present. No neighboring padding is claimed.
