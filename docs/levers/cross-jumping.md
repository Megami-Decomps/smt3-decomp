# Duplicated tails and post-reload cross-jumping


gcc 2.96 runs cross-jumping after reload, in `jump2`, so it compares
instructions by their **allocated hard registers**. Two consequences, both
measured on `dds3ParticleColorAnim`:

- Where retail computes a value in both arms of an `if` but converts or
  stores it once at the join, the source most likely duplicates the whole
  tail in both arms and gcc merged the common suffix. Writing one shared tail
  after the `if` instead loses the duplicated constant materialization that
  the merge leaves behind: a `lui`/`ori` pair scheduled earlier in each arm
  than the merged suffix stays in both arms, which is exactly what retail
  shows. The draft went from 45 of 83 words to 64 on that change alone.
- The merge stops as soon as one register differs, which is why retail can
  keep two of three float-to-int conversions inside the arms and share only
  the third. Do not read that asymmetry as asymmetric source.

Beware the inverse trap: making the two arms' last instructions *identical*
invites a merge retail does not have. Two arms that each end
`alpha = product; alpha &= 0xFF000000;` get the `and` cross-jumped into the
join and come out two words short, where the same two statements with the
mask folded into the first expression in one arm only do not.

Also: `objdump` collapses a run of identical instructions to `...`, so a pair
of `nop`s that ee-gcc emitted before a `div.s` can look absent when you
compare a disassembly by hand. Pass `-z`.


## Camera presets: direct stores before tail merging

`nbCameraShot_InvokeTargetPartyTop_Set` matches all 716 bytes with direct
camera-field stores in each preset arm. An ordinary-C control with six shared
float temporaries followed by common stores preserves size but differs in
61 words. The controls were measured in the complete owner at `57670f08`.
A qualified direct-store reconstruction gives the camera pointer
29 references over a live length of 121, across nine calls, choosing `$17`;
the temporary control has 11 references over length 103 and chooses `$18`.
The loop index exchanges `$17` for the native `$18`, and preset loads use
`$f0` through `$f5`. These are real stores to the camera record.

Real-compiler allocation and jump traces, without RTL dumps, verify output
parity. The direct-store trace merges the six-store suffix from three preset
arms into the fourth: `find_cross_jump` reports tails beginning at UIDs
271/303/346 versus 378, and `do_cross_jump` performs all three merges. Final
scheduling puts the first store in each incoming branch delay slot. The
compiler's shared suffix need not be a shared suffix in the source.

`effBTLGetGroupPos` takes a `f32 *` output in its defining unit and writes one
complete VU vector. The camera consumers pass their actual 16-byte vectors
through that interface; the declaration and calls preserve the owner bytes.
