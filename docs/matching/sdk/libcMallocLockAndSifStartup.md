# Matching notes: heap lock and SIF startup words

The heap lock uses signed owner and recursion-count words. The unlocked
owner starts at -1 and the count starts at zero. The SIF command
initialization guard and RPC initialization guard start at zero. The
native init paths load and set them as words; the exit paths reset them
with word stores. The command guard retains its existing unnamed label.

Each section attribute is necessary: removing it alone sends that word
to .sdata in both stock modes. The two pairs surround a separate 40-byte
raw range. Its version bytes, pointers, and two named references remain
untouched, and no existing caller or provider code changes.
