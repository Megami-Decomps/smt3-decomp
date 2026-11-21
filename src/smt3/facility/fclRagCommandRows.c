#include "common.h"

/* Same ten-byte record as the list builder in fclRagInit. */
typedef struct {
    u8 reserved00[2];
    s16 eventBit;
    u8 hearts[2];
    u8 list2[2];
    u8 list3[2];
} ragListConfig_t;

ragListConfig_t gCommandFlag_0[2] = {
    { { 0, 0 }, -1, { 1, 1 }, { 1, 1 }, { 1, 1 } },
    { { 0, 0 }, -1, { 1, 1 }, { 1, 1 }, { 1, 1 } },
};
