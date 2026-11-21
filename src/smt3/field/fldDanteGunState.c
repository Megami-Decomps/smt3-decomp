#include "field.h"

SIZE_CHECK(fldDanteGunStateSize, fldDanteGunState_t, 0x60);

/* Runtime field initialization fills in the starting gun and sky state. */
fldDanteGunState_t fldRunDanteGunFlag = { 0 };
