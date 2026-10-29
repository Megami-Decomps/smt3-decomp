# A caller can need its callee's definition, not just a prototype


Measured in `mc/slMain`: `slDrawInfo` at 183 of 189 words and `slDrawError`
at 293 of 299 both become exact the moment `slDrawWindow` is a C definition
above them, and while it is still `INCLUDE_ASM` they each emit six `bnel`
where retail has `bne`, on exactly the tests whose arms call it. Cracking one
callee therefore banked 801 words across three functions.

So when a caller sits within a couple of branch-annul words of exact and
calls something still in assembly, decompile the callee first rather than
permuting the caller.

It is not universal. An agent on `field/fldDraw` tested it directly, reverting
a converted callee to `INCLUDE_ASM` while changing nothing else, and its
caller still compiled exact; no near miss in that unit emitted a `bnel` for a
`bne` that a C callee fixed. Try it, but measure rather than assume.

It also runs the other way, which is worth knowing before you convert
something. `nbMaeCheckDone` in `newbattle/nbMakePacket` is **byte-exact on its
own** and is parked behind a `NON_MATCHING` fallback anyway, because compiling
it turns a neighbouring function's `bnel` into a `bne`. A matching function
can cost more than it earns, so check the whole unit's result after adding
one, not just the function you wrote. This is the same mechanism as the CONTEXT row below,
seen from the other side: what the compiler can see of the rest of the unit
changes the code it emits for one function.

A function retail calls `main` cannot be written as C named `main`: cc1 compares
the declaration's name string in `expand_function_start` and emits a leading
`jal __main` for it, which is two instructions retail does not have. Name the C
function something else and carry the retail name with an assembler alias,
`s32 sdfDefaultMain(void) asm("main");`, declared above the definition.
`sdf/sdfDefaultMain` is exact that way and 8 bytes long without it.

## A `void` callee frees `$2` for the next pseudo

The register the allocator gives the pseudo after a call depends on whether the
callee's declaration returns a value: a value-returning call keeps `$2` out of
the way even when the result is dropped, so the next address temp lands in `$3`.
`RunUgou` in `newbattle/nbE901` was two words out, `lui $2, %hi(text1)` against
retail's `lui $3`, until `nbCameraBoss_Sp_DOUTYO_TP_Set` was declared
`extern s32` instead of `extern void`; an unprototyped `extern s32 f();` does it
too. Before chasing a lone base-register difference after a call, check the
callee's declared return type.

The byte-returning font-mode provider supplies another measured case.
`frEnableMode(s32)` returns the previous shared flag byte as `u8`: native loads
it into `$2`, ORs the mask into `$4`, and returns with the byte store in the
delay slot. Its corrected C definition and all callers remain byte-identical.

In `facility/fclEncyc`, the ordinary `CreateFontExplain` body matches all
392 bytes with that return contract. A control changing only its declaration
to `void` still emits 392 bytes but differs in eight of 98 words: the race-table
index takes `$2` and the address/result takes `$3`, opposite retail. Qualified
`cc1probe --alloc --no-dumps` runs preserve hooked/unhooked output in both forms.
The value-returning call reserves `$2`, so the exact form places the following
index in `$3`. This example uses the provider's native byte result; return
types must follow the actual contract.

## One callback local for sequential table submissions

`pb/pbGame`'s `pb_proc_block_DB_Process` submits two tags through adjacent
ordering-table nodes. Direct `ot->regFunc(ot, tag)` expressions emit the native
740 bytes but differ in the second target load and indirect call: `$3` instead
of `$2`. Qualified `cc1probe --alloc --no-dumps` traces show two local quantities
with overlapping allocation ranges, `[8,18]` and `[16,26]`.

Reloading each callback into the same ordinary function-pointer local before
calling it matches all 740 bytes. Its four real references form one global
pseudo, with eight live units and no crossed calls, allocated to `$2`. The
callback is still fetched afresh for each node. This is value reuse across
sequential calls, with no dummy references or fixed-register constraint.

The same return-value reservation closes `code_0022C578::cmpUseSkillTokusyu`
at all 228 bytes. Its declaration had `void datExecSkill(u16, ...)`, while
`newdata/datCalc` defines `s32 datExecSkill(s32, ...)` and returns zero or one.
Correcting the caller declaration keeps the switch root constant 0x4A in
`$3`, matching retail; the void declaration puts it in `$2` and differs in
that constant load and its branch operand. The caller discards the result.
The other camp caller, `cmpExecRecover`, remains exact with the real contract.

`code_0022C578::camp_update_stock_Process` supplies a forward-declaration case.
Its first `cmpUpdateCursorSlots` call had an implicit integer result despite
the later definition and other consumers establishing a void contract. The
following global reload used `$3` instead of retail's `$2`, leaving two of
35 words different. A truthful void declaration before the caller matches
all 140 bytes; the updater remains assembly-backed under its existing guard.
The complete owner also matches with the process callback and data accessor
typed with `dds3ProcessID_t *`.

`pb/pbGame::gameKey1` combines a real data split with a provider correction.
The symbol map already exposes two signed-halfword cursor globals, so source
must use both rather than one combined record. At native 680-byte size, six
remaining register words after sound calls close with `void p2sdPlaySE(...)`,
as defined by `sdfSound`. The key mode result and action input are `u8`, as
defined by `pbKeyConv`. Initialize the movement mask to zero and set its four
bits conditionally; the first bit is `y < 19`. That ordinary mask-building
form closes the final three operand/conditional-move differences.

## Main battle diagnostic rendering

`nbDebugMainProcess` replaces the guarded reconstruction in `9c75db2e` with
2,672 exact bytes. The real unsigned-coordinate print API, a live vertical
position advanced between rows, and direct typed action-stack field reads
preserve the retail callback boundaries and schedule. Floating variadic
arguments use ordinary default promotion and the existing `fptodp` alias.
The alias target retains its provider's `long` declaration: it returns the
64-bit double representation, not a numeric integer conversion. Shared small
strings stay external while the other format literals belong to this owner.
