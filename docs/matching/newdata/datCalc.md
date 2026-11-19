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

Matches all 142 words at the native 568-byte extent, including the 128-byte
frame. Independent complete-function review accepts the native behavior,
localized constraints and evidence-backed live array capacity.

### Rate/chance phases and live array capacity

The composition separates the initialized `rate` converted from the reobserved
`badlevel` byte from the resulting `chance`. The existing
`FAKEMATCH_ASM_LIVE_F32(rate)` constrains a real initialized input consumed by
both probability arms. Division by 100, affinity multiplication, multiplication
by 100, and the flagged arm's optional 0.7 remain in native order; they are not
algebraically simplified. The existing `FAKEMATCH_ASM_LIVE_MEM` is a volatile
empty `+m` operand on the actual initialized badlevel byte, not an input-only
operand, runtime store or general memory barrier. It prevents reuse of the
sentinel observation so the subsequent real reload is retained. It does not
assert an intervening callback write. No new attribute or compiler flag is used.

With the 12-element array, the rate/chance split removes the interior
instruction differences but leaves 18 frame/save/restore immediates differing
by 16 bytes. The composition uses the already tested 16-element capacity of
the live `s32` candidate array, not a separate unused padding object. The
[pinned reconstructed-HD function](https://github.com/smt-ret/smt3-reconstructed-sources/blob/a03e0353e335f94a74985477d87df4b24aa331cd/src/datCalc.c#L2136-L2341)
explicitly allocates `NewSpecific(s32, 0x10u)` before the same dense status-bit
selection. The pinned [generator's `NewSpecific` handling](https://github.com/smt-ret/smt3-reconstructed-sources/blob/a03e0353e335f94a74985477d87df4b24aa331cd/tools/smt3/smt3dump.py)
interprets its second integer argument as an element count and removes TypeInfo
suffixes. This is reconstructed port evidence, not recovered original PS2
source or independent verification of the original HD allocation helper. The
[generator input notes](https://github.com/smt-ret/smt3-reconstructed-sources/blob/a03e0353e335f94a74985477d87df4b24aa331cd/tools/inputs/README.md)
identify original binary/decompiler inputs supplied separately.

The capacity evidence and a 16-element diagnostic were already recorded in
November 12 work; this is a new composition with the rate/chance split, not a
new discovery or a capacity sweep. The scan still visits only indices 0 through
11 and initializes only the selected prefix. Capacity 16 introduces no reads,
stores, initialization, pointer escape or changed RNG/loop bounds. Native
candidate stores occupy SP+0 through SP+0x2C and saved registers start at
SP+0x40. That frame is compatible with the capacity, but does not independently
prove the original allocation or spelling.

### Semantic scope and earlier nonmatching controls

All 288 current normal records were inspected; the 12 random-status records
have nonempty low-12-bit masks. This is not a universal table-writer or
valid-index proof. The zero-count native path advances RNG state before its
divide trap; no early return or invented initialized selection is introduced.
Portable modulo-zero semantics and resumption after an exception handler are
not claimed. Existing well-formed work/table object contracts remain; source
and target may alias. Current providers do not mutate those objects, and
badtype and final target-status observations retain their native phases.
Bounded low-16-bit affinity and u8 level keep chance finite and within s32
range. Conversion follows the pinned EE compiler behavior, not a host rounding
claim. The final status comparison remains numeric ordering, not a substituted
bit-overlap test.

Earlier guarded reconstruction reached 103/142 words at 568 bytes with a
112-byte frame and 12-entry array. Its ordinary indexed observations, late
success flag, zero-default affinity and promoted integer snapshot replaced an
earlier pointer/volatile formulation. A separate shared-float alternative
produced 560 bytes, 78 equal words and eight uncovered native bytes. Qualified
unmodified dumps retained its second badlevel load through local allocation,
then deleted it at global allocation/reload; conversion reused the first byte.
The retained integer-snapshot alternative had two exclusive conversion sites
and different float allocation. Neither explained the native frame. Those
negative results are not superseded retroactively by this composition.

The earlier guarded reconstruction's production/retained checks, plain/dump
parity, retail comparison, regression and draft-suite passes remain historical
results for that source. Its inherited `memset` diagnostic was not a
warning-clean claim. They are not qualification of the new composition.
The initial isolated result resolved all 11 relocation/literal sites by
finite attribution, with 81 peer bodies, allocated nontext and ordered
relocations unchanged. Fresh production owner checking subsequently reports
81 matches and zero differences. Production, SKIP_ASM and retained-C contexts
have literal assembly and raw ELF parity across plain, dump and strict builds.
Peer code and data contracts are preserved, including explicit attribution of
SKIP local-call and jump-table shifts rather than a raw-equality claim. Current
guarded-draft and IRX checks, regression suites, generated-source positive and
stale controls, literal retail comparison, checksum and no-op build pass.
These are checks of this source, not reuse of historical guarded-source gates.

## datSetAutoKaifukuSkill (newdata/datCalc, 0x140)

All 80 native words match at 320 bytes. A real typed base for the special
skill table precedes the unsigned slot offset; the two views resolve to
the actual signed halfwords at offsets zero and two within the selected
record. Both loads precede case selection. The function still checks skill
ownership first and dispatches HP-only, MP-only, and dual recovery as before.
The real unit-work pointer stays in a callee-saved register across its
calls. No empty asm operand is required.

Only positive signed divisors are used. The unsigned HP and MP maxima and
both positive results are sampled or calculated before recovery calls; HP
is applied before MP. Callbacks may change the work or table afterward
without changing the already calculated MP amount. Valid successful indices
remain 288 through 382. Known callers use 346 through 350.

Earlier guarded experiments reached 61/80 by putting the dual-increment
arm first. The complete landed owner retains those exact table destinations
and all previously exact peers. Earlier typed-base controls that collapsed
both address views shortened the code and are not retained.
