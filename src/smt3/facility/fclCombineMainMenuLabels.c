#include "common.h"

extern char cmbMenuFuse[8];
extern const char D_0053C7B0[24];
extern char cmbMenuTalk[8];
extern char cmbMenuLeave[8];

/* The cursor passes 4 entries to its list renderer. */
const char *D_0032D1A8[4] = {
    cmbMenuFuse,
    D_0053C7B0,
    cmbMenuTalk,
    cmbMenuLeave,
};
