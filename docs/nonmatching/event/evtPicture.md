# Non-matching drafts: event/evtPicture

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## evtPictureProcess1 (event/evtPicture, 0x228)

Thirty-three of a hundred and thirty-eight words differ, positionally and
aligned alike: the draft is the same 138 words with the same instruction
sequence apart from the position interpolator's schedule, so there is no
insertion for an alignment to absorb. `check_unit` reports the same 33 inside
the full unit as standalone.

The residual is three clusters. Twenty-one words are the `pos_cnt` block: in
retail the decremented counter takes `$2` and `sw $2, 0x44` lands immediately
after the `subu` that forms `t`, before the division's divide-by-zero `beql`
guard, and the x and y halves are interleaved (x's difference, x's `mult`,
y's difference, y's `mult`, both to pipe 0, then `div1` for x and `div` for
y). The draft forms both differences first and emits `mult` plus `mult1`, and
its store of `pos_cnt` sits at the end of the block. Four words are the alpha
block and eight the zoom block, and those are register names only: retail
puts the alpha block's difference temp in `$8` and the zoom block's four
converted values in `$f1`/`$f2`/`$f0`/`$f7` where the draft uses
`$f1`/`$f3`/`$f2`/`$f0`; the instructions and their order are identical.

Three source facts took this from 120 of 140 to 33 and are why the draft is
worth keeping.

The counter has to be a block-local `s32 cnt = work->pos_cnt - 1;` whose store
back to `work->pos_cnt` is the **last** statement of the block. That keeps
seven integer temps live at once in the block, local allocation reaches `$8`,
and the function-wide `work` pointer is pushed from `$8` to retail's `$9` with
`tex` behind it in `$10`. Every other spelling leaves six temps live and
shifts both pointers down one register, which alone is 119 of 140 differing
words. A plain `work->pos_cnt--;`, the store first, the store in the middle,
the store after the x assignment, `x0`/`y0` locals, a `time` local and locals
for the two stores were all measured and all give the six-temp allocation.

The flag test must be an early `if (!(work->flag & 1)) { return 0; }` with the
rest of the function in a block, not `if (work->flag & 1) { ... }` around it:
that is what puts the flag temp in `$3`.

The priority-table fields have to be written floats first, then the integers,
then `tex` and `ot` last. Only that order makes the first instruction after
the zoom block a float load, so the `blez` guard's delay slot takes the
counter decrement as retail does. With `tbl.ot = work->ot;` anywhere before
the float block, cc1 hoists its `lw` into an annulled `blezl` slot and emits
it a second time later, which is two words longer than retail at 140. All six
permutations of the three groups were measured: floats first and `tex`/`ot`
last is 53, floats first and the integers last is 66, and every order with
`tex`/`ot` first is 103.

One `f32` local reused for both the horizontal and the vertical delta, rather
than separate `dw` and `dh`, is worth twenty words: it reproduces retail's
whole floating-point register assignment for the packet builder. Writing the
delta expression twice instead of a local is 150 words, twelve too many.

Also ruled out: the three interpolator counters spelled as `cnt` locals in the
alpha and zoom blocks (50 and 33, no better); the zoom expression with
explicit `(f32)` casts on every operand, with the two multiplicands swapped,
and with a local for the endpoint difference (all 33 or worse); a `t` local in
the alpha block (62); `t` declared inside the position block instead of at
function scope (33, inert); computing y before x (33, inert); locals for the
two differences (35); the x1 and y1 sums reassociated with parentheses (55);
and the half-delta folded into the local (`dw * 0.5f` computed once, 140
words). Making `evtPicturePriAdd` an `INCLUDE_ASM` neighbour rather than a C
definition changes nothing here, so the callee-visibility lever does not
reach this function.
