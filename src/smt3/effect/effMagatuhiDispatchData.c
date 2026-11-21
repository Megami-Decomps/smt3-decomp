#include "effMagatuhiDispatch.h"

SIZE_CHECK(effMagatuhiDispatchEntrySize, effMagatuhiDispatch_t, 0x18);

void *effMagaSmokeConvCreate(void *params);
void effMagaSmokeConvDraw(void *handle);
void effMagaSmokeConvDelete(void *handle);
void *effMagaPoint4Create(void *params);
void effMagaPoint4Draw(void *handle);
void effMagaPoint4Delete(void *handle);
void *effMagaCylinderCreate(void *params);
void effMagaCylinderDraw(void *handle);
void effMagaCylinderDelete(void *handle);
void effMagaCylinderSetPosition(void *handle, void *pPos);
void effMagaCylinderSetRGBA(void *handle, u32 abgr);
void effMagaCylinderSetRotateMatrix(void *handle, void *pMatrix);
void *effMagaSpiralCreate(void *params);
void effMagaSpiralDraw(void *handle);
void effMagaSpiralDelete(void *handle);
void effMagaSpiralSetPosition(void *handle, void *pPos);
void effMagaSpiralSetRGBA(void *handle, u32 abgr);
void effMagaSpiralSetRotateMatrix(void *handle, void *pMatrix);
void *effMagaSmokeCreate(void *params);
void effMagaSmokeDraw(void *handle);
void effMagaSmokeDelete(void *handle);
void effMagaSmokeSetPosition(void *handle, void *pPos);
void effMagaSmokeSetRGBA(void *handle, u32 abgr);
void effMagaSmokeSetRotateMatrix(void *handle, void *pMatrix);

/* One operation record for each native effect kind. */
effMagatuhiDispatch_t dds3MagatuhiCreateTable[5] = {
    { effMagaSmokeConvCreate, effMagaSmokeConvDraw, effMagaSmokeConvDelete, NULL, NULL, NULL },
    { effMagaPoint4Create, effMagaPoint4Draw, effMagaPoint4Delete, NULL, NULL, NULL },
    { effMagaCylinderCreate, effMagaCylinderDraw, effMagaCylinderDelete, effMagaCylinderSetPosition, effMagaCylinderSetRGBA, effMagaCylinderSetRotateMatrix },
    { effMagaSpiralCreate, effMagaSpiralDraw, effMagaSpiralDelete, effMagaSpiralSetPosition, effMagaSpiralSetRGBA, effMagaSpiralSetRotateMatrix },
    { effMagaSmokeCreate, effMagaSmokeDraw, effMagaSmokeDelete, effMagaSmokeSetPosition, effMagaSmokeSetRGBA, effMagaSmokeSetRotateMatrix },
};
