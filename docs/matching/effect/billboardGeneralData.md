# General billboard rows

The native general-billboard bank holds eight eight-byte rows. Each has a
path pointer to an existing C-owned label, a signed halfword at offset 4
used for the constructor index, and a second halfword whose meaning is
unconfirmed. Startup advances the row address by eight for each resource;
the matching exit/count functions establish eight entries, which is also
the full interval of eight path pointers.

The native clone's `BillboardTable` analysis label begins at the first
row's constructor-index field. The C clone accesses that field through the
same typed row array as startup. The change relocates the load against the
array base with an addend of four rather than against a second label at
that address. This is not a second storage object, and the retail linked
instructions are identical. The preceding random state remains raw.
