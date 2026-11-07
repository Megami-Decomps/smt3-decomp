## fldPlayerMotionSe (field/fldPlayer, 0x5B0)

**Matched: all 364 words / 1,456 native bytes at 0x001304E8.** The single
remaining branch residual is resolved by a localized read/write memory operand
on the real default-category field immediately before its conditional read.
The independently reviewed final source also repairs the RNG consumer declaration
to the unchanged provider's canonical `u32 dds3GetRandA(void *name)` signature.
All calls pass the null shared-state selector; target ABI and behavior are unchanged.

The operand expands to empty volatile `+m` on initialized s32 storage at
`fldGlobalBase+0x40`. It emits no instruction or machine write and is not a
general memory barrier. It adds no fabricated local, false type, register
binding, dummy consumer, runtime arithmetic or live value across a call.
This is a target-specific reconstruction aid, not original source evidence.

Qualified stock dumps identify the dependency precisely. Without the operand,
pass 15.ce hoists the override decrement before the category branch. With the
operand, that decrement stays in the override block. Final pass 29.dbr then
uses the native taken-only delay slot and `bnel`; deleting only the operand
restores `bne` at +0xF8, the sole differing word, at the same 1,456-byte size.
The claim concerns observed stock RTL and output, not an unobserved internal
rejection predicate. Read-only and override-field operands are measured negative
controls, not alternative accepted source.

The function snapshots the current model frame and previous field frame, then
publishes the current frame before sound dispatch. It preserves special 15/20-frame
crossings, surface sound bases, distance clamps, randomized volume, RNG order,
transition sound, separate side variants and later volume rerolls. The existing
shared `fsqrtf` helper supplies the only instruction-emitting inline region,
`sqrt.s` at +0x1BC. That established helper is distinct from the new empty operand.

The single pre-existing post-call volume reset remains the permitted tail-call
blocker. Earlier qualified sibling/life traces, retained below, establish that
reset's dependency. It is not a new dead-store exception. Existing domain limits
are unchanged: an INT_MIN override cannot be decremented in representable s32 C;
no expanded invalid-state or special-floating-point guarantee is claimed.

Independent native/provider/operand review approves the exact final source.
A freshly compiled full-function host fixture checks the actual +m operand,
its erasure and a separately structured reference. Both optimized and UBSan
runs pass 457,056 cases, comparing complete provider/RNG/sound histories, field
state, model frame and RNG state, including callback mutations. This is finite
host evidence, not complete EE floating emulation. The earlier six PCSX2 call
vectors were not rerun. Target layout assertions separately verify both category
fields' four-byte widths and offsets 0x40/0x44. The inherited generated IDB export
still uses `char *` for the RNG parameter; no live database reconciliation or
manual generated-file edit is claimed here.

Final integrated qualification is on 6ee043af, preserving the cursor-update
landing and all earlier peer commits. It checks 27 complete plain/dump pairs
across guarded source and eight controls in production, SKIP_ASM and retained
modes. Exactly one same-length canonical scratch filename is normalized per
assembly/ELF pair. All 594 RTL files are losslessly archived and rehashed.
All 29/21/29 non-target peer instruction and resolved relocation contracts
remain unchanged. Production/retained owned data and NOBITS extents are exact;
SKIP_ASM adds the target's literal pools, with peer references compared to their
actual payloads rather than ignored. Final production target equals native
fallback; the linked image and source/object provenance prove compiler ownership.

Complete owner is 22 match / 0 differ; all 31 distinct peer-owner gates pass.
The three final strict implicit-declaration modes and native layout assertions
pass. Full retail/generated-source checks (including the injected stale-line
negative control), all 505 tooling tests without skips plus 818 layout assertions,
all EE/IRX retained drafts, inline policy and report scope pass. Retail SHA-1
remains `5facfbf7916836613449c922e26a4ec412207b32`. This adds one game function
and 1,456 game bytes; SDK reporting is unchanged.

### Earlier baseline and reset trace, superseded where noted

363 of 364 instruction words match at the native 1,456-byte size. Production
remains assembly-backed. The single residual is at +0xF8: C emits `bne`, retail
`bnel`; both use the same decrement in the delay slot and select the same
surface sound category. Reversing the source arms or using a conditional
expression leaves that residual. A separate override snapshot is also inert;
updating the category snapshot in place changes three register words. Loading
the default category before testing the override changes fifteen words.

The body snapshots the current model motion frame and the previous field frame,
then updates the field frame before dispatch. Old motion 13/11 has independent
15/20-frame crossing sounds. Other motions select both surface sound bases,
calculate distance-clamped or randomized volume, and retain random-call order,
the direct base+3 transition, separate left/right variants and crossing gates.
The distance is the square root of three squared coordinate differences, minus
500, clamped to 0..7000. The two late motion variants re-roll their volume.

The final volume reset is the single permitted post-call statement for the
shared sound-dispatch tail. Native uses ordinary calls plus an epilogue there,
but tail calls for the two early special sounds. Removing the reset generates
1,820 bytes with duplicated sibling-call epilogues; keeping it emits the native
size. A qualified `cc1probe --reorg` run on this source shows reset instruction
1052 (volume pseudo 98) after sibling selection in `rtl.01.sibling`, and absent
from `rtl.13.life`. The early special sound calls still select sibling calls.
Hooked/unhooked assembly and all dumps agree, and dumped/plain output parity
passes with compiler SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.

The same qualified trace diagnoses the remaining branch: the category decrement
is available before the branch and is accepted for its ordinary delay slot.
It does not establish a source property that requires the native annulled slot;
no branch-forcing exception or compiler option is used.

## fldPlayerCalc (field/fldPlayer, 0x2A4)

**Matched: all 169 words / 676 native bytes at 0x00134B20.** The native
mode switch and two mirror-check call sites are retained with a conservative
same-symbol ABI view of the unchanged `s32 fldPlayerMirChk(void)` provider.
The branch-local view has that exact return and parameter type, no extra
attributes, and the original assembler label. It introduces no wrapper,
indirection, provider relocation or translation-unit change.

The real signed halfword motion index at model offset `0x12` is promoted to
`s32` and bound locally to native `$4` for its comparison with 10. Assignment
follows the model-provider call; the value is dead before the first later call
and before `$4` is reused for `printf`. The comparison is its only consumer.
The binding and ABI view are target-specific reconstruction aids, not evidence
of historical source spelling.

Paired deletion controls establish both dependencies. Removing only the
binding leaves four differing words at the native size (165/169 words match).
Replacing only the branch-local ABI-view call with the direct call merges the
two mirror-check sites, emits 668 bytes and matches 143 of 167 emitted words.
Ordinary switch and two one-pass-block controls do not retain both native
calls. These are observations of qualified compiler output; no unobserved
internal pass mechanism is asserted.

The source preserves callback-sensitive reloads, signed area and counter
checks, byte-sized motion status reads, the world-map tail return, and the
mode-1 fallthrough into mode-3 processing. An independent native/source review
checks the provider ABI and register lifetime. An extracted-source fixture
against an independently address-labeled native control-flow transcription
passes 161,280 mutation and boundary cases at both `-O2` and ASan/UBSan. That
fixture is not raw MIPS emulation or proof for arbitrary external callbacks.

Promoting the function requires both existing padded Noroi strings. Configure
moves the five existing constant imports to the owner end and imports the
24-byte and 32-byte strings there. Independent checks resolve all 37 rodata
pointer relocations and confirm the full 352-byte rodata, 108-byte literal pool,
60-byte small-data section and all 24 imported objects against retail.

The combined owner preserves the independently reviewed `fldPlayerMotionSe`
match and its canonical RNG declaration. It checks at 23 match, 0 differ.
Twelve fresh whole-assembly/ELF plain/dump pairs cover the MotionSe baseline,
combined source and both deletion controls in production, SKIP_ASM and retained
contexts. All 29/22/29 peer functions, allocated data, NOBITS and ordered
relocations are unchanged; MotionSe is checked explicitly in every context.
Full retail, generated-source, regression, retained-draft, IRX, report-scope
and compiler-owned production-provenance gates pass on the publication base.
