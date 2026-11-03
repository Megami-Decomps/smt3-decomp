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

## nbResultLoadTextures (newbattle/nbResultProcess, 0x1F4)

18 of 125 words match in the ordinary guarded draft. It emits 512 bytes against 500 native bytes and has 12 excess bytes. It remains under `NON_MATCHING` with its assembly fallback and earns no matching credit. The reproduced inherited draft emitted 528 bytes and matched 6 of 125 native words.

Three edits remove unjustified `volatile` qualifiers from the score and global pointers and replace the cast through `global->akuma[index].flag` with the existing `&global->unitwork[index]` overlay. Both expressions address the native record at global + 0xA54 + 0xEC * index. The typed unit view preserves the unsigned halfword level at record + 0x14; the legacy akuma level field is not an interchangeable substitute. Calls, selected-entry initialization, signed quotient/remainder arithmetic and the minimum-one result are retained. No new bounds, types, register constraints, barriers or calls are added.

Actual assembled output retains dynamic division by 45, its first divide-by-zero check, and the fresh `resultadd` load after the quotient and optional increment stores. It also retains the fresh `DDS3_GBWK` load after `GetAddExp` when another result slot may be selected. Not all native reads survive: rejected entries reuse the cached `stockCount`, whereas native reloads that field at the shared loop tail. There is no intervening call or write on the rejected path; ordinary RAM semantics are assumed, not asynchronous mutation.

Remaining differences include an extra divide-by-zero check generated for the repeated quotient expression, an 80-byte rather than 48-byte frame, register allocation, the combined flag predicate, and scheduling of independent level/experience stores. Direct global field reads, score-pointer iteration, incrementing `resultadd`, and delaying construction of the unit pointer were tested in bounded controls. Pointer iteration reached the native 500-byte size but matched only 6 words; none of these controls matched or justified complicating the minimal cleanup. The repeated quotient was retained rather than choosing a shape merely for its size.

All 13 already matched functions in the complete owner remain exact. Strict compilation of all retained nonmatching bodies succeeds. Plain and dump-enabled pinned target compiler output agrees across allocated sections, symbols, ordered relocation contracts and normalized assembly. These are target compiler dumps, not debugger-hook evidence; no internal causal decision or original spelling is claimed.

The native initialization loops independently establish four 0x34-byte score rows at [0x574E80, 0x574F50) and 64 0x10-byte caption rows at [0x574F50, 0x575350). The existing score symbol's `size:0x30` annotation does not describe that full array. This cleanup changes neither symbol metadata nor object ownership and does not claim that annotation is fixed. Valid stock and unit indices remain inherited caller preconditions, not newly proven bounds.
