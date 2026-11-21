'# Matching notes: current pad button flags

This writable bank contains two sets of two pads, with sixteen byte-sized
button values per pad. Initialization clears all 64 bytes, and the kernel
updates the two banks from the previous-state array. The native bank address
is 0x2F5810 and the following menu table still starts at 0x2F5850.

Signed button reads keep their high-bit test. Readers that need an unsigned
byte and the existing volatile read continue to use those access modes.
The named message and model controls select the same original byte offsets.
No new slot, alignment fill, alias, or pointer table is defined.
