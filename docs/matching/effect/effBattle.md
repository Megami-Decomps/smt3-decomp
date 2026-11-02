# Matching notes: effect/effBattle

## effBTLCutDraw (effect/effBattle, 0x420)

264 of 264 words match at the native 1,056-byte size, including the six-entry
rotation dispatch table. The complete owner and owned data are checked.

Two local changes resolve the retained draft. Declaring the real `count`
scalar before `frame` and `color` gives their native spill order: count at
stack +0x150, frame at +0x154, color at +0x158. This is ordinary declaration
order, not fabricated storage or padding. That control alone is 186/263
matching words at 1,052 bytes (the previous order is 175/263).

The mark case stores the actual VU result into `marked`, immediately consumed
by the external `dds3EffectPartSetRotateMatrix` call. The existing
`SDF_VU0_STOREMATRIX_NOMEM(&marked)` emits the same four volatile VU stores
without a general compiler memory barrier. There is no intervening C access
to the matrix; native and emitted output both preserve the store-before-call
handoff. No fixed register, empty constraint, artificial pointer mutation,
extra storage or new instruction macro is retained.

The original memory-clobbering store shares the address with argument
register 5 and loses one native address materialization. The direct input-only
bare-address store reproduces both native addresses. This is measured output
evidence, not a claim about the original source or an internal optimizer pass.

An initial exact candidate used the sibling `effEveBEDCutDraw`'s read-write
address macro, but independent ablation showed its read-write operand and
pointer temporary were unnecessary. Both were removed; the direct input-only
form is independently exact. A fixed-register store-address control was only
263/264 and is also discarded. An ordinary shared matrix-pointer lifetime
is worse (162/263); count-first declaration alone is 186/263. Typed
parameter-slot locals limited to the position phase, final draw phase, or
both also fail. No offset-cast scheme or broad permutation search is used.

The original behavior is retained: snapshot currentFrame/list/formation
before initial callbacks, quake limits and printing, the three native
quaternions and aligned matrix storage, invoke-party-dependent rotation,
post-color count/flags snapshots, per-callback parameter-work reloads,
position/rotation/direction/area/color/draw ordering, and final conditional
callbacks followed by the fresh currentFrame read/update. `count` remains the
zero-extended 16-bit table count compared with the increasing signed index;
no invented array bound or input domain is introduced. The existing provider
return/parameter types are unchanged.

The three quaternion quadwords and diagnostic string remain explicitly owned
rodata at their current committed locations. Promotion does not move them,
replace the padded diagnostic string with a new literal, or change unrelated
owner functions.
