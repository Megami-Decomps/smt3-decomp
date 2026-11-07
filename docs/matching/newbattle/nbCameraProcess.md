# Battle camera process matching

## nbCameraScriptSet (newbattle/nbCameraProcess, 160 bytes)

All 40 of 40 words match. The existing guarded VU0 pipeline is promoted with
one localized change: the final target-vector store uses the existing
SDF_VU0_STOREVECTOR_NOMEM macro. The full-clobber ablation emits 152 bytes;
removing only that blanket clobber recovers the native 160-byte output.
No new macro, instruction, compiler flag, type or provider view is introduced.

This input-address-only store is justified only in the complete observed
context. Its destination is the target vector at camera offsets [0,0x10).
No C value is read or cached from that range. The following distance and fovy
stores touch [0x20,0x28), and the unchanged same-unit state setter only reads
its separate reset table and writes state fields at +0x28 and +0x2C. There are
no same-owner callers of this function. The known separate-owner caller sees
an ordinary external call in the non-LTO build. The store is not a general
C-memory barrier, and adding a C observer or changing this compilation context
requires another audit.

The preceding direction store retains its full memory clobber. All VU macros
remain volatile, and the verified output preserves native hardware ordering: pop calls, rotation
load, ToMatrix, direction calculation/store, position load, target addition
and target store. Inputs may alias camera vectors. In particular, a position
pointing to direction reads the newly stored direction; rotation is consumed
before either vector is overwritten. No nonaliasing promise is introduced.
ToMatrix retains its actual void(void) hardware ABI, consuming vf10 and
producing the matrix in vf28-vf31. The state call remains nbCameraSetState(0x1C).

The external vector D_00320E30 remains the existing complete (0,0,1,0) object.
Distance remains 1.0f, and fovy 0.6981316209f rounds to retail bits 0x3F32B8C1.
Production and retained-C allocated data are unchanged. The C-only SKIP_ASM
object gains only the expected literal for the newly visible target, whose
value and resolved address are independently verified. No data ownership moves.

Independent hardware, alias, caller and provider review accepts this narrow
store contract for the configured target. Public complete-owner checks give
26 match, 0 differ. Original, final and full-clobber ablation pass all nine
ordinary-context three-mode plain/dump complete assembly and ELF comparisons.
All three strict retained-C pairs also pass. Strict production and SKIP_ASM
builds retain the same two inherited implicit-declaration failures for
nbGetActionSeq and effBTLModelAllShow; no declaration changes conceal them.
All 26 production, 25 SKIP_ASM and 26 retained-C peers preserve instructions
and resolved relocations. The final target independently equals the native
160-byte span in all three modes.

Full retail, generated-source, regression, guarded-source and report gates
pass. Production provenance verifies the unique compiler-owned linked span
against all 160 retail bytes, including resolved literal references. Game
coverage gains one function and 160 bytes, with the same all-unit increase.

## nbCameraProcess_nbCameraProcess (newbattle/nbCameraProcess, 792 bytes)

198 of 198 words match. Ordinary conjunctions retain the native restart
exclusion order 6, 8, 9, 0, 10 and formation exclusion order 10, 0, 17.
The shared diagnostic/restart block and every callback-visible reload remain
in place. Clearing change state precedes init lookup; init callback precedes
calc lookup. Active-camera checks and all frame-rate hooks are unchanged.

The real eye vector precedes scale in the declarations, yielding native eye
at sp+0 and scale at sp+0x10. Scale is volatile because its initialized XYZ
fields are consumed only by a bare-address VU load. Explicit Z/X/Y stores
preserve native order; the volatile X/Y/Z control differs only in those three
store offsets. No dummy object, fabricated bounds or register binding is used.

Native scale.w at sp+0x1C is unwritten. Full-lane VU multiplication/addition
propagates that stack-derived value into eye.w, followed by a full memory-
clobber store and the pose setter's quadword copy. A later camera getter reads
pose.pos.w in C. W is therefore neither zero nor unused, and no initialization
or harmlessness assumption is introduced. The source does not read the
unwritten scale.w through a C expression; the existing hardware path remains.
The earlier nonvolatile exact form is superseded by this hardware-scratch form.

Independent complete-owner checking gives 27 match, 0 differ. Actual-source
scalar/callback tests pass 86,400 cases in optimized and ASan/UBSan builds,
covering state pairs, flags, active/debug paths, mutating callbacks and timer
signs. The VU model is intentionally opaque and does not read scale.w in C.
These tests verify scalar sequencing, not vector numeric behavior, PS2
rounding or arbitrary aliasing; native instruction and provider review cover
the real hardware path.

All twenty-six peers are preserved in production, SKIP_ASM and retained-C
contexts. Production and retained owned data are unchanged. SKIP_ASM adds the
target's 32-byte padded restart diagnostic before existing init strings;
their exact payloads and all affected local/literal relocations resolve
identically. Six same-path baseline/final plain/dump pairs compare complete
assembly and entire raw ELF files literally and equal ordinary-wrapper output.
All 5,568 resolved inputs are stable, including the compiler-directory symlink
and independently checked compiler/assembler loader chains. Both actual
loaders are included; superseded incomplete closure receipts are not used.
