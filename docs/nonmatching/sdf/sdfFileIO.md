# Non-matching drafts: sdf/sdfFileIO

## sdfFileCmdAnalyzeFileName (sdf/sdfFileIO, 0x120)

The draft is 288 bytes, the retail size, with 65 of 72 words matching.
Its prefix dispatch follows the retail `strchr(':')` test and fixed-size
comparisons against the existing shared labels. It preserves the special
`cdrom0:/movie/` path, returns -1 for an unknown explicit prefix, and uses the
configured default device when no colon is present. It passes the selected path
through the existing `sdfFileCmdMakeRealFileName`; it adds no strings or data
ownership.

Seven words remain different. At +0x68, the draft clears `$v0` where retail
clears `$a0`. The four selected prefix lengths at +0x74, +0x98, +0xC0 and
+0xE4 likewise land in `$v0` instead of `$a0`; at +0xEC, the final base-plus-
offset uses `$v0` instead of `$a0`. At +0xD8, the draft has `bnel` where retail
has `bne`. The source shape and behavior are clear; this remaining difference
is in the compiler's allocation of the offset value and the branch form.

Two direct alternatives were checked and were further from retail: putting the
no-colon path first produced 61 of 71 words at 284 bytes, and keeping a mutated
path pointer live across the comparisons produced 49 of 72 words at 292 bytes.
