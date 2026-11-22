# Kernel startup syscall entries

System-call setup reads the two named helper entries in
`systemCallEntryPairs`. Exec initialization installs its word-copy
helper and the actual resident entry at `0x80074000`, then uses the
third row's syscall index to find and disable an existing entry. The
row's unused entry value remains zero. Argument setup installs its
own byte-copy helper through the single `setArgEntryPairs` row.

The union distinguishes the actual function signatures from a plain
resident address. Existing registration wrappers still receive the
same EE addresses. Four function pointers retain their native R_MIPS_32
relocations; the resident address is a literal, not a relocation.

The final eight-byte SetArg row belongs to writable `.data`. Without
the local section annotation the stock compiler emits it in `.sdata`,
unlike retail. Normal and draft-mode checks preserve the reader's
entire stock object. Neighboring embedded payloads and TLB entries
are unchanged.

## Startup table pointers and TLB entry pairs

The system-call scan publishes one pointer at `0x0052C360`; the TLB
initializer publishes the argument-storage pointer at `0x0052D668`.
Both start null. Each is a four-byte EE pointer, followed by four bytes
of alignment before the next entry array.

The eight TLB entry pairs at `0x0052D670` preserve the 64 native bytes.
The first entry references the real `kCopy_3` provider. The next two
are literal EE entry addresses: the copied resident image at
`0x80075000` and the resume entry inside `_xlaunch` at `0x002E5100`.
The remaining five are initially null; the installer looks up their
runtime addresses. These entries are passed to the existing install
routine, not called through invented C prototypes. The opaque resident
code and its original copy sequence are unchanged.
