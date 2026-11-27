# nbResultProcess matching notes

## Result score and caption storage

The native initialization loops clear four score records of 0x34 bytes at
`0x574E80` and 64 caption records of 0x10 bytes at `0x574F50`,
ending at the untouched `nbStageState` at `0x575350`. The C owner
defines only those two arrays in zero-initialized storage. With the section
attributes removed, the stock compiler instead emits common symbols; the
attributes place the real storage in its native section. The handler keeps
the same shared types and external symbol references: declaring the arrays
in that unit instead would cause the assembler to fold its symbol references
into section-relative relocations.

The linker's synthetic `.main_bss` starts at the end of `.main`, before
the retail SBSS/BSS alignment boundaries. Making that start explicit
retains the existing loaded zero-fill prefix when an input BSS section has
stronger alignment. Leaving it implicit advances the envelope by four
bytes and adds a load segment to the intermediate ELF. The explicit start
preserves all program headers and native symbol addresses, and the delivered
retail image is byte-identical. These 1,232 bytes only exist in memory and
do not count as matched code or initialized data.

## nbFindTobiwarpLabel (newbattle/nbResultProcess, 0xE78)

788 of 926 words match at the native 3704-byte size; the first current
same-offset difference is +0x3AC. The function remains guarded and receives
no exact matching credit. Its four ordinary float-to-integer casts already
emit native conversion bits; see the corrected [CVT diagnosis](../../levers/impossible.md#corrected-ordinary-casts-can-produce-cvtws).

The item collection loop reads ordinary process RAM: item bytes at +0x98+i
and counts at +0x9C+i, for three iterations. Retail reads the same item byte
at 0x24B5F4 and 0x24B610, with no intervening call or store. The later stack
publication of the item-present flag does not explain that reload. There is
no demonstrated MMIO access or asynchronous writer to justify the draft's
inherited volatile cast.

Removing only that cast emits 3704 bytes with 661/926 words matching, while
all 13 compiled peers and owned data remain exact. The current loss is 127
matching words; the historical approximate 30-word observation is not
reproduced in this source context. Neither score justifies volatile.

One bounded ordinary control overwrites the real `u8 item` with its wrapped
`+0xC0` range-test value, then plainly re-reads the original byte for output.
The predicate still accepts exactly bytes 1..63 and 89..255. This produces
identical target bytes to the ordinary-RAM control, not a new match.

Qualified plain versus diagnostic compiler output agrees across five
allocated sections, 206 non-file symbols, 338 ordered relocations and
normalized assembly. Initial RTL has separate reads at UID932 and UID975.
The first CSE pass removes UID975 and makes output store UID977 consume the
first read's surviving pseudo261, despite the source variable's overwrite.
Thus source-level scalar reuse alone does not remove the original value's
availability. This is measured compiler behavior, not an original-source
claim.

Further work needs a truthful explanation for the native reload as well as
the remaining allocation/scheduling differences. No fabricated memory
barrier, aliasing store or replacement volatile access is warranted.

## nbResultLoadTextures (newbattle/nbResultProcess, 0x1F4)

125 of 125 words match at the native 500-byte size. The loader clears four
0x34-byte score rows and 64 0x10-byte caption rows. It loads the three textures
in order and clears the timer before publishing the third handle. A selected
stock entry has both low flag bits set. It supplies a signed EXP word and an
unsigned level halfword from the same 0xEC-byte unit. The stock view refreshes
after `GetAddExp` only when another score can fit; skipped entries still
reread the stock count at the shared loop tail. The signed quotient uses
truncation toward zero, increments for a positive remainder, and floors at one.
The clear loop does not touch the add-EXP or quotient fields of unused rows.

The score fill cursor points at the actual `resultadd` member; the fixed
negative offsets name other members of that same row. The two address
forms for level and EXP keep the real stock view and adjusted view distinct
until the cap load takes over the argument register. Target-local empty
operands and bindings retain initialized values and native access order.
There are no volatile RAM accesses. The initial stock index is set before
the count guard, giving delay reorg the real zero initializer for the
`blez` slot instead of copying the caption address from the exit path.
The field-displacement/one pair preserves the unit address and first score
value while the scheduler places the cap read before the EXP address add.
The constraints match the compiler output, not an original spelling.

The complete unit preserves all 13 previously matched functions, owned
data and ordered peer relocations. Plain and dump-enabled compiler output
agrees, and the stock link is byte-identical to retail. Native-byte replay
checks texture/EXP provider order and callback-sensitive root changes.
An independent 32-bit build of the same C computation checks score rows,
signed rounding, callbacks and caption clearing. Valid stock and unit
indices remain inherited caller preconditions.
