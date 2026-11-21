# Matching notes: mc/slOverwritePrompts

The memory-card overwrite dialog draws three consecutive English prompt
strings. The next aligned string contains alternate create-file wording;
the current code does not reference it. It remains a separate native
string rather than extending the preceding displayed message.

The four writable C strings are at 0x005286A0, 0x005286C0, 0x005286E8 and
0x00528708. Their native eight-byte placement is reproduced by the stock
compiler. The final string ends at file offset 0x429724, and the following
four zero bytes remain raw before the next existing message owner.
