# Battle camera start and end matching

## nbCameraState_Start_Set (newbattle/nbCameraStartEnd, 948 bytes)

237 of 237 words match at 0x001B2378. Preserve the active-formation width and
height scan, angle selection, provider calls and their delay slots, and both
four-row camera paths. Each path advances a pointer to its own three-float
rows. The X, Y and Z scalar writes keep their order, and the two camera
globals are written before starting the animation.

The two angle literals have the same value and distinct native pool slots.
Observing the initialized default angle before the width branch preserves
both slots. The existing VU0 barrier loads use the real scratch addresses:
the offset transform consumes XYZ, and the two endpoint W lanes can flow
through VU interpolation into scratch that only exposes XYZ to C. No W
initialization, typed whole-object input, or C read of an unwritten lane
is introduced. The local float constraints apply to actual angle, distance
and loop values. They reproduce allocation and ordering, not historical
source spelling.

All 108 bytes of the owner's literal pool remain unchanged. The complete
retail executable is reproduced byte for byte, with no change to other
linked functions or data.
