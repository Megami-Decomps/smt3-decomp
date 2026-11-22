# Battle camera state

The writable camera at `0x00320E40` has two vectors followed by scalar
camera, transition, and animation fields. Native camera reset writes
the four animation words at offsets `0x48` through `0x54`. The skill
owner's unused extra float before those words was not part of the
retail layout. The two vectors are also accessed as VU quadwords, so
the definition preserves their 16-byte alignment.

Only the 88 bytes supported by actual readers and writers are defined
in C. The next eight bytes remain raw before the separate camera axis.
The effect and boss-event callers now declare the same camera type and
actual provider signatures. In both stock modes the four changed reader
objects differ only in the size of the undefined camera symbol, not in
code, data, or relocations.
