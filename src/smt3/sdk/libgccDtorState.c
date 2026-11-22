#include "common.h"

extern void (*__DTOR_LIST__[2])(void);

/* The cursor starts after the list's leading sentinel. */
void (**dtorListCursor)(void) __attribute__((section(".data"))) = &__DTOR_LIST__[1];
s32 dtorFrameDeregistered __attribute__((section(".data"))) = 0;
