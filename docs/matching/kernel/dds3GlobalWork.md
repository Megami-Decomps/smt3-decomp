# Matching notes: dds3GlobalWork

## PrimLine 3D geometry data (kernel/dds3GlobalWork, 264 bytes)

The writable outline (208 bytes at `0x002FA820`), edge pairs (40 bytes at
`0x002FA8F0`) and scale (16 bytes at `0x002FA920`) are C arrays. The
scale's native 16-byte alignment leaves eight zero bytes between the
edge list and scale, outside both objects.

The three address loads in `dds3PrimLine3D` resolve to the same retail
locations. All 28 owner functions and the linked retail executable are
unchanged. The reports count the entire 272-byte data contribution,
including alignment, as matched. No function or code bytes were added.
