# Matching notes: field/fldMain

## fldReadyScene (field/fldMain, 0x594)

357 of 357 words match at 1428 bytes. Mode-2 title transitions use the actual
short-circuit OR of previous/current area pairs: 40->{41..45}, 41->{40,42},
42->{40,41,43}, 43->{40,42,44}, 44->{40,43,45}, 45->{44,40}. Right-nesting
the equality checks for 40, 41 and 43 preserves the logic without the
adjacent-range sequence emitted for the left-nested form. Both range subtractions cast to unsigned first, preserving native
modulo-32 behavior without signed overflow. Tests cover 41,209 previous/current
pairs drawn from [-100,100] plus INT_MIN/INT_MAX, not every possible input pair.

Previous-area lifetime is separate from later city predicates, which observe
globals freshly. All four restore-mode calls remain distinct, especially the
one after the mode-2 title callback; it cannot become a cached result or else
arm. Resource-reuse state remains captured before evtLoadArea, while scene
mode after dds3SetActiveScene, encounter fields after evtBitCheck, and camera
state after unit/pad/follow callbacks retain native fresh observations.
Ordinary camera branch stores avoid ternary movn lowering. Null-resource
fallback and final damage-counter clear remain unchanged.

The point pointer always names a real fldResourcePoint_t: resource/default,
the genuine safeUnit union member during rotation, or aligned adjustedPoint
for unit setup. Qualified paired compiler evidence establishes the effect of
this live phase reuse independently of the constraints below. Separating only
the safeUnit phase, while retaining its actual operand constraint, leaves the
same 1428-byte size but 83 differing words. Final RTL pseudo84 spans all phases,
survives local allocation and maps to hard17 globally. The control allocates
safePoint pseudo85 locally to hard16 and remaining point84 globally to hard19.
Each source separately passes normalized plain/dump full-owner assembly parity
and section-header/byte identity except debug path contents, including allocated
sections, relocations, symbols and function inventory. These are observed
allocation facts, not historical variable spelling or a guessed allocator rule.
Every pointer definition has a truthful type and real consumers.

Two local constraints address different residuals. Empty `+r(point)` after
the genuine VU rotation store retains the containing-point base for following
scalar reads; removal changes only +0x180 from `lwc1` via point+0x10 to the
same subobject via rot+0. It emits nothing and is not a memory barrier; the
actual VU store has its own memory clobber. The initialized firstArea value 32
is bound to `$6` for the first two city comparisons, with an empty `+r`
keeping it separate from the later independent literal 32. Unbinding it changes
seven words. Both constrained values have actual consumers and are dead before
any clobbering call. Earlier point lifetimes across rotation helpers are
compiler-allocated callee-saved values, not pinned caller-clobbered storage.
No ABI alias, provider change, fabricated layout or scalar opcode replay remains.

Independent complete-owner verification gives 51 match, 0 differ, preserving
all 50 peers and owned data. Existing rodata is unchanged; two native rotation
sites account for eight emitted literal bytes, with linked placement verified
by the full image gate. Full local gates passed for source
`01513b8f9b69342cf6c44e35e864848ce3ae5316`. Exact-source CI
[41060045539](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41060045539)
completed successfully, including every actual step and retail checksum.

## fldAnahoriSelectInit (field/fldMain, 0x7C)

31 of 31 words match at 124 bytes, excluding the following padding word.
The existing 16-record walk and four flag gates remain unchanged: bit 0 must
be set, bits 2/4/11 clear, and unsigned u16 hp nonzero. Only then does the actual
count increase. The real next count is locally bound to $4 and immediately
consumed by n = next. Counts remain 0..16 and live next values 1..16, so signed
addition cannot overflow. There is no fabricated zero, source XOR identity,
dummy consumer, empty operand, altered bound, type or array owner.

Native $4's prior flag value is dead after all four gates. The next-count
addiu at 0x123948 feeds movn at 0x123954 without any call or clobber in between.
The whole function is call-free and frameless. Speculating this harmless scalar
addition before the hp-dependent native MOVN is valid; the source computes it
only on the nonzero-hp path. The binding is a local allocation aid, not a
caller ABI requirement or claim of original source spelling.

Real-header EE assertions verify the complete work size 0x1A510, akuma base
0xA50, its exact 16 * 0xEC extent, record size 0xEC, u32 flag at +4 and u16 hp
at +0xA. Native loads the work pointer once, reads each flag in record order,
and performs hp LHU only after those gates pass. Stores remain index=0,
maximum=0, then final maximum=count in the return delay slot. No memory-order,
width, ownership, or header change is introduced.

The original direct increment and ordinary next-count local emit 120 bytes:
20/30 overlap words match, with four native bytes uncovered. Real hp bindings
and operands retain that result. A real next-count empty operand reaches 124
bytes but only 21/31 words match. The next-count binding and both comment-cleaned
final forms are exact. All eleven guarded/original/hp/increment/final complete
owners are freshly compiled in three modes: 33 entire-assembly/entire-ELF
plain/-da pairs agree after only one equal-length temporary source filename
substitution. Ordinary/dump diagnostic logs are empty.

The parity-qualified CE dumps reproduce the limited observation: original and
ordinary next-count forms contain no XOR-zero and report zero converted IFs;
bound and final forms contain one xor:SI with literal zero and one converted
IF. This is observed output/pass content, not proof of the ultimate internal
cause, a claim that the ordinary form creates then removes XOR, or permission
to manufacture an instruction with meaningless source arithmetic.

Final complete owner is 52 match, 0 differ. All 52/51/52 peers, owned sections,
NOBITS, semantic symbols and ordered paired relocation contracts agree for
every control, not just final. The audit merges all relocation sections rather
than overwriting nonempty REL entries with an empty RELA section. Compiler-local
label spelling is canonicalized only for cross-source symbol identity by actual
owner/offset; entire plain/dump ELF parity never uses that normalization.

Full -Werror builds pass without diagnostics in every mode. The explicit
-Werror-implicit-function-declaration option still fails production/SKIP with
identical inherited memset/strcpy/memcpy diagnostics and exit 33; retained mode
passes cleanly. This is diagnostic equivalence, not a claim every strict option
passes. No new warning or error is introduced.

A fresh independent raw-retail interpreter fixture passes 1,114,129 optimized
and 1,114,129 ASan/UBSan cases, checksum 0e4e7fbfc16e6613. All 31 instructions,
both outcomes of all five conditionals and both MOVN outcomes are reached. It
crosses all 16 relevant flag-bit combinations with every u16 hp value, checks
65,536 random complete arrays, and explicitly tests counts 0..16. Exact flag/hp
widths, conditional reads, record order, three stores, untouched work bytes and
registers are checked. Low-address mmap storage is bounds/round-trip verified
before native u32 addressing. This is bounded differential coverage, not an
exhaustive enumeration of every possible whole array. Only the target register
placement is erased for host execution; LeakSanitizer alone is disabled.

The final linked retail image is byte-identical. Generated-source controls,
505 tooling tests without optional skips, 818 layout assertions, frozen EE/IRX
drafts, inline-asm policy and primary/audit report scope pass. All six preceding
owners remain intact: facility 197/0, recovery 391/0, music 27/0, Flow10 33/0,
status 24/0 and world-map mask 17/0. Historical findings below retain their
original source revisions and limits, with this current-owner match replacing
the old nonmatching status.

Integration was refreshed onto 58eb5926 after five upstream data-table, motion
header, ActionFuncTable and data_to_c.py commits. All gates above were rerun
on the combined tree with 5,579 stable compiler inputs and 6,188 stable sweep
inputs. The seven upstream-affected owners also check clean: effBattleSystem
290/0, effManager 23/0, effPart 40/0, dds3AdminiProcess 15/0, nbActionProcess
91/0, nbCameraProcess 26/0 and sdfMotion 88/0. Their source, configuration,
header and tooling changes are preserved without alteration.

### Historical investigation and comparison controls

The historical ordinary draft has 10 of 30 words differing, one instruction
short of retail's 31. The final current-owner match above supersedes this
nonmatching status; the qualified investigations below retain their stated
source revisions and diagnostic limits. Everything else is
exact: the sixteen-entry downcounter, the four flag tests with their three
`bnel`s, the biased `addu $9, $8, 4` base for the flag load and the plain base
for the halfword, and the `movn` that does the counting. What is missing is the
`xori $3, $3, 0` retail puts between `lhu $3, 0xA($2)` and
`movn $6, $4, $3`; without it every later branch displacement is one short.

The `xori` can be the conditional-move lowering's compare-operand XOR,
not an explicit source `^ 0`. Pass timing matters: it need not mean that a
nonzero comparison operand folded to zero late, that a defining instruction
could not combine, or that a 64-bit copy or inline expansion was required.
The earlier explanation based only on the defining `lhu` versus `andi`/call
was too strong; the following qualified SMT3 observations distinguish the
creation and removal passes.

- Matched `nbGetJyokyoMes` (24 bytes), now in
  `src/smt3/newbattle/nbMisc.c`, uses ordinary
  `if ((data->stat & 8) != 0) return jyokyomes + 1; return jyokyomes;`.
  Its branch remains through `rtl.14.combine`; `rtl.15.ce` inserts
  `xor:SI` against literal zero (insn 45) and the conditional move (46).
- `func_0023AA90`, now in `src/smt3/code_0022C578.c`, uses
  `if (dds3FadeCheck() != 0) return 0; return D_0054FEBC;`.
  It likewise retains a branch through combine; `rtl.15.ce` creates the
  SI XOR-zero (48) and conditional move (49). An ordinary external call
  result therefore supplies another example without an inline helper.

These are unhooked, pinned ee-gcc 2.96 `-DSKIP_ASM` observations (compiler SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`).
Observed owner source SHA-256 values were
`dadb839ba590d67a2e5604529912fe130b5e61a3c3abf393ef8c79315900eedf`
(`newbattle/nbMisc.c`) and
`cd11bfad0b94314fd28d38144223b67284e3714d533a6d1e9d879cb8d008c336`
(`code_0022C578.c`). Ordinary and `-da` runs had identical whole-owner
assembly after normalizing only the temporary `.file` path, and identical
code/data and relocation section bytes. This is output-parity qualification,
not a claim that every later owner revision has been traced.

To repeat on the source revision being investigated, use separate output
names, compare outputs before relying on the dumps, and inspect the named
function in each pass:

```sh
unit=src/smt3/newbattle/nbMisc.c
SMT3_KEEP_S=/tmp/smt3-nb-plain.s tools/cc.sh -DSKIP_ASM "$unit" -o /tmp/smt3-nb-plain.o
SMT3_KEEP_S=/tmp/smt3-nb-stock.s tools/cc.sh -DSKIP_ASM -da \
    -dumpbase /tmp/smt3-nb-rtl "$unit" -o /tmp/smt3-nb-stock.o
```

Repeat with `src/smt3/code_0022C578.c` and distinct outputs for the call
example. For a candidate outside the source tree, set `SMT3_AS_UNIT` to its
canonical owner path; use `SMT3_I386_LIBDIR` if the host requires an explicit
32-bit runtime. See the [trace guide](../../cc1probe.md) for hooked observations
and their additional parity requirements. `-DSKIP_ASM` omits fallback assembly
from this diagnostic; parity is not complete-owner matching validation or a
retail-checksum result.

The nonmatching `SetAction_SKILL` draft in
`src/smt3/newbattle/nbActionProcess.c` (base `0031f32439077f658d984ba781555b97ed799fb8`)
with only this draft's guard enabled provides the opposite timing control:
early conversion creates XOR-zero in
`rtl.02.jump` (5069), then `rtl.03.cse` removes it. Replacing only the death
arm's `effectType = 3` with `effectType = (effectType & ~0x20) | 3` delays
conversion: the nonescaping scalar's reaching values are exactly `{1, 0x20}`,
so the real category clear-and-set yields exactly `3` for either. `combine`
folds that update to `3` while retaining the branch; `ce` then creates
XOR-zero (5152) and the conditional move (5153). Ordinary/`-da` whole-owner
assembly and relevant object sections also agree for this control. The full
candidate is still nonmatching (5108 versus 5160 bytes), not an improvement
over the 5120-byte draft. This is a mechanism, not a solution for this
function or evidence of original source spelling. Other late-fold mechanisms
remain possible. Do not add `^ 0`, uninitialized values, dummy consumers or
other artificial work to manufacture the instruction.

Historical diagnostic only, not a valid source candidate:
`hp != <an uninitialised s32 local>` compiles to 31 words with only
`xor $3, $3, $10` against retail's `xori $3, $3, 0` differing. This isolated
the immediate in that experiment; it did not establish a valid way to
preserve literal zero or prove the original source.

Ruled out: `!= 0`, `!= 0U`, `!= (u16)0`, `!= 0 + 0`, `hp ^ 0`, `!(hp == 0)`,
`(u16)hp != 0`, `(s32)(u16)hp != 0`, `0 != hp`, `hp - 0 != 0`, `hp | hp`, the
mask spellings `& 0xFFFF`, `& 0xFFFFFFFF` and `% 0x10000`, the same test
written twice under `&&`, `n += 1` and `n = n + 1` for the increment, the
ternary `n = (hp != 0) ? n + 1 : n`, an `if (hp == 0) continue;` inversion,
reading the field into a `u16` local first, and a `u16 *` local pointing at it.
Comparing against a zero-valued local or global (`s32 zero = 0`,
`gFldAnahoriAkumaIdx`, `gFldAnahoriAkumaIdxMax`) either emits no XOR
or costs an extra `daddu rd, $0, $0` to materialise the zero (4 words
out, same 31-word size).

A second pass added five more, all still 30 words with the one `xori` missing:
`!!hp`, `(hp != 0) != 0`, the same test twice under `||`, an `else { n = n; }`
arm, and the field read into a `u16` local with a second `if (hp != 0)` whose
body is a self-assignment, which was meant to give the compared pseudo a second
use. Two spellings that move the shape the wrong way: `n = (hp != 0) + n;` and
`n = n + (hp != 0);` drop the `movn` for an `sltu` and are 20 of 30 words out,
and an `fldAnahoriAkuma_t *a = &DDS3_GBWK->akuma[i];` local for the hp read
collapses the two base registers retail keeps (`$8` for the struct, `$9 = $8 +
4` for the flag word) into one and is 26 of 29. The `$8`/`$9` pair is itself
worth noting: retail biases a second base by 4 for the flag at `+0x04` and uses
the unbiased one for the halfword at `+0x0A`, which is what the two separate
`DDS3_GBWK->akuma[i]` expressions in the draft already produce.
