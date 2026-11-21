# SDK alarm entry pairs

The native alarm table at 0x52D2B0 consists of eight writable, eight-byte
rows. Its only source consumer is `InitAlarm`, and both translation
units use the same declaration in `sdk_kernel_alarm_entries.h`.

Row zero initializes and reads the `copy` member with the real
`kCopy_2` signature. The EE address conversion passes that
function address as the second argument of the existing syscall wrapper
`gcc2_compiled__17`, which does not call it as a C function. Row one
initializes and reads the `resident` address of the copied payload. The
other six rows' stored pointers are null and are never read by
`InitAlarm`, which uses their indices to find the resident entries.
No access reads a different union member from the one initialized.

The table has one named R_MIPS_32 reference, at +4 to `kCopy_2`. The
consumer object is unchanged. Both stock modes match all 64 native bytes,
and the private whole link matches retail. The two adjacent raw payloads
and all TLB data are unchanged.
