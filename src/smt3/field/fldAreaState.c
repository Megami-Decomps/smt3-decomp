#include "field.h"

SIZE_CHECK(fldAreaStateSize, fldAreaWork_t, 0x138);

/* The original field state starts as zeros in writable .data. */
fldAreaWork_t fldGlobalBase = { 0 };
