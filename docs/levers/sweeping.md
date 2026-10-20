# Sweep permutations in the real file, not an isolated copy


`check_unit.py --source <file>` compiles an experiment as the unit, which is
the right tool for a single question but the wrong one for a sweep. One
agent's natural store order was **exact in the real file and ten words out in
a `--source` copy** that happened to lack two neighbouring C functions, and a
720-permutation sweep of the isolated form found nothing better than seven.
Whatever the sweep is over, run the winner in the real unit before believing
it, and prefer sweeping in place.
- **A loop-invariant constant hoisted out of a `for` lands before the
  for-init, so splitting the init out as `i = 0;` with a `while` is how a
  second invariant gets ordered after it.** `effPCPBossKAGU2AttackGetSplitPoint`
  wants `lui $7, 0x3F400000` (the 0.75f bits its mid-point scale needs) after
  `daddu $4, $0, $0` and before the hoisted `addiu $8, $0, 0x2`. As a literal
  inside the loop body the two hoists come out in body order (the `2` first);
  assigned to a `u32` local before the `for` the `lui` precedes the init. The
  one spelling that reaches it is `i = 0;` then the local, then `while (i < 5)`
  with the increment at the end of the body: two words, and the last two in
  the function.
