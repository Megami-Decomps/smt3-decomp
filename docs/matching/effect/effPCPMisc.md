# Matching notes: effect/effPCPMisc

## Effect face indices and Radiate axis

Ring draws submit at most 12 packed triangle words against 12 vertices,
or six against eight in the shorter batch. The ONGY, ONGY2, and BUNSIN
paths submit at most 16 words against 32 vertices. Ordinary packed
three-index initializers recover these two live prefixes; the longer
unsubmitted suffixes and the adjacent order-table pointers remain raw.

Radiate's writable four-float axis is loaded twice through VU0.
Keeping the real 16-byte alignment is required for `lqc2`; without the
attribute the stock object is only eight-byte aligned. A writable
declaration puts it in native `.data` without a section attribute; both
producer and consumer objects were identical to the placed-const form.
