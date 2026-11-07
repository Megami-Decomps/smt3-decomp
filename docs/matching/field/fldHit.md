# Matching notes: field/fldHit

## fldHitChkUnitDamage (field/fldHit, 0x370)

219 of 220 words match. The guarded reconstruction emits 880 bytes, equal to
retail's 0x370-byte extent, with the native 0xC0 frame. The sole difference is
the distance-failure branch at +0x2E0. The assembly fallback remains active;
this is a semantic reconstruction improvement and earns no exact-match credit.

### Ordinary storage and callback observations

All three native callers pass their stack position vector: `fldPlayerCalc_Nml`
at 0x00132334, `fldPlayerCalc_Wm2` at 0x00133728, and
`fldPlayerCalc_Nml_syukan` at 0x001345F0. The damage flag is the ordinary s32
field at `fldGlobalBase + 0xB0`. Neither storage is MMIO or a hardware-observed
bare-address hand-off here. The earlier volatile views were unsupported and
are removed. Native coordinate accesses remain 32-bit `lwc1`/`swc1`, and the
flag remains a 32-bit `sw`; no alias promise or memory barrier is introduced.
The three previous-position words at 0x002FCAE0/4/8 are named separately as
`fldHitChkWork`, `fldHitChkWork2`, and `fldHitChkWork3`. Their contiguous native
use does not establish a larger primary-symbol allocation.

Provider ABIs and call ordering are unchanged. `MATH_VectorLength` and
`fldVecDot` read XYZ, not W. `HitPositionInside2` retains its six canonical
parameters, including radius in `$f12`; the damage decision uses its written
float clearance, not its integer return. Keep `!(clearance < 0.0f)` and the
strict distance/cutoff comparisons. The cutoff's native literal is
0x3DCCCCCC, not nearest IEEE binary32 0.1.

Count is observed after `fldHitChkUnitElvOnly`, after `fldGbChkHit`, and after
processed-record/helper paths. Indexed record access preserves native table
reloads after `fldGbChkHit` and `fldVecDot`; no cached face is carried across
them. Mode-only skips retain count because no callback intervenes. Current
coordinates are not recaptured after the elevator call. The native 0xE4
record stride and existing signed 16-bit mode/edge fields, scalar float
coordinates, pointers and 32-bit attributes are unchanged.

### Native frame contents and retained copied code

Current, previous and delta occupy SP+0x00/+0x10/+0x20. Their snapshots occupy
SP+0x30/+0x40, and absolute travel X/Z occupy SP+0x50/+0x58. Current's snapshot
feeds collision coordinates; travel feeds axis selection. These actual
native fields, not an unused padding aggregate, explain the frame. No unused
W component is consumed. Per-area radius/height branches retain the matched
`fldHitChkUnitElvOnly` shape, and natural XYZ source restoration produces the
native X/Z/Y stores on the early-return path.

The previous-position chain requires a narrower justification: after the
initial cutoff and axis calculation, its translated XYZ only feed the next
snapshot/subtraction and have **no final gameplay consumer** in this damage
path. Do not call the entire chain live. Retail nevertheless keeps the
previous snapshot stores at 0x00139C6C/70/78 and the translated previous stores
at 0x00139CAC/B8/C4. Removing that chain cannot reproduce those native
operations through a remaining live consumer.

The already matched `fldHitChkUnitElvOnly` supplies the copied source shape:
its previous snapshot stores are at 0x001396DC/E0/E8 and its corresponding
translated stores at 0x00139720/2C/3C. Its adjacent current snapshot/translation
has real collision consumers. Its retained C uses the same paired
`currentSave`/`previousSave` pattern, followed by a restoration phase. This
precise native trace plus matched live/copy-shape evidence supports only the
canonical **dead code that retail still shows** exception. It is not an
all-live claim or permission to add arbitrary dead stores. Damage does not
have the sibling's restoration: both translations accumulate across records
and passes, as retail does. The source comment states this qualification.

### Residual and qualified compiler snapshots

At native PC 0x00139D00 (+0x2E0), candidate word 0x4500000D branches to the
`inside == 1` comparison at 0x00139D38. Retail word 0x4500000F skips directly to
the count reload at 0x00139D40. Both execute `inside = 0` in the delay slot.
The extra candidate comparison is therefore false and performs no flag
store. It is behaviorally harmless here but is still a byte mismatch.

Qualified ordinary compiler snapshots expose this continuation from
expansion onward: in `00.rtl`, initialization UID 646 gives inside pseudo 151
zero, distance comparison UID 648 and branch UIDs 649/650 route failure to
label 744, and label 744 leads to comparison UID 748 before flag store UID
755. Label 759 leads to count reload UID 762. In `27.jump2`, inside occupies
register 16 and distance branch UID 649 still targets label 744; `29.dbr`
retains that destination. These are observed pass states, not a hooked
explanation of an internal scheduler or allocator decision.

The complete target-enabled owner retains its 16 exact peers and passes
owned-data checks. Plain output, actual-compiler diagnostic output (`-da`),
and nonfatal implicit-declaration diagnostics agree on normalized assembly,
all allocated sections, non-file symbols and ordered relocations. The
pinned compiler SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
GDB is unavailable, so no hooked decision trace is claimed. In that
pre-cleanup damage comparison, guarded production output was identical to
the original owner and strict `NON_MATCHING`/`SKIP_ASM` compilation passed.
Strict `SKIP_ASM` alone failed before and after the damage change because
`fldMakePlane` could not see the guarded `fldAllocPlaneTable` declaration.

A subsequent declaration-only cleanup exposes the existing `void(void)`
prototype before that call. It corrects the former implicit int-returning
C declaration's mismatch with the unchanged void provider; the caller
ignores the return value, and no native runtime or ABI defect is demonstrated.
Strict implicit-declaration compilation now passes in production, `SKIP_ASM`,
and `NON_MATCHING`/`SKIP_ASM`. Ordinary baseline/candidate and candidate
ordinary/strict comparisons preserve allocated-section bytes and metadata,
ordered non-file symbols, ordered relocations and normalized assembly in
all three modes. Production remains 16/0, and the guarded target remains 219/220 words at 880 bytes with
the same branch residual. Matching coverage is unchanged. The 92 inherited
`fldVecDot` incompatible-pointer warnings per mode remain separate; this is
not a warning-clean owner claim.

### Rejected structural controls

Moving the final inside comparison/store into the distance block while
keeping zero initialization before it changes the desired continuation but
causes 153 of 220 words to differ at the same 880-byte extent. An explicit
negated-distance continuation with the required count reload has the same
result. Neither is an overlooked one-line exact fix. Storing literal 1
instead of the equivalent inside value leaves the same one-word residual.
Replacing vector locals with the sibling's float-array form did not improve
the earlier indexed-field control. These controls retained all 16 exact
peers; do not repeat them without new evidence.

No volatile, fixed register, compiler constraint, fabricated bounds/layouts,
dummy consumers, general memory barrier, or instruction replay is added.
The guarded draft preserves retail behavior; whole-project verification
remains required for integration, and enabling this C as matching source
requires resolving the remaining branch exactly.

## fldHitWM_Mask (field/fldHit, 0x2B8)

174 of 174 words match at 696 bytes. The index is initialized before render
setup. A separate u32 byte offset advances by the actual collision_s size before
row callbacks, and each row observes the current table. Count is read after
setup and refreshed only after a real triangle/quad draw, not after skipped
rows. A missing matter preserves the preceding XYZ translation, including
translation obtained from a skipped row. Native signed edge/special tests,
metadata reads, scalar additions and complete call ordering are unchanged.

The address sum uses the existing EE_ADDR/EE_ADDR_AS target idiom on the actual
allocation. fldAllocPlaneTable allocates and zeros 0x72000 bytes, enough for
2048 native 0xE4 records, and fldMakePlane fills real records there. Valid records
and their referents must remain within that allocation. The explicit operand
order reproduces the native real-address addu at 0x13B710; it adds no fake
bounds, cancelling arithmetic, qualifier or dependency. This is the existing
32-bit EE address ABI, not a portable-ISO-C pointer representation claim.

Exactly three local bindings remain. drawFace in $7 supplies metadata and
triangle accesses and dies before call clobbers. Quad reads use the separate
face pointer in $8 after $7 becomes the fourth color at 0x13B808. Each arm's
second signed32 color argument is initialized to (s32)0x80000000U and bound to
$5 only until its one consuming call. No constrained value crosses a call,
no empty operand remains, and provider declarations retain the canonical ABI.
Poly3's colors precede its float coordinates; Poly4 interleaves color/XYZ source
arguments. Their integer/FPU streams and 8-byte outgoing stack slots remain
native-exact. These are localized allocation aids, not original-source claims.

The enabled original emits 700 bytes with 24/174 native-prefix words matching
and 4 excess bytes. Ordinary two-role C emits 688 bytes with 38/172 overlap
words matching and 8 uncovered native bytes. Draw binding alone reaches
169/174 at 696 bytes; adding the two color bindings reaches 173/174; the real
address-sum order supplies the last word. Removing draw binding gives 37/172
at 688 bytes; removing either color binding gives 172/174 at 696 bytes. Removing
all bindings or using the all-ordinary pointer walk gives 38/172 at 688 bytes.

W is dead here. Retail explicitly stores 1.0f at SP+0x2C (0x13B6F0) beside the
live XYZ stores, but no instruction reads W or passes the vector address to a
consumer. The complete homogeneous-vector initialization is retained under
the native-dead-code exception: the native residual is that W store, with the
same member-initialization shape as adjacent live XYZ and engine patterns such
as fldAnaStart/fldEveHit_ChkDir. No live W consumer can truthfully be invented.
The source comment says only XYZ is consumed. The no-W control changes 141 of
170 overlap words at 680 bytes; that score is supporting evidence, not policy
permission or an unused-frame-padding justification.

Fresh integration covers thirteen guarded/original/binding/address/ordinary/
no-W/final snapshots in all three modes: 39 whole-assembly/entire-ELF plain/-da
pairs agree after only one equal-length temporary source filename substitution.
The complete final owner is 17 match, 0 differ. All 19/16/19 peers, allocated
data including lit4, NOBITS, COMMON, symbol attributes and ordered paired
relocation contracts are preserved. There are no COMMON symbols; unowned text
is zero alignment padding. SKIP_ASM adds only the expected nine undefined
renderer/state callees because its guarded baseline omits this function.

All implicit-declaration strict builds pass with the same 62 inherited fldVecDot
pointer warnings. Full -Werror fails guarded and final identically with exit 33;
this owner is not warning-clean. Strict target-layout checks independently
verify the native collision/metadata sizes and accessed field/pose offsets.

A fresh independent raw-retail interpreter comparison passes 98,304 optimized
and 98,304 ASan/UBSan cases with trace hash c8c6c3710c06d44d. It reaches all
173 reachable instructions; the sole unexecuted word is alignment nop at
0x13B7E4. Both outcomes of all seven conditionals execute. Tests cover every
signed16 special value, signed counts, triangles/quads, attributes, translation
persistence, callback count/table/pose/metadata/point changes, exact mixed-class
call arguments, clobbered caller registers and saved frame restoration.

Actual EE address macros run over a checked MAP_32BIT host arena. Native records
are explicitly serialized at 228 bytes rather than pretending host 240-byte
pointer-bearing records share the native layout. Finite scalar additions run
without fast math or contraction. This is not a full R5900 NaN/denormal/exception
or rounding emulator; exact native add.s operations establish target behavior.
Only target register placement is erased for host execution. LeakSanitizer alone
is disabled for executor compatibility; address/undefined-behavior checks pass.

Full linked retail bytes, generated-source negative controls, 505 tooling tests
without optional skips, 818 layout assertions, frozen EE/IRX drafts, inline-asm
policy and primary/audit report scope pass. All preceding landed owners remain
intact: facility 197/0, recovery 391/0, music 27/0, Flow10 33/0 and status 24/0.
