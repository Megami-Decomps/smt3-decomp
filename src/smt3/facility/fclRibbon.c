#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 stat;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 lw_cur[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 pwork;
} fclEncyc_instance_tag;

typedef struct
{
    s32 pm[4];
} sdfIVector4_t;

/* Native entries embed four 16-byte rectangles and two texture handles. */
typedef struct
{
    sdfIVector4_t pos_act;
    sdfIVector4_t pos_deact;
    sdfIVector4_t uv_act;
    sdfIVector4_t uv_deact;
    sdfTexHandle_t *tex[2];
} fclribbon_elem_t;

typedef struct
{
    /* 0x00 */ void *allocation;
    /* 0x04 */ sdfTexHandle_t *tex[2];
    /* 0x0C */ s32 stat;
    /* 0x10 */ s16 act;
    /* 0x12 */ u16 deact;
    /* 0x14 */ s32 ctr_act;
    /* 0x18 */ s32 ctr_deact;
    /* 0x1C */ s32 ctr;
    /* 0x20 */ s32 num;
    /* 0x24 */ fclribbon_elem_t pelem[0];
} fclRibbon_t;

extern void sdfMemFreeDirect(void *aHandle);

extern void MoveActive(fclRibbon_t *phandle);

extern void MoveDeactive(fclRibbon_t *phandle);

extern void fclRbnRelease(fclRibbon_t *phandle);

extern void fclRbnSwitchActive(fclRibbon_t *phandle, s32 index);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern void fclRbnRelease(fclRibbon_t *phandle);

extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);

fclRibbon_t *fclRbnInit(s32 num, s32 act, sdfTexHandle_t *tex0, sdfTexHandle_t *tex1)
{
    sdfMemHandle_t *allocation;
    fclRibbon_t *handle;

    allocation = sdfMemAllocDirect(sizeof(*handle) + num * sizeof(fclribbon_elem_t));
    handle = sdfMemLock(allocation);
    handle->num = num;
    handle->allocation = allocation;
    handle->tex[0] = tex0;
    handle->tex[1] = tex1;
    handle->act = act;
    handle->deact = -1;
    handle->ctr_act = 0;
    handle->ctr_deact = 0;
    handle->ctr = 0;
    handle->stat = 0;
    return handle;
}

void fclRbnRelease(fclRibbon_t *phandle)
{
    sdfMemFreeDirect(phandle->allocation);
}

void fclRbnCalc(fclRibbon_t *phandle)
{
    if (phandle->stat & 1) {
        MoveActive(phandle);
    }
    if (phandle->stat & 2) {
        MoveDeactive(phandle);
    }
    phandle->ctr = (phandle->ctr - 2) & 0x7F;
}

INCLUDE_ASM(const s32, "facility/fclRibbon", fclRbnDraw);

void fclRbnSwitchActive(fclRibbon_t *phandle, s32 index)
{
    if (index >= 0 && index < phandle->num && index != phandle->act)
    {
        phandle->deact = phandle->act;
        phandle->stat |= 3;
        phandle->act = index;
        phandle->ctr_act = 0x40;
        phandle->ctr_deact = 0x80;
        phandle->ctr = 0;
    }
}

void fclRbnSetPara(fclRibbon_t *handle, s32 index,
    const sdfIVector4_t *pos_act, const sdfIVector4_t *pos_deact,
    const sdfIVector4_t *uv_act, const sdfIVector4_t *uv_deact,
    sdfTexHandle_t *tex_act, sdfTexHandle_t *tex_deact)
{
    fclribbon_elem_t *entry;

    if (index >= 0 && index < handle->num) {
        entry = &handle->pelem[index];
        memcpy(&entry->pos_act, pos_act, sizeof(*pos_act));
        memcpy(&entry->pos_deact, pos_deact, sizeof(*pos_deact));
        memcpy(&entry->uv_act, uv_act, sizeof(*uv_act));
        memcpy(&entry->uv_deact, uv_deact, sizeof(*uv_deact));
        entry->tex[0] = tex_act;
        entry->tex[1] = tex_deact;
    }
}

INCLUDE_ASM(const s32, "facility/fclRibbon", dumpCursorInfo);

void MoveActive(fclRibbon_t *phandle)
{
    phandle->ctr_act -= 8;
    if (phandle->ctr_act <= 0) {
        phandle->ctr_act = 0;
        phandle->stat &= ~1;
    }
}

void MoveDeactive(fclRibbon_t *phandle)
{
    phandle->ctr_deact -= 0x10;
    if (phandle->ctr_deact <= 0) {
        phandle->ctr_deact = 0;
        phandle->stat &= ~2;
    }
}

INCLUDE_ASM(const s32, "facility/fclRibbon", DrawBackFrame);

INCLUDE_ASM(const s32, "facility/fclRibbon", DrawBack);

INCLUDE_ASM(const s32, "facility/fclRibbon", DrawHexagon);

INCLUDE_ASM(const s32, "facility/fclRibbon", DrawHexagonElem);

INCLUDE_ASM(const s32, "facility/fclRibbon", DrawElem);

INCLUDE_RODATA(const s32, "facility/fclRibbon", D_0053D9D8);

INCLUDE_RODATA(const s32, "facility/fclRibbon", D_0053D9E8);

INCLUDE_RODATA(const s32, "facility/fclRibbon", D_0053D9F8);

INCLUDE_RODATA(const s32, "facility/fclRibbon", D_0053DA08);

