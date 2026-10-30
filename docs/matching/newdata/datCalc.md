# Matching notes: newdata/datCalc

## datGetPlayerParam (newdata/datCalc, 0x50)

7 of 20 words differ, exact size. The structure is right and the remaining
difference is which register holds the hearts bonus. Retail puts it in `$2`:
the `beqz` that skips `cmbGetHeartsParam` carries `daddu $2, $0, $0` in its
delay slot and falls into a single `subu $2, $16, $2`, so the zero and the
call result share the return register and the `$gp`-relative `DDS3_GBWK` load
has to avoid `$2` and lands in `$3`. Every shape tried either puts the bonus
in `$6` with a copy after the call (12 of 20) or duplicates the `subu` and the
epilogue into both arms (88 bytes against retail's 80).

Ruled out: `bonus = 0` before the `if`; `if`/`else` in both polarities; a
ternary at the return and in a local; a `goto` into a shared tail from either
arm; duplicating `return param - bonus` in both arms to invite cross-jumping;
`hearts` as `u8`, as `s8` and as an `s32` with the cast at the read; and all
36 combinations of declaration order and initialisation order of the three
locals.

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
