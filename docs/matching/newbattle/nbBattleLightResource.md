# Matching notes: newbattle/nbBattleLightResource

Two battle stage light allocations pass the same 52-byte resource to
the existing light setter. Both sides now use its original typed
layout, including the range, diffuse and ambient fields. Their compiled
code and relocations are unchanged.

The following twelve bytes remain raw, before the existing aligned
vector owner.
