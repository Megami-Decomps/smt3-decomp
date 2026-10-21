# Non-matching drafts: fclPanel

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

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
