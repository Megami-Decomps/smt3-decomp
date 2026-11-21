#include "common.h"

extern const char D_0053C788[40];
extern const char D_0053C760[40];
extern const char D_0053C738[40];
extern const char D_0053C710[40];
extern const char D_0053C6E8[40];
extern const char D_0053C6C0[40];
extern const char D_0053C6A0[32];

/* Seven texture names; the loading loop stops before the alignment word. */
char *cmbTexturePathTable[7] = {
    (char *)D_0053C788,
    (char *)D_0053C760,
    (char *)D_0053C738,
    (char *)D_0053C710,
    (char *)D_0053C6E8,
    (char *)D_0053C6C0,
    (char *)D_0053C6A0,
};
