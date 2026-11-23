# SDF image upload queue control storage

The native control block occupies 20 zero-fill bytes at 0x581018. Its five
word fields are the semaphore ID, release-list head and tail, and DMA-tag
head and tail. The image upload owner is its only user. Initialization sets
the semaphore and clears the four queue fields; transfer snapshots the
queue before clearing it under the same semaphore. No consumer, member type,
pointer operation, or lifetime changes. The four bytes at 0x58102C and
neighboring graphics display structure remain raw and separately owned.
