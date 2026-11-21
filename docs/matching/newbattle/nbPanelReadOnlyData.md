# Matching notes: newbattle/nbPanelProcess read-only data

## Texture paths and battle debug labels

`nbPnlLoad` passes the four texture paths to the file loader in order.
The four existing 32-byte strings occupy the first 128 bytes of this
read-only area at `0x00545230`, before the first compiler jump table.

The 448 bytes at `0x005454F0` through `0x005456B0` hold the existing battle
debug strings, unit counter and affinity labels, four numeric display
formats, and motion labels. The original unit and motion tables contain
40 named string pointers and two trailing null entries in the motion
table. Keep these names, their native addresses, and the intermediate
compiler switches separate from the two independently owned data spans.
