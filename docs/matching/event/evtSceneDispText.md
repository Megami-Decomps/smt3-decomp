# Scene display small-data labels

The scene display rows reference thirteen eight-byte labels, including the
empty final label, followed by two eight-byte count and serial formats.
They occupy 0x54F7A8 through 0x54F81F. The contiguous block now has a
standalone C small-data owner. The preceding evtMoon small data, including
the process name, camera format, and four-byte gap, remains unchanged.

Stock EE GCC aligns eight-byte character arrays to eight bytes. Putting
the literal block in evtMoon itself shifted the native placement.
Starting its own small-data unit at the native eight-byte-aligned address
preserves each symbol without special compiler attributes. Both stock
modes match the retail literals; evtMoon code and relocations are unchanged.

The final format already occupies an eight-byte slot in the original
owner. Its text and terminator take six bytes; the last two bytes are
zero. Splat can render them as terminal fill outside the format's
label, which reduces Objdiff's attributed match despite identical
section bytes. No reference metadata is changed to count those bytes.
