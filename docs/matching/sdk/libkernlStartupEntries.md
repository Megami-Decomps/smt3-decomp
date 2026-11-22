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
