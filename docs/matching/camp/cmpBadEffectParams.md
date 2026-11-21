# Camp bad-effect work record

The record at ROM 0x24A300 starts zeroed. The Camp initializer copies
three aligned vectors and fills its radius, height, scale and model pointer,
then the effect creator copies exactly 0x40 bytes. Both sides use the
same typed record and pointer ABI. The following 0x40 bytes remain raw:
the observed accesses do not establish a second record.
