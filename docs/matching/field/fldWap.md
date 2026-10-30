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
