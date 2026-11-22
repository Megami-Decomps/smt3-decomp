# Material multitexture alpha table

`sdfMatUpdatePacketMultiTex` indexes the GS ALPHA_2 values with
`sdfMat_t.multiTexMethod`, scaling the index by two and loading an unsigned
halfword. The initialized writable span at `0x00529438` contains six nonzero
values and two trailing zero halfwords. DDS1 and DDS2 preserve the same span
and contents between their respective model default name and motion-node table.
Keep those trailing halfwords without assigning them method names or
changing the native lookup or its callers.
