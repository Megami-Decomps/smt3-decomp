# Late battle-event dispatch and formation positions

Event 909 dispatches sequence steps through seven consecutive callbacks from
`nbE909Table`. The receiver, every provider and the other event sequence
tables use the same `s32 (nbEventProcessData_t *)` ABI. The interior
`formlist` analysis label sits on the third callback, has no allocated
reference, and is not a second object. The four bytes after slot six remain
outside this table.

The two 16-byte-aligned 911 formation positions are both loaded whole by
event 912's VU0 movement helper. The first also feeds the 912 setup via
`nbSetFormPosRot`. The event 912 runtime sequence dispatch owns five
callbacks, and events 913, 914 and 915 each own two. The mutable 912
cut-packet allocation is a separate C owner. The alignment following the 912
dispatch table remains raw. These four C units own 104 retail data bytes,
including 18 original function-pointer relocations. No battle-event code
or embedded resource is changed.
