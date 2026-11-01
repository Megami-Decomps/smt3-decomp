# Matching notes: newbattle/nbKoukaProcess

## RunNumEffectKouka (newbattle/nbKoukaProcess, 0x144)

81 of 81 words match in `3e13612e`: 324 bytes plus four bytes of alignment
fill, complete owner 57 match / 0 differ with data checks. The actual published
commit rebuilt the exact retail image; a second `ninja` reported no work, with
a clean tracked tree and validated object and link inputs.

The ordinary reconstruction matched 73/81 words. Its remaining eight
words were initial constant/party-access scheduling: the compiler delayed the
forward Z store into the inactive-guard branch delay slot. One local volatile
empty `"m"` input operand on initialized `forward.z`, immediately after loading
`k->data`, retains the native schedule without adding instructions. Its real
consumer is the later VU load for `dds3RotationArc`; the operand is not a general
memory barrier. This is an SMT3 reconstruction aid, not original-source evidence.
Qualified stock diagnostic dumps preserved production code/data output.

The source zeroes a vector at stack+0 and sets its Z to -1 before selecting the
packet's party. An inactive party returns. The signed formation index selects
a 0xF0-byte record. ROT sets bit0x20 in `stat` at +0x78 and copies the packet's
quadword vector to stack+0x10. HOMEROT clears that same status bit, loads orgrot,
builds the matrix and transforms the VF0 vector with Z set to +1. Do not update
the neighboring `flag` word at +0x7C; the older off-build draft used that wrong
member. Other packet types still reach the common arc path with the destination
stack slot untouched, matching native behavior.

The shared path loads the -Z source and destination into VF10/VF11, calls
`dds3RotationArc`, stores the result and calls `evtSetUnitRot1`. Keep the real
no-argument VU helper declarations and the rotation provider's three-argument
void signature. Load `form->hunit` and the packet's signed parameter after the
arc call. The existing quadword-copy and identity-vector macros describe the
native operations; an aggregate C copy emits a different transfer.

A ROT/HOMEROT switch preserves the native dispatch. Existing NOMEM quaternion
store macros retain the native final output-address rematerialization, and the
party-index pointer expression retains its native operand order. Removing only
the empty operand from the final owner returns the 73/81-word ordinary result;
placing it before the `k->data` load gives 79/81, exchanging that load and the Z
store. Tied memory/float constraints regress and are not retained. The final
source has no register binding, synthetic arithmetic, new call or runtime memory
access. All 26 regression scripts, normal-draft and IRX checks passed before
publication.

## nbClipKoukaNum (newbattle/nbKoukaProcess, 0x234)

141 of 141 words match at 564 bytes. The first color switch places real case 4
before 2/3, matching native body order. A fresh signed dispatch after
nbClipEffectNum draws once for 0..3, twice for 4..5 and never for any other
signed value. Colors intentionally use pre-clip data1, while dispatch reloads
it afterward. Initial callbacks precede the form snapshot; the second draw
reloads data/texture, time and party number without inventing a form consumer.
Existing color, alpha, VU operations and provider types remain unchanged.

One empty `+r(form)` after the first VU position load retains the actual
initialized form base for its later real position load. No callback intervenes,
no register is fixed, and no extra runtime consumer or memory access is added.
It emits nothing and is not a memory barrier. Qualified ordinary-control GCSE
shows PRE retaining form+32 through a new derived-address register; the
form-only control avoids that grouping. This is an observed compiler effect
and localized reconstruction aid, not historical source evidence.

Ordinary unsigned time arithmetic removes the diagnostic second constraint.
The first double-sprite Y argument uses
`screen[1] - ((u32)(p->time + 12) << 2)`. Real u16 time bounds the addition to
12..65547 and shifted value to 48..262188; unsigned subtraction preserves
native low32 wrapping before conversion to the truthful s32 argument. Pinned
EE compiler probes establish widths and representative negative conversions,
and emit the actual add12/shift2/subtract sequence. This deliberately relies
on the target implementation, not arbitrary-platform signed conversions.
The form-only signed-arithmetic control instead scales raw time then subtracts
48 later; qualified final CSE/combine retain add12 before shift/subtraction.
No time operand, binding, volatile or fabricated bound remains.

Ordinary, form-only and final complete-owner plain/dump pairs pass normalized
assembly, allocated-section contracts, relocations and function-inventory
parity, independently rerun. Bounded models include all 65,536 times against
chosen/seeded screen patterns: 2,097,152 implementation Y cases and 524,288
independent final conversion cases, plus dispatch/alpha checks. They are not
complete screen/time Cartesian products or callback/memory/VU execution;
the low32 identity and exact native operations remain essential evidence.

Independent complete-owner verification gives 58 match, 0 differ, preserving
all 57 peers and owned data. The owner is now entirely C: removal of its sole
guarded draft reduces the inventory from 148 owners / 550 drafts to 147 / 549.
The old-count guard attempt failed closed and is not counted as a pass. An
independently audited count-only private runner change then compiled all
147 production-listed owners with stable input snapshots; production guard
logic and common owner entries were unchanged. Retail checksum, 427 tests
across 26 tooling files, seven IRX owners and report gates also passed for
source `f618ab07cbe3aca25a5a6a6b09364972e652d093`. Exact-source CI
[41077035750](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41077035750)
completed successfully, with every actual step passing and none skipped;
hosted logs confirm all 147 guarded owners / 549 drafts.
