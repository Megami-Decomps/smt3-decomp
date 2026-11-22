# GCC destructor state

The cursor starts at the second slot of the native `__DTOR_LIST__`, after
the all-ones sentinel. The destructor loop reads it, advances only after
a non-null callback, and uses the initially zero companion flag for
one-time frame deregistration.

Both words are in native writable `.data`. Without the section attributes,
the stock compiler moves them to `.sdata` instead. The cursor emits one
ordinary relocation to `__DTOR_LIST__ + 4`. The constructor and destructor
tables are still native assembly data; the following raw reginfo and
the eight bytes after the flag stay untouched. Only eight bytes move to C.
