# Matching notes: newdata/datCalc

## datGetPlayerParam (newdata/datCalc, 0x50)

13 of 20 words match at the native 80-byte size; the target remains guarded.
The earlier ordinary trials highlighted the hearts-bonus register; the
qualified controls below also expose a call-argument dependency. Retail puts
the bonus in `$2`:
the `beqz` that skips `cmbGetHeartsParam` carries `daddu $2, $0, $0` in its
delay slot and falls into a single `subu $2, $16, $2`, so the zero and the
call result share the return register and the `$gp`-relative `DDS3_GBWK` load
has to avoid `$2` and lands in `$3`. The historical shared-tail forms put the
bonus
in `$6` with a copy after the call (12 of 20) or duplicate the `subu` and the
epilogue into both arms (88 bytes against retail's 80).

Ruled out: `bonus = 0` before the `if`; `if`/`else` in both polarities; a
ternary at the return and in a local; a `goto` into a shared tail from either
arm; duplicating `return param - bonus` in both arms to invite cross-jumping;
`hearts` as `u8`, as `s8` and as an `s32` with the cast at the read; and all
36 combinations of declaration order and initialisation order of the three
locals.


### Qualified return-value and argument controls

The original address spelling manufactures a pointer from the integer index.
The guarded cleanup instead indexes the actual global work object's byte
representation using `(s32)((u32)id + 0xA6Au)` as its byte offset. The unsigned
sum wraps modulo 2^32 without signed overflow. Conversion to `s32` is
implementation-defined in ISO C; the pinned compiler retains the low-word
interpretation. This avoids a new signed-overflow assumption for unchecked
script integers, not invalid-address behavior. It preserves the 13/20 score
and native signed byte access, though the address addition operands commute. The genuine player
parameter field is six bytes at unit offset `0x16`, with the player unit at
work offset `0xA54`. Known native callers use indices zero, four and five;
the script wrapper forwards an unchecked integer, so no new bounds or safety
promise is inferred. No adjacent helper is changed.

An initialized bonus held in the native return register, assigned zero in the
no-call arm or the actual call result in the other, emits 76 bytes: four fewer
than retail. It does not keep a caller-clobbered zero live across the call.
A direct typed parameter-field control also emits 76 bytes. Both preserve the
79 existing C functions and owned data. Ordinary bonus and shared-subtraction
source emits 80 bytes with nine matching words; genuine signed-argument-to-
result scalar reuse emits 88 bytes and is not retained.

The qualified if-conversion dump moves the zero assignment before the branch.
The signed call-argument conversion is still in the call arm at that stage;
later scheduling and delay filling place its shifts into the branch and call
slots. Native instead converts before the branch, uses zero in the branch
slot and leaves the call slot empty. Thus a return-register assignment alone
does not recover the native dependency.

A final control captures one unsigned hearts byte, derives its real signed
call argument, and places a localized volatile operand on that argument before
the guard. It emits 88 bytes, eight too many. Initial RTL through life has one
field observation and a register conversion, but combine replaces those
consumers with separate signed and unsigned loads of the same field. The
provider argument is also narrowed again after the opaque operand. The zero
and empty-call slots are recovered, but the extra accesses and size are not
native. This is a compiler-created duplicate load, not two source observations;
no operand from this control is retained and the family is retired.

Plain and diagnostic builds qualify all five allocated sections, non-file
symbols, 359 ordered relocations and normalized assembly for each control.
The minimal guarded address cleanup also has identical output with
uninitialized-value diagnostics; the owner's existing `memset` built-in-type
warning remains. At the time of these controls, strict implicit-declaration
diagnostics rejected the production/SKIP_ASM baseline and cleanup for
`datCheckSyojiSkill` and `datAddHearts`: their existing prototypes were guarded
by `NON_MATCHING`. The address cleanup did not change those declarations;
the later declaration correction is recorded below.
The final explicit low-word offset has identical whole-owner output to the
earlier signed-offset cleanup. Diagnostic flags are not production settings.
These observations locate the unresolved argument/lifetime boundary, not an
instrumented allocator decision or proof of original source spelling.

## datGetSkillKouka (newdata/datCalc, 0x3F8)

254 of 254 words match in `9b5853f0` (1016 bytes). The earlier private
ordinary-C draft was 243/254
words exact; that residual is retired. A localized empty read/write constraint
on the real selected dispatch type holds it in `$5` immediately before the
switch. The binding ends before any case calls. Ordering the existing type
and current-value initializations resolves the remaining two scheduled words.
This target-specific allocation aid is not a claim about original source
spelling; the ordinary bridge without the constraint remains nonmatching.

The complete owner passes 78 match, 0 differ. Both dispatch tables, the exact
jitter literal `0x3d4ccccc`, full owner text, rodata and lit4, and the complete
retail image were verified. Actual published source receipts and the next
no-op build also pass. `datGetPlayerParam` above remains unresolved.

## datBossRashGetRec (newdata/datCalc, 0x108)

66 of 66 words match at 264 bytes. In the `type == 0` branch, an ordinary
local result starts at 999 and is replaced by the signed 16-bit record when
`best > type`. The dominating branch proves that comparison equivalent to
`best > 0`; other type values remain unrestricted. The default has a genuine
return consumer when the stored record is nonpositive. No dead initializer,
artificial consumer, compiler constraint, type or provider change is added.

The original singleton emits 268 bytes with 17 differing words and retains
999 in an extra saved register. The branch-local result and native comparison
operand produce the exact 264-byte output. This describes observed output,
not an unverified internal optimization mechanism. The type-one callback,
its subsequent global reloads, the other-type return and loop behavior stay
unchanged.

Independent semantic review and canonical complete-owner checking pass
79 match, 0 differ, with owned data exact. Review also exhaustively compares
the changed type-zero selection over all 65,536 signed 16-bit record values;
this is a bounded selection check, not a whole-function execution proof.

Combined local gates pass the retail checksum, 26 tooling test files / 427
cases, all 147 guarded owners / 537 drafts with stable source/compiler inputs,
seven IRX owners, and reconciled production reports.

## Unconditional forward declarations

The existing `s32 datCheckSyojiSkill(datUnitWork_t *, s32)` and
`s32 datAddHearts(s32)` prototypes now apply in every build, rather than only
under `NON_MATCHING`. They exactly match their same-unit providers. Native
skill comparison uses the full integer argument and returns 0 or 1. Hearts
insertion likewise compares the full ID before its byte stores and returns
0 for an existing entry or 1 for an insertion. Ignoring that return in
`datAddItem` does not make the provider `void`.

Only `datGetMaxHp`, `datGetMaxMp` and `datAddItem` gain declaration visibility
before their calls; their existing pointer/integer arguments require no new
conversion. Function bodies, fallback guards, call targets, provider types,
headers and flags are unchanged. Production and SKIP_ASM previously failed
`-Werror-implicit-function-declaration`; NON_MATCHING/SKIP_ASM already passed.
All three complete-owner modes now pass this strict check. The separate
`memset` built-in-type warning remains, so this is not a warning-clean claim.

Paired baseline/corrected builds preserve all five allocated sections,
normalized assembly, ordered non-file symbols and ordered relocations in each
mode: production has 415 symbols / 358 relocations, SKIP_ASM 388 / 334, and
NON_MATCHING/SKIP_ASM 414 / 355. Plain and strict-diagnostic candidate output
also agrees in every mode. Complete retail owner checking remains 79 match,
0 differ, with owned data unchanged; all three guarded drafts still compile.
This is an ordinary truthful declaration correction, with no new matching
credit or compiler-constraint dependency. The paired negotiation-owner
correction is recorded in [NbNegoFlowTwo](../newbattle/NbNegoFlowTwo.md).

## datGetSkillBadKouka (newdata/datCalc, 0x238)

103 of 142 words match at the native 568-byte extent.

The draft remains behind `NON_MATCHING`. Target-only compilation produces the native 568-byte extent, with 103 of 142 words equal; the production owner remains 79 matches / 0 differences. Equal total extent does not mean an exact frame: native reserves 128 bytes, while this draft reserves 112. Its 12-entry candidate array is unchanged; no unexplained padding or larger capacity was invented.

Ordinary indexed observations, a late success flag, zero-default affinity, and a real promoted badlevel snapshot replace the earlier pointer/volatile formulation. The source-side flag branch is live: only one arm applies 0.7. Both arms preserve division by 100, affinity multiplication, multiplication by 100, and optional 0.7 in that order. RNG100 still precedes the native CVT.W.S comparison; badtype and target status are observed afterward. The exact owned 0.7 cell is represented by its literal.

Dense enumeration initializes every selected candidate for nonempty 12-bit masks. All 288 initialized normal records were audited; the 12 random-status records have nonempty masks. The inspected dynamic normal-skill writer does not alter their badtype/status/level fields. This is not a global table-writer or valid-index proof. The native zero-count path calls the state-mutating RNG before its divide trap; the draft does not invent a safe return, and portable C modulo-zero semantics or exception-handler recovery are outside the proof. Valid work pointers and normal-table indices remain caller preconditions. The bounded low 16-bit affinity and u8 level ensure the chance conversion is finite and in signed 32-bit range on those inputs.

A separate ordinary shared-float alternative produces 560 bytes, 78 equal words and 8 uncovered native bytes. Its qualified unmodified compiler dumps preserve the second badlevel load through local allocation, then show its deletion by the global-allocation/reload stage; the conversion consumes the first byte value instead. The retained integer-snapshot draft has two exclusive conversion sites, one executed per path, with different float allocation from native. Neither alternative reconstructs the native redundant reload or explains the 16-byte local-frame difference. No constraint, false volatile, compiler flag or ownership change is retained. Original source spelling is unknown.

Final production and target-active plain/dump output parity, full-owner/data checks, and strict retained-C checks are recorded in the review packet. The inherited `memset` built-in diagnostic remains; this is not a warning-clean claim. Independent review approved this guarded reconstruction. Final owners, three strict modes, production/retained plain-dump parity, retail byte comparison/checksum/no-op, 450 tests, 817 layout assertions, fresh stable 141-owner/481-draft checks and seven IRX checks pass. Reports remain unchanged. No new matching credit is claimed.

## datSetAutoKaifukuSkill (newdata/datCalc, 0x140)

61 of 80 words match at the native 320-byte extent; the draft remains guarded.

The retained `NON_MATCHING` draft puts the genuine dual HP/MP recovery arm
(0x15C) first, matching the native block layout, ahead of HP-only (0x15A/0x15D)
and MP-only (0x15B/0x15E). Both divisor guards, independent initialized outputs,
pre-switch signed table observations and shared HP-then-MP call tail are unchanged.
This is an ordinary source reconstruction, not evidence of original spelling.

On the 2026-11-03 owner, activating only this target improves from 42/80 to
61/80 matching words at the exact 320-byte extent. All 79 existing exact code
peers remain exact. The five target jump-table destinations now match; the
complete 24-byte table-plus-zero region, all 231 emitted owner rodata bytes,
the final one-byte native alignment pad and all 60 owned literal bytes are
qualified against retail. The extra native MFLO reads arise naturally, without
dummy operations. Saved work/skill register allocation, table addressing/load
schedule and the final MP branch still differ. The fallback remains assembly;
this change receives no exact matching credit.

An ordinary explicit typed special-table base and a paired empty operand
constraint on that real initialized pointer were negative controls. Both
shortened the function to 312 bytes and disturbed table destinations. Neither
is retained; no register or arbitrary case-order sweep was used.

For defined C accesses, successful skill ownership must still be accompanied
by a valid special-table index (288 through 382), a valid work object and
bounded skill inventory/devil indices. Identified field/battle callers use
346 through 350; this is not a claim of global writer or arbitrary-call closure.
The ownership check does not itself prove the index bounds. Both signed 16-bit
parameters are read before switch gating, including unrecognized but owned
special skills. Positive denominator guards and unsigned 16-bit maxima make every
reached quotient defined and every positive HP/MP addition fit signed 32-bit.
Truthful existing s32 provider returns and native call ordering are preserved.

Complete-owner ordinary and unhooked diagnostic dump outputs agree in normalized
assembly, allocated sections, symbol contracts and ordered relocations. This
is pass-snapshot/output-parity evidence, not a GDB or hooked-decision claim.
Production, SKIP_ASM and NON_MATCHING/SKIP_ASM strict-declaration modes compile,
and each has full plain/dump parity; the guarded change has identical production
and SKIP_ASM output to the starting owner. The inherited memset built-in declaration warning remains; no warning-clean
claim is made. Final integrated owner 79/0, target-only 61/80, all three strict-mode
plain/dump comparisons, retail checksum/byte comparison/no-op, 450 tests,
817 layout assertions, fresh stable 141-owner/481-draft qualification and
seven IRX checks pass. Reports are unchanged.

### Shared-tail allocation frontier

A separate ordinary shared-tail diagnostic initializes a signed bonus to zero,
conditionally replaces it with `cmbGetHeartsParam`, then subtracts it from the
captured parameter. Its shared subtraction exists in initial RTL and survives
combine and if-conversion; the latter converts no blocks. This differs from
the retained early-return draft, whose zero-hearts path already bypasses the
subtraction in initial RTL.

The shared-tail diagnostic's extra post-call copy is present before scheduling
and survives allocation. Local allocation assigns the global-work base and
parameter address to `$2`; global allocation reports a `$2` conflict for the
bonus and assigns it to `$6`. The zero initialization is live before the base's
last use, so simply coalescing the bonus with the call return cannot remove the
copy in that allocation. This identifies a lifetime/allocation frontier, not
an if-conversion failure or proof of the allocator's original historical choice.
The diagnostic remains nine matching words at 80 bytes and is not retained.

Complete-owner assembly and raw-object equality between plain and `-da` builds
passes in production, `SKIP_ASM`, and `NON_MATCHING` plus `SKIP_ASM` contexts.
These six successful compiles qualify the observations, not a new exact match
or a semantic approval of unrelated drafts. The retained target stays 13/20.
