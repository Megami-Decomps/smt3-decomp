#include "common.h"
#include "model.h"

/* Atlas U/V, width/height, origin X/Y and two reserved halfwords per icon. */
s16 fldAutoMapIconOfsTable[16][8] = {
    { 25, 59, 11, 11, 5, 8, 0, 0 },
    { 1, 59, 23, 18, 12, 20, 0, 0 },
    { 1, 79, 30, 11, 15, 6, 0, 0 },
    { 1, 56, 22, 22, 11, 11, 0, 0 },
    { 24, 33, 22, 22, 11, 11, 0, 0 },
    { 47, 33, 22, 22, 11, 11, 0, 0 },
    { 24, 56, 22, 22, 11, 11, 0, 0 },
    { 1, 33, 22, 22, 11, 11, 0, 0 },
    { 95, 56, 20, 14, 0, 0, 0, 0 },
    { 96, 41, 20, 14, 0, 0, 0, 0 },
    { 47, 56, 22, 22, 11, 11, 0, 0 },
    { 70, 27, 11, 16, 6, 8, 0, 0 },
    { 111, 17, 11, 16, 6, 8, 0, 0 },
    { 111, 17, 11, 18, 6, 13, 0, 0 },
    { 82, 27, 11, 15, 5, 8, 0, 0 },
    { 111, 1, 13, 15, 6, 7, 0, 0 },
};

/* The retail array is initialized in writable data, not BSS. */
sdfModelRootNode_t *fldAutoMapModels[32] __attribute__((section(".data"))) = { NULL };
