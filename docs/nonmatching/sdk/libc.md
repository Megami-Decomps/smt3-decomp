
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

## gcc2_compiled_

Heap extension emits 584 of 600 native bytes, with 76 of 146 emitted words
matching and 70 differing. Sixteen bytes remain uncovered; first difference
is an exit displacement at +0xA8. The prologue and initial request sequence
match. The second request simplifies to a 32-bit subtract instead of native
64-bit subtraction followed by sign narrowing; later temporaries and the
small-old-top exit also differ. Assembly remains active, with no exact credit.

The request adds the 64-bit top pad to the zero-extended 32-bit need and 16,
then narrows to a signed break increment. A known break base enables page
rounding. A failed first request, or a backward break with a real old top,
returns immediately. Contiguous growth extends the fresh top header.
Noncontiguous growth first charges the request, then charges the gap from
the old top end when the base was already known; it does not replace that
accounting with an assumed break-minus-base identity.

The new header aligns its user pointer to 16 bytes. The second request adds
the front adjustment and a page complement, including a full page for zero
remainder. Failure leaves earlier arena/base updates intact but skips new
top, fencepost and maximum-memory updates. Success uses the entire second
increment in both the arena charge and the new-top size.

A real old top smaller than 16 poisons the NEW top size to one and returns.
Otherwise its usable size is `(oldSize - 12) & ~15`, its previous-in-use bit
is preserved, and two fencepost size words at offsets +4/+8 receive five.
Only a usable old remainder of at least 16 is freed. The typed twelve-byte
fencepost view leaves its first word untouched and avoids treating the
second size word as a free-list pointer. Maximum counters compare the
sign-extended 32-bit arena word as unsigned 64-bit and store that same value.

The initial overlapping-header spelling emits 588 bytes; the truthful
fencepost record emits 584. Explicit signed/unsigned long page constants
and physical field order do not retain the missing conversions. The actual
`_sbrk_r` prototype is supplied before this earlier helper; it is compiler-inert.
No extra pointer copy or dummy width operation was added.
