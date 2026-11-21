#include "common.h"

extern const char D_0053DCD0[24];
extern const char D_0053DCE8[16];
extern const char D_0053DCF8[16];
extern const char D_0053DD08[16];
extern const char D_0053DD18[24];
extern const char D_0053DD30[24];
extern const char D_0053DD48[16];
extern const char D_0053DD58[16];

/* The menu renderer reads these strings as unsigned encoded bytes. */
const u8 *D_00342FD0[2] __attribute__((section(".data"))) = {
    (const u8 *)D_0053DCE8, (const u8 *)D_0053DCD0,
};
const u8 *D_00342FD8[2] __attribute__((section(".data"))) = {
    (const u8 *)D_0053DCE8, (const u8 *)D_0053DCF8,
};
const u8 *D_00342FE0[2] __attribute__((section(".data"))) = {
    (const u8 *)D_0053DD18, (const u8 *)D_0053DD08,
};
const u8 *D_00342FE8[4] = {
    (const u8 *)D_0053DD58, (const u8 *)D_0053DD48, (const u8 *)D_0053DD30, NULL,
};
