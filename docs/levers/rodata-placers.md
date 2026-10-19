# Do not run the rodata and sdata placers standalone


`tools/include_rodata.py` and `tools/include_sdata.py` rewrite every source
file they can reach, not just the unit you are working on: one standalone run
touched 21 unrelated files. `configure.py` runs them as part of its normal
pass and leaves the tree clean, so `configure.py --force-split` is the only
thing to call.

A string that splat folded into a now-decompiled function's asm file needs no
manual `INCLUDE_RODATA` either: `configure.py` places the line and emits the
split file, but `build/eeasm/` only catches up after a `ninja`, so
`check_unit.py` reports `NOASM` until then.
