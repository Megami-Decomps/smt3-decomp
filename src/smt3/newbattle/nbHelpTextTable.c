#include "common.h"

extern const char D_00545868[24];
extern const char D_00545850[24];

/* The pointed-to encoded messages are immutable; the table is native .data. */
const char *nbHelpTextTable[2] __attribute__((section(".data"))) = {
    D_00545868,
    D_00545850,
};
