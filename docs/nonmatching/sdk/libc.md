
## _free_r

The draft emits all 756 native instruction bytes: 86 of 189 words match and
103 differ, first at+0x38. Allocation and coalescing-path scheduling remain
unresolved. Normal builds retain assembly; no exact credit is claimed.

The chunk header precedes the user pointer by 8 bytes. Its previous size and
size are 32-bit words; free-list forward/backward links occupy +8/+0xC. The
current size clears only bit 0, while the successor size clears both low bits.
Backward coalescing consumes the previous-size word only when bit 0 is clear.
Forward coalescing checks the in-use flag in the chunk after the successor.
The top merge publishes the combined chunk and invokes the matched trim
helper only when its zero-extended size reaches the 64-bit threshold.

The arena is 0x408 bytes, with bitmap +4, top +8 and 127 pairs of bin links at
+0x10. Bin sentinels are overlapping chunk-header views whose links reside
in those pairs; their address is arena+index*8. Last-remainder sentinel is
arena+8. The integer address expressions derive those actual header positions
inside the arena, without pointers before the arena or synthetic padding.
Both last-remainder merge cases preserve list membership and bypass insertion.

Small sizes below 0x200 use size>>3. Large sizes use size>>9 to select the
native thresholds 5,0x15,0x55,0x155,0x555 and the corresponding shifts/offsets;
the otherwise unreachable zero case remains explicit. Bitmap shifts use a
64-bit one before narrowing. Empty large bins publish the bitmap; populated
bins traverse forward links in descending masked-size order.

Actual link values replace a long-lived cached sentinel; branch-local link
stores before the small-bin bitmap update recover native size and 86 matching
words. A cached arena pointer shortened the body to 716 bytes; combining the
large-bin anchor and predecessor locals shortened it to 748 bytes. Both were
removed. No declaration permutations, register constraints or dummy uses.

A qualified `cc1probe.sh` observation (`--func _free_r --alloc --no-dumps
--cflag -DSKIP_ASM`) preserves plain/hooked/unhooked output parity. Global
allocation assigns size pseudo 91 (27 references, 120 live instructions,
priority 9000) to GPR5, successor pseudo 87 (8/33, priority 7272) to GPR8, and
chunk pseudo 86 (27/153, priority 7058) to GPR9. Native keeps the chunk in GPR9
but uses size GPR8 and successor GPR5. A new ordinary lifetime/control premise
is needed; types and output semantics do not justify pinning that pair.
