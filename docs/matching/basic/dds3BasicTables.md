# Matching notes: Basic callback tables

## Scene, Area and Mover callback tables (basic, 72 bytes)

The native writable callback tables start at `0x002FA628`, `0x002FA640`
and `0x002FA658`. Each is an existing six-slot `dds3BasicFunction_t`.
Scene and Area have four callbacks and two null slots; Mover has three
callbacks and three null slots. All eleven pointer relocations and the
existing dispatch order are preserved. World and the suffix beginning at
`0x002FA670` stay in place.

Scene's two callback result types now match the table's signed 32-bit
contract; their native 0/1 returns and generated code stay unchanged.
Mover retains its existing small-data alignment and bytes. All 51 owner
functions and the full linked retail image match.

The game report now includes these 72 bytes in its data total. The full
report transfers the same bytes out of the raw suffix. Neither report
gains matched functions, code, or aggregate matched-data bytes.

## Unit, Matter, Effect, Event, Light, Path and Model callback tables (basic, 168 bytes)

Seven more writable six-slot `dds3BasicFunction_t` records occupy
`0x002FA728` through `0x002FA7CF`. Their 23 callback pointers and
19 null slots match retail. Event's constructor now declares the signed
32-bit result required by the shared callback type; its native code is
unchanged. The Path table references the existing allocator in the Light
owner and the existing destructor in the Path owner. The Camera table
and the earlier Basic tables remain unchanged.

All 90 functions in the seven owners and the complete linked image match
retail. The game report includes 168 more bytes of source-owned data;
the full report takes the same bytes out of the raw data region. Both
reports credit 24 additional matched-data bytes (the Path table). The
other six tables match in the linked image, but the object report scores
some relocation fields differently. Function and code totals are unchanged.

## Type registry and camera callbacks (basic, 96 bytes)

The constructor path in dds3AllocBasic accepts type values 0 through 17
and indexes the table directly. That proves the final null pointer is
entry 17, not padding. The four earlier null entries stay null; type 16
points to the separate raw vtable at D_002FA7E8. The 16 preceding data
bytes and that raw vtable are not part of this migration.

The camera vtable uses the same typed constructor, destructor, main
and draw ABI as the existing basic vtables. The generic draw dispatcher
passes the basic object. The camera draw callback ignores that actual
argument and still returns one; naming the argument makes the declaration
agree with the native dispatch without changing a machine instruction.
