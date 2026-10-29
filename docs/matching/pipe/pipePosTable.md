# Matching notes: pipe/pipePosTable

## pipeFileLoad (pipe/pipePosTable, 424 bytes)

106 of 106 words match. Promoted in `5d31d1d9`: the complete owner checks
8 match, 0 differ, with ordinary/stock-`-da` output parity and a byte-identical
retail image. The retained algorithm is now ordinary C; the former three-load
residual is not a current exclusion.

The canonical `tagPipePosTable`, `tagPipeTable` and `tagPipeLinkInfo` records
supply the native pointer and count fields. The source preserves the cached
file request, completion polling, three file-relative count/offset pairs,
32-byte position records, eight-byte object entries plus their byte-flag tail,
and the eight-byte link source passed to `LoadLinkData`. Ordered result-member
copies and callback-sensitive count/request reloads remain intact.

The file-provider declarations use the actual opaque request tag, byte-pointer
load result and ignored integer free result. The explicit word-pointer
conversion reads the real resource header, not a typed runtime struct.

Removing the fallback also transfers its three debug-string imports to the C
owner. `D_00548E78`, `D_00548E88` and `D_00548E98` retain all 56 native bytes;
the owner's 16 small-data bytes are unchanged. A text-only check without those
imports is not sufficient qualification.

The historical 103-word result and alternative-copy trials used an older
local result/declaration context. Current exactness does not establish the
original source spelling or identify the compiler pass that closed that
scheduling residual; no new scheduling trick or compiler flag was introduced.
