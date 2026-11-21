#include "fclCombineTimeline.h"

extern cmbTimeTableEntry_t D_0032F5A0[4];
extern cmbTimeTableEntry_t D_0032F5D0[7];

SIZE_CHECK(cmbTimeTableSize, cmbTimeTable_t, 8);

/* Ordinary and sacrifice fusion select their respective event schedules. */
cmbTimeTable_t cmbTimeTable[2] = {
    { D_0032F5A0, 4, { 0 } },
    { D_0032F5D0, 7, { 0 } },
};
