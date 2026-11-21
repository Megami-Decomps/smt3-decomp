# Puzzle Boy stage progression records

The existing three-pointer level catalog reaches runs of four-byte
slots. The first slot in each run holds the clear mask and its
neighboring halfword. All later slots hold the stage number and label.
The walkers read through the negative stage-number sentinel; the
corresponding records have 20, 1 and 1 playable entries followed by
one sentinel each. The two short runs remain distinct objects with
distinct catalog addresses even though their contents agree.

Each run is an array with its actual bound, using a union to describe
the two roles of a slot without indexing beyond a one-element member.
All four stock compiler modes retain the exact game and catalog
allocations and relocations. The three neighboring four-byte regions
are not part of the records and remain raw.
