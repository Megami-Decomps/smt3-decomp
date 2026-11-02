# Data-object width mismatches

The data-object counterpart to
[return-width mismatches](return-width-mismatches.md): a global variable whose
true storage width disagrees with some consumer's `extern` declaration. Same
risk, same reason it is invisible on retail hardware (the EE reads and writes
whatever width the declaration says, and nothing checks it against another
TU), same reason it matters to a native port (the host ABI makes no promise
about bits a declaration didn't ask for, and a bad width here reads or writes
real neighbouring bytes rather than failing to link).

## Method

Most of these globals have no real C definition at all. `INCLUDE_SDATA` and
`INCLUDE_RODATA` (`include/include_asm.h`) splice in a not-yet-decompiled
datum's literal retail bytes from `asm/<version>/nonmatchings/<unit>/<sym>.s`;
their `TYPE` argument is discarded by the macro in every build mode (the
normal build emits raw `__asm__` that never mentions it, and the
M2CTX/PERMUTER/SKIP_ASM build expands the macro to nothing). `tools/include_sdata.py`
confirms this from the writing side: every line it generates reads literally
`INCLUDE_SDATA(const s32, "<unit>", <sym>);`, the same fixed string, regardless
of the symbol's real type. Grepping the whole tree shows every existing
`INCLUDE_SDATA`/`INCLUDE_RODATA` call uses that same literal "const s32".
It is leftover boilerplate from the macro's template, not a type declaration,
and must not be read as one. The generalisation: an `INCLUDE_` directive is
never a declaration, so the only authorities on a split-assembled object's
width are `symbol_addrs.txt`'s `type:` and `size:` fields, the retail bytes
themselves, and the neighbouring symbols' extents; that false signal sits
identically on every one of the 4657 `INCLUDE_SDATA`/`INCLUDE_RODATA` call
sites in `src/smt3/` (`grep -ro "INCLUDE_SDATA(\|INCLUDE_RODATA(" src/smt3/ |
wc -l`), so reading any one of them as a width claim is one step from the
same error this document exists to head off.

For these objects the true width comes from the retail bytes themselves (the
directive in the `.s` file: `.byte`/`.short`/`.word` and its count) and from
`config/smt3/symbol_addrs.txt`'s `size:` field, which is computed from the
same IDA export's byte span, not its `type:` label. The `type:` label is an
IDA default and is not always trustworthy on its own (see the one near-hit
below); `size:`, backed by the actual `.s` bytes, is. That true width is then
checked against every `extern` declaration that reads or writes the symbol,
the same cross-check the function audit did against a definition's declared
return type.

A handful of these globals do get a real C-level definition once decompiled
(an initialized or BSS variable instead of `INCLUDE_SDATA`); those are checked
directly against every other TU's `extern` declaration of the same name.

## `dds3VblanksPerFrame`: investigated, not an instance of this class

Reported shape: defined `INCLUDE_SDATA(const s32, "kernel/dds3KernelMain",
dds3VblanksPerFrame)` at `src/smt3/kernel/dds3KernelMain.c:291`, declared
`extern u16` in all three consumers (`dds3KernelMain.c`, `mviManager.c`,
`stfMain.c`), written to by `stfMain.c` despite the apparent `const`. On its
face this looks like exactly the function-return shape, in a data object.

It verifies as a false alarm, for the reason the Method section gives: the
`const s32` is the dead macro argument, not a definition. The real facts:

- The retail bytes are `.short 0x0002` at `0x0054E82C`
  (`asm/smt3/nonmatchings/kernel/dds3KernelMain/dds3VblanksPerFrame.s`): two
  bytes.
- `symbol_addrs.txt` agrees: `dds3VblanksPerFrame = 0x0054E82C; // type:u16
  size:0x2`.
- The lower neighbour settles where it starts: `dds3KernelMain.c:17` declares
  `extern s16 D_0054E82A`, two bytes at `0x0054E82A`, ending exactly at
  `0x0054E82C`.
- The upper neighbour is a confirmation, not load-bearing proof, but it too is
  in tracked source: `dds3KernelMain.c:293` carries
  `INCLUDE_SDATA(const s32, "kernel/dds3KernelMain", D_0054E82E);`, two lines
  below the directive for `dds3VblanksPerFrame` itself, so a separately
  addressed object exists there regardless of its value. The value needs the
  build-generated side: splat, run through `python configure.py` against the
  local retail ELF per `config/smt3/`, writes
  `asm/smt3/nonmatchings/kernel/dds3KernelMain/{D_0054E82A,dds3VblanksPerFrame,D_0054E82E}.s`,
  which read, back to back, one `.short` each: `.short 0x0000` at
  `0x0054E82A`, `.short 0x0002` at `0x0054E82C`, `.short 0x0000` at
  `0x0054E82E`. `asm/` is gitignored (it is derived from the copyrighted
  retail image, which this repo never tracks; see the project's `CLAUDE.md`
  on the Remote), so those files will not exist until `configure.py` has been
  run with the retail ELF in place, and a grep limited to tracked files will
  not find the value, only the directive that proves the object exists.
  Without the value: `size:0x2` in `symbol_addrs.txt` and the lower
  neighbour's end address already fix both the extent and the start, which is
  sufficient on its own for the conclusion that follows.
- All three consumers already declare `extern u16 dds3VblanksPerFrame;`, no
  `const`. They agree with each other and with the true width. `stfMain.c`'s
  writes (`dds3VblanksPerFrame = 2;` / `= 1;`) are ordinary assignment through
  a non-`const` `extern`, not undefined behaviour: there is no `const`
  qualifier on any actual declaration of this object anywhere in the tree.

No width disagreement exists to fix. A declaration change (centralizing the
three identical `extern u16` lines into a header, or widening any of them)
was considered and rejected:

- **Widening to `s32`/4 bytes** (the natural "outcome 2" shape, and a form
  that a `NON_MATCHING`-guarded declaration could in principle give a native
  build without touching the matching build, the same way
  `include/fpu.h`'s `NATIVE_FPU` bodies or `sdfMovie.c`'s `movieMemcpy`
  split do for other properties) is not just unneeded here, it is wrong: per
  the layout above, the extra two bytes are `D_0054E82E`'s, not slack. The
  three reads (`dds3KernelMain.c:229`, `:230`, `mviManager.c:118`) would not
  announce this on a little-endian target: the EE and a typical x86-64 port
  host are both little-endian, `D_0054E82E` currently holds `0x0000`, and a
  4-byte load at `0x0054E82C` reads `02 00 00 00`, still `2`. Only a
  big-endian target would get the wrong number from the read. The real,
  byte-order-independent damage is in the writes. `stfMain.c:324` and `:357`
  assign `2` and `1` through the `extern u16` at `stfMain.c:18`; widened to
  `s32`, those become 4-byte stores that overwrite `D_0054E82E`'s two bytes
  with whatever lands in the upper half of the stored value, `0x0000` for
  both `2` and `1`. That happens to match what is already there, so the
  corruption would not change a single observable bit today. It depends on
  `D_0054E82E` never meaning anything, which nothing here establishes:
  `D_0054E82E` has no current reader or writer in this source tree (plausibly
  still-unrecovered retail behaviour, or padding), so nothing guarantees it
  stays irrelevant. Widening would have produced a correct frame rate, a
  correct movie divider, and a silent four-byte store over a neighbour,
  passing any smoke test and waiting for `D_0054E82E` to acquire a use. A
  visibly wrong value gets found and fixed; this would have been accepted and
  stayed, which is exactly the kind of silent failure this audit exists to
  catch.
- **Centralizing the three `extern u16` declarations into a shared header**
  would be a pure style change with no defect behind it: all three already
  say the same thing, matching retail's own data. This family of kernel
  globals (`dds3FrameCount`, `dds3PauseSign`, `dds3RunCount`, and the rest
  declared beside `dds3VblanksPerFrame` in `dds3KernelMain.c`) is declared
  locally per consumer throughout this codebase, not through a shared header;
  moving just this one against that convention, for a variable with no
  drift problem, was not done.
- **A documented ABI-view allowance** does not apply either: that allowance
  is for a real, deliberate disagreement the matching policy accepts under
  stated conditions. There is no disagreement here to document as deliberate.

The outcome is: no code change, because there is nothing to change. This
entry is the record, so the next person (or the next port session) who
notices `INCLUDE_SDATA(const s32, ...)` next to `extern u16` does not have to
redo this investigation.

## Sweep for the same shape

A scripted sweep checked every scalar global in `src/smt3/` two ways:

1. **Split-assembled objects** (no real C definition, placed by
   `INCLUDE_SDATA`/`INCLUDE_RODATA`): true width from `symbol_addrs.txt`'s
   `size:` field, checked against every scalar `extern` declaration of the
   same name in `src/smt3/**/*.c` and `include/*.h`.
2. **Real C-level scalar definitions** (an initialized or BSS variable, not
   an `INCLUDE_` placeholder): the defining TU's own declared type, checked
   against every other TU's `extern` declaration of the same name.

Both passes compare byte width (`s8`/`u8` = 1, `s16`/`u16` = 2,
`s32`/`u32`/`f32` = 4, `s64`/`u64`/`f64` = 8), the same dangerous shape as the
function-return audit. Result: **zero genuine width mismatches** in either
pass, after one near-hit resolved as a false positive (below). Part 2 also
found zero cases of a `const`-qualified definition reaching an unqualified
`extern` elsewhere (the write-through-`const` shape the report described).

### Near-hit: `sdf3DLightDefault`

`symbol_addrs.txt` lists `sdf3DLightDefault = 0x005518B0; // type:u16
size:0x4`: a `type:` label of `u16` next to a `size:` of 4 bytes, which
disagree with each other before any C code enters the picture. The two real
consumers do not disagree: `sdf3D.c` has `extern u32 sdf3DLightDefault;`
(stored as `(u32)aLight`), `sdfModel.c` has `extern void *sdf3DLightDefault;`
(read back as a pointer); both are 4-byte types on this target, both agree
with the 4-byte span, and the storage itself (`INCLUDE_SDATA` in
`sdfModel.c`) is sized to match. This is `symbol_addrs.txt`'s own `type:`
label being an IDA default that does not match its `size:` field, the thing
the Method section above already discounts `type:` for; it is not a
consumer-visible disagreement and needed no fix.

### What this sweep did not cover

- **Signedness-only differences** (same width, `s` vs `u`, e.g. `symbol_addrs.txt`
  says `u32` where the hand-written C says `s32`): 538 of them, excluded from
  the table above because they carry no storage or truncation risk, the
  danger this audit and the return-width one are both about. They are almost
  certainly `symbol_addrs.txt`'s IDA-derived `type:` defaulting to unsigned
  for a plain data word of unestablished sign (consistent with this project's
  own caution that IDB names, and by extension loosely-recovered type
  annotations, do not carry the same weight as boundaries; see the project's
  `CLAUDE.md`). They are a different bug class from a width mismatch, since a
  sign difference can still change a comparison's result (`(s32)x < 0` vs.
  `(u32)x < 0`) without touching storage width or breaking on a native port's
  ABI the way a width mismatch does. Left unaudited; flagging the count so
  the next pass does not have to rediscover it from zero.
- **Arrays, pointers, and structs.** Both passes match scalar primitive
  declarations only (the regex family `s8`/`u8`/.../`f64`, no `[`, no `*`).
  A width or layout disagreement on an array or struct member is the same
  risk in principle but needs a different check (the declared element count
  and the struct layout, not a single width) and was not attempted here.

Result for this round: one reported case, investigated and found to be a
false alarm (the macro-argument misreading above), and an automated sweep
across every scalar global the method covers that found nothing else of the
dangerous shape. The method and this negative result are the lasting value;
the signedness and aggregate gaps above are exactly scoped, not swept under
anything, for whoever picks this up next.

## A related class the checksum cannot see: emission width

A data object's `type:` in `config/smt3/symbol_addrs.txt` controls how splat
emits it, and for an object holding pointers that choice decides whether the
pointer words carry a label reference or a literal address.

`dds3LightParam2` at `0x002F5EE0` was annotated `type:u8`, so splat emitted
its 0x38 bytes as 56 `.byte` directives. Its first four words are pointers,
dereferenced through `dds3LightParam2.parallel` at
`kernel/dds3KernelProcess.c:107`, and `include/kernel_draw.h`'s
`dds3LightState_t` declares them that way. Byte-wise emission gave those words
no relocation, so they stayed literal retail addresses: on the EE that is
correct, and in a native host build the dereference faults, which is how it
was found. Typed `u32` the same object emits `.word D_002F5E20` and a host
link relocates it.

**The retail checksum is blind to this.** The same bytes emitted as `.byte` or
as `.word` assemble identically, so the image matches either way and no check
in this repository fails. That makes `type:` on a pointer-bearing data object
load-bearing for consumers and unverifiable here. Two consequences worth
keeping:

- a before-and-after extent check across a `type:` change must count bytes on
  both sides, never directives: a directive count is not a byte count until
  you know the directive's width, and the width is exactly what the annotation
  under test controls. 56 `.byte` and 14 `.word` are the same 0x38 bytes and
  the same declared size;
- the type belongs in the IDB, not in the generated block of
  `symbol_addrs.txt`. Correcting it there is the documented exception for a
  type the retail code proves wrong.

Where the right type depends on a struct layout nobody has measured, say so
rather than deciding it in passing. The IDB puts `sdf3DFogParam_t` at 0x14
where `kernel_draw.h`'s offsets imply 0x18; that disagreement is left visible
rather than resolved inside a commit about an annotation.
