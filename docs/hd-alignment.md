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

### Applying: by resolved type, never by guess

A member use is only ever `.name` or `->name`, and which struct it reads is
settled by the declared type of its object expression, not by the text of the
line. Two mechanisms apply a rename, in this order:

1. **Tree-wide**, when no other type anywhere in the tree declares that name.
   175 of the accepted renames qualify.
2. **Per access**, when the name is shared, using `tools/hd_resolve.py` to
   resolve each use. This is what the other 340 needed.

Of the applied set: 163 types and 515 members across 143 files, the image still
hashing to `5facfbf7916836613449c922e26a4ec412207b32`, every changed C unit
matching retail function for function under `tools/check_unit.py`, and the full
`tools/test_*.py` suite passing. `tools/header_layout.c` carries the renamed
spellings, so its 755 size and offset claims still compile.

### Compiling and guessing does not work

Before the resolver existed, the shared names were attempted by renaming the
struct body only and letting the compiler point at every use that stopped
compiling: a line that errors with *no member named `color`* is a use of a type
that just lost `color`, so rewrite its next `->color` to the candidate, rebuild,
repeat. It reached zero unresolved uses after three fixes and still produced a
wrong image. Each fix is worth recording, because each one looked like the last.

- **The struct body locator.** `cparse.struct_fields` reports a definition's line,
  and in some files that line is wrong, so the body rename edited the
  *neighbouring* struct. Bodies are now found by name (`struct_spans`).
- **A candidate can itself be a deferred old name.** `flags` offers `flag`, and
  `flag` is separately renamed to `Ztest` or `Stat`. The error for `flag` then
  read as a fresh rename rather than a failed guess, and the loop abandoned
  `flags`'s remaining candidates. That cost took unresolved uses from 1226 to 92.
- **Two members collapsing onto one candidate.** `angle` and `angleStep` both
  offer `rotate`, and taking it for both produced `part->rotate += part->rotate`.
  That compiles. `effPCPFlashPolySphere3PartRotate` came back 4 of 7 words wrong,
  reading +0x08 where retail reads +0x18.

The question the loop asks is unanswerable from a line of text, which is why it
converged on a different wrong tree each time: an access's member belongs to the
declared type of its object expression. `tools/hd_resolve.py` answers that
question instead, and the guessing is gone.

**The checksum alone would not have caught this.** A collapse whose bytes happen
to cancel would not move the image, and a failed link leaves the previous image
in place so `sha1sum -c` can report OK on a build that did not happen. The gate
is `ninja`'s own exit status followed by `tools/check_unit.py` over every changed
unit. That scan is what named `effPCPFlash.c` and `effPCPMisc.c` out of 159.

### Resolving an access to its declared type

`tools/hd_resolve.py` reads the declarations in scope, per function and at file
scope, and walks the access chain from its base, so `work->head.degreesA`
resolves against the type of `head` rather than of `work`. A rename is then
written exactly where it belongs: of 719 `->color` accesses in the tree, 17
belong to `dds3Effect_Blur_t` and only those became `->abgr`.

That took the applied set from 175 members to 678, every one of the accepted
proposals, with nothing excluded. 1281 uses were placed in the final pass, and
`--fix` places an access the resolver cannot when the whole plan renames that
name to one target and the line holds exactly one unplaceable use.

Six constructs each resolved wrongly first, and each broke exactly one unit,
which `tools/check_unit.py` named:

- **Old-style (K&R) definitions** declare their parameters between the header and
  the brace, at file scope, so three functions' `source` parameters collapsed
  onto one type and `effPCPHamaCardSpiralCopy` read the wrong struct. A K&R
  definition is now its own scope and its parameters stay out of file scope.
- **`return work;` has the shape of a declaration**, which typed `work` as
  `return`. Keywords are refused as declaration types.
- **`planar[0] = pos[1];` parses as a declaration** of type `plana` named `r`
  with an initialiser, which typed a real `collision_s *r` parameter as a struct
  that does not exist. A declaration always separates type from name by a space
  or a star; an assignment to an array element does not. This is the subtlest of
  the six and the one to remember.
- **A bare cast types the result, not the base.** `->` binds tighter than a cast,
  so `(sdfVector4_t *)r->plane` casts the member. Only a parenthesised cast types
  the base, and it may wrap a whole expression: `((dds3SceneBasic_t *)b->workdata)->unit`.
- **A one-line record has no members at all** as far as `cparse.struct_fields` is
  concerned, so every chain through `itfMesInstance_t` failed. Worse,
  `filePacRequest_t` puts its whole body on one line with interleaved comments,
  so splitting on `;` produced nonsense until the comments were stripped.
- **A typedef alias names one record twice.** Following
  `typedef EffBlurTemplateBody EffPCPTexturedBlurParams;` is what makes
  `params->res.source.color` resolve, which is the chain that feeds
  `dds3Effect_Blur_t`.

Where a chain resolves to a type that has no such member, the rename is simply
not applied there, which is the right answer rather than a forced guess:
`((filePacRequest_t *)x)->packet.head` resolves to `sdfPacDecodeCb_t`, and that
record has no `head`.

A type absent from the tree's own records cannot be one being renamed, so such a
chain resolves to a sentinel meaning "not the type in question" rather than
blocking. A member missing from the record it should belong to still resolves to
nothing, which is the conservative branch.

### What is left

Nothing of the agents' accepted output. The residue is in the reports rather than
the application: 1439 proposals the validator refused, most of them because two
of our types claim one HD type, or because a proposal came from a single function
not named for the type. Those need reading, not machinery.
