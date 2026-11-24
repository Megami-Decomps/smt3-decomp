# Matching notes: SDF triangle workspaces

The core renderer writes three 32-bit vertex pointers for each accepted
triangle and reads them back as a 12-byte row when building draw packets.
It records back-facing triangle indices as 16-bit entries and uses
those indices to look up the source triangles when building the back-face list.

The pointer list at `0x581030` ends at the index list's base, `0x58D030`.
The index list ends at the first face-division parameter, `0x58F030`.
These boundaries give 4,096 entries in each list. They describe the native
storage, not a new runtime capacity check. The four raw bytes after the image
queue and the following parameters remain separate. The numeric labels
generated strictly inside the pointer list have no separate references.
