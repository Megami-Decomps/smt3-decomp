# nbResultProcess matching notes

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
