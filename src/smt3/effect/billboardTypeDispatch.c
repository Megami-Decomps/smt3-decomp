#include "billboardDispatch.h"

extern struct billHandle_t *dds3Billboard_NormalCreate(void *data);
extern void dds3Billboard_NormalDraw(struct billHandle_t *billboard);
extern void dds3Billboard_NormalDelete(struct billHandle_t *billboard);
extern struct billHandle_t *dds3Billboard_P2ACreate(void *data);
extern void dds3Billboard_P2ADraw(struct billHandle_t *billboard);
extern void dds3Billboard_P2ADelete(struct billHandle_t *billboard);

dds3EffectFunc3_t dds3BillboardFuncTable[2] = {
    { dds3Billboard_NormalCreate, dds3Billboard_NormalDraw, dds3Billboard_NormalDelete },
    { dds3Billboard_P2ACreate, dds3Billboard_P2ADraw, dds3Billboard_P2ADelete },
};
