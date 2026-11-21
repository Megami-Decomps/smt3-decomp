# Model debug fog controls

The four 24-byte controls edit the existing fog max density, near distance,
min density and far distance, in that order. The pointers address the float
members of the live light-state object. The separate title uses a 12-byte
record and one native format pointer; the next four bytes remain raw.
