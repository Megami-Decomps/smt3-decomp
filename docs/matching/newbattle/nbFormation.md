# Battle formation

## nbSetPartyFormation (newbattle/nbFormation, 0x418)

All 262 words match at 0x0028A340. Three scans collect eligible units,
sum their angular spans, and assign positions in formation order.
Group bit 0x100 selects the side, rotation, and interval destination.
The Albion correction retains the native group, model ID, and formation
checks. The diagnostic string keeps its existing aligned owner.

The position scratch has two views of the same aligned 16-byte storage:
the initialized XYZ fields, and the address passed through the existing
vector ABI for a raw quadword transfer. Neither the caller nor the position
setters reads the unwritten fourth word as a C scalar. The existing
position setters transfer physical bytes to the formation and backing pose.
The original-position field receives the same physical bytes. The fourth
word is not assumed initialized, constant, or ignored.

A scoped replay starts at the enemy formation call, varies the existing
stack word, and follows the first position through formation and backing
pose writes. Retail and the linked candidate propagate that word without
a new writer on the observed path. The matched BSMIZ boss command likewise
preserves native unwritten-lane propagation through raw hardware operations.
This reconstructs the retail EE transfer, not a portable guarantee of W.
