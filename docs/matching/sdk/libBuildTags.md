# PS2 SDK build tags

Seven existing SDK tags in writable data are 16 bytes each: the fixed-width
`PsIIlib` prefix, a space-padded library name and the build number `2500`.
They are initialized character fields, not NUL-terminated API strings.
The graph, DMA, pad, CD/DVD, RPC, IPU and memory-card fields contain exactly
the original 112 bytes. Existing address labels are retained or recovered
at the native field starts; no runtime references or new uses are added.

The raw zero intervals before the CD/DVD, RPC and IPU tags remain separate.
They are not part of the character fields or credited as C-owned data.
The kernel tag spans a separate existing provider boundary and is not
included. Whole-image identity verifies metadata placement, not a claim
about which original SDK source file declared each field.
