#ifndef FCLNAMEDSLOTS_H
#define FCLNAMEDSLOTS_H

#include "kernel.h"

/* Named scheduler slots: retail strides entries by 0x44 bytes. */
typedef struct
{
    s32 kind;
    char name[32];
    dds3ProcessID_t *task;
    dds3ProcessFunc_t update;
    /* Cleanup callbacks have no process argument (see the debug registrants). */
    void (*destroy)(void);
    void (*setState)(s32);
    s32 enabled;
    void *userValue;
    s32 previous;
    s32 next;
} fclNamedSlot_t;

SIZE_CHECK(fclNamedSlotSizeCheck, fclNamedSlot_t, 0x44);

extern fclNamedSlot_t fclEncycTable[32];
#endif
