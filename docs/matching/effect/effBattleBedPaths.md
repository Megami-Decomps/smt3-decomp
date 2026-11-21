# Battle debug BED paths

The initial resident-effect and selected-preview paths are writable strings
read through the existing pointer variables used by the battle loader and
debug preview. They occupy 25 and 26 bytes including their terminating NULs.
Their original addresses and the pointer initializers are unchanged.

The seven and six following zero bytes remain raw, including the original
four-byte pad label before the neighboring debug descriptor. That descriptor
keeps its 64 bytes and four named relocations. The native data-tail helper
completes only the comparison references; production links the strings.
