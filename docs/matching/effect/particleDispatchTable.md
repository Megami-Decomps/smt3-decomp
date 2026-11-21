# Particle dispatch table

The particle manager uses one writable table of thirteen create, draw, and
delete rows. Its native callers compute a twelve-byte stride from the
particle kind. The three offset-particle rows immediately follow the first
ten rows and use the same base; no caller addresses an independent offset
table. One four-byte linker pad follows the table and stays raw.

The existing callbacks accept the generic argument (and return a generic
pointer where appropriate) at the dispatch boundary. Each callback retains
its original concrete object view internally. No adapter functions or
function-pointer casts are needed. The entire particle manager object and
all thirty-nine native relocation slots were checked in every build mode.
