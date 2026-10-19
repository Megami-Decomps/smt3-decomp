# Check the whole unit, not the function you edited


ee-gcc's output for one function can change with code earlier in the file, so
a function that matched can stop matching because of an unrelated edit.
`tools/check_unit.py` reports that as `CONTEXT <name>: compiles differently
inside the full unit`. Always read the whole unit's result, not the line for
the function you were working on.
