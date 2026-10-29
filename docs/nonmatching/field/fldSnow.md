# Non-matching drafts: field/fldSnow

## fldSnowInit (field/fldSnow, 0x1B4)

107 of 109 instruction words match at the native 436-byte size. Assembly
remains active. The former draft emitted 444 bytes with 28 matching words.
Native instruction bytes exclude the four-byte alignment after the return.

The two area cases have independent floor locals. The floor-zero path and
completed area-0x18 path branch forward to common seed allocation; only the
remaining area-0x1F floors pass through the final height-step assignment.
This ordinary forward-branch shape reproduces every native branch and
allocation instruction. It avoids duplicating the allocation/lock sequence.

Coordinate chains preserve equal X/Z initialization. Resetting the mode before
those coordinates, and defining the area-0x18 speed before its height step,
recover the native constant registers and first-area store order. The life and
width defaults precede their floor-specific overrides. Qualified compiler
probes confirm normal/dumped/hooked assembly and all RTL output parity for the
nearby control and final draft; independent native/provider review confirms
every early return, value and shared allocation path.

Only +0xE0 and +0xF0 differ: native stores the integer mode before the floor-five
branch and the floating speed in its delay slot; C exchanges those two stores.
Their values and all other instructions agree. Reversing the two area cases
or changing to a switch loses native dispatch; reusing the Y scalar for speed
changes floating allocation. A separate speed local and default-setting order
changes do not close the store exchange. No qualifier, register constraint,
extra operation or artificial dependency forces their order.
