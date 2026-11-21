#include "fclCombineTimeline.h"

extern void cmb_205E68(void);
extern void func_00205EA0(void);
extern void func_00205DB8(void);
extern void cmb_205CE8(void);
extern void cmb_205DF0(void);
extern void func_00205AB8(void);

SIZE_CHECK(cmbSacrificeTimeTableEntrySize, cmbTimeTableEntry_t, 12);

/* Fusion timeline when a third stock devil is selected as sacrifice. */
cmbTimeTableEntry_t D_0032F5D0[7] = {
    {   0, 0, { 0 }, NULL },
    { 280, 0, { 0 }, cmb_205E68 },
    { 281, 1, { 0 }, func_00205EA0 },
    { 385, 0, { 0 }, func_00205DB8 },
    { 420, 1, { 0 }, cmb_205CE8 },
    { 630, 0, { 0 }, cmb_205DF0 },
    { 780, 0, { 0 }, func_00205AB8 },
};
