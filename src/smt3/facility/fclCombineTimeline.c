#include "fclCombineTimeline.h"

extern void cmb_205CE8(void), cmb_205DF0(void), func_00205AB8(void);

SIZE_CHECK(cmbTimeTableEntrySize, cmbTimeTableEntry_t, 12);

/* Ordinary two-demon fusion timeline. */
cmbTimeTableEntry_t D_0032F5A0[4] = {
    { 0,   0, { 0 }, NULL },
    { 250, 1, { 0 }, cmb_205CE8 },
    { 430, 0, { 0 }, cmb_205DF0 },
    { 580, 0, { 0 }, func_00205AB8 },
};
