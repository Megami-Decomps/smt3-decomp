# Memory-card RPC failure diagnostic

The error branch of `sceMcUdCheckNewCard` passes the NUL-terminated
diagnostic at `0x0054AEE8` to `scePrintf`. Its original spelling and
newline are preserved. The diagnostic is 31 bytes including the NUL.

The final alignment byte is not part of the string. The four bytes at
`0x0054AF08` remain raw, as do the four bytes at `0x0054AF5C`. Between
them are 80 bytes of retained fdlibm constants, in
[`sdk/libmRetainedConstants`](libmRetainedConstants.md).
