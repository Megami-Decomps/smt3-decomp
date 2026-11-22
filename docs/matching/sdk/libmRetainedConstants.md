# Retained fdlibm constants

The 80-byte span at `0x0054AF0C` contains fourteen binary32 constants
from `acosf`, followed by six from `atan2f`, in the order of the
corresponding fdlibm declarations. The retail routines materialize the
constants they use directly; the retained objects are not a live lookup
table. No code was redirected to them. The initial and final raw words at
`0x0054AF08` and `0x0054AF5C` are unchanged.

Use individual float objects in `.rodata` so they retain the native
four-byte alignment and don't migrate to small data. Two arrays emit the
same words but raise the section alignment to eight, which would insert
padding at the start. The retained objects match the retail binary32
values, including where upstream spellings would round differently.
