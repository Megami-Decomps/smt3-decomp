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

typedef struct
{
    s32 pm[3];
} sdfIVector3_t;

typedef struct
{
    s32 x;
    s32 y;
} itfVertex_t;

typedef struct
{
    u32 components[4];
} itfColor_t;

extern void sdfMemFreeDirect(void *aHandle);

extern void MoveActive(fclRibbon_t *phandle);

extern void MoveDeactive(fclRibbon_t *phandle);

extern void fclRbnRelease(fclRibbon_t *phandle);

extern itfColor_t D_00332438;

extern itfColor_t D_00332448;

extern void itfMiscDrawSpritePrim2(itfVertex_t *pos, itfColor_t *uv, itfColor_t *color,
                                   u32 z, s32 context, sdfDrawTag_t *pdrawtag);

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

extern void *sdfGrAllocPacketBuff(u32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern sdfGrOt_t sdfGrOtTable[85];
extern void DrawBackFrame(fclRibbon_t *handle, s32 x, s32 y, u32 z,
    s32 alpha, s32 flag, sdfDrawTag_t *tag);
extern void DrawBack(fclRibbon_t *handle, s32 x, s32 y, u32 z,
    s32 alpha, sdfDrawTag_t *tag);
extern void DrawHexagon(fclRibbon_t *handle, s32 x, s32 y, u32 z,
    s32 alpha, s32 width, s32 fixed, sdfDrawTag_t *tag);
extern void DrawElem(fclRibbon_t *handle, s32 x, s32 y, u32 z,
    s32 alpha, sdfDrawTag_t *tag);

void fclRbnDraw(fclRibbon_t *handle, s32 x, s32 y, u32 z,
    s32 alpha, s32 flag, s32 width, s32 ot)
{
    sdfDrawTag_t *tag;
    sdfGrOt_t *entry;

    if (alpha != 0 || (flag & 1) != 0) {
        tag = sdfGrAllocPacketBuff(sizeof(*tag));
        sdfDrawTagInit(tag);
        DrawBackFrame(handle, x, y, z, alpha, flag, tag);
        if (alpha > 0) {
            if ((flag & 2) == 0) {
                DrawBack(handle, x, y, z, alpha, tag);
            }
            DrawHexagon(handle, x, y, z, alpha, width, flag & 4, tag);
            DrawElem(handle, x, y, z, alpha, tag);
        }
        entry = &sdfGrOtTable[ot];
        entry->regFunc(entry, tag);
    }
}

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

extern itfColor_t D_003323E8, D_003323F8;
extern void itfMiscDrawSpritePrim(void *position, void *uv, void *color, s32 z,
                                  sdfTexHandle_t *texture, s32 context, sdfDrawTag_t *tag);

void DrawBackFrame(fclRibbon_t *handle, s32 x, s32 y, u32 z, s32 alpha,
                   s32 flag, sdfDrawTag_t *tag)
{
    itfColor_t color = {{128, 128, 128, 128}};
    itfVertex_t pos[2];

    if (flag & 1) {
        pos[0].x = 0;
        pos[0].y = y;
        pos[1].x = 0x6A0;
        pos[1].y = y + 0x128;
        itfMiscDrawSpritePrim(pos, &D_003323E8, &color, z, handle->tex[0], 0, tag);

        pos[0].x = 0x2000;
        pos[1].x = 0x1960;
        itfMiscDrawSpritePrim2(pos, &D_003323E8, &color, z, 0, tag);
        pos[0].x = 0x6A0;
        pos[0].y = y;
        pos[1].x = 0x1960;
        pos[1].y = y + 0x128;
        itfMiscDrawSpritePrim2(pos, &D_003323F8, &color, z, 0, tag);
    } else if (alpha > 0) {
        color.components[3] = (128 * alpha) >> 7;
        pos[0].x = x;
        pos[0].y = y;
        pos[1].x = x + 0x6A0;
        pos[1].y = y + 0x128;
        itfMiscDrawSpritePrim(pos, &D_003323E8, &color, z, handle->tex[0], 0, tag);

        pos[0].x = x + 0x2000;
        pos[1].x = x + 0x1960;
        itfMiscDrawSpritePrim2(pos, &D_003323E8, &color, z, 0, tag);
        pos[0].x = x + 0x6A0;
        pos[0].y = y;
        pos[1].x = x + 0x1960;
        pos[1].y = y + 0x128;
        itfMiscDrawSpritePrim2(pos, &D_003323F8, &color, z, 0, tag);
    }
}

INCLUDE_RODATA(const s32, "facility/fclRibbon", D_0053D9E8);

INCLUDE_ASM(const s32, "facility/fclRibbon", DrawBack);

extern void DrawHexagonElem(sdfIVector3_t *position, itfColor_t *color,
                            u32 z, sdfDrawTag_t *tag);
extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *tag, sdfTexHandle_t *texture, s32 context);

void DrawHexagon(fclRibbon_t *handle, s32 x, s32 y, u32 z, s32 alpha,
                 s32 width, s32 fixed, sdfDrawTag_t *tag)
{
    itfColor_t color = {{128, 128, 128, 128}};
    sdfIVector3_t position;
    fclribbon_elem_t *element;
    s32 count;
    s32 halfWidth;

    color.components[3] = (128 * alpha) >> 7;
    sdf2DLinkTexPacket(tag, handle->tex[1], 0);

    count = handle->num;
    element = handle->pelem;
    if (count > 0) {
        s32 height = y + 0x48;
        s32 remaining = count;
        halfWidth = width >> 1;
        do {
            if (fixed != 0) {
                s32 left = element->pos_deact.pm[0];
                s32 right = element->pos_deact.pm[2];
                s32 center = x + left + ((right - left) >> 1);

                position.pm[2] = height;
                position.pm[0] = center - halfWidth;
                position.pm[1] = center + halfWidth;
            } else {
                position.pm[2] = height;
                position.pm[0] = x + element->pos_deact.pm[0] - width;
                position.pm[1] = x + element->pos_deact.pm[2] + width;
            }
            DrawHexagonElem(&position, &color, z, tag);
            element++;
        } while (--remaining != 0);
    }
}

void DrawHexagonElem(sdfIVector3_t *std_pos, itfColor_t *color, u32 z, sdfDrawTag_t *pdrawtag)
{
    itfVertex_t pos[2];

    pos[0].x = std_pos->pm[0] - 320;
    pos[0].y = std_pos->pm[2];
    pos[1].x = std_pos->pm[0];
    pos[1].y = std_pos->pm[2] + 160;
    itfMiscDrawSpritePrim2(pos, &D_00332438, color, z, 0, pdrawtag);

    pos[0].x = std_pos->pm[0];
    pos[1].x = std_pos->pm[1];
    itfMiscDrawSpritePrim2(pos, &D_00332448, color, z, 0, pdrawtag);

    pos[0].x = std_pos->pm[1] + 320;
    pos[1].x = std_pos->pm[1];
    itfMiscDrawSpritePrim2(pos, &D_00332438, color, z, 0, pdrawtag);
}

INCLUDE_ASM(const s32, "facility/fclRibbon", DrawElem);




INCLUDE_RODATA(const s32, "facility/fclRibbon", D_0053DA08);

