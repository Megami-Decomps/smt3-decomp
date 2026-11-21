#include "common.h"

/* eglLinkPoly4 and eglLinkTexPoly4 load these two triangle words together. */
#define FACE(a, b, c) ((u64)(a) | ((u64)(b) << 8) | ((u64)(c) << 16))

u64 eglPolyStripWord[1] __attribute__((section(".data"))) = {
    FACE(0, 1, 2) | (FACE(0, 2, 3) << 32),
};

#undef FACE
