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
GDB is unavailable, so no hooked decision trace is claimed. Guarded production
output is identical to the original owner; guarded `NON_MATCHING`/`SKIP_ASM`
strict implicit-declaration compilation succeeds. Strict `SKIP_ASM` alone
fails identically before and after this change because the existing
`fldMakePlane` path lacks the guarded `fldAllocPlaneTable` declaration; that
mode is not reported as passing.

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
