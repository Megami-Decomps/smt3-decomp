# Pipe section loader state and camera position

The three initial loader records are the 16-byte `tagPipeLinkInfo`,
`tagPipePosTable` and `tagPipeTable` declared in `pipe.h`. They start at zero
and `pipeFileLoad` fills the records in place. Later C reads use the named
fields, including the position table's count and points and the object
table's entry pointer. The corresponding interior D-labels in reference
disassembly have no allocated relocation in the existing production objects;
there is no second storage object to retain.

The camera position starts at (0, 0, -500, 1). Camera updates write XYZ,
but the position setter copies all 16 bytes into the pose, so the fourth
homogeneous component belongs to the vector rather than padding. This is
the same four-lane layout as the adjacent initial eye and up vectors. The
two data-only owners account for 64 bytes; no consumer code changes.
