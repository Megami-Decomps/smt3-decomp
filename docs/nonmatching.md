# Non-matching drafts

Reconstructions that are close but not exact, kept in the source behind
`#ifdef NON_MATCHING` so the next person starts from the draft instead of
from scratch. The build never defines `NON_MATCHING`, takes the assembly
fallback, and stays byte-identical; the function still counts as unmatched.

The source carries one line saying how close the draft is and what differs.
This page carries the rest: what the remaining difference actually is, and
which source shapes have already been ruled out. Read the entry before
picking a draft up, or you will repeat the search.

A draft is not evidence of anything. It is not compiled by the build, so
`python3 tools/check_nonmatching.py` compiles every draft and checks that each
has an entry here. `python3 tools/progress.py` counts the drafts as their own
measure, apart from the matched functions and bytes, and the progress site
shows the same count and marks the drafted functions in each unit.

**Write the measurement as "N of M words match", never "N of M words".** The
bare form is ambiguous and has already misled: an entry reading "10 of 146"
was taken for a near miss when it meant ten words matching out of 146, a
draft 136 words out. Say `match` or `differ` explicitly, and give the emitted
size against retail's when they differ.

## Where the entries are

One page per unit under [nonmatching/](nonmatching/), so an agent reads only
its own unit. `docs/nonmatching/INDEX.tsv` is the searchable list: one row per
draft with its unit, size, distance, and the file and line to read. Grep that
first and open the range, rather than reading a whole page.

    grep -P '\tnbCameraBoss\t' docs/nonmatching/INDEX.tsv
    sed -n '120,190p' docs/nonmatching/newbattle/nbCameraBoss.md

Add a new entry to the unit's page (create it if the unit has none), then run
`python3 tools/docs_index.py` to rebuild the index and
`python3 tools/check_nonmatching.py` to confirm every draft has one.
