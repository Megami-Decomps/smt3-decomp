# Matching notes: fclPanel

## fclDrawPanel (fclPanel, 0x54)

8 of 21 words differ, at the exact 84-byte size. Draws the panel's background sprite,
calls the panel's own `draw` hook with the panel and the id, and tail-calls the
foreground sprite.

Four of the eight words are the same `ori` rotation recorded for `cmp_23A918`
above: retail emits each sprite id's `ori` immediately after its `lui`, and
ee-gcc's second scheduling pass moves it past the `sd`/`ld` of a saved register.

The other four are the function-pointer load and its `jalr`: retail uses `$2`,
immediately after a `jal fclDrawPanelSprite` whose result is ignored, and this
uses `$3` because the ignored `s32` result still reserves `$2`. The fix by the
levers page is a `void` declaration of `fclDrawPanelSprite`, but it is defined
in this unit as `s32` with `return cmpDrawSprite(...)` and matches that way, and
three other callers ignore its result as well, so turning it `void` would have
to be measured across all of them at once.

Also settled: the `draw` field at `+0x38` takes two parameters, the panel and
the id, which is why `fclPanel_t` declares it that way; `panel->draw(panel)`
does not compile to retail's `$5` setup.

## fclDrawPanelType0Grad (facility/fclPanel, 0x100)

6 of 64 words differ at the exact 256-byte size. Two gradient triangles sit
between sprite passes 0x48 and 0x44. Each triangle reloads panel Z after the
preceding callback; the third vertex keeps the first color's low-byte alpha.

The paired `coords[2][3]` array preserves contiguous X/Y triples. Colors precede
vertex assignments, and coordinates are assigned by vertex. Only the second
color's `ori` differs in scheduling: retail places it at +0x98, while the draft
places it at +0xAC after five argument/constant moves. All stored values agree.

Separate X/Y arrays change the stack layout. Chained tail-color assignments
swap two stores; a byte cast reloads the alpha and adds eight bytes. The word
mask preserves the native size and the six-word scheduling residual.
