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


### Iterating, and what each round costs

The reconstruction is canonical for nearly all of the source, so this converges
rather than exhausting. Each round names types, which makes their members
addressable, which the mechanical fills then take for free:

| Round | Types | Members |
|---|---|---|
| Tree-wide rewrites only | 163 | 175 |
| Resolver over the same reports | 0 | 503 |
| IDB layout fills | 0 | 43 |
| Corpus widened to the IDB | 44 | 95 |
| Types read off their own users | 9 | 13 |
| IDB fills again | 0 | 46 |
| Anchored member pairings | 0 | 11 |
| Second swarm round, fresh pairs | 35 | 375 |
| IDB fills a third time | 0 | 20 |

Four changes to what the validator accepts came out of a census of its 1,936
refusals, and together they are worth more than any single method:

- **The corpus is HD plus the IDB's own layouts**, 1,042 records to 1,698. The
  IDB holds records the reconstruction's headers do not, `datUnitWork_t` among
  them, and both carry the same symbol recovery.
- **il2cpp decorations are undone**: an `_o` object wrapper, and a file-qualified
  name like `effBattleSystem_effBTLSystem_Attack_tag`. Where the IDB itself
  carries the decorated spelling, that spelling is what the tree adopts.
- **A single report is corroborated by shape**, a size that fits inside the
  IDB's record or an equal member count, which is the rule type recovery already
  settled on. Size stays one-way: ours being larger is as likely to mean our
  shape is wrong as the name.
- **The documented pointerization is accepted in the class check**, and a
  disagreement is settled when one candidate has twice the reports of the next.

### An il2cpp artifact is not a name

`_0` is what il2cpp calls an embedded base class, and the IDB import carries it.
Taking it overwrote `effBTLSystem_Attack_tag.pFormation`, `pFormation` again on
the AttackC record, `nbCameraHelper_effFileListHandle_t.resourceCount` and
`nbCameraHelper_effFLWinHandle_t.x`. All four are back, and `_0`-style indices,
`m_*`, `klass`, `monitor`, `vtable` and the `_array`/`_Fields` wrappers are now
refused as both member and type targets. Three more `_0` members in
`effPCPMisc.c` predate this work and are the same mistake.

### A proposal onto a name the record already has

The report path refuses this; the plan path, which applies another tool's
proposals, did not. A rename onto a name already in the record reads a different
field and still compiles, which is the one mistake a rename makes silently. Both
paths refuse it now.

### What the sdf layer needs, and why it is not a rename

Every record in HD's `libsdf.h` exists on the PS2, so each of the 36 remaining
invented `Sdf*` records is one of them. No signature says which: they are packet
records local to `sdf*.c` and HD passes them as `void *`. Layout alone does not
separate them either, since a record of four 4-byte words fits dozens;
`tools/hd_header_layout.py` scopes the match to one header and still forces only
`SdfDrawNode` onto `sdfModelNode_t` (252 bytes, 13 offsets agreeing) and
`SdfSceneDrawPacket` onto `sdfGrDrawEnv_t` (368 exactly).

What does settle one is the one typed signature HD kept. Its
`sdf2DMakePrimPacketHeader` takes `sdfGrPacketHeader_t *aPacketPtr` where ours
takes `SdfDrawPacket *aPacketPtr`, so our record is that one. But the tree
already has `sdfGrPacketHeader_t`, and ours is a **finer** reading of it: our
`quadwords`, `pad02`, `pad08` and `command` all sit inside its 16-byte `dmatag`.
Replacing one with the other changes how the packet is written, not what it is
called, so it is a merge to be read and verified per record rather than a rename
to apply. `SdfTextureDrawPacket` is the same shape of problem, 32 bytes of that
header plus three register writes.

### What is left

Nothing of the agents' accepted output. The residue is in the reports rather than
the application: 1439 proposals the validator refused, most of them because two
of our types claim one HD type, or because a proposal came from a single function
not named for the type. Those need reading, not machinery.

## A third corpus: the remaster's own metadata

The reconstruction's headers declare 1,042 records, only what its sources
needed, so a member in any other record had nothing to be checked against. The
remaster's PC build carries every class in its il2cpp metadata, and IDA's import
of it names each one's fields: `tools/hd_il2cpp_types.py` exports the
`<Name>__Fields` records of that database to `config/smt3/hd_il2cpp_types.txt`,
4,504 of this game's records with 18,560 named members, 3,748 of them records the
headers never declare. Offsets there are the managed 64-bit layout and do not
transfer; the member names and their order do, which is what the validator
checks. `hd_align.py`, `hd_members.py` and the batch builders all read it as a
third corpus behind HD and the IDB.

It bought less than its size suggests: 65 more unnamed members sit in a record it
knows, and seven more gaps close by member order alone. Most of our unnamed
members are in records no corpus has under any name.

## Our name for their record

Some of our types carry the right members under a name of our own, so nothing
keyed on the name finds them. `rstWork_t` is HD's `rstData_t`: HD's
`rstAddSkill` reads `GBWK->LevelUpCnt`, `GBWK->EventParam`,
`GBWK->GetHeartsSkill` and `GBWK->pCurrentStock` where ours reads `unk_1B`,
`unk_22`, `unk_24` and `unk_16C`.

`tools/hd_type_map.py` finds those pairs without a name to go on. It resolves
every member access in both copies of each function it can, by the declared type
of the object expression, and counts which of their records meets which of ours.
Co-occurrence alone maps any two records one function reads together, so a pair
also has to name a record our tree does not have, beat its runner-up twice over,
and agree on a member name our copy already carries (or carry four reports). That
leaves 64 pairs, among them the five facility work records (`shpUpdateWork_t` is
`fclDataShop_t`, `ragUpdateWork_t` is `fclDataRag_t`) which hold 70 unnamed
members between them.

The map does not rename anything. Our record is usually the wider one, holding
inline what the remaster points to, so `--type-map` only tells the validator
which record a member proposal is checked against.

## A merge, where the target name is taken

The commonest refusal for an invented type is that the tree already has the name:
13 of the 30 the type swarm proposed. That is a merge rather than a rename, and
`tools/hd_merge.py` does the ones that are mechanical. It compares both layouts
in our own tree, pairs the members that sit at the same place with the same
extent, and replaces one of ours covering several of theirs with their own
declaration lines, which is only sound while nothing reads it.

One of the thirteen passed: `BillObj` is `billHandle_t`, same 96 bytes, five
members renamed. The rest are refused for a reason worth keeping: our record
reads the same bytes **more coarsely** and the covering member is read. `QuadF32`
is `sdfVector4_t` but its one `components[4]` is indexed; `SdfSceneDrawPacket` is
`sdfGrDrawEnv_t` but its `blendState` covers their four register writes.
Rewriting those uses is decompilation, not renaming.

## What cannot be done by gap arithmetic

`hd_members.py` pairs an unk_ member when the gap between two shared names holds
as many of HD's members as ours. The obvious extension, choosing from a wider gap
by scalar class, was written and then deleted: of 17 placements it forced, 15 were
wrong, and each in a way worth recording.

- A candidate dropped for being padding is often part of what our member covers.
  `mdlEffectResrc_IPU_t.unk0A` spans HD's `pad` and its `tex`, and the rule read
  it as `tex`.
- A member of ours that HD does not have shifts everything after it:
  `effPCP_PBCamera_tag.part` is their `hPB`, so our `unk58` took `hPB` while it
  is their `ex`.
- Where we hold a vector inline and they point to it, our first words are their
  whole field: `effPCPHamaon_tag`'s `_0`, `unk04`, `unk08` and `pad0C` are their
  one `pos`, and `unk04` took `pos` while `unk08` took `abgr`.

Only two of the seventeen survived reading, `effBTL_SkillLight_t.unknown10` as
`color_anim` and `dds3ScatterPolyList_tag.unk1C` as `scale`, and both were
applied by hand. The lesson is the one the resolver already taught: a member is
placed by what the code does with it, not by what fits beside it.

## Type names: the IDB's own type at the same place

The strongest evidence for an invented type is not a body at all. The IDB types
every parameter slot, return value and struct member it recovered, so wherever
one of ours stands in a place the IDB types too, that type is the answer.
`tools/ida_type_votes.py` collects those votes from three places (a parameter
slot where the arity agrees, a return type, and a member at the same offset and
extent of a record the IDB has) and writes them out as a report for `hd_align.py`
and a merge plan for `hd_merge.py`.

Fourteen types came back with two or more votes. Only one was a rename
(`MovObjStream` is `movieDecodeCtx_t`); the other thirteen named a record the tree
already has, which is what makes `hd_merge.py` the tool that matters here. Two
proofs stood out for being independent of each other: every `*Predraw` prototype
types its first parameter `sdfPreDrawHook_t`, and `sdfPreDrawTag_t`'s `head` and
`tail` are the same record, so our `SdfPacketPatchLink` was that record twice over.

### What the merges needed

`hd_merge.py` grew two rules to take them, both of which keep the build as the
proof:

- **Ours may stop short of theirs.** `VuWork` is `corePktCb_t` without its last
  two words: 31 members pairing one for one from offset 0, which renamed all 31
  and placed 89 uses. A record of ours that is *larger* is still refused.
- **A member of ours covering several of theirs may be split**, and their padding
  names among the pieces no longer refuse it, as long as one piece is a real
  name. `SdfDrawNode`'s `pad3C` became `pad0`, `userData`, `pad1` on the way to
  `sdfModelNode_t`.

The sort-OT records needed reading rather than either rule. `SdfPool` is
`sdfGrOt_t` word for word, including the two callbacks our copy had as `release`
and `update`; `SdfKeyPoolNode` and `SdfKeyTreeItem` are both `sdfDrawTag_t`, the
pool reading its sort key as the free marker and the AVL comparing it as a float.
Folding those two into one record meant taking the IDB's `f32 z` and reading its
bits at the pool's comparison, which the unit comparison then confirmed.

Two placeholder records went the other way, to a type rather than a record:
`Quad` and `QuadF32` existed to keep a 16-byte memory operand from decaying to a
pointer in the VU0 macros, which `u128` does by itself, and the float one is the
engine's `sdfVector4_t` (seven IDB slots say so).

### A type defined in several units has to be renamed in all of them

`SdfPool` and `SdfKeyPoolNode` were also declared in `src/smt3/sdk/libc.c` and
`libgcc.c`, so renaming them in `sdfSortOt.c` moved the drift count by one
instead of three. The drift check counts a name once across the tree, which is
the right behaviour and also the reason a rename has to find every copy.

### What layout alone still cannot do

Matching an invented record against the tree's own conventionally named records
by layout produced 5,461 candidate pairs for 96 types. A record of a few words
fits thousands of others, so the only use for layout here is confirming a pair
some other evidence already named.

## Picking the rest by hand

The types left after all of that are the ones where no rule decides and a person
has to look. `tools/type_match.py --serve` puts them on a page: our record and a
candidate's laid out side by side, aligned by offset, with every record in the
four corpora ranked behind it.

The ranking mixes the IDB slot votes, how many of our offsets and extents the
candidate reproduces (counted only when both sides have real PS2 layouts),
agreement between the member names, the closeness of the sizes, and how much of
our name survives in theirs. Each candidate also says whether `hd_merge.py` could
take it as it stands or what would hold it up, which separates a decision that
applies from one that needs the record read first. `/` searches all 5,823 records
for the one the ranking missed.

Under the layouts, both records' uses sit side by side, because what settles a
candidate is usually not its shape but what the code does with it: the prototypes
that take it, the remaster's functions that declare one, the records that hold
one, the functions in our tree that read one, and the plain references, which are
all a type of ours has when it only ever appears in declarations.

Decisions land in `config/smt3/type_decisions.tsv` as they are made, and
`--write-plans DIR` turns them into the inputs `hd_align.py` and `hd_merge.py`
already take, so nothing about the apply path or its verification changes.

The first thing it surfaced was a class nothing else had looked for: five of our
invented records already exist in the tree under a conventional spelling, two
contributors having named the same record twice. Four of them (`BillRenderPair`
as `billRenderPair_t`, `EffRandState`, `EffResourceRectDrawParams`, `ParSystem`)
merged with no member renames at all.
