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
