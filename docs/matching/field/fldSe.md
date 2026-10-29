# Matching notes: field/fldSe

## fldBgmBitChk (field/fldSe, 0x7C)

30 of 31 words match at the exact 124-byte size when compiled independently.
The first event-result branch at +0x2C lacks native's annul bit. The area-0x22
gate, opposite event-bit tests and two fade-out/play paths are preserved.
`fldGb` is a scalar in `.data`; `gFldAreaBgm` is a signed halfword in `.data`.
The actual event-bit provider takes and returns `s32`.

Current-unit isolation identifies a concrete dependency: making only the real
`fldBgmPlay` C definition visible closes all 31 words. Making each of the
other three preceding guarded definitions visible individually leaves 30.
Playback remains nonmatching, so the caller retains its assembly fallback.

Paired qualified reorg traces preserve normal, dumped and instrumented assembly
and all intermediate representations. First-branch UID59 retains probability
3999 and `mostly_true_jump` returns zero in both controls. With the playback
callee opaque, `can_throw` stops the fallthrough liveness scan at call UID74;
the scan reports only the stack pointer live and accepts return-address
restore UID174 with flags 0x11, without annulling. With the C definition
visible, the scan reaches the return-address use and reports both stack and
return-address registers live; the restore is selected with annulment. This
qualifies a callee-visibility effect rather than a probability change. Promotion
awaits the complete playback implementation.

## fldBgmPlay (field/fldSe, 0x2D0)

161 of 180 words match at the native 720-byte size. The older nested playback
gate emitted 692 bytes with 142 matching words in the native overlap. Returning
early when `gFldStart == 1` leaves the actual playback call at the function
tail, recovering native's sibling call and separate epilogue. The 19 residual
words concern caller-saved register choices and scheduling in the initial
bank lookup, the area-14 refresh and the final event-table bank lookup.

Native reads the start gate immediately after direct track resolution. Fallback
paths reload it after their event callbacks; the shared late source test
preserves both cases. An unconditional pre-fallback snapshot would not model
those reads. The decimal-area-20 floor routing remains distinct from the later
hexadecimal area 0x20 event-table case. Both event-table arms use `floor + 1`,
update the cached event result, then refresh the current bank and track.

Independent native/provider review confirms the initial floor skip, area bounds,
area-14 stop/reset path, special floors, event-table reloads and final audio
updates. Reading the just-published bank through the global rather than its
local leaves the same 19 differences. Assembly remains active and no exact
matching credit is claimed.
