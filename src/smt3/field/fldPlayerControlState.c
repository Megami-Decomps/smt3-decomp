#include "field.h"

SIZE_CHECK(fldPlayerPointSize, fldResourcePoint_t, 0x30);
SIZE_CHECK(fldUnitControlSize, fldUnitControl_t, 0x0C);

/* World-space point published by the player and retained by its scene object. */
fldResourcePoint_t fldPlayerPosRotWork = {
    .pos = { 0.0f, 0.0f, 0.0f, 1.0f },
    .rot = { 0.0f, 0.0f, 0.0f, 1.0f },
};
fldUnitControl_t fldUnitControl = { 0 };
