#include "common.h"

/* The four legacy message lines retain their native eight-byte alignment.
 * The active memory-card messages are separate. */
char D_00528180[] __attribute__((aligned(8))) = "MEMORY CARD slot 1";
char slRetainedCardSlot2[] __attribute__((aligned(8))) = "MEMORY CARD slot 2";
char slRetainedCardSelectPrefix[] __attribute__((aligned(8))) = "Please select a";
char slRetainedCardSelectSuffix[] __attribute__((aligned(8))) = "MEMORY CARD slot.";
