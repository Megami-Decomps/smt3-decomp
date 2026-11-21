# Matching notes: ripple blur editor setup

The ripple-blur editor exposes fifteen controls. It copies the 44-byte
parameter record to its native backup before editing, restores it on reset,
and uses its first editor resource pointer for create, draw and destroy.
A typed handle and an unclassified zero word retain the original eight-byte
resource storage layout.
The provider implements the shared editor callback signatures and recovers
the concrete parameter/work type at entry. Direct callers and the provider
use the same declarations.

Both stock modes preserve all allocated sections and relocations in the
three existing provider/reader units. The standalone descriptor covers
the original 64-byte span with the same nine named address references.
