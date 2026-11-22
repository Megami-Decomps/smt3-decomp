# Memory-card RPC failure diagnostic

The error branch of `sceMcUdCheckNewCard` passes the NUL-terminated
diagnostic at `0x0054AEE8` to `scePrintf`. Its original spelling and
newline are preserved. The diagnostic is 31 bytes including the NUL.

The final alignment byte is not part of the string. The following 88
readonly bytes at `0x0054AF08` remain raw, with their original extent
and contents. No ownership or semantics are inferred for that span.
