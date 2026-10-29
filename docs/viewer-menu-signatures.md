# Viewer item callback signatures

The menu dispatcher invokes item callbacks with a tag, encoded X/Y and a signed
row ordinal. Its existing `evtMenuItem_t` typedef is
`void (*)(void *, s32, s32, s32)`. Native `evtMenuDisp` passes exactly those four
arguments and ignores any callback return value.

The local `evtFrameListItem` declaration and definition now return `void`.
The body has no return expression, and its only source use is the frame-list
menu's item callback. No caller consumes a result. The four local callbacks
`evtFloatChangeItem`, `evtIntChangeItem`, `evtFrameChangeItem` and
`evtFileName2Item` now take the same signed `s32` ordinal as the dispatcher and
all its other item callbacks. Their actual menus use nonnegative row indices;
none relies on an unsigned ordering or a value outside the signed range. Their
operations and every switch arm remain unchanged.

This removes incompatible C callback function types rather than hiding them
with casts or claiming that equal EE register transport makes them compatible.
Changing the false return type to `void` exposes sibling calls and emits
248 bytes against native's 224. The body therefore uses one shared terminal
`key = NULL` after either rendering arm, closing the actual child-list traversal
cursor. This is the narrow post-call own-state reset exception in the canonical
matching rules, not a dummy return, fake consumer, or assertion that native
contains a reset store. The cursor is genuinely initialized, traversed and
passed to `evtFrameListItemSub`; the trailing new-frame arm has no selected key.
Native retains JAL plus epilogue at `0x001F71F8` and `0x001F7230`.

Qualified stock RTL preserves cursor pseudo88's reset UID228 in `00.rtl`,
`01.sibling` and `11.cfg`, then removes it in `13.life`; the actual Sub/Link calls
UID159/222 remain ordinary calls after the sibling pass. Complete plain/`-da`
assembly parity passes, normalizing only random scratch `.file` prefixes.
This is stock pass evidence, not a hooked GDB decision trace. The compiler is
unchanged. A native/DDS return-value contract is not invented to suppress calls.

No shared header, runtime provider or data layout changes. It is
a zero-credit prerequisite: with `evtMenuDisp` still using its original ASM
fallback, all 56 existing owner functions and native data remain exact and
`ninja` reproduces retail SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`. The separate dispatcher promotion
adds the new 448-byte match.
