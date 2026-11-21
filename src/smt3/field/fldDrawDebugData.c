#include "libsdf.h"

/* Numeric debug formats followed by the current draw-resource values. */
u8 fldDbgFmtDec[8] = "%d";
u8 fldDbgFmtHex[4] = "%Xh";
addr_t fldDrawEnv = 0;
sdfTexHandle_t *fldDrawSpriteTex = NULL;
