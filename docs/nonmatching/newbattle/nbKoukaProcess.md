# Non-matching drafts: newbattle/nbKoukaProcess

## NumEffectClip (newbattle/nbKoukaProcess, 0x40C)

The draft emits 1044 bytes against the native 1036, with 62 of the 259 native
instruction words matching and eight excess bytes. A scratch promotion leaves
the target and its data ownership unmatched; the normal owner retains all 55
C matches through the assembly fallback.

The input and output position travel in `vf10`. The initial projected point is
accepted for signed pixel X in [13,500) and Y in [75,324), and returns the
preserved incoming vector. The fallback starts with the packet formation's
sphere center. Skill IDs 0x107, 0x108, 0x10B or 0x10D with invoke-model ID 0xC0
raise its Y by `h * scl * 0.35f`. Skill 0x9E instead checks targets for model ID
0x158. Keep fresh `packet->form` reads after provider callbacks.

The other path projects model marks 10 and 0, using the sphere center when a
mark query fails. Convert successful screen arrays in place with signed shifts
4 and 3. Failed mark projections become pixel sentinels (512,448), with metric
zero. The two rounded screen-distance results occupy a real two-element array;
this reproduces the native 192-byte frame and its +0x70/+0x74 stores. Using two
register scalars instead emits a 176-byte frame.

Choose mark 10 only when its screen metric is smaller than mark 0's and its
pixel position is valid. Otherwise choose a valid mark 0. If neither qualifies,
compare the 3D distance from mark 0 to the saved sphere center: ordered distance
below 1000 lowers center Y by `h * scl * 0.35f`; all other results choose mark
10. The six diagnostics have separate padded data symbols. Keep their full
retail extents when promoting; external references alone leave the promoted
unit's rodata layout wrong. No string bytes are moved by the guarded draft.

Native also reads the initial screen words after a failed projection, although
`nbClipEffectNum` can return without writing them. The draft preserves that
uninitialized-stack behavior rather than silently initializing the array.
The remaining code-generation questions are saved-register roles and branch /
epilogue structure. A common outer guard improves overlap over an early-return
form, but does not close the function. No padding or artificial uses are added.
