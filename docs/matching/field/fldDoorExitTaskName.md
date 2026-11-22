# Pending field door script name

Six native paths in `fldWap` copy the selected WAP row's `taskName` into
`fldDoorExitMode` at `0x00303040`. The name occupies 15 bytes at offset
`0x55` in a `0x64`-byte WAP row. The field process and event-hit code pass
the destination to script start/end; reset paths clear byte zero. The
declaration describes the bytes supported by those real string consumers.
It neither introduces bounds handling nor changes any copy or reset.

The following byte at `0x0030304F` remains a separate raw byte. The
comparison-only tail helper restores three native bytes that the reference
splitter would trim; the compiler provides all 15 declared bytes, and the
retail link still retains the next object at `0x00303050`.
