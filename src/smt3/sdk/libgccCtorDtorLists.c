#include "common.h"

/* The EE GCC runtime lists use an all-ones sentinel and a null terminator.
 * Neither list in this image contains a callback between them. */
void (*__CTOR_LIST__[2])(void) __attribute__((section(".data"))) = {
    (void (*)(void))-1, NULL
};

void (*__DTOR_LIST__[2])(void) __attribute__((section(".data"))) = {
    (void (*)(void))-1, NULL
};
