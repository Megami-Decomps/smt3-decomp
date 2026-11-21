# Event viewer menu tables

## Viewer action dispatch data

The action dispatcher indexes the 13 slots at file offset `0x22DB48` by
viewer mode. Mode zero has no callback; all 12 other callbacks return a
signed result and are called with the menu's X/Y coordinates. The effect
control callback ignores both coordinates. The historic `evtMenuTable`
label is an unreferenced name for slot 1, not a second native object.
The four zero bytes following slot 12 remain outside the C owner.

## Viewer frame-set menu labels

The writable pointer pair at `0x22D6E0` holds Yes and No in that order.
`EnterMain` installs the pair via the existing `itemNames` field, which
`evtSelectMenu` uses to render the labels. Both pointers retain their
original symbol relocations; the character arrays are unchanged.
