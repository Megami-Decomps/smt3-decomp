# Matching levers

What to change in the source when a function is close but not exact. Every
item here came from a function that matched afterwards, or from one that was
reverted after the lever was ruled out. [Contributing](CONTRIBUTING.md) covers
the workflow; this is the list to work through before reaching for a
permuter.

Read the retail assembly first and ask which of these it is. Guessing at
source shapes at random costs more than the reading does.

Three references in [dds-decomp](https://github.com/Megami-Decomps/dds-decomp)
go further than this page, since it is the same engine and compiler:
`docs/idioms.md` is 14,000 lines of source shapes confirmed to produce
specific ee-gcc 2.96 output, `docs/compiler-decision-atlas.md` locates which
pass is responsible for a mismatch, and `docs/matching-investigation.md` is
the method for finding the missing source fact. Check each claim here, since
it was measured on DDS and not on this ELF.

## Where the levers are

One page per section under [levers/](levers/). `docs/levers/INDEX.tsv` is the
searchable list: one row per lever with the mnemonics it concerns, a one-line
tell, and the file and line to read. Grep it for the instruction you are
looking at rather than reading a page end to end.

    grep -P '\bmovz\b' docs/levers/INDEX.tsv
    sed -n '340,360p' docs/levers/addressing-and-types.md

Add a lever to the section page it belongs in, then run
`python3 tools/docs_index.py` to rebuild the index.
