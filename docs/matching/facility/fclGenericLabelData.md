# Facility label pointers

The ten native pointers at 0x330900 point to existing encoded messages
and facility labels; the camp item-count formatter reads the first slot
of the second group at 0x330914. The two groups are represented together
so the second has its native four-byte alignment. Two separate five-entry
arrays compile with an unwanted four-byte gap. The last eight bytes are
left raw: there is no demonstrated null-pointer consumer.

The existing interior symbol remains a linker alias at +0x14. The camp
reader keeps its existing HI16/LO16 reference to this part of the data,
with a five-entry declaration and read-only pointed-to bytes.
Both stock data objects preserve all ten native pointer relocations.
