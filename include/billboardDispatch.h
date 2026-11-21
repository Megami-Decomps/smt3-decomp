#ifndef BILLBOARD_DISPATCH_H
#define BILLBOARD_DISPATCH_H

struct billHandle_t;

typedef struct dds3EffectFunc3_t {
    struct billHandle_t *(*create)(void *);
    void (*draw)(struct billHandle_t *);
    void (*destroy)(struct billHandle_t *);
} dds3EffectFunc3_t;

extern dds3EffectFunc3_t dds3BillboardFuncTable[];

#endif
