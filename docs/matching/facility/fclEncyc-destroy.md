# Encyclopedia registration teardown owner

## DestPhaseRegistTop (facility/fclEncyc, byte-neutral)

`DestPhaseRegistTop` uses the existing `fclEncycRegistAllWork_t` for its
request, seven-font array and process pointer. The constructor allocates the
same 0x1200-byte phase work, initializes the process member and passes its
address to the script starter. The teardown releases fonts before reading
that process pointer, conditionally kills it, then releases the phase work.

The member offsets remain request 0x11DC, fonts 0x11E0 and process 0x11FC.
The process is a pointer accepted by the canonical `dds3KillProcess` provider,
not a signed integer process ID. The typed member access removes the previous
integer-to-pointer argument warning and the destructor's raw member aliases.
It preserves null and sentinel behavior, load/store widths, callback ordering,
and the original snapshot of the phase-work allocation.

The original and corrected complete owners produce identical assembly and
ELF objects in production, SKIP_ASM and retained-C contexts. Direct and
configured-wrapper builds agree, including with implicit-declaration errors
enabled. Independent native and semantic review verifies the existing owner
and provider contracts; no structure, prototype, compilation flag or fallback
changes are required. This is a byte-neutral correctness repair and earns no
new matching credit.

The script starter's preexisting integer-address storage remains separate
work. This correction does not claim general host portability or resolve the
unmatched list-index clamp.
