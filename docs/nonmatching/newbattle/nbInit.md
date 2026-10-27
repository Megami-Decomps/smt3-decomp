# Non-matching drafts: newbattle/nbInit

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## nbCallNewBattle (newbattle/nbInit, 0xA0)

29 of 40 words differ, at the exact 160-byte size, and every instruction retail
emits is present.

What is left is which of the two tables' addresses gcc hoists above the
`encpackno > 0` branch. Retail hoists both scaled indices (`encno * 38` and
`encpackno * 420`) plus `&nbEncountTable[encpackno]` into the entry block, and
keeps `nbEncountFormTable`'s own `lui`/`%lo` inside the else arm, where it adds
a separately hoisted `encno * 38 + 16` and reads the two fields at
displacements 12 and 14. The draft hoists `&nbEncountFormTable[encno]` instead
and reads at 0x1C and 0x1E, so the `nbEncountTable` arm's address setup moves
below the branch and the whole block rotates.

Confirmed on the way: `nbEncountTable`'s stride is 420 bytes with `stagemajor`
at `+0x0` and `stageminor` at `+0x2`; `nbEncountFormTable`'s is 38 bytes with
`stagemajor` at `+0x1C` and `stageminor` at `+0x1E`; `nbEncountFormTable` is at
`0x003AEFE0` and `nbEncountTable` at `0x003BADE0`; the zero fallback is
`stagemajor = 0xC9, stageminor = 1` when both read zero; and the call is a
sibling call, `nbInitMainProcess(encpackno, (u32)code >> 24,
(code >> 16) & 0xFF, encno, stagemajor, stageminor)`, with arguments five and
six in `$8` and `$9`.

Ruled out: the plain subscript in both arms (168 bytes, 2 instructions long); a
`s32 formofs` byte offset before the `if`, with and without the `+0x10` folded
in (164 bytes); a `nbEncountEntry_t *pack` hoist with the form left as a
subscript (164 bytes); both pointers hoisted (152 bytes, 2 short); the arms
written in the other order with `encpackno <= 0` first (172 bytes); the zero
test nested as two `if`s rather than an `&&` (188 bytes); a plain `s32`
index local, which is inert; and splitting `nbEncountForm_t` so that `+0x10`
is a nested sub-struct and the fields sit at 12 and 14 inside it, which gets
the displacements retail has but leaves the multiply in the arm (164 bytes)
and is one word better than the draft only when combined with a pointer to the
sub-struct hoisted above the `if`, which is the same rotation.
