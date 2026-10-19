# Sweep permutations in the real file, not an isolated copy


`check_unit.py --source <file>` compiles an experiment as the unit, which is
the right tool for a single question but the wrong one for a sweep. One
agent's natural store order was **exact in the real file and ten words out in
a `--source` copy** that happened to lack two neighbouring C functions, and a
720-permutation sweep of the isolated form found nothing better than seven.
Whatever the sweep is over, run the winner in the real unit before believing
it, and prefer sweeping in place.
