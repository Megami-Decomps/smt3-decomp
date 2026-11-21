# Ring polygon draw ordering

The ring polygon list selects one of four native render ordering tables by
blend mode, then calls its existing register-tag callback with the chosen
table and draw list. The four pointers resolve to `sdfGrOtTable` entries 43,
43, 44 and 45. This preserves the original targets and relocation addends.
The preceding raw triangle-like words and the eight following raw zero
bytes stay outside the C owner. No draw or creator code changes.
