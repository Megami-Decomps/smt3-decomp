# Matching notes: field/fldWap

## fldSetupDoorScenes (field/fldWap, 0xBC8)

754 of 754 words match at 3016 bytes, including both generated eight-entry
mode switches and their owned tables. Modes 1/2 produce opposite horizontal
7.5 vectors; 5/6 use 3.75; 3/4 and 7/8 produce negative/positive vertical
motion at the corresponding speed. Invalid modes or missing matters clear
all three components to positive zero. Horizontal direction switches have no
default stores, preserving prior components for an invalid direction.
Independent bitwise tests of the actual movement macro pass 393,216 cases:
all 65,536 signed-16 modes crossed with directions -1 through 4, including
positive-zero stores and invalid-direction no-write cases.

The loop traverses 256 records with 100-byte door and 0x54-byte work strides.
Both direction variables initialize once before the loop and persist between
doors. They change only after a found matter's matching resource is processed;
a found matter without a matching resource retains the previous direction.
Wrong floor and unrelated kinds leave work untouched. Kind 2 processes every
matching named resource, updating ID and angle with the last match winning;
no match preserves those fields, before the native status/counter/remaining
coordinate clears. Kind 1 initializes each matter ID to -1, performs its
independent primary/secondary lookup and resource scan, then initializes
status, signed modes, counters and coordinates. Native reads of the resource
array and count remain fresh after callbacks, including reloading the actual
resource after strcmp; there is no first-match break or cached pre-call pointer.

Quaternion scratch is ordinary aligned storage initialized to (0,0,0,1).
Component writes through a volatile-qualified view feed the genuine existing
SDF_VU0_LOADVECTOR handoff to vf10. EulerAngleAxisY takes no C argument;
its ToMatrix/VU0 consumer reads vf10. The three nonempty asm regions are these
real hardware loads, not scalar opcode replay or empty-constraint steering.
Angle calculation retains sequential multiply by 180.0f then divide by 3.14f;
the target compiler produces native denominator bits 0x4048F5C2. Wrapping uses
repeated <0 and >360 tests, without integer conversion. Inclusive direction
ranges are tested in order: 45..135, 135..225, 225..315, otherwise 2; thus
135 selects 1, 225 selects 0, 315 selects 3, and NaN falls through to 2.

Ordinary blocks for angle/direction helpers remove 16 excess bytes of join
padding from the do/while(0) control. One resource-pointer binding to `$2`,
activated by an empty `+r` at each fresh resource load, feeds actual ID and
transform reads and dies before EulerAngleAxisY. Removing that binding leaves
21 differing words. The secondary -1 sentinel has no fixed binding: an empty
volatile `+r` retains its native scheduling before the secondary-mode read,
then its real matter-ID store consumes it. Direct assignment leaves three
words different; the nonvolatile operand control leaves eleven. All operands
are initialized truthful values, with no call-crossing clobbered lifetime,
dummy use or claimed memory barrier. These are target-specific reconstruction
aids, not evidence of original source spelling.

The local scene-search declaration now agrees with its provider's
`(dds3Basic_t *, s32, const char *)` contract; actual char-array names need no
u8 casts. Both new lookups and the existing camera caller agree. This scoped
correction does **not** repair the unrelated pre-existing fldFileResolver
const-u8 declaration, which remains a separate type-cleanup opportunity.

Independent complete-owner verification gives 25 match, 0 differ, preserving
all 24 peers, including fldDoor_WarpEx, and checking owned tables/literals.
Integrated retail checksum, full regression, guarded EE/IRX and report gates
passed for source commit `10238297501c18b5320709478fa951516d7120c3`.
Exact-source CI
[41040154047](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41040154047)
completed successfully, including every build/deploy step and retail checksum.

## fldDoor_Warp (field/fldWap, 0x5B8)

366 of 366 words match at 1464 bytes, including the generated six-entry
sequence jump table and the three original padded strings. The same owner
check preserves all 25 peers, including `fldDoor_WarpEx` and
`fldSetupDoorScenes`.

The routine selects the pending or named door, checks the signed floor and
acceptance result, then builds the scene record for the selected warp mode.
A zero destination field falls back to the corresponding full-width world
value, not a truncated halfword. Native callback boundaries are kept:
the selected row's kind can change during acceptance, and the signed link
and code are read after the fade callback. Ordinary record and argument
snapshots retain the actual mode-dependent BGM, elevator, and sequence
setup. The mode-zero paths share one Param4 call but keep the separate
Param3 path for doors without that link.

Localized compiler aids keep the real pending-name, accepted result,
destination operands, and resume index in their native phases. The black
fade's red and green arguments are actual initialized zero inputs in their
outgoing registers. The link conversion uses the original link byte and
its actual signed Param4 use; the code and floor are the real next
arguments. No constraint emits an instruction or stands in for a callback
or general memory barrier. The known loaded-data contracts are unchanged;
no new handling for malformed resource indexes is implied.

## fldDoorParamTable (25,760 data bytes)

The native writable block at `0x303050..0x3094F0` belongs to `fldWap`.
Its existing waypoint type now defines the zero-initialized block in
file-backed data, preserving the separate raw ranges and the following
BGM table. No Door function body changes.

`gFld_ElvId` and `fldDoorTable` keep their native offsets of 4 and 160
bytes into that block. They are non-owning names, not extra C objects.
Configuration and owner checks bind both names to the same native
bounded allocation, and the production proof verifies the owner and
both linked alias addresses without counting them as additional data.
The 34 existing references in `fldWap` and `fldInf` keep their resolved
addresses. Existing NOBITS alias handling is unchanged.
