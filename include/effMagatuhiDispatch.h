#ifndef EFF_MAGATUHI_DISPATCH_H
#define EFF_MAGATUHI_DISPATCH_H

#include "common.h"

/* Each effect kind has six distinct callback slots. */
typedef struct effMagatuhiDispatch_t {
    void *(*create)(void *);
    void (*draw)(void *);
    void (*destroy)(void *);
    void (*setPosition)(void *, void *);
    void (*setRGBA)(void *, u32);
    void (*setRotateMatrix)(void *, void *);
} effMagatuhiDispatch_t;

extern effMagatuhiDispatch_t dds3MagatuhiCreateTable[5];

#endif
