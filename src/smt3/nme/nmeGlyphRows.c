#include "common.h"

extern const u8 D_00548690[];
extern const u8 D_005485C8[];
extern const u8 D_00548500[];

const u8 *nmeCharTables[3] = { D_00548690, D_005485C8, D_00548500 };

extern const u8 D_00546978[7048];

/* Mode 3's string-backed rows; cursor movement advances by 32 bytes. */
const u8 *nmeNameListRows __attribute__((section(".data"))) = D_00546978;
