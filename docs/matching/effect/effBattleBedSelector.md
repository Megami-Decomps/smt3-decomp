# Battle BED file picker

The writable editor record at 0x31C7D0 is the BED SELECT file picker, not
a text literal. The first word points to the existing title at 0x533A40;
its little-endian bytes resemble ASCII, so the reference needs an explicit
pointer relocation. The record has no items and preserves its unused self
pointer at +0x10. Input and drawing code check the zero item count before
indexing that slot; drawing still invokes the file-picker update callback.

The three callbacks use the editor's generic signatures. The factory still
returns its input, and the update and cleanup functions ignore their input.
No calls or memory accesses change. The provider retains its compiled bytes,
and the descriptor keeps all five native address relocations.
