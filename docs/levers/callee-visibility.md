# Callee visibility: the cause of most one-word branch-likely misses


This is the single most productive mechanism found so far, and it is one cheap
test rather than a search. Whether ee-gcc emits `beq` or `beql`, `bne` or
`bnel`, `bgez` or `bgezl` for a given test is decided by whether the function
that test's arm **calls** is visible in the unit as a C definition. Nothing
about the branch's own source shape reaches it.

It runs in both directions, so when a function is one or two words out and the
difference is an annul bit:

1. Find the callee in the arm whose branch differs.
2. If it is still `INCLUDE_ASM`, the caller will probably close by itself once
   the callee is C. Do not permute the caller.
3. If it is already a C definition, test the reverse: in a scratch copy, rename
   that definition so only the file's `extern` remains, and recheck the caller.

An earlier context test found that hiding `nbFreeEffectSlot` closed four
callers at 1, 1, 1 and 4 differing words. That is a historical result, not a
current parking requirement: the helper is now ordinary C in
`newbattle/nbEventProcess`, matches all32 bytes, and its complete normal owner
passes21 match/0 differ. Recheck the current owner before carrying a visibility
exclusion forward.

Ruled out as causes, measured: every spelling of the caller's own guard, the
callee's declaration style including K&R and no prototype at all, and cc1 heap
shift, since twenty unused `extern`s above the callee change nothing. It is the
definition's identity alone.

A side effect worth noticing: the four callers that discriminate all sit above
that callee's address, which is evidence for a retail translation-unit boundary
between them. A callee-visibility result is also a boundary hint.

### Resolved animated-billboard context

Earlier `effect/billManager` compiles made `dds3Billboard_P2ADraw` one annul
word different when `dds3Billboard_P2AAnimDraw` was visible. Hiding that helper
closed the caller; hiding `dds3Billboard_P2ASetNumber` did not. Qualified reorg
traces measured the later mask's delay-slot eligibility changing while branch
probability stayed the same. Those observations described the tested context,
not a permanent restriction on the helper.

[79519a4d](https://github.com/Megami-Decomps/smt3-decomp/commit/79519a4db080c32f6688248dd61792e49279f13a)
promotes the animation body. In the current normal owner both it (464 bytes)
and `P2ADraw` (696 bytes) match; the complete owner passes63 match/0 differ.
The former nonmatching entry is retired. This check establishes the current
result, without attributing the closure to an unmeasured compiler mechanism.

Four source properties remain useful from the matched caller: form the plural
output pointer with index before base; place the negative-frame arm after the
drawing arm; restore rotation before colour in the plural branch; and assign
the packet kind directly in each selection arm so the compiler can share the
store. A local packet-kind value instead produced conditional moves.

Historical controls reproduce the old trade in `c1223151` and `e275e63c`;
it no longer reproduces after the next relevant owner/context change,
`e27ec762`. See [billboard matching](../billboard-matching.md#animation-drawing)
for the preserved controls and current qualification. This comparison does
not isolate a compiler-internal mechanism.

Confirmed again in `event/evtPolygonMovie`, and this time on a callee that does
*not* match: `evtHokanFilter` is 71 of 72 words with a single `bnel` for
retail's `bne`, and compiling the parked `evtHokanColor` draft unguarded makes
it byte-exact while `evtHokanColor` itself stays 9 words out. So the mechanism
also tells you which pair to attack: a one-word annul miss next to a parked
draft is a reason to spend the budget on the draft rather than on the caller.

### One-pass model-load phase in `fldAutoMap`

`fldAutoMapModelLoad` is 100 bytes and exact when its 372-byte provider,
`fldAmModelLoadFile`, is a visible C definition earlier in `field/fldAutoMap`.
With only the caller body changed to ordinary nested `if`s, it grows to 108
bytes. The matching source wraps only the model-load phase after
`fldAutoMapFontInit` in a one-pass `do` block, with `break` for the re-read
`area < 10` case. That gives ee-gcc the basic-block boundary it needs without
changing the phase's behavior.

The qualified final-source reorg trace (`verify: OK`) shows
`NOTE_INSN_LOOP_BEG` uid44 and `NOTE_INSN_LOOP_END` uid81 around that phase;
the phase is a separate block (uid47), its threshold branch is uid56, and the
provider call is uid60. At reorg, the visible call has no EH region and
`mark_target_live_regs` reports only `$29` live. The restore of `$16` (uid111)
is therefore eligible for the branch delay slot; cc1 emits the retail `bne`
form with that restore. This relies on ordinary same-unit C visibility, with no
`nothrow` attribute or stub.

Reproduce the qualified trace with `tools/cc1probe.sh src/smt3/field/fldAutoMap.c --func fldAutoMapModelLoad --reorg --out /tmp/automap-reorg`; require `verify: OK` before using its instruction IDs.

### Skill-attack camera scheduling

The unchanged `nbCameraState_SkillAttack_Calc` draft becomes exact at 364 bytes
with the matched `nbCameraShot_GroundCollision` definition visible earlier in
`newbattle/nbCameraSkill`. Hiding only that definition behind its assembly
fallback restores all six old differences: the attack-step branch gains an
annul bit, and the camera-argument move and following call/branch shift out of
the native delay slots. The isolated promotion preserves all 29 C functions
in the owner. Check recently matched callees before rewriting a parked caller;
this case needs no new declaration, attribute or control-flow adjustment.


### Snapshot each message glyph before its visibility guard

`DrawMesFlow` reads the speaker, message and selection font-chain pointers
before their respective flag guards. Native loads at +0x3C, +0x5C and +0xB0
execute in the unconditional delay slots of the guards; loading only inside
the guarded arms left two branch-annul differences and their displacement
cascade. The established DDS `itfUpdateSoundSelectorPanel` uses the same
live glyph snapshots. Its direct selection-field argument, after initializing
an unselected row to zero, and its status-before-fade update also reproduce
the SMT3 native sequence. All 99 words (396 bytes) match without false
attributes, artificial dependencies or a statement-order sweep.

Once the provider is visible, `DrawMsgMng_Process` matches all 28 words
(112 bytes) with the ordinary post-callback `node = node->next` traversal.
Retail loads the next link at +0x40, after `DrawMesFlow` returns. The old
pre-call snapshot draft and its claim that this snapshot was required were
not the native callback contract; do not preserve a stale next link merely
to approach the bytes while the provider is opaque. The canonical DDS loop
also reads the next link after rendering.

Adapted from redthing1's [published DDS message-manager source](https://github.com/Megami-Decomps/dds-decomp/blob/f79f86dc00f7fbd266fd5144e71faa86bef12aa4/src/dds1/game/code_0019DB88.c).
SMT3's native field offsets, selector initialization, flag values and tail
calls were reviewed independently. The complete message owner checks
118 match / 0 differ, and the full retail image remains byte-identical.

### Camera selector reset after the final call

`pbInitGameCamera` matches all 148 bytes with the unsigned local selector and
one `type = 0` reset after case 2's `pbSetGameCamera2()` call. This resets only
the function's own selector; it does not update shared camera state. Native
case 2 uses `jal` and a common epilogue, while case 1 genuinely tail-calls its
provider. All providers retain their established `void` return types.

This uses the canonical matching rules' narrow post-call-statement exception.
The qualified default-owner trace, with pinned cc1 SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`,
reports `verify: OK`: dumps match plain compilation and hooked output matches
unhooked output. In `rtl.01.sibling`, call uid60 is followed by uid64 setting
the local selector pseudo 84 to zero. In `rtl.13.life`, the call remains but
uid64 has disappeared, and control reaches the common label 81. Thus sibling
selection sees the reset before later dead-store deletion. The complete normal
owner checks 61 match / 0 differ.

Reproduce with `tools/cc1probe.sh src/smt3/pb/pbGame.c --func pbInitGameCamera --reorg --out /tmp/camera-selector`; require `verify: OK` before interpreting the dumps.
