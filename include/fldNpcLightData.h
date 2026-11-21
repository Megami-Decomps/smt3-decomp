#ifndef FLD_NPC_LIGHT_DATA_H
#define FLD_NPC_LIGHT_DATA_H

#include "common.h"

typedef struct {
    u8 diffuse[3];
    u8 ambient[3];
    u8 faceDiv;
    u8 directionMode;
} fldNpcLightRow_t;

typedef struct {
    fldNpcLightRow_t rows[32];
} fldNpcPalette_t;

extern fldNpcLightRow_t fldNpcLightTable[32];
extern fldNpcLightRow_t fldNpcLightTbl[31]; /* Retail row-1 interior alias. */

#endif
