# Aligning names with SMT3HD by reading the bodies

[Type recovery](type-recovery.md) and [naming drift](naming-drift.md) matched
this tree against the IDB and the HD reconstruction by shape: sizes, offsets,
prototype slots, member order. Shape stalled where the IDB layout is HD's rather
than the PS2's (`nbMainProcessData_t` holds inline arrays there and pointers
here, so no offset keys matched), and where a type's name gave nothing to look
up. What was left is settled by reading the two bodies of the same function
side by side, which is what this page records.

The premise, taken from the reconstruction's own README: SMT3HD is the original
C transpiled, so for a function defined in both trees, HD's body is the same
statements over the same members under their original names. Our body is a
matching decompilation of the same statements under names a contributor
chose. Where a call, a constant or an assignment lines up, the member names
line up with it.

## Parameters first, and without reading anything

The IDB prototypes carry parameter names from the HD symbol recovery, and
they agree with HD's own signatures on 10,340 of 10,355 slots. So parameter
names need no alignment at all: `tools/ida_protos.py` exports every prototype
to `config/smt3/ida_protos.txt`, and `tools/hd_params.py` renames each
parameter of each C definition whose arity equals the prototype's, throughout
that one function, all names at once.

That renamed 5,260 parameters in 2,799 functions and 3,990 declarations, with
no codegen change. The commonest were `work` to `hData` (388), `pCamera` to
`pNbCamera`, `color` to `abgr`, `source` to `pPos`. Sixteen functions were
skipped because the new name was already an identifier in the body, and 419
because the arities differ.

The 15 slots where the IDB and HD disagree are IDB edits made after the
import (`a`, `b`, `c` for `priority`, `scrdata`, `startproc`), so HD's name
wins where both exist.

Three parser mistakes each broke the build once, and all three are now
refused by the tool: `const f32 *` is an unnamed slot, not a parameter
called `f32`; IDA spells a slot it could not name `_$A1`; and a rewrite
that consumed the whitespace before an identifier turned `f32 z3` into
`f32z3`.

## Then the bodies, read by agents and checked by a tool

`tools/hd_pairs.py` selects every function defined in both trees whose body
touches an invented type, an `unk_` member, a member HD's copy of the type
lacks, or a type name that neither HD nor the IDB has. For each it writes a
record: our definition, HD's, our struct definitions for the types the body
uses, and HD's member lists for the same-named types and the types HD's
signature names. Records are batched by unit, ten to a file.

One haiku agent reads each batch and reports, per function, which HD type
each of our non-HD types is and which HD member each of our drifted members
is, with one line of evidence quoting the two statements. The prompt tells
it what il2cpp output looks like (a `v[k]` slot on a `u64 *` is byte
`8k-16` of the HD layout, a 64-bit constant packs two members low word
first) and that an omission costs nothing while a wrong pair costs a lot.

The reports are proposals. `tools/hd_align.py` turns them into renames only
where every one of these holds:

- the HD type exists, the HD member is one of its members, and our member
  exists in our definition;
- the scalar class agrees (`u32` is not `f32`, a pointer is not a scalar),
  except that HD's pointer may stand for a struct we hold inline;
- proposals for one of our members agree across every function that
  reported it, and two of our members never map to one HD member;
- the renamed members keep HD's member order, which is the one thing the
  corpus is authoritative on;
- the new name is not already a member of that definition, which is the
  swap trap [naming drift](naming-drift.md#the-rename-that-still-compiles-and-is-still-wrong)
  records;
- a type rename has two reports, or one from a function named for the type;
  the HD name is not already a struct here; and no other type of ours claims
  the same HD name, unless one claimant has three reports for every one of
  the other's.

The last rule came from the pilot: `EffBillboardSetWork` and `EffPCPRingWork`
were both renamed to `effPCPSparkleSphere_tag` and the file then defined it
twice. Two of our types claiming one HD type are two readings of one record,
which is a merge for `tools/merge_duplicates.py`, not a rename.

### Applying, and letting the compiler attribute the rest

A member use is only ever `.name` or `->name`, so a rename whose old name no
other type anywhere declares is rewritten tree-wide. Most old names are
shared (`color`, `scale`, `handle`, and every `unk00`), and those are
rewritten only in the struct body; the compiler then reports every use that
stopped compiling, and since exactly the renamed types lost that member, a
line that errors with *no member named `color`* is a use of one of them.
`--fix` rewrites the next `->color` on that line to the candidate, rebuilds,
and repeats. When one old name has several candidates (`unk00` on two
types), a later error naming the candidate it tried means the guess was
wrong for that access, so it is put back and the next candidate tried; an
access that fits none is restored and reported.

Judge the result by `ninja`'s exit status. A failed link leaves the previous
image in place, so `sha1sum -c` can say OK on a build that did not happen.
