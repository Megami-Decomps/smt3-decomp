# Addressing and types


- A struct pointer local is what keeps two index scalings apart. For
  `base[port].arr[slot]`, writing it out folds both scalings into one
  `(port * N + slot) * 4`, and a row pointer (`u32 *f = base[port].arr;`)
  materialises the whole address into a register and stores at offset 0.
  `sdfMcControl_t *cb = &base[port];` then `cb->arr[slot]` gives retail's
  separate `sll` per index, their sum, and the base folded into the
  reference's `%lo`. All three `sdfMcCb` slot-flag accessors turned on it.
- A `u8` field compared against a small constant folds to `sltu`, since cc1
  knows the promoted value is non-negative. Copying the field into an `s32`
  local first and testing that gives retail's `slti`, and the local is reused
  for the `+ 1` store as retail does.
- A byte offset added to a **reloaded** global on every iteration is a second
  variable in the `for` init, not an expression. `for (i = 0, ofs = 0x1160;
  i < 3; i++, ofs += 4)` with the accesses written
  `*(T *)((u8 *)g + ofs + c)` gives retail's offset induction variable plus a
  fresh `lw` of the global per iteration; `&g->field + i` lets gcc
  strength-reduce the subscript into a pointer and hoist the global's load
  into the preheader, which is 31 of 32 words out in `campDhCheckTextures`.
  Which of the two init expressions comes **first** decides the saved
  registers: `ofs` first gives retail's pair in `campDhCheckTextures` and
  `campDhScanEntries`, `i` first swaps them and costs six more words.
- `andi 1` followed by a redundant `andi 0xFF` on a value that only feeds a
  branch is a `u8` local for the masked bit, `u8 usable = flags & 1; if
  (usable == 0)`. cc1 then zero-extends the QImode pseudo before the test
  where the direct `if ((flags & 1) == 0)` drops the mask. `cmpItemIsUsable`
  and `cmpChkDoNotUseSkill` in `code_0022C578` both need it. This is the
  readable side of `cmpItemHasFlag`'s residual, where the same pair feeds a
  **return value** and no spelling reaches it.
- A clear loop written up-counting, whose index the body uses only as a
  subscript, is what gcc reverses and strength-reduces into a decrementing
  address plus a separate `bgez` downcounter.
  `for (i = 0; i < 0x40; i++) tab[i] = ' ';` is exact in
  `dds3PrintProcessTree`; writing the same loop down-counting defeats the
  reduction and leaves an `addu` of base and index in the body.
- The motion-speed setter is inlined in `newbattle/nbKoukaProcess`: retail writes
  `form->hunit->owner->first->speed` with no `jal`, which pins `mdlObj_t + 0x1C`
  as `first` and `sdfMotionCtrlr_t + 0x20` as `speed`. Hoist both hops into
  locals when a `switch` uses them in more than one arm: `RunMotionStopKouka`
  needs `evtUnit_t *hunit` *and* `mdlObj_t *obj` assigned before the dispatch,
  since retail loads `hunit->owner` in the dispatch's delay slot.
- Three identical call sites through **one** function-scope pointer local give
  that local nine refs and one pseudo, so the index temp feeding it takes `$2`
  and the loaded function pointer `$3`. Declaring the pointer per site, in a
  braced block, gives each site its own pseudo that ties the index into `$4` and
  matches retail's `lw $4, 0x2C($22)` / `sll` / `addu` / `lw $2, 0x10($4)`.
  `effSwordTrackDraw` has three `ot = &sdfGrOtTable[work->otIndex];
  ot->regFunc(ot, list);` sites and sat at 15 of 176 words with the shared local;
  the per-site declaration took it to exact. Six spellings of the address
  (`sdfGrOtTable + i`, a `u8 *` cast, a table pointer local, the two-step `+=`,
  the inline `sdfGrOtTable[i].regFunc(&sdfGrOtTable[i], list)`, a cast on
  `regFunc`) all leave the shared local at 15.
- In a comparison between an `f32` field and an integer cast to `f32`, gcc
  expands the cast first whatever the operand order, because it is the more
  complex operand. With a `u32` the cast is a branch (`bltz`, then the
  `srl`/`andi`/`or`/`add.s` pair), and the field load lands inside both arms of
  it. Reading the field into an `f32` local on its own statement first puts the
  load above the branch, where retail has it, and leaves the cast in `$f0` with
  the field in `$f1`. `effSwordTrackDraw` went from 162 of 175 words to 15 on
  `frame = work->params.object->first->frame;` ahead of the `if`.
- Externs of 8 bytes or less are addressed through `$gp`. Where retail uses
  `lui`/`%lo`, declare the symbol as an incomplete array (`extern s32 x[];`)
  to force absolute addressing.
  **The incomplete array goes too far for a constant read inside a loop.**
  `extern f64 D_00533E30[]` with `D_00533E30[0]` at the use makes cc1 treat
  the array base as loop invariant: it hoists the `%hi`/`%lo` pair of both
  doubles into callee-saved registers ahead of the loop and grows the frame by
  16. Retail materialises `lui %hi` plus `ld %lo` fresh at each use, which is
  what a plain scalar `extern f64 D_00533E30;` gives, since the symbol lives in
  `.rodata` and is never `$gp`-addressed anyway. That was 11 of the 119
  differing words in `dds3EffectDistortionBlurDraw`, the frame size and two
  saved registers. So reach for the array form only where the symbol really is
  small-data, and read the frame size before and after.
- `div` against `divu`, and `lb` against `lbu`, come from a cast at the use
  site, not from narrowing the field's declared type.
- A symbol that looks standalone may be one element of a table already
  declared. `check_unit.py` reporting "retail uses a different address" on a
  `%lo` that otherwise matches is the tell: `fldGetBgmTrack`'s five track
  globals are `gFldBgmTop[50]` through `[56]`, and only the `ninja` checksum
  catches it otherwise.
- A row stride that is not a power of two comes from subscripting a
  two-dimensional array. `char names[][28]` gives retail's `sll 3`, `subu`,
  `sll 2`, and `[][26]` gives `sll 1`, `addu`, `sll 2`, `addu`, `sll 1`.
- Reuse the loop index variable for a value clamped after the loops. Where
  retail's `movn` writes a saved register, a fresh local gives `movz` with the
  value already in `$2`.
- A store to a struct field aliases a global table of the same type, so
  reading `tbl[i].f` after the store forces a reload. A local holding the
  field is what matches.
- **A `u8` read is alias set 0 and serialises every store around it**, which
  is a scheduling fact before it is an addressing one. ee-gcc 2.96 gives
  character types the universal alias set, so one `source->color[0]` read
  conflicts with every store through an unrelated packet pointer, and a run of
  `lbu`/`sw` colour pairs can then no longer have anything scheduled into each
  load's latency: the pairs come out back to back with the float stores parked
  on one side of them. Retail interleaves one coordinate store into every
  load's latency and reuses one register for all four bytes, which is exactly
  what the same body emits when those four reads are `s32` fields instead, so
  the tell is reproducible: swap the byte reads for same-sized integer reads
  and see whether the schedule falls into place. A `u32 r : 8` bitfield does
  give `lbu` and does break the false dependence, but it swings past the
  target, since a bitfield read conflicts with neither the integer nor the
  float stores and cc1 then hoists all four loads into four registers ahead of
  every store. `dds3EffectFilterDrawTexQuad` sits on this at 22 of 110 words;
  see [Non-matching drafts](nonmatching.md).
- A colour read-modify-write whose new value comes out of a float-to-unsigned
  conversion needs the converted value in its own `u32` local. Written as one
  expression, `p->color = (p->color & 0xFFFFFF) | ((u32)(...) << 24)`, cc1
  hoists the `lw` of the old colour above the conversion's select and loses the
  branch-likely with it; `level = (u32)(...); p->color = (p->color & 0xFFFFFF)
  | (level << 24);` keeps the load inside the two arms where retail has it.
  `dds3EffectRippleBlurDraw` went from 43 differing words to 19 on that one
  local, where the reversed `or` operands and a `* 0x1000000` in place of the
  shift are 21 and 43.
- A float expression needed both before and after a call, where retail copies
  it with `mov.s`, is the **expression written twice**, not a named `f32`
  local. The local is live across the call, takes a second callee-saved float
  register and grows the frame by 16; written at both use sites, cse turns the
  second one into the copy. `dds3EffectRippleBlurDraw`'s `1.0f - slot->phase`
  feeds the displacement multiply and the alpha scale and needs this.
- A run of packet builders that each allocate a buffer, fill it and link it
  takes **one function-scope pointer for some of them and a block-scope
  pointer for the rest**, and which is which is worth sweeping mechanically.
  A shared local gets its own pseudo and a `daddu rd, $2, $0` copy; a
  block-scope one coalesces with `$2` and stores straight through it. Retail
  does both inside one function: `dds3EffectBlurDrawFrameMask` copies the first
  two packets into `$7` and stores the last two through `$2`, and all sixteen
  combinations of its four blocks were measured, with the right one exact and
  the next best 45 of 146 words. `dds3EffectBlur`, with three packet blocks,
  wants block scope for all three.
- The argument setup for a tail call that follows a counted loop gets hoisted
  above the loop's guard and lands in its `blez` delay slot, where retail fills
  that slot with the first load from inside the guarded block. Nothing in the
  source reaches it: eight loop spellings (`do`/`while` with the decrement
  before or inside the test, a `for` inside the guard, a bare `for`, a `for`
  with a `> 0` bound, a `while`, the induction pointer initialised in the `for`
  init, and separate bound and counter locals in both orders), and an early
  `return` in the skip arm carrying its own copy of the call, all leave it or
  make it far worse. `dds3EffectRippleBlurDraw` and
  `dds3EffectDistortionBlurDraw` are both capped by exactly this, at 18 of 122
  and 8 of 130 words.
- Where retail **reloads** a `void *` handle after a store made through a
  pointer derived from it, cast the store's lvalue to `void **`. ee-gcc 2.96
  gives each pointed-to type its own alias set, so a store to a
  `struct sdf3DLightParallelPacket_t *` field does not kill an available load
  of a `void *` field and cc1 keeps the handle in a register across it, one
  word short and in the wrong register. `*(void **)&obj->inner->light = light;`
  puts the store in the same alias set as the load and restores the reload.
  That was the last 6 words of `dds3ObjectBaseModelLinkOt`. The cast at the
  dereference is what matters, not the stored value's declared type: a `u32`
  local and a `u32 *` store do not reach it, while `(void **)` on the field's
  address, a `(void **)` subscript of the base and a `(void **)` cast of a
  `u8 *` sum are all exact.
- `for (i = 0, ofs = 0; ...)` against `ofs = 0;` above the loop decides which
  of the two variables is zeroed first.
- A row offset of `n - 1` folds into the load displacement on a two
  dimensional array but not on a flat index: `tbl[max - 1][r]` with
  `extern s32 tbl[][10]` gives retail's `lw -0x28(reg)`, where the same
  arithmetic as a single index keeps an `addiu -10`.
- Two `if`/`else` arms that read-modify-write the same kind of address get
  cross-jumped into one `ld`/`or`/`sd` tail, where retail has the tail twice.
  Put the byte offset of one arm in a `u32` local, `u32 ofs = 0x17F10 +
  (floor * 32 + room) * 8;`, and dereference `(u8 *)GLOBAL + ofs`. That is
  what changes gcc's association of the three-term sum from
  `(index + base) + const` to `(index + const) + base`, so the two arms end up
  with the pointer in different registers and the tail is no longer
  cross-jumpable. Parenthesising the constant with the index in the expression
  does not work: gcc reassociates every form tried.
  `fldAutoMapOpenBlock` went from 137 of 168 words to exact, and
  `fldAutoMapOpenBlock2` from 28 of 40, with the local in the `>= 30` arm
  only.
- A store through a cast pointer may alias a `gp`-addressed pointer global, so
  gcc reloads the global inside the inner loop. Cache it in a local `u8 *`
  first, once per loop where retail reloads it once per loop. That took
  `fldAutoMapClear` from 35 differing words to 6.
- An inner loop that walks up (`addiu ptr, 8`) while its counter walks down
  comes from `*p++` on a row pointer. An address derived from the index
  instead, `base + (i * 32 + j) * 8`, makes gcc reverse the loop, so the
  pointer descends from the last element and the row constant is the last
  element's offset.
- Where retail folds a constant offset into a symbol's `%hi`/`%lo` pair and
  then uses small displacements, the source holds a pointer to the sub-object:
  `p = (T *)&dds3LightParam2[0x20];`. Indexing the array at each use leaves the
  `0x20` in the store displacements instead.
- Storing `0.0f` to several float fields gives `sw $0` per store. Where retail
  materialises the zero once with `mtc1 $0` and stores it with `swc1`, which is
  one word more in total, assign it to an `f32` local first and store the local.
  Copying one field into the others works as well.
- An `s16` table field used once at `* 16` and once at `* 8` comes out as a
  second `lhu` plus `sll 16`, `sra 13` for the narrower scale. Read the field
  into an `s32` local and scale the local, which is what gives retail's single
  `lh` and two `sll`s.
- `s16 *t = table[index];` as an initializer at the declaration and
  `t = table[index];` as a statement after the function's leading calls differ
  by one saved register and by whether `index * 16` or the whole pointer is the
  value kept across the calls.
- Do not cache a value a call consumes in each arm. Written at the call site,
  `effColorBlend(work->color & 0xFFFFFF, work->color, blend)` is exact; the
  same value in a local spends a saved register and copies it into `$5`. That
  took one function from 42 differing words to 0.
- An uninitialised local passed as an argument is a real retail shape:
  `Create(work, NULL)` with `work` not yet assigned is how one register serves
  as both the argument and the result.
- A `Create` that reads its parameter block at offset 0 needs the parameter to
  have its own type, not the work type. Where the parameter store is not the
  first store, read it into a local first so the `lw` lands ahead of the
  stores.
- A local caching a `gp`-addressed global pointer (`evtViewer_t *viewer =
  viewerinfo;`) held across the whole body costs one pseudo and shifts every
  later allocation. Where retail re-reads the global, write `viewerinfo->`
  at every use and let gcc keep the one load: stores through the pointer
  invalidate the cached load by themselves, so retail's reload pattern falls
  out. Four functions in `event/evtEventViewer` turned on that alone, one of
  them from 58 differing words to exact.
- A loop bound used once belongs in the `for` condition, not in a named local.
  `for (i = 0; i < a->x + a->y; i++)` gives retail's `addu` into `$2` and a
  copy into the compare register; `bound = a->x + a->y;` coalesces the add
  into the compare register and loses two words.
- Where retail has `movn rd, rs, rs`, the cmov whose value and condition are
  the same register, write an explicit `if (x != 0) y = x; else y = <default>;`.
  The `y = <default>; if (x != 0) y = x;` form reads better but gcc sinks the
  default into the test and emits a branch pair; no ternary spelling of it
  reaches the cmov either.
- A string the retail pool keeps next to padding or an empty string is safer
  as `INCLUDE_RODATA` plus `extern char D_XXXXXXXX[];` than as a literal in
  the body. `include_rodata.py` picks up any rodata symbol named this way,
  including one splat had folded into a nonmatching function's own asm file.
- Two `addiu` of the same frame address where the C uses `&local` twice means
  the store before it is `SDF_VU0_STOREVECTOR_NOMEM`, not
  `SDF_VU0_STOREVECTOR`. The `"memory"` clobber lets cc1 keep the address
  pseudo across the asm, so a following `SDF_VU0_LOADVECTOR(&local)` reuses
  the register; without it cc1 rematerialises `$29 + offset` for every asm
  operand, which is what the `nbCameraShot_*_Set` family does throughout.
  `nbCameraShot_InvokeTargetNear_Set` went from 73 differing words to 4 on
  that one macro, and its neighbour from 82 to 11.
- An accumulate into a saved fp register (`add.s $f20, $f20, $f1`) needs
  `x += ...; x *= ...;` as separate statements. `x = (x + a) * b` puts the sum
  in the addend's register and adds a `mov.s` to the variable's register at
  the end.
- The alignment of a struct assignment comes from the assigned type, not from
  the object it sits in, so one set of bytes can need two struct types. In
  `slSaveData_writeFile` the save header inside the global work copies its
  20-byte block as `ldl`/`ldr` plus a plain `lw` for the trailing word
  (alignment 4, from a `struct { s32 w[5]; }` member), while the same 20 bytes
  in the on-card record copy as `ldl`/`ldr` plus `lwl`/`lwr` (alignment 1,
  from a `struct { s8 ch[20]; }` member). One struct for both gets one of the
  two copies wrong whichever way it is declared.
- A `$gp` reload landing in `$3` where retail uses `$2`, immediately after a
  call whose result is ignored, means that callee is `void` in retail's
  compile: a non-void declaration keeps the result pseudo alive and gcc skips
  `$2` for the reload. If turning the callee `void` then makes its own body
  sibcall its last call, block that with a dead store to the parameter rather
  than the `s32`-with-no-`return` form, which is what forced the callers to
  skip `$2` in the first place. `fclDestroyRag` and `fclDestroyRecover` went
  from two and four differing words to exact when `fclScriptForcedEnd` and
  `fclRecoverFreeTextures` became `void` with a `script = NULL;` dead store.
- A constant offset that retail leaves in the load displacement while
  hoisting only the bare symbol comes from a pointer local:
  `info = &tbl[i]; info->field`. Written as `tbl[i].field` gcc hoists
  `tbl + offsetof(field)` out of the loop and loads at `0(reg)`.
- Where retail shifts a run of parameters in argument order but stores the
  shifted values in some other order, the shifts are their own statements:
  `u0 <<= 4; v0 <<= 4; u1 <<= 4; v1 <<= 4;` above the stores. Written inline
  at the stores the shifts follow the store order instead. `fclSetSprite`
  came down to that plus the store list in retail's emitted order.
- Where retail recomputes the same `addiu rd, $29, N` or `addiu rd, base, N`
  for every VU0 access to one local, and the C holds it in a saved register
  instead, use `SDF_VU0_STOREVECTOR_NOMEM` for the store. The `"memory"`
  clobber on the plain store is what lets gcse share the address expression
  across the next call, and dropping it makes gcc rematerialise it the way
  retail does. Volatile asms keep their order either way, so a following
  `SDF_VU0_COPYVECTOR` or `_LOADVECTOR` of the same local still reads it.
  `nbCameraBoss_Sp_JAREIH_Run` lost a saved register and
  `nbCameraBoss_BS{KAGU,LUSI}_Command_Calc` each lost an `addiu` on that one
  change. Only use it where the C does not read the stored vector back by
  field, since gcc is then free to treat the local as unwritten.
- Two successive divides into the same destination come from reusing one
  float local, not from two named locals. `nbCameraShot_BossEX02_Set` writes
  `d = half / angle; pCamera->distance = d; d = f(party) / angle;`, which
  keeps both quotients in `$f0`; a second local moves the first one into a
  callee-saved register and rotates six words.
- Writing the derived value at its uses, `(gSlWindowCnt - 2) * 0xE9`, lets
  gcc distribute the multiply (`gSlWindowCnt * 0xE9 - 466`), two words more
  than retail's subtract and multiply. A local blocks that.
- The register the reused local gets follows its neighbours. gcc's
  `expand_preferences` unions the hard-register preferences of any two global
  pseudos joined by a single set whose source dies in it, so a counter
  multiplied into a value that is copied into an argument register is pulled
  into that argument register itself. Keeping the value that reaches the call
  out of the chain (here the scaled alpha is its own expression at the call,
  and the unscaled one shares the offset local that retail copies into `$4`)
  is what leaves the counter in `$4`.
- Where retail folds a float constant's register into the result of the
  expression that consumes it (`lwc1 $f20, 3.14f`, then
  `mul.s $f20, $f0, $f20`, then `div.s $f20, $f20, $f1`), the source assigns
  the constant to the destination variable first and then rewrites it in
  separate statements: `angle = 3.14f; angle = -x * angle;
  angle = angle / 180.0f;`. Any one-expression spelling gives the constant a
  temp and the quotient a different saved register.
- A second loop-invariant float constant used across the same calls needs its
  own named local, assigned right after the first one, or it takes the lower
  saved register. `calcCamNormalNPC` went from 16 differing words to 6 on
  `dist = 200.0f;` in place of the literal at its four use sites, and to exact
  once the angle was written as the three statements above.

- A jump table needs enough `case` **nodes**, but the threshold is not 10: an
  eight-entry table and a six-entry one both emit tables here, so the earlier
  claim of `case_values_threshold == 10` is wrong as stated. The actionable
  mechanism is that the node count is taken **before** empty arms are merged.
  Where retail has a table and the reconstruction has a compare tree, give
  each extra arm a dead store to a local nothing reads after the switch,
  `case 8: seq = 0; break;`. Flow analysis then deletes every store and those
  entries land on the shared exit. One function went from 59 of 79 words
  differing to 1 on that alone, where a live assignment leaves the block in
  and runs 16 bytes over. Confirmed again at seven entries and at six, and the
  node count is still not the whole story: `basic/dds3ObjectBase` has four
  `switch`es over the same `u32` field with the same case values, and the three
  in `dds3ObjectBaseModelLinkOt` (cases 0 to 6 twice, 0 to 5 once) emit tables
  while `dds3ObjectBaseMatterAnimeSpeed`'s cases 0 to 6, with two stores in
  every arm and no empty arm at all, emits a compare tree and matches that way.
  So read which one retail has rather than predicting it from the arm count.
- A float argument read from a global that the same body also stores: with
  **any** other use of the loaded value, cse rewrites the argument setup into
  `mov.s $f12, pseudo` and no source shape removes that word. Where retail has
  a bare `lwc1 $f12, 0(reg)` feeding both the argument and a run of `swc1`
  stores of the same value, the source writes **every store as the literal and
  the argument as the literal too**. `dds3CrossfadeStart` zeroes six floats
  across two vectors and passes the aspect to `dds3AllocRenderTexture`: the
  natural `D_002F5C20[0] = 0.0f;` then `D_002F5C20[1] = D_002F5C20[0];` run was
  46 of 58 words out and two words long, and all six written `= 0.0f` with the
  argument `0.0f` is exact. The first store stays `sw $0` (the constant class
  holds no float register yet) and cse then feeds the remaining five from the
  argument's own load. Ruled out on the copy: all 120 store orders, an `f32`
  local for the value, separate locals for the stores and the argument, a
  `(f32)` cast and a `*(f32 *)&` deref at the argument, an `f32`/`void`/
  four-argument callee declaration, the aspect as the first parameter, an
  `*(s32 *)` cast on the first store (42 words), and reading the argument from
  the last slot stored. One use of the loaded value is the only case that keeps
  the direct load.
- Two callback sites through `sdfGrOtTable` want a `sdfGrOt_t *ot` declared in
  each site's own braced block. One function-scope local for both leaves gcc
  holding `%hi` in one register and adding `%lo + offset` into another
  (`lui $2` then `addiu $4, $2, 0x40`), where retail's single `la` pair writes
  one register; `PS2_SYMBOL_AT` on the element does not reach it, since the
  assembler folds the alias back into the table symbol anyway. That was the
  last four words of `dds3CrossfadeRun`, and it is the two-site reading of the
  `effSwordTrackDraw` row above.
- A `u32` global store placed among a run of float stores decides the register
  the constant takes and whether its own store lands before or after the first
  `swc1`. `dds3CrossfadeStart` wants `dds3RenderDrawEnv = 0x80808080;` written
  after the six float zeros and before the call; the same statement anywhere
  earlier rotates the two symbol-address registers against the constant's.
- The "constant assigned in both arms" row reaches a **compare operand**, not
  only an assigned value. Where an `if`/`else` selects which byte to compare
  and retail materialises the compared character twice, give it a named local
  and assign it in each arm: `period = '.';` alongside the byte read in both
  arms of `dds3EffectFileListCreate`'s extension-dot test took it from ten
  differing words to six, where the whole `if (... != '.') { continue; }`
  written in each arm instead runs four words long.
- Two **byte-identical** `if`/`else` arms are a reachable retail shape here.
  `dds3EffectFileListCreate` allocates `strlen(path) + 1` and `strcpy`s into it
  in both arms of `if (sdfHddValid != 0)`, and ee-gcc 2.96 keeps both copies
  from an `if`/`else` whose arms are the same statements. That is the opposite
  of the `func_00214FD8` row below, so measure rather than assuming either.
- `extern f32 fabsf(f32);` and an ordinary call compile to `abs.s` directly,
  verified against this toolchain. `EE_ABS_F32` is what fails to match where
  retail has a plain `abs.s`. **But the choice is also a scheduling lever, and
  it runs the other way in a VU0 body**: where retail materialises the other
  operand's constant *after* the `abs.s`, `fabsf` lets sched1 hoist the
  `li.s` into the `mtc1`-to-`abs.s` latency and `EE_ABS_F32` does not, because
  its inline asm is a barrier. `dds3BillboardGetDirectionScale`'s
  `1.0f - fabs(dot)` sat at 3 of 26 words with the call and every placement of
  the constant (its own local before or after the `abs`, a reused local, a
  result local) and went exact with `EE_ABS_F32(dot, dot)`. Confirmed again
  outside a VU0 body, on a run of magnitude tests: `evtCommand_MODEL_LIGHT_DIR`
  compares `fabs` of three vector components against one constant, and with
  `fabsf` the constant and the retained third component take each other's
  float registers and the constant's `lwc1` lands after the component copy,
  8 of 53 words out under every shape tried (an `&&` chain, nested `if`s,
  `goto`s, a named bound local at four positions, an `f32` local per
  component, `sdfVector4_t` for the array, and `_MEMOPS` for the `lqc2`).
  `EE_ABS_F32` into **one reused** magnitude local, with the three tests
  nested, is exact: the barrier is what forces the constant to be materialised
  before the copy. Confirmed a third time on a plain scalar body with no VU0 in
  it: `effPCPHamaCardScatterDraw` compares `tiltHalfAngle * 0.5f` against the
  magnitude of the new tilt angle, and retail materialises the `0.5f` with
  `lui`/`mtc1` *before* every load in the block. `fabsf` is 12 of 343 words
  there, with the constant hoisted into the load shadow and the abs result and
  the height sum in each other's registers; `EE_ABS_F32` into its own `f32`
  local is exact. So read which side of the `abs.s` the other operand's
  constant sits on and pick the macro from that.
- The same thing happens by accident when a `void` callee defined later in the
  unit has no prototype above the caller: the implicit declaration returns
  `int`, so the load after the `jal` lands in `$3` where retail uses `$2`.
  Adding the `void` prototype above the caller is the fix. `SetMakaPacket`
  came down to that one word, on `nbSetJyokyoMes`.
- Where retail computes a table row as `addu rd, index, base` *into the
  register the index already occupies*, the index has to reach the add as a
  pointer: `(T *)((u8 *)(i * sizeof(T)) + (u32)base)`. Written `base[i]` gcc
  emits `addu rd, base, index` into a fresh temp, which costs the operand
  order and the register. `nbRunKoukaType1` and `nbRunKoukaType2` each went
  from 20 differing words to exact on that one cast.
- Stack vectors a function stores with `sqc2` and later copies with `lq`/`sq`
  have to be local *aggregates*, not scalar `u128`s, when retail's frame layout
  follows the declaration order. gcc gives an aggregate its slot at
  `expand_decl`, in declaration order, but gives a scalar its slot only when its
  address is first taken, so a run of `u128` locals comes out in first-use order
  instead. `effect/effPCPBoss` declares
  `union effPCPBossVector_t { u128 q; f32 f[4]; }` for this: the `u128` member
  supplies the 16-byte alignment `sqc2` needs and the union makes it an
  aggregate. `effPCPBossDengekiDraw`'s ten vectors land at `0x30` through `0xC0`
  only in that form.
- A `SDF_VU0_STOREVECTOR` to a local that a later `SDF_VU0_COPYVECTOR` reads as
  its source has to be `SDF_VU0_STOREVECTOR_NOMEM`. The plain form's `"memory"`
  clobber lets gcse share the `addiu rd, $29, N` between the store and the copy,
  which is one instruction fewer than retail and parks the address in a saved
  register across the intervening call. That took `effPCPBossDengekiDraw` from
  182 differing words to 16. It is needed even where the C reads the stored
  vector back by field, against the caution on the macro itself.
- The `effEveBEDCutSetParams` placement block in the draw routines is written
  zeros first: `payload[3..6] = 0.0f`, then `payload[7]`, `[8]`, `[9]`, `[10]`,
  then `payload[0..2]` from the position, then `color`. Every order that follows
  the field numbers leaves the last `swc1` on the wrong side of the three
  `sw $0`, and no position for `payload[8]` alone fixes it:
  `effPCPBossDengekiDraw` and `effPCPBossKAGUThunderboltDrawSub` both sat at 4
  and 8 differing words until the zeros moved to the front, then went exact.
- An interpolation factor that retail builds with a bare `lui`/`ori` pair is a
  `u32` local holding the binary32 bits, assigned just before the call that
  precedes the loop, and passed to `SDF_VU0_INTERPOLATE`. As a literal inside
  the loop body gcc hoists the materialisation into the preheader and rotates
  every saved-register assignment; `PS2_FLOAT_BITS` is wrong here because it
  adds the `mtc1`/`mfc1` pair retail does not have.
- A local that gets spilled competes for slot order with the others. Where
  retail gives the lower spill slot to a value read in the prologue, declare the
  loop-local one (a `found` pointer, say) after every other local;
  `effPCPBossMETATOBeamDraw` swaps its two spill slots otherwise.
- A call that sets only `$4` where the callee really takes three parameters is
  an unprototyped declaration in the caller's unit, not a wrong signature on the
  callee. `fldAddressProcess2` calls `fldDrawAddressPanel(0x80)` and
  `fldDrawAreaBlockFont(0)`, which `field/fld_Npc` defines as
  `(s32, s32, s32)` and `field/fldAutoMap` as `(void)`; `extern void
  fldDrawAddressPanel();` plus the one-argument call in `field/fldPanel` is
  exact and leaves both definitions alone.
- `Hatudo` and `nbE915_Test` call `nbGetEventForm` with no argument, which
  only an unprototyped declaration produces, and that function is defined
  with a prototype in the same unit. An unprototyped `extern` does work for
  callers placed above the definition, which matters if the unit is split.
- Where retail updates a base coordinate in place, `addiu $19, $19, -2`, and
  you fold the constant into every use instead, write the adjustment as its
  own statement on the same variable: `x = (s32)(...); x -= 2;`, not
  `x = (s32)(...) - 2;`. In the one-expression form the unadjusted value stays
  live and cc1 folds the offset into the first use, so the base is
  materialised and bypassed at once. That fixed the whole prologue of
  `fldVLDraw`.
- Locals that each `switch` arm assigns and uses for itself belong inside the
  arm's own block, not at function scope. At function scope the arms share one
  pair of allocnos whose live range spans the switch, which rotates them
  against whatever else is live across it: `fldVLDraw`'s seven sprite arms put
  the alpha value in the register retail gives the coordinates and were 45 of
  370 words out, and went exact with `s32 sx; s32 sy;` declared in each arm.
- A block-scope aggregate initializer is not the only way to reach a rodata
  table the unit already carries. Declare the splat symbol with the real type
  and assign it, `axis = D_00531148;` with `extern sdfVector4_t D_00531148;`,
  which gives retail's `ldl`/`ldr` plus `sdl`/`sdr` copy from `%hi`/`%lo` of
  the symbol and leaves the `INCLUDE_RODATA` line and the pool order alone.
  An initializer at the declaration instead emits the copy in the prologue,
  before anything else in the body, and moves the data into the compiler's own
  rodata, which is what the function-local-array section below is about.
- Where retail forms a symbol's address and then adds a constant in a separate
  `addiu`, put the bare symbol in a pointer local inside the block that uses
  it and add the offset at the use: `char *roof = D_002FBD40;` then
  `roof + 0x30`. Written `&D_002FBD40[0x30]` gcc folds the offset into the
  `%lo` and is one word short. `fldElvGetFloorDigits` went from 19 differing
  words to 8 on that alone.
- A quotient and a remainder of the same division belong in one reused local,
  not two. `digit = n / 10; ... digit = n % 10;` gives retail's `mflo`/`mfhi`
  register pair and keeps the second divide-by-zero trap at the `%`
  statement's position; two named locals rotate four registers, and writing
  either expression twice adds a third trap.
- A `sw $0` that sits in the wrong place *among* a run of other `sw $0` stores
  is the same tell read the other way: the field is `f32`, not `s32`. In
  `fldClearEveRngTable` the zero store to `fldRoomState_t + 0x114` came out
  four words early, ahead of the two integer zeros next to it, and no ordering
  of the thirteen statements in that tail moved it; declaring the field `f32`
  and writing `0.0f` put it exactly where retail has it and took the function
  from five differing words to exact.
- An `andi 0xffff` after the `andi` of a bit test comes from reading a `u16`
  struct field directly in the condition. Read it into an `s32` local first
  and the truncation goes away; `SetBattleFlag` turned on that alone. Retail
  does emit the pair where the field really is tested in place
  (`nbGetUnitWorkPointer` and `CheckErrorActionStack` both have it), so the
  tell is which side has the extra word, not the shape.
- Two bits cleared in one mask come out as one `addiu`/`and`; retail's two
  `addiu`/`and` pairs with a single store come from two separate statements,
  `x &= ~0x10; x &= ~0x80;`. gcc merges the dead store but not the masks. One
  function in `newbattle/nbMainProcess` turned on that alone.
- Two reads of the same pointer field in one function, where retail puts them
  in different registers, need two distinct locals. Reassigning one local for
  the second read keeps retail's reload but not its register.
- A placeholder field declared `u8 unk_NN[4]` in a shared header decays to its
  own address, so `if (data->unk118 == 0)` compiles to a compare against a
  constant and silently tests nothing. Read it through `*(s32 *)data->unk118`
  until the field has a type.
- Where retail loads two function pointers out of the same kind of struct and
  calls each, into the *same* register, put the pointer in one local and reuse
  it: `reg = ot->regFunc; reg(ot, tag); ot = ...; reg = ot->regFunc;
  reg(ot, tag2);`. Calling `ot->regFunc(ot, tag)` directly puts the second
  load in `$3`, since `$2` is still the first call's result register.
  `nbUnitDrawProcess1` went from two differing words to exact. This is the
  reused-destination form of the plain-copy rule above.
- A callee defined in the same unit can take a parameter it never reads, and
  still match: `nbGetUnitMarkPos` has to be declared with three parameters
  because `nbUnitDrawProcess1` passes a draw tag it ignores. An unused
  parameter costs no instructions, so adding one is free on the callee and is
  what lets the caller compile at all.
- Which of `beq`/`beql`, `bne`/`bnel` and `bgez`/`bgezl` ee-gcc picks is the
  delay-slot filler deciding whether the slot's instruction is safe on the
  untaken path, and it is not reachable from the branch's own source shape.
  Three functions here (`nbRunAi`, `nbE901Process_1`, `nbE905_End`) sit one
  word from exact on that bit alone after every spelling of the branch,
  the loop and the callee's declaration. Restructuring the block the slot
  instruction belongs to is what moves it, as in the `NegoDebug` row above;
  varying the branch does not.
- One float local reused for two unrelated values is how retail gets both into
  one register, and it is the commonest residual in the `nbCameraShot_*_Set2`
  family. Where retail's atan result and a later negated distance share `$f3`,
  or the `0.5f` and the quotient that replaces it share `$f21`, the source has
  one local doing both jobs: `scale = sdfAtan(...); radius /= scale; ...
  scale = -radius; neg.x = scale;`. Two named locals leave the second value one
  register higher and every later assignment rotates.
  `nbCameraShot_Target_Set2` sat at 2 of 178 words on exactly that and went
  exact; `nbCameraShot_All_Set2`'s accumulator, its `0.5f` and its
  `SDF_VU0_GETLENGTH` output are all one local for the same reason.
- `SDF_VU0_STOREVECTOR` against `_NOMEM` cuts both ways within one function,
  so sweep the pair mechanically rather than reasoning about it. In
  `nbCameraShot_Target_Set2` the *first* store of a stack vector has to be
  `_NOMEM` so that a later store of the same vector rematerialises
  `addiu $2, $29, N` instead of reusing the saved register the load got: the
  plain form there was 47 of 177 words out and `_NOMEM` took it to 2. In
- Where a global's value lives in one register across a whole loop and is
  reloaded only after a call, nothing else in the loop may clobber memory.
  `dds3SceneLightUpdateFirst` keeps `dds3DrawFlag` in `$7` for three
  iterations and reloads it only after `Lerp` and `ToMatrix`, so both of its
  colour unpacks are `MMI_RGBA_UNPACK_NOMEM` and both vector stores are
  `SDF_VU0_STOREVECTOR_NOMEM`; a `"memory"` clobber on any one of them adds a
  `lw` reload at every flag clear and at the loop top, and took the function
  from 145 of 383 matching words to 21. Read whether retail reloads the global
  after each asm block before choosing the macro, since that reload is the
  clobber's visible signature.
  `nbCameraShot_AttackTuibiReverse_Set` the plain form's `"memory"` clobber on
  the second of two stores let cse keep a `0.5f` in a callee-saved register
  across the intervening call, collapsing retail's two `lui`/`mtc1` pairs into
  one; `_NOMEM` on that store restored them and the function went from 48 of
  151 words to exact. Nine combinations of the two macros took a second to
  measure.
- A float constant that retail materialises with `lui`/`mtc1` *after* the
  multiply whose operand register it reuses comes from one reused float local
  holding both values: `t = p->scl; radius = width * t; t = 100.0f;
  if (t < radius) { t = t / radius; radius *= t; }`. As a literal the constant
  is its own pseudo, sched1 fills the operand load's latency with the
  `lui`/`mtc1` ahead of the multiply, and the overlapping live range then
  costs it the multiply's register. The write to the shared local is what
  pins the order. `nbCameraShot_InvokeTarget_Set2` and
  `nbCameraShot_InvokeTargetReverse_Set2` each went from 5 differing words to
  exact on that one local.
- A run of locals whose initializers are constants materialised in the prologue
  takes its registers from the *declaration* order, and that order is a real
  lever. `fldAutoMapDrawBgOfsRF` passes eight constant texture coordinates to
  `sdfPrimLinkFt4`, each in its own callee-saved register; declaring them
  `au, av, du, dv, bu, bv, cu, cv` (the fourth corner's pair moved ahead of the
  second and third) took it from 10 differing words to exact, where all 24
  orders of the pairs and all 16 u/v swaps inside them were swept to find it.
  The same permutation in its sibling `fldAutoMapDrawIconOfsRF` is *not*
  reachable this way, because there the four values are computed from a table
  mid-function rather than initialized: declaration order and assignment order
  both have no effect at all on them.
- Where a function fills several stack vectors and then transforms each through
  VU0, two orders are separate levers and both are worth sweeping: the order the
  vectors are filled in, and the order of the field assignments within each one.
  `fldAutoMapDrawBgOfsRF` and `fldAutoMapDrawIconOfsRF` both want the four
  vectors filled `v0`, `v3`, `v1`, `v2` (8 words each over the natural order),
  and both want `.z` written before `.x` in the *second* vector only and `.x`
  first in the other three (14 words). Nothing about the asm suggests either;
  24 and 16 variants are cheap to measure.
- An inner loop whose counter walks down while its pointer walks up, with the
  outer base recomputed from the index rather than strength-reduced, comes from
  a pointer local whose base is formed `(u8 *)(i * stride) + (u32)base` and whose
  constant offset is then added in a *separate statement*. `fldAutoMapClear`
  needed `u64 *row = (u64 *)((u8 *)(i * 256) + (u32)gb); row = (u64 *)((u8 *)row
  + 0xFC58);` exactly: with the constant inside the cast sum, or with the base
  written `gb + 0xFC58 + i * 256`, ee-gcc strength-reduces the outer base and
  drops the `sll` retail keeps (two words short); with the address derived from
  the index at the store it reverses the inner loop instead (two words, the
  wrong direction). Subscripting the row pointer by the constant
  (`row[0x1F8B]`) keeps the direction but spends a `lui`/`addu` pair on the
  displacement.
- Two subtractions of 1 from two different parameters, where retail evaluates
  one before a `div` and the other after the divide-by-zero trap, *and* keeps
  the first one's result out of its own parameter register. The two are coupled
  and C cannot separate them: the `beql`/`break` trap is one insn pattern
  rather than a basic-block boundary, so the scheduler emits whichever
  subtract is evaluated first in RTL ahead of the `div`, and that same first
  subtract is the one that coalesces with its parameter's register. A comma
  expression proves the rule is RTL evaluation order and not statement order.
  `fldAutoMapCloseBlock2` sits two words from exact on it; see
  [Non-matching drafts](nonmatching.md).
- A constant that retail adds to the *scaled index* rather than to the base
  (`addiu rd, index, 0xC0` and then one `addu`) comes from an integer offset
  local: `s32 ofs = index * stride + 0xC0;` and a cast pointer off the work
  base. `&work->arr[index]` associates the other way, `(work + index*stride) +
  0xC0`, and leaves the `0xC0` in the load displacements.
  `cmpStockHasRoom` went from 17 of 21 words to exact on that alone. Its work
  type is two arrays indexed by one parameter, `cmpLocalStock_t stock[2]` of
  stride 0x56 at 0 and a 0x103C-byte record at 0xC0, which is the same pair
  `cmpGetStockInfoIndex` reads.
- A constant byte offset into a global table folds into the symbol's
  `%hi`/`%lo` pair only when the source names a flat array at that address.
  `extern s8 D_00339E06[]` indexed `race * 66 + index` gives retail's
  `lb $2, %lo(D_00339E06)($1)`; the same bytes reached as
  `D_00339E00[race].field[index]` with the row struct split keep the `+6` in
  the load displacement and cost four words per access.
  `cmbGetHeartsParamByIndex` went from 12 of 22 words to 8 on that.
- A `lui`/`addu`/load triple using `$at` is `ee-as` expanding `lw rd, sym(rx)`,
  not cc1 splitting the address. The assembler uses `rd` itself as the temp
  whenever `rd != rx`, so a triple through `$at` means retail's index and load
  destination are the *same* register. Read it as a register-allocation tell,
  not an addressing one: `trmGetJumpTerminalNo` and
  `cmbGetHeartsParamByIndex` both sit on it.
- An index whose stride is 4 on what the struct declares as a byte array means
  the record is four bytes wide. Read it with a cast at the use site,
  `*(s8 *)((u8 *)WORK + sel * 4 + 0x4110)`, rather than reshaping a field other
  functions already use: `cmbGetBirthDevilPanel` was 19 of 19 words out with
  `slotIndex[sel]` and exact with the cast.
- A `u8` parameter used as a loop counter has to be copied into an `s32` local.
  Decrementing the `u8` itself emits `addiu rd, rs, 0xFF` plus `andi 0xFF` in
  every arm where retail has one `addiu -1`; the entry `andi 0xFF` the `u8`
  parameter gives is still correct. `cmbCountEvoCandidates` turned on that.
- Where retail copies a loop bound into a second register (`daddu $3, $2, $0`)
  before the loop, the source has a separate counter local initialised from the
  bound and decrements the local, leaving the bound untouched. Reusing the one
  variable gives no copy. `func_00242710` turned on that plus a `u8 *` cast for
  the `lbu` of an `s8` field.
- A `gp`-addressed work pointer that retail holds in two registers comes from a
  local assigned from the global plus the global written at the other uses, and
  which half gets the local matters: `rstUpdateEvoBgFade` wants the local for
  the first test and the store, and `startPos->` for the clamp.
- An `f32` field that retail keeps in `$f0` across a read-modify-write is
  written in place, `work->ofs -= 16.0f;`, not through a named `f32` local. The
  local takes `$f1` and leaves `$f0` to the literal, and no declaration order
  of the value, the step and the bound swaps them back.
  `MoveScreenOffset` went from 9 of 22 words to exact. A local declared inside
  each arm works too.
- `fsqrtf` from `include/fpu.h` is what produces a bare `sqrt.s $f0, $f0`, and
  ee-gcc does not fold it even on a constant: `fclRagCalcTexSize` really does
  compute `sqrtf((f32)(size * 16))` at run time, and
  `campGetCursorColorAlt` computes `(s32)fsqrtf(512.0f)` and squares it.
  A `mult`/`mult1` pair falls out of the two multiplies with no help.
- A global pointer read inside a loop beats caching it. `g_pinst->texHandle[i]
  = src[i]` gives retail's one `lw` in the preheader, the `addiu 0x38`, two
  walking pointers and an `slti` up-counter; an `s32 *dst` local for the
  destination makes gcc reverse the loop into a downcounter, and `dst[i] =
  src[i]` with the local adds a third induction variable.
- An epilogue duplicated into a branch-likely delay slot, where ee-gcc emits
  the plain branch. **Steerable when the branch is a sign test**: spell it
  `(x & 0x80000000) != 0` rather than `x < 0`, which took one function from 8
  differing words to exact. Only the non-sign-test cases have resisted. Note the narrow scope: ee-gcc 2.96 emits `bnel` from
  ordinary C readily, 42 times in `field/fldDraw` and 126 in
  `newbattle/nbMakePacket`, so a `bnel` in the retail code is not a reason to
  skip a function. Only this one use of it has resisted.
- Two arms of an `if` holding the *same* call is a real retail shape, not only a
  cross-jumping artifact. `RunBadMotKaifukuKouka` has two `jal nbSetUnitMotion`
  with identical argument setup hoisted above the branch, and an `if`/`else`
  whose arms are the same statement reproduces it; the condition has to be
  spelled `== 0` rather than inverted, which is what decides the `bnez`/`beqz`
  order. The `func_00214FD8` row below is the case where gcc collapses them, so
  measure rather than assuming either way.
- Two arms of an `if` that compute the same thing. gcc 2.96 cross-jumps them
  whatever the source does: `func_00214FD8` has the same sign test on
  `dds3PadButtonState[0x22]` in both arms, laid out differently in each, and
  every spelling (duplicated test, `>= 0` against `< 0`, the mask form, a
  ternary, an explicit `else`, and even *two different symbols* that resolve to
  the same address) collapses to one arm and 36 of retail's 56 bytes.
- A conditional move whose two arms are the same value. Retail's
  `daddu rd, rs, $0` / `xor` / `movn rd, rt, rs` selecting between a field and
  the old value of the global it is stored into is not reachable: combine's
  `simplify_if_then_else` rewrites `(ne A B) ? A : B` to `A`, and cse reaches it
  through an explicit `xor` too, so every spelling folds to one `sw`. A `u64`
  local keeps the cmov but pays `lwu` and two `dsll32`/`dsra32` pairs for it.
  `movieWaitSema` in `sdf/sdfMovie` sits on this; see
  [Non-matching drafts](nonmatching.md).
- A single-bit mask test ee-gcc folds. Retail's `andi 1` followed by a
  redundant `andi 0xFF` and an explicit branch is not reachable: see
  `cmpItemHasFlag` in the drafts.
- Where in a tail call's argument setup the second half of a `lui`/`ori`
  constant lands. ee-gcc's second scheduling pass moves the `ori` past the
  register restores; `cmp_23A918` and `fclDrawPanel` both sit on it and no
  spelling of the constant (a local, an unsigned literal, dropping the
  `return`) moves it.
- A negated comparison assigned to a variable needs the comparison in its own
  local first. `ok = (prev >= 0x13);` and every spelling of it (`!(prev <
  0x13)`, `== 0`, `!= 1`, `? 0 : 1`, `> 0x12`) gives `sltiu` then
  `xori rd, rs, 1`, because gcc knows the inner value is 0 or 1. Retail's
  second `sltiu rd, rs, 1` is the generic `== 0` expansion, which needs
  `t = prev < 0x13; ok = t == 0;` as two statements. That was the last word of
  `nbActionProcess1`. It holds across a call too: in `Flow11_3` retail keeps
  the `slt` in `$16` over a `jal` and applies the `sltiu` after it, which
  needs `alive = drain < hp;` before the call and `dead = !alive;` after it.
  The two locals coalesce into one callee-saved register, so the second name
  costs nothing.
- `case_values_threshold` is 10 in this build: a `switch` needs at least ten
  `case` nodes before ee-gcc emits a jump table rather than a compare tree,
  independently of the range test. Where retail has a jump table and the
  reconstruction has fewer labels, the missing ones share the `default` body.
- An address that looks like a global may be a `.lit4` pool entry holding a
  float constant, and must be written as a literal.
  `tools/check_unit.py` says so when you get it wrong. This also empties a
  branch delay slot: as an `extern` the `lwc1` gets scheduled into it, while
  the literal becomes a `li.s` macro that `ee-as` cannot place there, which
  is how retail's `nop` appears.
- Holding a struct address in a local (`nbCamera_t *p = &pNbCamera;`)
  reproduces retail keeping it in a saved register. Placement matters:
  initialising it at the declaration puts an `addiu` in the prologue and
  spends a saved register, while declaring it and assigning just before the
  first use does not. Assigning it after a leading call keeps it out of `$16`.
- The cheapest sibling-call blocker measured is `s32 dummy; s32 *p = &dummy;`,
  which costs nothing at all: the frame still reports `vars= 0` and no extra
  instruction is emitted. `volatile`, an array and a store each grow the frame
  by 16, while an old-style declaration, a varargs callee and a mismatched
  return signedness do not block it at all. It blocks **every** sibling call in
  the function, so it is the wrong tool where retail sibcalls some calls and not
  others: `dds3ObjectBaseModelLinkOt` has two `j callee` tail calls on early
  return paths and a `jal` plus epilogue on the last one, and the dummy pointer
  took it from 20 of 242 differing words to 207 of 236. A dead store after the
  one call that must not sibcall (`ot = NULL;` on an unused parameter) blocks
  only that one and took it to 6. Any dead store reaches it; three different
  variables gave the same result.
- Blocking the sibling call where retail uses `jal` and a full epilogue has
  two other forms, and which one works varies by unit. Giving a void function an
  `s32` return type with no `return` statement collapses a duplicated epilogue
  too, and took one function from ten differing words to exact. It failed for
  all seven functions tried in `code_0029B780`, where the answer was a dead
  store instead: introduce a local for one of an earlier call's constant
  arguments and assign 0 to it after the last call. Try both.
- An ignored return value still reserves `$2` across the call. When retail
  skips `$2` for the address temp after a `jal`, declare the callee `s32`
  rather than `void`.
- The `void`-callee remedy for an unused result reserving `$2` is not always
  available. In `code_001FAE80`, `fclDrawPanelSprite` is defined `s32` in the
  same unit, matches that way with `return cmpDrawSprite(...)`, and three other
  callers ignore its result, so changing it has to be measured across all of
  them at once rather than for the one caller in hand.
- An implicit declaration is itself a void against non-void trap. A callee
  defined further down the unit is implicitly `int` at the call site and
  reserves `$2`, so a forward prototype alone can fix a near miss: one added
  prototype took four functions from two words off to exact. When a `$gp`
  reload lands in the wrong register, check whether the callee is merely
  implicitly declared before concluding its return type is wrong.
- A local whose only initializer is a constant gets that constant materialised
  in the *prologue*, which costs a prologue instruction and rotates every
  saved register. Where retail sets it next to the code that first needs it,
  declare the local bare and assign it as a statement there.
  `frSetFontString` went from 72 of 115 words to exact on moving
  `glyph = NULL;` out of the declaration and down beside `i = 0;` and
  `previous = chain->lastChild;`, in that order. Declaration order is *not*
  the lever here: all 24 permutations of its four locals emit byte-identical
  code, and only the statement's position moves anything.
- An index read and then advanced reaches `addu base, i` plus `addiu i, 1`
  into one register only as a post-increment in a single expression,
  `text[(*offset)++]`. Written as two statements, the read and
  `(*offset)++`, gcc keeps the old and the new value in two registers.
  `frAnalyzeMessage` went from 20 differing words to 10 and `frAnalyzeTag`
  gained its whole prologue on that one change.
- One variable whose comparisons are signed (`slti`, `slt`) but whose byte
  extraction is `srl` needs the cast at that one use, `(u32)code >> 8`. An
  unsigned declaration fixes the shift and breaks both compares.
- A guard arm that is a single instruction belongs in the `else`, not the
  `then`: the delay-slot filler then absorbs it into the branch and the `b`
  over it disappears. `frPreCalcStrWidth` went from 32 differing words and one
  word long to 8 at the exact size by inverting
  `if (code >= 0x80) { <long> } else { index = code - 0x20; }` to
  `if (code < 0x80) { index = code - 0x20; } else { <long> }`, and
  `frCopyTagArgs` turned on the same inversion twice, once per nested test.
- Subscript the whole expression instead of caching an element address.
  `e->data->form[i].stat` matches where `form = &e->data->form[i];` then
  `form->stat` swaps the `addu` operands and shifts a register. The converse
  holds inside a loop retail re-reads: go through the outer pointer each
  iteration, since a cached local gets hoisted out and costs an instruction.
- `addu` operand order follows the cast order: `(u8 *)ofs + (u32)base` emits
  `addu rd, ofs, base`, and `(u8 *)base + ofs` emits the reverse. The same
  distinction separates pointer from integer arithmetic: `base + i` emits
  `addu base, i`, and retail's `addu i, base` needs the base cast to an
  integer first.
- A `gp`-addressed global can be cached in a local for part of a function.
  Where retail reloads it once for a loop and once for the tail, use the
  global inside the loop and a local pointer for the tail.
- `andi 0xff` after a `jal` comes from declaring the callee `s32` and casting
  at the use site, `(u8)f()`, not from giving it a `u8` return type.
- When retail biases a base once and stores at `0(reg)`, the induction
  variable is a pointer to the field, not to the struct. A struct pointer with
  a constant displacement gives `sw $0, 0xD0(reg)` instead.
- An induction pointer can be based one byte past its record: reading the
  weight at `p[-1]` and the item at `p[0]` matched where walking a
  `{rate; item}` struct lost 20 words.
- Split a packed argument into named locals in retail's computation order,
  `type = joken >> 16; para = joken & 0xFFFF;`, rather than using the
  expressions at their use sites. The parameter has to be `u32`, not an `s32`
  cast where it is read.
- A local assigned before the first call lands in a callee-saved register.
  Moving `sum = 0;` above a leading `jal` shifts every later allocation into
  place.
- A struct field read whose live range crosses a `jal` is a **named local
  assigned in front of that call**, not the field expression at its use site.
  `lv = n->ework->level; if (datGetPlayerLevel() < lv)` is what gives retail
  the extra callee-saved register; the inline form loads the field after the
  call, leaves the frame one register short and shifts every callee-saved
  number down by one.
- A weighted-table scan compares before it accumulates: `if (r < sum + *p)
  return i; sum += *p;` gives retail's temp, compare, then copy. Accumulating
  first misses by several words.
- Pack a colour word through explicit `s32` locals assigned in `r`, `g`, `b`
  order. The same expression with the `(s32)(f * 128.0f)` conversions inline
  gives the right registers and `or` chain but a different mul and cvt
  schedule.
- A `u16` or `u8` parameter adds an `andi` on entry that retail does not
  have. Where retail copies the argument with `daddu`, declare the parameter
  `s32` and let the caller narrow: `fclEncycSearch`, `SetRecordToUnit` and
  `RequestModelLoad` each came down to that one word.
- A run of adjacent constant stores comes out right-rotated by one, so the
  source order is retail's list left-rotated: the store retail emits first is
  the one written last. `InitializeInstance` and `movieViewerFree` both
  matched on that rotation.
- Where retail adds the index to the base (`addu rd, index, base`) and leaves
  the rest of the offset in the load displacement, write
  `((u8 *)index + (u32)base)[disp]`. The plain `work->field[index]` form costs
  one register copy.
- Subscript the array with the loop index rather than walking a pointer when
  retail computes the pointer after the loop guard. A `*p++` walk makes gcc
  sink the pointer init into the guard's delay slot; `arr[i]` leaves gcc to
  create the walk itself, in retail's order.
- The EABI stack argument slot is eight bytes wide here, not four: the ninth
  and tenth arguments of a call sit at `0x0` and `0x8` of the outgoing frame.
- Pick the VU0 load by how retail forms the address:
  `SDF_VU0_LOADVECTOR(ptr)` where it computes the address into a register,
  `SDF_VU0_LOADVECTOR_MEMOPS` where it addresses the frame directly.
- `SDF_VU0_STOREVECTOR_NOMEM` is not only for globals and frame addresses
  shared across a call. It applies to a stack local inside a loop too, where
  the plain form's `"memory"` clobber makes the shared address a loop
  invariant and spends a callee-saved register on it. That took one function
  from 61 of 171 differing words to exact.
- Copy a vector quadword with `SDF_VU0_COPYVECTOR(dst, src)`, not a struct or
  `u128` assignment. The macro is one asm block, so the `lq`/`sq` pair stays
  together and the delay slot goes to the next call's argument; a `u128`
  assignment is two insns the scheduler splits, and it also hoists the `lq`
  above a store to the source that precedes it.
- A loop-invariant float constant that retail materialises into a saved fp
  register ahead of the loop index's init comes from a named local assigned
  before the loop. Written as a literal in the body gcc still hoists it, but
  into the preheader after the index init, which rotates four instructions.
- `sltiu` on a function result comes from a `u32` at the use site, either an
  unsigned return type or a cast. `effBTLModelGetGroupNumber(mask) < 2` with
  an `s32` declaration gives `slti`.
- Where retail forms a global's address twice, once into a temp for an `sqc2`
  and again as the following call's argument, use
  `SDF_VU0_STOREVECTOR_NOMEM(g)` rather than `SDF_VU0_STOREVECTOR(g)`. The
  `"memory"` clobber on the plain form does not block CSE of the address, so
  cc1 keeps one register and feeds both the store and the argument from it,
  two words short. Without the clobber it rematerialises. `nbSetFormRot` went
  from 16 differing words to exact on that one change.
- Where retail materialises `%hi`/`%lo` of `symbol + offset` fresh while the
  C holds the bare symbol's address in a register, declare the sub-object with
  `PS2_SYMBOL_AT`. In `effBTLFieldColorRun` a `SDF_VU0_COPYVECTOR` from
  `dds3LightParam.ambient` shared its base with the `dds3LightParam.sources[0]`
  load two statements earlier, so gcc kept the full address in a register,
  derived the ambient address with an `addiu +0x10`, and ran one word short.
  `PS2_SYMBOL_AT(effBTLVector4_t, effBTLSceneAmbient, dds3LightParam, 0x10)`
  gives the member its own relocation, which also lets the `%lo` of the
  pointer load fold back into the `lw`. That took the function from 13
  differing words to exact.
- A `u16` struct field used both as a loop bound test and as the loop counter's
  initial value needs **two** locals, and the bound one has to be `u16`. In
  `effBTLCutCreate` writing `if (cut->list->count != 0) { remaining =
  cut->list->count;` puts the count in `$2` and the reloaded `cut->list` in
  `$3` where retail has them the other way round; `u16 count = cut->list->count;`
  read before the pointer's other two members, with `remaining = count;` inside
  the guard, swaps the pair back and took the function from 9 differing words
  to 3.
- A `movz` selecting between two member addresses takes its two temporaries in
  the order the statement appears relative to its neighbours, not from how the
  selection is written. `effBTLCutCreate`'s `list = (kind < 2) ?
  &owner->banks[0].list : &owner->banks[1].list;` was 3 words out as the second
  statement of the function and exact once moved below the two `cutList`
  linkage stores; eleven spellings of the selection (an `if` either way round,
  both ternary polarities, an index expression, a byte-offset ternary and an
  explicit flag local) all give the same 3 or 4, so sweep the statement's
  position rather than its form.
- A varargs format string that retail keeps in a callee-saved register across
  the calls that compute the arguments is a named local assigned before them,
  and the arguments then have to be locals too. `effBTLFieldColorON` went from
  205 of 238 differing words to 41 with `format = D_00532FE8;` placed after the
  last VU0 macro and the six `sdfFloatToULong` results in their own `s32`
  locals; with the literal at the call site gcc builds the address in `$4`
  immediately before the `jal`. Anywhere from "after the last statement before
  the first argument" through "before the fourth argument" gives the same
  result; later than the fifth loses 40 words.
- The `(T *)((u8 *)(i * sizeof(T)) + (u32)base)` cast for retail's
  `addu rd, index, base` **can make things much worse**, so measure it rather
  than applying it on the operand order alone. It also stops gcc sharing the
  scaled index, so in a function that recomputes the row in two places
  (`effBTLFieldColorON` computes `id * 0x14` once for the colour reads and
  again for the printf arguments) the cast costs more than the operand order
  earns: 13 differing words became 221 with the cast on three accesses and 230
  with it on all of them. `effBTLCutDraw` shows the same thing for a loop row,
  88 of 263 words with plain subscripts against 149 of 260 with the cast.
- A dead `sw` to the frame after a colour-pack macro is a dead store to a
  stack aggregate's element, not a compiler artefact. `effBTLCutGetAllPartColor`
  ends `SDF_VU0_dds3ColorFtoI(color, 128.0f); work[0].word = color; return
  color;` with `work` a one-element 16-byte struct array, which is the same
  idiom `effColorBlend` in `effect/effMath` already carries.
- A copy of a struct's whole leading image, where retail emits a runtime
  8-byte-alignment dispatch over an `ld`/`sd` loop and an `ldl`/`ldr` loop plus
  an unconditional `ldl`/`ldr` tail, is one struct assignment of a 4-aligned
  type of exactly that size. `effBTLCreate`'s 0xC38-byte copy matched first try
  as `*(effBTLImage_t *)work = *(effBTLImage_t *)data;` with
  `typedef struct { s32 words[0xC38 / 4]; } effBTLImage_t;` declared in the
  unit, which avoids reshaping the shared `effBTL_t` header.
- Two pointer locals that walk in step through one loop, each initialised from
  a different global inside the same guard, couple their register assignment to
  their `lui` emission order and pull opposite ways. The saved registers follow
  statement order, so assigning the one retail gives the lower register first
  gets the registers right and the two `lui` the wrong way round; assigning the
  other first gets the `lui` order right and swaps the registers.
  `dds3BillboardManagerInit` sits two words from exact on that, and neither
  declaration order nor any loop-body or guard spelling separates them.
- Two pointers of the same type that a function needs again after its calls,
  where retail spills them to two adjacent 4-byte stack slots instead of using
  callee-saved registers, are **elements of a local array**, `T *batch[2]`, not
  two scalar locals. gcc gives the array a stack slot and reloads from it, and
  copies the two values into caller-saved registers for the loop, which is
  exactly retail's `sw`/`lw` pair plus two `daddu`. With two scalars it keeps
  them in `$17`/`$18` and the prologue grows. That took
  `dds3MultiBillboardPktLink` from 85 of 88 words differing to 4.
- Where retail folds a constant element offset into a symbol's `%lo` and then
  loads a member at a small displacement, `&table[n]` is not enough: both
  `&sdfGrOtTable[44]` and `sdfGrOtTable[44].regFunc` leave gcc holding the bare
  symbol and spending the `0x580` in the displacements, and a
  `(T *)((u8 *)table + 0x580)` pointer local is worse. `PS2_SYMBOL_AT(sdfGrOt_t,
  dds3MultiBillboardOt, sdfGrOtTable, 0x580)` and the member read through the
  alias is what gives retail's `addiu rd, $18, %lo(sym + 0x580)` with a shared
  `%hi` in a saved register. `dds3MultiBillboardLinkOt` went from 15 of 90
  words to exact on that one declaration.
- Two float products that share one converted operand take their registers
  from the source order, and the **second** one written is the one that reuses
  the shared operand's register. `evtDrawDeadMoonSprite` scales a width and a
  height by one integer scale factor; retail computes the height product into
  a fresh `$f15` and the width product back into `$f14`, the scale's own
  register, so the width statement is written second even though retail emits
  the height multiply first. Written the other way round the four coordinate
  conversions rotate as well, 22 of 112 words. An `f32` local for the scale and
  splitting each product from its divide both leave it unchanged.
- An integer parameter that retail converts to `f32` early and then multiplies
  by another converted integer needs its own `f32` local. Written `h * scale`
  with both `s32`, gcc emits an integer `mult` and one `cvt.s.w` of the
  product, which is two words over and shifts every saved register;
  `height = h;` ahead of `height * scale` gives retail's two `cvt.s.w` in the
  leading call's shadow and one `mul.s`. `evtDrawDeadMoonSprite` went from 56
  of 112 words and two words long to 22 at the exact size on that one local.
- A header word a function needs both before its calls and again after them
  belongs at its second use site as the field expression, not in a local. A
  local for it takes a callee-saved register and shifts every later allocation;
  `dds3BillAnimLoadP2A` wants `*(s32 *)resourceData` written again as `memcpy`'s
  length while the same read feeds the allocation size above it, and that moved
  10 of its 118 words.
- `SDF_VU0_LOADVECTOR_MEMOPS` and `SDF_VU0_STOREVECTOR_MEMOPS` take an
  **lvalue**, and an array's name is not one. `SDF_VU0_LOADVECTOR_MEMOPS(delta)`
  on `f32 delta[4]` makes cc1 write the decayed pointer to a fresh stack slot
  and then read *that slot* as the quadword, which also grows the frame by 16
  and moves every other local; `delta[0]` is the correct spelling and gives
  retail's bare `lqc2 vf10, 0($29)`. `dds3BillboardGetDirectionRotate` went from
  23 of 94 words to exact on that alone.
- The double-precision helpers are explicit calls in this engine's source, not
  compiler soft float. `sdfFloatToULong(x)` widens an `f32`, `dpdiv(a, b)` is
  the **comparison** (its result's sign is what `bgez` tests, despite the name),
  `dpsub(a, b)` subtracts, and `sdfDoubleToULong` returns the narrowed value in
  `$f0`, so declare it `f32`. A `1.0` double is retail's `ori rd, $0, 0xFFC0`
  plus `dsll32 rd, rd, 14`. `dds3BillboardTagLink` and `script/scrTraceCode`
  both use the quartet this way.


- A run of field copies between two objects of the same struct type gets its
  loads and stores regrouped by the scheduler, and the alias set is the lever,
  not the statement order. In `sdfModelCopyStat` retail copies `light`
  (a `sdf3DLightParallelPacket_t *`), `faceDivSoften`, `fog`
  (a `sdf3DFogPacket_t *`) and `faceDivBias` as two load/store pairs, while the
  plain field assignments hoist both pointer loads together and spend a second
  register: ee-gcc 2.96 gives each pointed-to type its own alias set, so a store
  to `light` does not block a load of `fog` and the scheduler interleaves them.
  Copying both through **one** set, `*(u32 *)&dst->light = *(u32 *)&src->light;`
  and the same for `fog`, restores retail's grouping and took the function from
  12 of 154 words to 4. `void **` on both reaches it too; `u32` on only one of
  them, or on all four fields including the two `f32`s, does not. The last four
  words were the fog and bias pair written above the `nodeList` reads rather
  than below them, and all 120 permutations of the eight statements in that
  tail were swept to find it: twelve of them sit at 2 words and exactly one is
  exact, so sweep rather than reason.
- The alias-set lever reaches a run of **zero stores**, not only a run of field
  copies, and the type to share is `u32` rather than a pointer type. The
  allocation initializer in `dds3AllocBasic` writes fourteen fields of a fresh
  `dds3Basic_t`; retail emits them in source order, while the plain field
  assignments come out rotated by whole pairs (gcc groups the stores by alias
  set and emits the last pair first), and no permutation of the six statements
  in the affected group reaches retail's order, all 1440 of them with the
  vtable read at either position. Writing just the two `void *` fields as
  `*(u32 *)&basic->staticdata = 0;` puts them in the same set as the two `u32`
  fields next to them and the natural source order is then exact. `void **` on
  the pointer fields instead is not equivalent and leaves six words out, so
  reach for `u32` first, and the `char *` and vtable fields between them do not
  need the cast at all.
- A table read whose index retail keeps in `$4` wants the read placed **after**
  the first store of the object it initialises, not before it.
  `dds3AllocBasic` reads `dds3BasicFuncTablePtr[type]` between
  `basic->type = type << 24;` and the rest of the initializer; read before the
  first store, the scaled index lands in `$6` and costs the `addu`'s operand
  register as well.
- A function whose early exits all return 0 does **not** want the
  `s32 ret = 0; ... return ret;` form just because retail materialises the zero
  in the prologue. `dds3AllocBasic` has `daddu $2, $0, $0` as its second
  instruction and two plain `if (...) return NULL;` guards reach it, because
  the guard's delay slot is already full; the result local instead runs eight
  bytes long and leaves the whole body shifted.
- An index into an array that is a **struct member** associates as
  `(base + index * scale) + offsetof`, which is what leaves retail's constant in
  the store displacement. A pointer local for the member
  (`f32 *w = (f32 *)(tag + 1);` then `w[k]`) associates the other way,
  `(base + offsetof) + index * scale`, and folds the constant into the base.
  `sdfModelLinkShapedDrawPacket`'s two blend weights need a unit-local
  `struct { sdfDmaTag_t dmatag; f32 weight[2]; u32 code0; u32 code1; }` with the
  running `sdfDmaTag_t *` cast to it at each access, and advancing that one
  pointer by `tag += 2` rather than keeping a second header pointer is what
  gives retail's `addiu rd, rd, 32` in place.

- A `jal fptodp` anywhere in the output means a float argument reaches a callee
  with no prototype in scope: the implicit declaration promotes it to double.
  This bites hard when a unit's helpers are **defined below** their callers, as
  splat's INCLUDE_ASM ordering usually leaves them. Seven functions in
  `event/evtPolygonMovie` were 45 or more words out until the six `evtHokan*`
  prototypes were added above the first caller, and all seven then came within
  a couple of words.
- A static out buffer used three times at different offsets does **not** want
  the pointer local the `fldElvGetFloorDigits` row above asks for. Where retail
  keeps the symbol in a saved register and adds the offsets with separate
  `addiu`s, writing `D_0056CFF0`, `&D_0056CFF0[4]` and `&D_0056CFF0[8]` at the
  three call sites is exact, while `f32 *out = D_0056CFF0;` plus `out`,
  `out + 4`, `out + 8` spends the *other* saved register on it and rotates
  `$20`/`$21`. `SLightFrameFunc` sat at 21 of 102 words on that one local, and
  no declaration or assignment order of `out` against the `hokan` local moved
  it. So sweep the local against the bare symbol rather than assuming either.
- Where a run of sibling callbacks reads a key frame and its successor, which
  arm of `now >= f->frame` is written first decides the whole body's branch
  direction. Retail falls through to the `now >= f->frame` arm, so that arm is
  the `if` and the `now < f->frame` arm is the `else`. Written the other way
  round all seven key-frame interpolators in `event/evtPolygonMovie`
  (`SFogFrameFunc`, `Blur2FrameFunc`, `MBlurFrameFunc`, `DBlurFrameFunc`,
  `RBlurFrameFunc`, `MFilterFrameFunc`, `FilterFrameFunc`) sat at 45 to 51 of
  65 to 74 words, and all seven went exact together when the arms were swapped.
- Two values computed by the same expression shape and merged at one exit take
  their registers from the order of the two assignments, and the computing arm
  and the default arm have to **agree**. `BlurFrameFunc`'s alpha and scale are
  assigned alpha-first in both the interpolating arm and the two default arms;
  alpha-first in one and scale-first in the other is 32 to 34 of 42 words out,
  and scale-first in both is 16 of 44. The test order in the following `||`
  chain and the order of the three stores have no effect at all.
- The `void`-to-`s32` return-type sibcall blocker has to be swept per function,
  not applied to a family. In `event/evtPolygonMovie` it is what
  `QuakeFrameFunc`, `BokasiFrameFunc`, `FadeFrameFunc`, `LightFrameFunc` and
  `BgmFrameFunc` need, while `SeFrameFunc`, `SkyFrameFunc`, `UnitFrameFunc` and
  `CameraFrameFunc` match as `void` and *break* when given the same `s32`
  return type, even though all nine sit in the same frame-callback table. For
  `evtRunUnitMode` the `s32` return type made it worse (26 of 52 words) and a
  dead store on the switch index in the last arm, `mode = 0;`, took it to
  exact.
- A run of stores inside a loop whose addresses are hoisted base pointers has
  **two** orders to get right, and they run opposite ways. The stores themselves
  follow the right-rotation rule (`func_001AA5A0` writes the word array, then
  `dds3PadButtonPrev`, then `dds3PadButtonState`, and retail emits Prev, State,
  word), while the order of the three hoisted `%hi`/`%lo` base pairs in the
  preheader follows the source order directly. Both fall out of one statement
  order, so sweep the permutations rather than reasoning about either rule alone:
  all six were measured and only one is exact.
- A single-statement guard arm belongs in the `else` even when the statement is a
  *store*, not just an index computation. `eff_1AA3B0`'s pad toggle written
  `if (a == 0 || b != 0) { a ^= 1; } else { b = 1; }` is 15 of 104 words out and
  one word long; inverted to `if (a != 0 && b == 0) { b = 1; } else { a ^= 1; }`
  it is exact. In the same function a cached `u8` local for the toggled global is
  far worse than reading the global at each of its three uses (85 of 100 words
  against 15 of 104), and the `lbu` retail uses for a symbol other callers read
  with `lb` comes from a `(u8)` cast at the use site, not from the declaration.
- A two-load chain `work->owner->field` whose base is also a later call's
  argument can leave the base in `$4` across the whole function, so that a path
  which bypasses the loop reaches the call with it already there and the `jal`'s
  delay slot stays a `nop`. Nothing in the source reaches this: an owner local
  (one assignment, reassigned after the loop, or mixed with the inline
  expression), every ordering of the prologue loads and every declaration order
  leave ee-gcc loading the base into `$2` and letting the delay-slot filler take
  the reload for both paths. Retail itself goes both ways on it inside one unit,
  so it is a gcse availability accident rather than a source fact: the three
  `effMaga*Draw` drafts in `effect/effMagatuhi` sit on it at 77 percent of their
  words while their sibling `effMagaSmokeConvDraw`, with the identical tail,
  matches with the load in the delay slot.
- A function whose body is more than half inline assembly is only refused by the
  production report when it carries no exemption comment. `check_unit.py`'s
  `inline_asm_share` skips any function whose definition is preceded within three
  lines by a `vu0 routine:` or `libvu0:` comment, which is why the VU0-dense
  setters in `effect/effMagatuhi` count. `func_001AA6D0` there is 223 VU0
  instructions of 394 and needs that comment to earn anything.

- A `(s16)` cast on a `u16` struct field compiles to a bare `lh`, not to `lhu`
  plus `sll`/`sra`: ee-gcc 2.96 folds the sign extension into the load.
  `GetFontHandleUnit` needed `(s16)chain->...cellHeight` where retail has
  `lh`, and the cast cost nothing. Reading it through an `s16 *` cast of the
  field's address gives the same thing, so prefer the plain cast.
- Where every retail read of a `u16` field is `lh`, retype the field `s16` in
  the header rather than casting at each use. A `> 0` guard is the cheap tell:
  on a `u16` it compiles to a plain nonzero test (`beq`/`bne`), on an `s16` to
  `blez`. `scrHeader_t.LocalIntCount` and `LocalFloatCount` were `u16` and cost
  `scrPrintProcedureStart` an `lhu` plus a `beq` per array; retyping both `s16`
  (same offset, same size) was the last change before it matched, and no other
  unit shifted.
- A `base[index].field` expression used several times keeps `index * sizeof` in
  a register and re-adds `base` at each use. Hoisting it into a pointer local
  (`row = &base[index];`) instead keeps the whole address in a callee-saved
  register and renumbers everything around it. `scrPrintProcedureStart` reads
  `proctable[procno]` four times and wanted the subscript spelled out each
  time; the `proc` local moved 123 of 154 words.
- One extra *statement* on a value raises its register-allocation priority,
  which is a cheaper lever than any declaration order. `GetFontHandleUnit`
  held four values across its loop and gcc gave them `$16` to `$19` one place
  rotated from retail; splitting `y += h * 8 + 0xA;` into
  `y += h * 8; y += 0xA;` added a reference to `y`, moved it from `$19` to
  `$16`, and took the function from 22 differing words to exact. Same
  instruction count either way. gcc's `allocno_compare` sorts on
  `floor_log2(n_refs) * freq / live_length`, so one more reference can cross a
  threshold.
- A named local for a *divisor* decides which register the constant takes, and
  its declaration position among the other locals decides the prologue order.
  `ConvertZenkakuNumber` divides by ten in a loop; `s32 radix = 10;` declared
  between the digit counter and the digit pointer is exact, where the literal
  leaves the 10 in `$4` and the loop bound's `slti` in `$6` (retail has them
  the other way round), and the same local declared first or last, or assigned
  as a statement, is 2 to 7 words out.
- One `u32` local for the switched value, **reused** across two successive
  `switch`es in the same function, is what gives both their registers.
  `CalcWindow` dispatches on `panel->flags & 0x300` and then on
  `panel->flags & 0x3000`; `mode = panel->flags & 0x300; switch (mode)` and
  `mode = panel->flags & 0x3000; switch (mode)` is exact, where the
  expressions written in the `switch` heads leave the value one register low
  and rotate the two panel pointers (16 of 81 words), and so do a `flags`
  local and a second re-read of it.
- A `switch` arm that writes one byte of a stack array in each of two paths
  wants a pointer local assigned *before* the `if`, with the stores written
  `*p = c;`. In `itfMesMngSetVar` the `p = text;` assignment above the
  `value >= 0x3E9` test is what puts `text`'s address in a saved register and
  turns both `text[0]` stores into `sb rd, 0(reg)`; with the assignment inside
  the `else` the arm addresses the frame directly and the function is 26 of 124
  words out, and the second store has to stay `text[1] = 0` (frame-addressed)
  rather than `p[1]`.
- A frame address that a VU0 store and a following call argument both use is one
  pseudo for both, and inside a loop that pseudo is loop invariant: gcc hoists
  the `addiu rd, $29, N` into the preheader, spends the last callee-saved
  register on it and spills whatever loses the race (in
  `effEveBEDCutDraw` a colour word, which also grew the frame by 16 and left it
  71 of 119 words out). Retail rematerialises the address at each use instead,
  from two short live ranges that never cross the call. Neither
  `SDF_VU0_STOREMATRIX` nor `_NOMEM` reaches that, and a
  `sdfMatrix4_t *mp = &marked;` local is folded straight back into the shared
  pseudo. `SDF_VU0_STOREMATRIX_RWADDR`, whose address operand is `"+r"` so cc1
  has to give the asm its own copy, is what reproduces it; the function went to 2
  of 119 words. `_MEMOPS` frees the register too but addresses the frame
  directly, so the four `sqc2` and everything after them shift by a word.
- A clamp whose result is stored straight into a vector field belongs in the
  field, not in an `f32` local: `if (half < radius) { offset.z = -radius; } else
  { offset.z = -half; }`. With a `depth` local gcc coalesces it with the compared
  value and negates in place (`neg.s $f2, $f2`) where retail negates into a fresh
  register, which also rotates the compared value one register down and moves the
  store past the call's argument setup. That was the last 12 words of
  `effEveBEDPartGetPositionPtr`, and no declaration order of its five `f32`
  locals moved any of them.
- A constant assigned once above an `if (a == K1 || a == K2)` whose body
  overwrites it comes out hoisted and shared. Retail's two copies, one between
  the two tests and one at the top of the body, come from writing the assignment
  in **both** arms: `if (dir == 8 || dir == 0xA) { y = -1.0f; if (...) { y =
  -radius; } } else { y = -1.0f; }`. `effEveBEDPartGetPositionPtr` went from 114
  of 138 words to 84 on that duplication alone, and the `&&` spelling of the
  inner pair changes nothing.
- Where retail materialises one zero with `mtc1 $0` and stores it with `swc1`
  twice, the source assigns an `f32` local as a *statement* positioned after the
  first non-zero store of the same vector, and stores the local into both fields.
  `offset.y = offset.x` instead gives `sw $0` plus a `lwc1` of the slot back, and
  the same function stores its other two zeros as plain `0.0f` (`sw $0`) in the
  other arm, so the two spellings really do coexist in one body. The four stores
  around it were swept: `offset.z`, the zero local, `offset.y`, `offset.x`,
  `position.y` is the only order within two words.
- A run of stores that each repeat the whole subscript needs the row to be one
  flat array, not an array of two-field structs. `effEVECombParSmokeDraw` writes
  eight texture coordinates per quad; spelled `batch.uv[n][k].u` and `.v` on an
  `UV_t uv[15][4]`, cc1 builds two base pseudos, `$29 + 0xC` for the `.u` stores
  and `$29 + 0xE` for the `.v` stores, shares neither, and materialises a whole
  address per store with a zero displacement. Declared `s16 uv[15][8]` and
  written `batch.uv[n][k]`, it builds the one `$29 + 0xC` base retail has, with
  the `0x120` to `0x12E` displacements and the seven `daddu rd, rs, $0` copies
  that give each `sh` its own register. Worth 30 words and both base registers.
  The index of such a run belongs in a local read once, `n = batch.count;`:
  written `batch.uv[batch.count][k]` the counter is re-read for each of the
  three scaled forms (`sll 2`, `sll 4`, `sll 5`) where retail reads it once and
  shifts that, and the increment after the run still re-reads the field. One
  local reused across two such batches is what gives retail's single register.
- Four corner offsets that each rotate a signed half extent come from **two**
  float locals reassigned between the pairs, not from the expressions written
  out: `x = cx - hw; y = cy - hh;` for the first corner, `x = cx + hw;` for the
  second, `y = cy + hh;` for the third and `x = cx - hw;` again for the fourth.
  `effEVECombParSmokeDraw` reuses one register for each of the pair and
  recomputes the repeated value, which is six words more than the expressions
  written at the eight stores; that one change also took the body to within six
  words of retail's size. `cx` and `cy` are both `0.0f` there and share one
  `mtc1 $0` pseudo, and assigning them as statements above the half extents is
  what puts that pseudo in `$f25` rather than behind the `0.0625f`.
- The read-write address operand generalises past the matrix store. Where
  several VU0 accesses to one frame vector sit inside a loop, the plain and
  `_NOMEM` forms share one address pseudo, which is loop invariant, gets
  hoisted into the preheader and spends a callee-saved register, so whatever
  loses the race moves out of its spill slot and the frame grows.
  `SDF_VU0_LOADVECTOR_RWADDR`, `SDF_VU0_STOREVECTOR_RWADDR` and
  `SDF_VU0_COPYVECTOR_RWSRC` take the address through a pointer local as an
  `"+r"` operand, so cc1 gives each asm its own copy and retail's separate
  `addiu rd, $29, N` per access come back. That took
  `effEVECombParSmokeDraw` from 524 of 706 words and a 16-byte larger frame to
  the exact frame size, every saved register and all thirteen spill slots.
- Two pointer pairs retail holds in adjacent four-byte slots are **not**
  necessarily a local array. gcc rounds every stack aggregate up to 16 bytes, so
  `sdfDrawTag_t *tag[2]` followed by `sdfTexHandle_t *tex[2]` lands at `0x870`
  and `0x880` with an eight-byte hole and a 16-byte larger frame, where retail
  packs all four at `0x870` to `0x87C` as ordinary reload spill slots. Read the
  gap between the slots before reaching for the `dds3MultiBillboardPktLink`
  array row above: contiguous four-byte slots mean plain locals that spill.
- Two locals that both spill take their stack slots in **declaration** order,
  and their load order in the prologue follows the statement order separately.
  In `eff_1AEC58` retail gives `0x30` to the `u8 repeat` and `0x34` to the `u32`
  particle count; declaring `repeat` first is exact and declaring it last swaps
  both slots and their four reloads (5 of 262 words). The statement order is a
  second, independent lever on the same pair: reading the particle pointer before
  the count took it from 9 of 262 to 5, and all six orders of the three leading
  reads differ. An `s32` for the `u8` field undoes the slot fix.
- The whole spill-slot sequence is readable, not just a pair, and it gives the
  declaration list back. gcc assigns frame slots in this order: addressed
  locals (an array) at `expand_decl` time, then every spilled pseudo in pseudo
  number order, which is the parameters first, then the declared scalars in
  declaration order, then the temps the statements create, then the pseudos
  the loop optimizer creates. So retail's slots name the locals in order.
  `sdfModelLinkOt`'s eight spilled locals read off its frame as `i`,
  `rootColor`, `drawAttr`, `attr`, `light`, `drew`, the shifted soften bias
  and the 64-bit draw-parameter word, with the colour sign flag and the
  hoisted `frame * 4` as the two trailing temps; declaring them in that order
  took it from 134 to 112 differing words and fixed the frame size, and the
  same list explains `sdfModelTempPacketLinkOt`'s frame, which spills three
  more of the same variables.
- A local assigned **inside** a loop and then hoisted out loses its place in
  that sequence: gcc's loop invariant motion gives the moved value a fresh
  high-numbered pseudo, so it lands after the loop-created temps and the frame
  grows. Assigning it before the loop keeps the declared slot but leaves the
  computation before the loop guard instead of in the preheader. In
  `sdfModelLinkOt` the two halves could not be had together for
  `corePktSoftenBias << 16`.
- `(flags & bit) != 0` stored as a flag folds to `srl`/`andi`. Where retail
  has `andi` then `sltu rd, $0, rs`, assign the masked value to its own local
  and test *that* local: gcc's `fold_single_bit_test` only fires when the
  comparison's operand is the `BIT_AND_EXPR` itself, so one extra local
  blocks it.
- `x < 0` stored as a 0/1 flag is **always** `srl rd, x, 31` from C. gcc
  2.96's `do_store_flag` passes `normalizep = 1` to `emit_store_flag`, which
  takes the sign-bit shortcut before trying `slt`. Seventeen spellings and
  four flag types were measured for `sdfModelLinkOt` and every one gives the
  shift. `slti rd, x, 0` in this tree only ever comes out of
  `emit_conditional_move`: `sdfChipFreeDirect`'s signed divide by 0x1000
  emits `slti`/`movn` for the rounding adjust. Treat a bare `slti r, x, 0`
  whose result is then stored as an unsolved shape, not a spelling to sweep.
- A word load whose only use is `>> 16` is narrowed to `lhu` at +2. Eight
  spellings were ruled out on `sdfModelTempPacketLinkOt`'s shape-header read,
  including a `u32` struct member, `*cursor++ >> 16` and `/ 0x10000`. Only a
  second live use of the low half keeps the `lw`/`srl` pair;
  `(w & 0xFFFF0000) >> 16` keeps it in a small function but is narrowed again
  once the value feeds an index, so the pattern in
  `sdfModelSetNormalDrawPacket` works because `qwc` is used too, not because
  of the cast.
- The three `nop`s between the `sw $0` and the `bne` of retail's four-slot
  draw-tag clear loops are ee-as's short-loop padding, not source. A plain
  `for (j = 0; j != 4; j++) tags[j] = NULL;` reproduces them word for word.
  The padding is not limited to branch-likely closers: `ragGetListItem2` in
  `code_001FAE80` carries one `nop` after the `mfhi` and one between the
  `slti` and the plain `bnez` that closes a seven-iteration weight scan, and
  a draft whose register allocation differs from retail in 22 of 51 words
  still lands both `nop`s on the exact retail words. So a `nop` run inside a
  short loop is never the thing to chase; look at the registers around it.

- A packet builder whose stores all go to one array element wants the element
  indexed at every store, `packet_array[frame].field = v;`, not a
  `T *p = &packet_array[frame];` local. The pointer form folds the whole body
  into displacements off one base; retail computes `(index * size + offset) +
  base` per field and spills the ones it cannot keep, which is 65 words more
  in `dds3DrawFade`. Within an A+D register entry write `.reg` before `.data`:
  that is the order the scheduler emits, and `.data` first costs a word per
  pair in both `dds3DrawFade` and `dds3InitZClearDrawTag`.
- Where retail multiplies a loop index by a struct size with an odd factor and
  a shift (`sll idx, 5` with the induction variable stepping 0xB for a 0x160
  row), index the element inside the inner loop, `a[outer][inner]`. Hoisting a
  row pointer to the outer loop (`p = a[outer];` plus `p++`) makes gcc build a
  single induction variable stepping the whole row size and costs two words.
  `dds3InitZClearDrawTag` and `dds3DrawFade` both show the factored form.
- A loop that reads three arrays, where retail walks two by pointer and
  subscripts the third by the counter, comes from subscripting *all three* by
  the counter. gcc then reduces two of them to pointers on its own and leaves
  the third indexed, which is what keeps the counter an up-counter with an
  `slti` bound. Incrementing two pointers in the source instead lets gcc
  reduce the third as well, the counter goes dead, and the loop becomes an
  `addiu -1` downcounter: `dds3RunQuake` was 126 words that way against
  retail's 129, and 13 of 129 with every subscript written out.
- An eight-byte `extern` struct copied by value is addressed `gp`-relative,
  with the `ldl`/`ldr` pair reading off `$28`. Where retail builds the address
  in a register first, declare the symbol as an incomplete array and copy its
  first element, `extern T sym[]; T local = sym[0];`. The unknown extent keeps
  it out of small data. `dds3RunQuake`'s pair of axis gains needs this.
- Two references to one symbol with different addends that retail materialises
  as two independent `lui`/`addiu` pairs are two symbols in the source. Give
  the second one a `PS2_SYMBOL_AT` name and pass that: `dds3InitDrawEnvSub`
  hands `sdf3DMakeLightParallelPacket` the light record and its ambient term,
  and sharing the `%hi` through `&dds3LightParam.ambient` runs a word short per
  call.
- Row 78's `ldl`/`ldr` plus 8-byte-alignment dispatch is reproduced exactly by
  `extern const T g; T local = g;` on a struct of plain `char` arrays, as long as
  the size is a multiple of 8: `DATA_ALIGNMENT` lifts a global aggregate to four
  bytes, so cc1 knows it can use doublewords but still has to test the source
  address's low three bits at run time. `dds3ObjectBaseMakeUnit` has fourteen
  copies of a 360-byte `{char name[20]; char parts[17][20];}` and every one
  matched first try. The locals take 16-byte-aligned frame slots in declaration
  order, so a 0x168 type gets a 0x170 stride and the declaration order is
  readable straight off the stack offsets. Declaration initializers against plain
  assignments decide only where the copies land relative to the function's
  leading calls; retail has them after `mdlLoad`, which is the assignment form.

- **Every repeated scale factor in a run of coordinate expressions is a reused
  `f32` local, not a literal at the use site**, and this is the largest single
  lever measured on a long float body. `effPCPBezierBillDraw` builds 35 bezier
  control points as `spin[k] * K1 + axis[k] * K2 + center[k]`; with the
  constants written as literals each point's factor takes a fresh float
  register, every temp in all five `switch` arms rotates, and the function sits
  at 936 of 1497 words. With `scale`, `bend` and `reach` declared once and
  **reassigned before each point**, retail's single `$f7`, `$f5` and `$f8` fall
  out and it goes to 1276. The tell for the local rather than the literal is a
  bare `mov.s $f8, $f20` where the factor is the unscaled value: that copy comes
  from `reach = radius;` and not from `axis[k] * radius` written out. Read which
  float register retail reuses across a repetitive block before writing one of
  these out.
- The operand order of a commutative `mul.s` follows the **source** order, so
  `reach * axis[k]` and `axis[k] * reach` are one word apart at every use. In
  `effPCPBezierBillDraw` that was 90 words over 95 multiplies. The order is not
  uniform within a function: the same expression wants the variable first where
  its load is scheduled before the constant's `lui`/`mtc1` and the constant
  first where a `SDF_VU0_APPLYMATRIX_XYZ` between two points puts the constant
  in a register ahead of the load, so read each block rather than applying one
  rule.
- Where several VU0 accesses to one frame vector sit in a loop *or* in a
  straight run punctuated by calls, the plain `SDF_VU0_STOREVECTOR` is what
  costs the frame. `effPCPBezierBillDraw` keeps three stack quadwords and one
  spin vector this way: with the plain form the shared `addiu rd, $29, N`
  becomes a loop invariant in a callee-saved register, one more pointer spills,
  a third quadword spill slot appears at `0x170` and the frame grows from
  `0x1F0` to `0x200`. `_NOMEM` on those four stores was worth 46 words, the
  spill slot and the whole frame, **even though the C reads the stored vectors
  back by field** immediately afterwards, against the caution on the macro.
- A second read of a field that a store to an addressable local sits between is
  a reload, and the cheap fix is to read the local back instead of the field.
  `effPCPBezierBillDraw` writes `place.payload[0..2]` from `slot->position[]`
  and then needs two of those components again for an atan2; written
  `center[0] - slot->position[0]` the store to `place` kills the load and gcc
  reloads, 15 words out, while `center[0] - place.payload[0]` is
  store-to-load forwarded into exactly the register retail uses. Reading the
  three components into `f32` locals first is the obvious alternative and is
  worse, because all three are then live at once where retail reuses one
  register for the second and the third.
- A colour word built by a float-to-unsigned conversion and stored through a
  pointer field wants **no intermediate local at all** when retail loads that
  pointer in the block head.
  `slot->fragment->color = ((u32)(fade * 128.0f) << 24) | 0x808080;` as one
  statement lets gcc hoist the `lw` of `slot->fragment` above the unrelated
  `slot->fadeScale` store into the entry block; with a `u32 color` local the
  load cannot move and the whole 19-word block shifts by one instruction.
  That was 18 words in `effPCPBezierBillDraw`, and a `u32` local for just the
  conversion (the `dds3EffectRippleBlurDraw` row above) does not reach it, so
  the two shapes are opposites and which one applies depends on whether retail
  loads the pointer early.
- A float local reused for two values with **disjoint** live ranges is how
  retail gets the long-lived one into the lowest callee-saved register, and
  splitting it is what fixes a rotation of three. In `effPCPBezierBillDraw` the
  `-height * rand()` term dies at its one use and the `SDF_VU0_GETLENGTH`
  result lives across the whole `switch`; two locals give the short one `$f20`
  on priority and let the long one reuse `$f20` after it dies, which is
  retail's assignment, while one local for both leaves `$f20`, `$f21` and
  `$f22` rotated by one. This is the inverse of the one-local rows above: read
  whether the two values overlap before choosing.
- A `.lit4` constant whose decimal is obvious needs no reverse engineering:
  write the natural literal and let ee-gcc truncate it. `0.15f` in the source
  gives retail's `1.499999911e-01` word, and the same holds for `0.3f`,
  `0.45f`, `0.6f`, `0.14f` through `0.84f` and `0.333333333f`. The float-literal
  section's warning is about transcribing a *rounded value read out of a
  disassembly*, not about ordinary decimals.
- A byte counter decremented in place gives `addiu rd, rs, 0xFF` when the
  global is `u8` and `addiu rd, rs, -1` when it is `s8`, since the unsigned
  form stays in QImode. `sdfDispThread` in `sdf/sdfGraph` needed
  `extern volatile u8 sdfReqDrawDelayCount;` for its `sdfReqDrawDelayCount--`,
  where `sdf/sdfDraw` declares the same symbol `volatile s8` and matches with
  it. `volatile` is what gives retail's second `lbu`, the test and the
  decrement reading the byte twice.
- `!x` on a value that reached `s32` through a 64-bit truncation emits
  `sltiu rd, rs, 1`, because the `dsll32`/`dsra32` pair hides the `andi 1`'s
  range from the expander. Where retail has `xori rd, rs, 1`, write the
  exclusive or: `sdfDispThread`'s `sdfFrameEvenOdd = field ^ 1;` on
  `field = (~HW_READ(GS_CSR) >> 13) & 1;` is exact, and `!field` and
  `field == 0` are both one word out.
- A row pointer that shares the scaled index's own register is visible in the
  assembly as the `%hi`/`%lo` pair landing in `$1`: ee-as spends `$at` on
  `la $2, sym($2)` because it cannot clobber the index before the add, and
  emits the pair into the destination register for any other destination. So
  `lui $1`/`addiu $1`/`addu rd, $1, rd` is the tell that the pointer and the
  index are one pseudo, and in `sdfGrSetFrameSetMode` the thing that reached
  it was not the address spelling but the store order: five of the field
  copies' 120 store permutations get the shared register and the rest emit
  `lui`/`addiu` into a second register.
- `PS2_SYMBOL_AT` for `dds3LightParam + 0x10` confirmed a third time, and the
  two halves of one unit disagree. In `basic/dds3LightObjectBasic`,
  `lightmainprocedure` shares the `%hi(dds3LightParam)` it already formed for
  the parallel-light pointer and derives the ambient with an `addiu +0x10`,
  four words short, until the member gets its own alias; its neighbour
  `dds3LinkSceneLight` has the identical pair of accesses and is exact with the
  plain `&dds3LightParam.ambient`. So read whether retail's `%hi` is shared
  rather than applying the alias to every access of the member.
- One reused handle local for two successive allocate-and-lock pairs is what
  gives retail's single callee-saved register and its `daddu rd, $2, $0` per
  pair. `itfPnlRegist2` assigns `allocation` twice, once for the panel record
  and once for the kind's payload; two named locals store the second call's
  result straight out of `$2` and lose the copy.
- **A callee with no prototype in scope drops every argument conversion**, so
  on a unit that leaves most callees implicit, a caller sitting above its
  callee's definition needs a forward prototype before anything else is worth
  trying. `cmbGetParamBase` in `code_001FAE80` calls `cmbGetHeartsParam(s8,
  s8)`, which the unit defines 200 lines below it: with no declaration in
  scope ee-gcc passed both arguments as they stood, so the two `sll 24`/
  `sra 24` pairs were absent, one argument needed no instruction at all
  because the parameter was already in `$5`, and the function sat at 4 of 26
  words. One `extern s32 cmbGetHeartsParam(s8 devilId, s8 index);` above it
  made it exact. This is the companion to the `fptodp` row: the tell for a
  missing float prototype is `jal fptodp`, and the tell for a missing integer
  one is a narrowing conversion the retail text has and the draft does not.
- **A narrow parameter forwarded to a narrow parameter inside a loop wants to
  be declared `s32`**, with the prototype doing the conversion. With the
  parameter declared `s8`, cc1 materialises the whole `(s8)` value once into a
  callee-saved register and copies it into the argument register per
  iteration; declared `s32`, the conversion is expanded as part of the
  argument setup, so loop-invariant motion hoists only the `sll 24` into the
  saved register and leaves the `sra 24` writing the argument register inside
  the loop, which is what retail has. `cmbAddHeartsParam` and
  `cmbSubHeartsParam` went from 1 of 26 words to exact on that one declaration,
  and `fclTerminalSetMode`'s `(s8 mode, u16 id, work *)` had to become
  `(s32, s32, work *)` for the same reason: retail stores the raw byte and
  halfword and normalises at each call, where the narrow declarations normalise
  once up front.
- Where a loop both loads from a table and stores into a slot, retail usually
  **walks the store pointer and indexes the load**, and the source has to say
  so: `*tex = f(table[i]); tex++;` gives retail's `sll`/`addu`/`lw` on the
  table against `sw 0(ptr)` plus `addiu ptr, 4`. Indexing both makes gcc share
  one `i * 4` across the call and spend a fourth callee-saved register on it;
  walking both loses the indexed load. `fclTerminalLoadTextures` and
  `cmbAddHeartsParam` are both this shape.
- **A signed byte loop counter is a biv ee-gcc's strength reduction
  recognises; an unsigned one is not.** `s8 i` is set by
  `(sign_extend (plus i 1))`, which `basic_induction_variable` accepts, so
  every array subscript on `i` in the body is reduced to its own walking
  pointer; `u8 i` is set by `(and (plus i 1) 0xff)`, which it does not, so the
  subscripts stay as `sll`/`addu`/load. Read which one the retail body has
  before choosing the counter's type: `fclTerminalLoadTextures` needs the
  unsigned form to keep its indexed table load and is one word short because
  of it, since retail's counter normalises with `sll 24`/`sra 24`.
- **Two address bases in one loop come from two pointer locals computed before
  the guard, and which one lands in the guard's delay slot is decided by which
  is assigned after it.** `func_0023ED18` walks a 0x2E4-byte list testing two
  bytes and one word per entry; one struct pointer gives one base with
  displacements 0x18, 0x1B and 0x38, where retail has a base at `+0x10` with
  displacements 8 and 0xB and a second at `+0x38` with displacement 0. A
  `s32 *` local for the word field plus a cast to a view struct based at
  `+0x10` for the bytes reproduces both, and moving the view pointer's
  assignment below the `if (list == NULL)` guard is what puts its `addiu` in
  the `bnez` delay slot where retail has it. 22 of 25 words to exact; the
  declaration order of the two locals changes nothing, as the declaration-order
  row says.
- An argument that is a cast of another argument belongs **at the call**, not
  in a local: `cmpDrawScrollBar(..., (cmpCursorWork_t *)((u8 *)work + 0x15C),
  ...)` is exact in `lstDrawListWindowCursor` where the same expression in a
  local ahead of the call rotates three of the field loads and is 23 of 26.
- The mirror of the pointer-local rows: where retail computes the **same
  address twice into two registers plus a copy**, write the subscript out at
  every use. `rstSetMotionReqFromTbl` passes three fields of one table entry to
  one callee and is exact with `D_00332458[i].unk_00`, `[i].unk_01` and
  `[i].unk_02` written three times; a `*entry` pointer local for two of the
  three collapses it to one address and makes ee-as materialise the symbol
  through `$1`, 22 of 26 words.
- A guard on a call's result whose failure returns zero wants
  `flag = f(); if (flag == 0) { return flag; }`, not `if (f() == 0) { return
  0; }`. The first lets the epilogue's `ld $31` fill the `beqz` delay slot and
  returns the callee's own `$2`; the second materialises a second zero into the
  annulled slot of a `beqzl`. `fclCombineUpdate` went from 2 of 31 words to
  exact on that. This is the `func_001C61B8` row read on a call result rather
  than on a local.
- Four same-typed parameters copied into a local array in order, all with the
  same live range, do not take consecutive callee-saved registers: which one
  lands in the *highest* one is set by their pseudo numbering, so where they sit
  in the parameter list matters even though the ABI puts them in the same
  argument registers either way. `fldDrawPoly4` and `fldDrawTexPoly4` are
  `(s32 color0, f32 x0, f32 y0, f32 z0, s32 color1, f32 x1, ...)`, one colour per
  vertex; grouped as `(s32 color0, s32 color1, s32 color2, s32 color3, f32 x0,
  ...)` they are exact apart from `$21` and `$22` swapped on `color0` and
  `color3`, and all 24 orders of the `colors[i] = colorN;` statements only trade
  that swap for a wrong store order. Floats first and interleaved both give
  retail.
- A 0x40-byte inline block move (eight `ld` then eight `sd`) whose temporaries
  retail draws partly from the callee-saved file wants the copy statement
  *above* the element-by-element fills of the other locals, not after them.
  `fldDrawPoly4` copies a face-index table into a stack local and fills four
  vertices and four colours; with the copy written last gcc splits it into a
  six-register and a two-register chunk, loses two callee-saved registers and
  16 bytes of frame, and is 126 of 131 words out. Written before the vertices it
  is one eight-register chunk and the function is 6 of 135 words out. Eight
  separate `u64` assignments and a `memcpy` of the whole array give byte-identical
  code, so the spelling of the copy is free and only its position is not.
- A symbol-plus-index load in three instructions (`lui %hi(sym)`, `addu`,
  `lh %lo(sym)(rd)`) where your C emits four, with the symbol's address
  materialised into its own register first: the base must not be reused
  elsewhere in the function. `fldDoor_ElvExit` reads a floor count and an
  elevator block out of the same table; written as
  `fldDoorParamTable.headers[i].floorCount` and
  `...headers[i].body.elevator.blocks[target]` the symbol is common to both and
  gcc materialises it once, and reading the second through `gFld_ElvId`, the
  IDB's own symbol four bytes into the same table, keeps the two apart and folds
  the first one's `%lo` into its load. The matched `fldDoor_ElvTakasaToFloor` in
  the same unit shows the four-instruction form, where the base genuinely is
  reused.
- `lb` on a field a header declares `u8` means the field is signed. On MIPS
  ee-gcc plain `char` is signed, so the original headers spell these `char`;
  `fldInfChkFastPanel` reads `fldInfSet_t.eventHit`,
  `fldInfFlagSelector_t.offTarget` and `.onTarget` and
  `fldInfMessageRow_t.targets[0]` with `lb`, and nothing else in `field/fldDraw`
  reads any of the four, so all four declarations become `s8`.
- Where retail computes a scaled index plus a constant **before** adding the
  base (`addiu rd, rd, C` then `addu rd, base, rd`), and every pointer
  spelling gives the other association (`addu` then `addiu`), the fix is an
  **explicit integer offset local**. `nbSetPlayerStartData` in
  `newbattle/nbStageProcess` reaches retail's two separate bases only as
  `s32 off = id * sizeof(fldAnahoriAkuma_t) + 0xAC0;` followed by
  `(u8 *)DDS3_GBWK + off`; `(u8 *)&DDS3_GBWK->akuma[id] + 0x70`,
  `(u8 *)DDS3_GBWK + id * sizeof(...) + 0xAC0`, the same with the sum
  parenthesised, and the `&((u8 *)DDS3_GBWK)[...]` form all compile to the
  identical object, 15 of 73 words out. Splitting the constant off the local
  (`off = id * sizeof(...)` then `+ (off + 0xAC0)` at the use) is reassociated
  back. `u32` and `s32` for the offset are interchangeable.
- A quadword structure copy with retail's `addiu` address temp plus `lq`/`sq`
  is **`SDF_VU0_COPYVECTOR(dst, src)`**, whose two `"r"` operands are where the
  temp comes from. A `*(u128 *)` cast assignment folds the offset into the
  `lq`'s displacement and emits no temp, a struct assignment of a type marked
  `aligned(16)` drops to `ld`/`sd` pairs because the block mover's widest mode
  here is `DImode`, and a `u128 *` pointer local is propagated into the load.
  `RunSake` in `newbattle/nbStageProcess` copies `form->pos` twice, in two
  blocks neither of which dominates the other, and gets one `addiu` per site.
- **A 12-byte struct whose members are all `short` is the unaligned copy.**
  `*dst = *src` between two `struct { short a, b, c, d, e, f; }` pointers is
  exactly retail's `ldl 7`/`ldr 0` plus `lwl 11`/`lwr 8` on the source and
  `sdl`/`sdr` plus `swl`/`swr` on the destination: 12 bytes at alignment 2 take
  the same path as alignment 1, where alignment 4 gives `ldl`/`ldr` plus a
  plain `lw`. `fclSetSpriteEx` in `code_001FAE80` copies a 12-byte UV rect out
  of a 14-byte record this way. The destination there is reached through a
  separate pointer local (`r = (fclSpriteRect_t *)&sprite->unk_04;`), which is
  what gives retail's `addiu $3, $17, 4` and the `7($3)`/`0($3)` offsets; a
  member reference on the containing struct keeps the base register and biases
  the displacements instead.
- **A loop-invariant address base plus a load displacement reads off the real
  field offset.** Where retail hoists `addiu $18, base, C1` and loads at
  `C2($18)`, the array field is at offset `C1` in the struct and the subscript
  carries `C2`, not some other split summing to `C1 + C2`. `lstMoveCursor` and
  `lstMoveCursor2` sit at `addiu $18, $16, 9` with `lbu 0x50($2)`: declaring
  the array at 0x09 and writing `work->unk_0009[pos + 0x50]` is exact, where
  the same byte reached as `work->unk_0024[pos + 0x35]` gives `addiu 4` with
  `lbu 0x55` and is two words out, and every `u8 *` cast spelling loses the
  hoist altogether and costs a callee-saved register.
- **A narrow parameter masked at function entry is narrow in the signature.**
  The companion to the `s32`-forwarding row above, read in the other
  direction: an `andi 0xff` of an argument register in the prologue, with the
  masked value then passed through unchanged, means the parameter is `u8`.
  Declared `s32` the mask moves to the call site instead and costs a word plus
  whatever the register shuffle drags with it. `fclDrawSpriteAlpha`,
  `fclDrawSpriteFilter`, `fclDrawSpriteFull` and `fclDrawSpriteSizeFilter` all
  closed on exactly that one change, the last two from 30 and 42 differing
  words. The `id` and `idx` parameters of the same functions are masked at the
  call site and so stay `s32`, and an incoming stack argument read with `lhu`
  rather than `lw` plus `andi` is `u16` for the same reason.
- **An indirect call whose loaded pointer retail puts in `$2`, in a function
  that cannot be `void`, is the function's return value.** Where the `void`
  lever above is blocked because a caller uses the result,
  `return ((s32 (*)(args))p->fn)(args);` frees `$2` the same way:
  `lw $2, 0x10($4)` then `jalr $2`, against `lw $3` plus `jalr $3` for a
  statement call followed by a bare fall-through. `cmpDrawSprite` and
  `cmpDrawSprite2` in `code_001FAE80` are two words out as statements and
  exact with the cast return, and `cmpDrawSpriteIndexed`/`...Indexed2`, whose
  callers ignore the result, take the plain `void` form instead.
- **Where a table's `%hi`/`%lo` pair lands among a run of calls is the
  statement position of the pointer local that holds it.** Retail computes
  `&sdfGrOtTable[ot]` at a definite point in `code_001FAE80`'s packet
  builders, and sweeping the one assignment through the statement list finds
  it: `cmpDisableRGB` and `cmpAllocRefGifTag` want it immediately before the
  use, with the subscript form written out twice instead placing the pair too
  late (26 of 43 words) and an initialiser on the declaration too early (40 of
  42); `cmpDrawSpriteIndexed` wants it after the last statement, and
  `fclDrawSpriteCore` is indifferent. Sweep the position rather than choosing
  between the local and the subscript.
- A `qsort` comparator that re-reads one operand and caches the other is
  reading both through pointers with no local at all. `cmpSkillOrder` and
  `func_00213190` in `code_001FAE80` have `lhu` of `*a` twice and one `andi
  0xffff` on `*b`, which looks like a cached `u16` local for `b`; the `andi`
  is gcc re-truncating the CSE'd load, and `*pa`/`*pb` at every use is exact
  where any `sb` local is 12 of 20 words out. Both were parked on this for
  longer than they needed to be.
- Operand order in an `addu` that sums a reloaded base with a scaled index
  follows whether the row is a named pointer or written out at every use. In a
  loop whose body calls, so the base is reloaded each iteration, retail's
  `addu $3, $17, $3` puts the offset induction variable first;
  `p = &e->data->form[i];` with `p->` uses gives `addu $3, $3, $17` with the
  operands the other way round, and so do `e->data->form + i`,
  `i + e->data->form` and `&(e->data->form)[i]`. Writing the deref out twice,
  `e->data->form[i].stat |= 0x1000;` then
  `f(e->data->form[i].basic, ...)`, is what gives retail's order. `Hontai`
  went from 1 of 61 differing words to exact on that one line, and the same
  rewrite as a `while` or `do`/`while` keeps the wrong order.
- A format string reaching `printf` as a local gets its own callee-saved
  register and a copy, where the array name written at the call is
  materialised straight into `$4`. Retail's `lui $17, %hi(fmt)` hoisted above
  three calls, its `addiu $17, $17, %lo(fmt)` far below, and
  `daddu $4, $17, $0` at the call is the tell. `nbGetMagicKaifuku` needed
  `fmt = D_00544D30;` as its own statement right before the call; the array
  name inline, `&D_00544D30[0]`, and a `char *fmt = D_00544D30;` initialiser
  at the top of the function (which hoists the whole address, not just the
  `lui`) are each one word out or worse.

- **A 4-byte tail on an inline block copy is `lw`/`sw` only when both pointer
  expressions are known 4-byte aligned, so a `void *` parameter is what forces
  `lwl`/`lwr` there.** The `Create` family in `effect/effPCPBoss` copies a
  parameter head whose size is `0x64`, `0x6C` or `0x3C4`; with the parameter
  declared `void *` the residue came out as an unaligned pair and the function
  ran 8 bytes long, and typing it as a pointer to the work struct (align 4)
  fixed `effPCPBossThunderboltCreate` and `effPCPBossKAGUInvokeCreate` on that
  line alone. The 8-byte residue is `ldl`/`ldr` either way, which is why
  `effPCPBossMETATOBeamCreate` (0x70) and `effPCPBossDengekiCreate` (0x58)
  matched with `void *`. A `u8` array field of an align-4 struct counts as
  aligned, so `memcpy(&dst.head, work->unk10, 0x54)` needs no cast.
- **`SDF_VU0_STOREVECTOR_NOMEM` is also how retail recomputes a frame address
  that the plain form makes gcc share.** Where retail has `addiu $2, $29, N`
  for the store and a second `addiu` of the same address for a later call
  argument or loop load, the `"memory"` clobber is what collapses the two into
  one pseudo, which then needs a callee-saved register. `_NOMEM` on the store
  took `effPCPBossKAGU2AttackGetSplitPoint` from 49 differing words and one
  extra saved register to 2, promoted the parked
  `effPCPDANTE_Thunder_ProcessDraw` draft outright, and removed the commoned
  `i + 0x14` that had `effPCPBossKAGU2MUHENKDraw` a word long. Try it before
  any of the address-shape rewrites: all three had been read as a duplicated
  address needing a `u128 *` local or an array form, and none of those reach
  it.
- **An argument that has to leave `$4`-`$7` before a call is copied where the
  source assigns it, so a late `addiu rd, $4, $0` is a named local.** In
  `effPCPBossKAGU2AttackGetSplitPoint` the output pointer arrives in `$4`,
  which the `SetRotateMatrixAxis` call needs for its own argument, and retail's
  `daddu $17, $4, $0` sits between the `0.25f` materialisation and the radius
  stores, not in the prologue. Using the parameter directly in the loop puts
  the copy first and rotates 45 words; `point = out;` as its own statement at
  that position is exact.
- **Two results of the same allocator call in one function need two locals.**
  Reusing one `u32 id` for two `dds3AllocObjectId()` calls makes gcc keep the
  second result in the callee-saved register the first one earned, which costs
  a `daddu rd, $2, $0` and, where the function is already at nine callee-saved
  registers, spills a parameter to the frame.
  `fldEveHit_AddEveHitItemBox` in `field/fldDraw` is 512 of 504 bytes with one
  `id` and the frame 16 bytes too large; two locals give retail's 0x70 frame
  and 93 of 126 words straight away. This is the mirror of the
  `itfPnlRegist2` row above, where the reuse is what retail has, so read which
  shape retail is in before choosing: the tell for two locals is the second
  result being used out of `$2` with no copy.
- **A record-array field fill is per-field subscripts for the small blocks and
  one pointer local for the big one.** Where retail forms the scaled index once
  and then a separate `addiu rd, symbol, K`/`addu` pair per group of fields,
  `D_002FCFA0[i].ball.type = 0;` style subscripts give exactly that for a
  group of one or two words, but a run of twelve floats written the same way
  materialises `symbol + offset` for every field: `fldEveHit_AddEveHitNpc` in
  `field/fldDraw` is 136 excess bytes with all subscripts and 84 bytes *short*
  with a single `fldEveHitSource_t *rec` for the whole record, because then one
  base register serves everything. A pointer local for the 0x30-byte transform
  block and subscripts for the rest is the exact size. The pointer's assignment
  position inside the block is inert; which group it anchors is not, and the
  first address pseudo gcc forms becomes the anchor every other one is derived
  from, so a pointer taken before the first subscript moves the whole set.
- **A function whose result the caller ignores wants `void`, because `$2` is
  then free for an index chain.** `fldEveHit_AddEveHitNpc` ends by scaling
  `fldEveHit_AddEveHit`'s return by `0x130` to index `fldEveHitTable`; retail
  does that in `$3` and leaves the value in `$2`, which only happens when
  nothing has to be returned. Declared `s32` with a `return`, gcc copies the
  value out of `$2` first and the function is one instruction long. This is the
  companion to the `jalr $2` row: there the cast return frees `$2`, here the
  `void` does.
- **Where retail emits the same address twice, once long-lived and once
  rematerialised, the second one is not a second C pointer.** Three functions
  in `field/fldDraw` show the shape: `fldEveHit_ChkDir2` keeps the frame
  vector's address in `$18` for the `memset` and the `sq` and rematerialises
  `addiu $3, $29, 0x10` for the `lqc2`; `fldPlayerFastSet` keeps `lui $18,
  %hi(fldGlobalBase)` for the whole function and emits a fresh `addiu rd, $18,
  %lo(fldGlobalBase)` in each basic block. A second pointer local assigned from
  the first gives a `move` instead of the fresh computation, which is the right
  instruction *count* in the same place but the wrong instruction, and
  initialising that pointer at its declaration instead lets gcse fold it back
  into the first and loses the instruction altogether. For the asm-macro case
  `SDF_VU0_LOADVECTOR_RWADDR` on a pointer assigned at the use site is what
  gets the count right. `-fno-cse-follow-jumps` restores the per-block `%lo` in
  `fldPlayerFastSet`, which puts the mechanism in cse extending its block
  through the conditional branch, not in anything the source can state: the
  pseudo is separate in retail only because it is dead before the first `jal`
  and so lands in a caller-saved register.
- **A quadword copy out of a `f32 *` into a stack vector is `*(u128 *)`, not
  `*(Quad *)`.** `Quad` is four `u32` at alignment 4, so gcc copies it with
  `lq 7`/`sq` style unaligned pairs and six extra words; the `u128` cast gives
  retail's single `lq`/`sq`. `fldEveHit_ChkDir2` copies `hit->workdata` this
  way. The `SDF_VU0_COPYVECTOR` row above still applies where retail wants an
  `addiu` address temp; use `u128` where the store is at a plain frame offset.
- **`addu base, off` against `addu off, base` in a byte index is the offset's
  own local.** Every spelling of the index expression gives the offset first:
  `p[(sz * j + i) * 2]`, `*((u8 *)p + ...)`, `*(u8 *)((u32)p + ...)`,
  `2 * (...)`, `(i + sz * j) * 2`, and a two-byte struct array indexed by the
  unscaled value all emit `addu rd, off, base`. Assigning the byte offset to a
  named `s32` first, `offset = (sz * j + i) * 2; p[offset]`, gives retail's
  `addu rd, base, off`; a `u8 *` local holding the whole address does too.
  `FroFtoI` was one word out on this alone.
- **A global pointer read inside a loop beats caching it, second case.**
  `FroFtoI` writes `((fldRunMapGrid_t *)pFroMapHead)->sizeX` at all three uses
  (the inner bound, the row multiply and nothing else) and
  `(u8 *)fldRunMapScale` at the one use, and comes out frameless with the two
  counts reloaded every iteration the way retail has them. A `grid` local plus
  a `cells` local, both initialised at their declarations, makes gcc hoist both
  counts into registers, which costs one callee-saved register and a 16-byte
  frame: 79 of 80 differing words against 1. Converting the two loop-invariant
  `s16` globals to `f32` at the top as named locals is the other half of it:
  `f32 ox = fldRun_MAP_OX;` gives retail's `lh` plus `mtc1`/`cvt.s.w` in the
  prologue, while leaving them inline keeps them integers and spends the
  registers on `lhu` plus `sll`/`sra` pairs instead.
- **An `s32` cast has to sit inside the unsigned shift, not around the sum.**
  Where retail has `srl` for a shift and a signed `%` on the enclosing sum,
  write `(s32)(((u32)f() >> 4) & 1)`. Without the inner `(u32)` the shift is
  `sra`; without the outer `(s32)` the whole sum is unsigned and `% 4` collapses
  to `andi 3`. `fldRunDanteSe` needed both, on an implicitly declared
  `dds3GetRandA`; giving that callee its real `u32` prototype loses the signed
  modulo and is 60 of 169 words out.
- **Where one frame vector is both a VU0 store target and a later call
  argument, the store is `SDF_VU0_STOREVECTOR_NOMEM`.** The plain
  memory-clobbering store lets cse coalesce its address pseudo with the
  long-lived one the calls use, so the `addiu rd, $29, N` is emitted once;
  retail emits it twice, once dead into a caller-saved register for the `sqc2`
  and once into the callee-saved register the calls read. `fldRunTamaInit`
  stores the rotation quaternion and then passes `&rot` to three
  `dds3AddUnitObjectBasic` calls, and went from 210 of 248 differing words to
  exact on the macro alone. A second `sdfVector4_t *` local for the calls does
  not reach it, and neither does splitting the `dds3AllocObjectId` call out of
  the argument list.
