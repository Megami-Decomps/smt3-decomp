# Matching notes: sdf/sdfMotion

### Model visibility callbacks

The visibility constructor allocates a 20-byte model flag node and returns
its embedded motion node. All three structures start at the same address:
the flag node begins with the model base, which begins with the motion
node. The seek and blend-seek callbacks need only the model base, while
blend initialization writes the flag node’s blend state at offset 0x10.

The three callback definitions use the motion vtable’s exact signatures
and recover the containing structure inside the callback. This avoids
calling through incompatible function pointer types. The visibility table’s
five entries, caller arguments, callback machine code, and linked retail bytes
are unchanged. Other motion tables are outside this change.
