# Partial multi-blur editor descriptor

The writable 0x40-byte descriptor (mblurDebugMenu) starts at 0x00320558
(retail file offset 0x221558). It serves the 11-item MULTI BLUR menu,
with the source at mblurParam and a separate 0x2C-byte reset copy in
the BSS reservation at 0x0056B0B8. The reservation spans 0x30 bytes;
the remaining four bytes are not copied. The separate 0x0054F2C8 slot
holds the live pointer; its trailing zero word is retained.
Mode one uses the same factory for initial setup and recreation.

The editor dispatch and mblurProcess call the factory and the draw/delete
entries using generic one-pointer signatures. Use those signatures at the
actual providers rather than converting incompatible function pointers.
The factory internally treats the supplied 0x2C-byte input as read-only
and creates a 0x30-byte owner with its texture at +0x2C. The direct event,
field, and PCP callers pass mutable parameter objects and share the same
concrete parameter/owner types; PCP previously repeated that layout locally.

Verification: stock EE compilation emits 64 data bytes with nine R_MIPS_32
relocations. Resolving them to their retail symbol addresses reconstructs
the full raw descriptor. The provider and existing direct callers retain
all allocated bytes, relocations and global symbols.
