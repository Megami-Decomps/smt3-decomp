# Matching notes: Scene, Area and Mover Basic

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
