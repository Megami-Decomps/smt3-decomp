# pb/pbModel residuals

## pb_draw_mapbase_Process (pb/pbModel, 0x214)

133 of 133 words match at the native 532-byte size. The complete owner has
28 matches and no differences, including the unchanged 284-byte block renderer.
A single local binding puts the real panel pointer in native register 4. It is
initialized from `id->workdata`, consumed by the state/work loads and possible
flag clear, and dead before the first call. The binding is a localized compiler
reconstruction aid, not a claim about original source spelling. No empty asm,
new memory access or artificial consumer is used.

The block renderer accepts the signed 32-bit callback result and converts it to
its actual unsigned-byte draw flag at entry. Native callers pass the wide value
without masks; native provider immediately masks it to eight bits. Its public
prototype and definition now agree on that wide input with an explicit `u8`
local. This keeps its own instructions exact and removes two artificial caller
masks induced by the previous narrow parameter. All known declarations/calls
are in this owner; the two call sites and provider are checked together.

The map contains 20 by 20 unsigned halfwords at work offset 0x1C. Its draw
counter is at 0x36C. Each tile first draws unless bit 0x800 is set, then draws
its second face independently when its upper three bits equal one. The second
predicate reloads the map after the first draw. Each draw also reloads the
global work pointer before updating its counter. Callback coordinates remain
signed halfwords; table ownership, callback results, accumulator updates and
ordering-table registration are unchanged.

The latest ordinary baseline matches 124/133 words. Binding the panel alone
matches 131/133, retaining two caller masks. Correcting the provider boundary
alone matches 126/133; both changes give 133/133. Moving the explicit flag
conversion past the clears changes provider code and was rejected. Actual
plain/dump compiler output has equal allocated sections, relocations and
normalized assembly. Independent current-tree qualification passes: all owned code/data, four
remaining drafts, the exact retail checksum, 26 tool programs and seven IRX
groups. Whole-tree draft evidence is reused only for unchanged inputs.

## pbDrawMapPolyTransformed

The retained source emits 1060 bytes against 1056 native bytes. Its bounded
prefix has 29 matching words and 235 differing words; four emitted bytes
exceed the native extent. The frame is 0x570 versus native 0x560. Allocation
and scheduling differences remain broad; assembly remains active.

Native geometry uses two distinct 36-vector local arrays and the existing
44-byte request. Only five faces are emitted, six vertices per enabled face;
the signed point count is returned. Six-face template data corroborates the
array capacities. Each active face snapshots all four base components before
the inner conversion calls.

Selector 6 scales xyz in binary64 by 0.5, leaving w intact. Selector 1 applies
its binary32 height change before sequential binary64 x/z adjustments; each
assignment rounds back to float. Opposing mask bits retain both operations.
Four distinct readonly labels hold the double word 0x3FC9999999999999.
Mutable geometry/color declarations are preserved: read-only use in this
function does not establish global const storage.

Replacing only the normal-copy primitive with a plain-C quadword copy emits
1040 bytes: 8 prefix words match, 252 differ and 16 native bytes are uncovered.
That control was discarded. A separate indexed/literal reconstruction emits
1048 bytes with 10 of 262 words matching and unqualified readonly relocation
ownership; an explicit input-row offset is inert. Its cursor form emits
1064 bytes with 24 matching prefix words. Those duplicate bodies were retired;
the existing published source remains preferred, with no additional credit.

## pb_draw_exit_Process (pb/pbModel, 0x1C0)

112 of 112 words match at 448 bytes in ordinary C. A state switch handles
1 / 5 / default, followed by a positive drawing guard and shared return.
State 1 clears the full panel flags when the panel exists; state 5 returns
-1; every other unsigned-16 state continues. Any nonzero signed-32 drawing
flag, including negative values, draws. Both real local-array clears remain
before dispatch. No constraints, register bindings or type changes are used.

Actual literal ownership matters here. The configured legacy compiler emits
native double bits `0x3f947ae147ae147a` from `0.02`; this is not a claim about
modern decimal parsing or rounding. The real binary32 tau literal is written
`6.28318500518798828125f`: a shorter tested decimal emitted one ULP too low.
The four native float words are `40490fda / 40c90fda / c0490fda / 40c90fda`.
The target's now-reproduced `D_00535C80` rodata import is removed; reference
and ownership checks found no other consuming function. Other imports remain
unchanged, and complete-owner checks verify the resulting owned data.

Angle update keeps float-to-double promotion, the native addition and one
float assignment before strict upper/lower tests. It wraps once, leaves equal
endpoints alone and preserves the native exceptional-value instruction
sequence rather than substituting host behavior. Signed-16 coordinates times
40 minus 380 fit signed32 for the full input range. Material, matrix, polygon
and late-loaded ordering-table callbacks retain their order and observations;
no pointer is cached across a callback where the native code reloads it.
Existing pointer-validity requirements remain unchanged.

Independent complete-owner verification gives 27 match, 0 differ, preserving
26 peers and normal owned-data checks. Full qualification passes the retail
checksum, 149 guarded owners / 574 drafts, seven IRX owners, 26 tooling files /
427 tests, shell mocks and docs checks. Actual-published verification repeats
the owner, retail, report, all 28 owner dependencies, all 780 tracked files
and final Ninja no-op checks. Source commit
`326dd982de60131a3bd0dc421456a83a75fdbd51` has exact-source hosted CI
[41003758538](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41003758538):
build and deploy both completed successfully.

