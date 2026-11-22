# Moon process and camera debug text

ROM `0x450780` holds the five bytes of `"moon"`, including its terminator;
`evtInitMoonProcess` passes that name to the process allocator. ROM `0x450790`
holds the three bytes of `"%s"`; the camera debugger passes the selected
camera name to that format. The following zeros are alignment, not
message characters. The existing camera default pointer, page, event-number selector,
and scene display state keep their native writable storage and addresses.

ee-gcc 2.96 aligns these strings to eight bytes, but the original moon
small-data prefix starts on only a four-byte boundary. The data-only fragment
starts at the native aligned string address; the linked positions, original
code, and fallback camera-name pointer target do not change.
