#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

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

#ifdef NON_MATCHING
extern void *memcpy(void *, const void *, unsigned int);
#endif

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

extern s32 printf(const char *format, ...);

#ifdef NON_MATCHING
void dumpCursorInfo(fclRibbon_t *handle)
{
    s32 count = handle->num;
    fclribbon_elem_t *entry = handle->pelem;
    s32 i;

    for (i = 0; i < count; i++, entry++) {
        printf("**[%2d]**********************\n", i);
        printf("  pos_act  = %3d, %3d, %3d, %3d\n",
               entry->pos_act.pm[0] >> 4, entry->pos_act.pm[1] >> 3,
               entry->pos_act.pm[2] >> 4, entry->pos_act.pm[3] >> 3);
        printf("  pos_deact= %3d, %3d, %3d, %3d\n",
               entry->pos_deact.pm[0] >> 4, entry->pos_deact.pm[1] >> 3,
               entry->pos_deact.pm[2] >> 4, entry->pos_deact.pm[3] >> 3);
        printf("  uv_act   = %3d, %3d, %3d, %3d\n",
               entry->uv_act.pm[0] >> 4, entry->uv_act.pm[1] >> 4,
               entry->uv_act.pm[2] >> 4, entry->uv_act.pm[3] >> 4);
        printf("  uv_deact = %3d, %3d, %3d, %3d\n",
               entry->uv_deact.pm[0] >> 4, entry->uv_deact.pm[1] >> 4,
               entry->uv_deact.pm[2] >> 4, entry->uv_deact.pm[3] >> 4);
        printf("  texhandle = %08x\n", (u32)&entry->tex[0]);
    }
}
#else
INCLUDE_ASM(const s32, "facility/fclRibbon", dumpCursorInfo);
#endif

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

extern itfColor_t D_00332408, D_00332418, D_00332428;

void DrawBack(fclRibbon_t *handle, s32 x, s32 y, u32 z, s32 alpha, sdfDrawTag_t *tag)
{
    itfColor_t color = {{128, 128, 128, 128}};
    itfVertex_t position[2];

    color.components[3] = (128 * alpha) >> 7;
    position[0].x = x;
    position[0].y = y + 0x90;
    position[1].x = x + 0x6E0;
    position[1].y = y + 0xA0;
    itfMiscDrawSpritePrim(position, &D_00332408, &color, z, handle->tex[0], 0, tag);
    position[0].x = x + 0x2000;
    position[1].x = x + 0x1920;
    itfMiscDrawSpritePrim2(position, &D_00332408, &color, z, 0, tag);
    position[0].x = x + 0x1E0;
    position[0].y = y + 0x30;
    position[1].x = x + 0x500;
    position[1].y = y + 0xF8;
    itfMiscDrawSpritePrim2(position, &D_00332418, &color, z, 0, tag);
    position[0].x = x + 0x1B00;
    position[1].x = x + 0x1E20;
    itfMiscDrawSpritePrim2(position, &D_00332428, &color, z, 0, tag);
}

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

extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *tag, s32 type, s32 context);
extern itfColor_t D_0053DA08;

#ifdef NON_MATCHING
void DrawElem(fclRibbon_t *handle, s32 x, s32 y, u32 z,
              s32 alpha, sdfDrawTag_t *tag)
{
    itfColor_t color = D_0053DA08;
    itfVertex_t pos[2];
    fclribbon_elem_t *entry = handle->pelem;
    s32 count = handle->num;
    s32 i;
    s32 spread;

    color.components[3] = (s32)((u32)alpha << 7) >> 7;
    for (i = 0; i < count; i++, entry++) {
        pos[0].x = x + entry->pos_deact.pm[0];
        pos[0].y = y + entry->pos_deact.pm[1];
        pos[1].x = x + entry->pos_deact.pm[2];
        pos[1].y = y + entry->pos_deact.pm[3];
        itfMiscDrawSpritePrim(pos, &entry->uv_deact, &color, z, entry->tex[0], 0, tag);
        if (handle->act == i) {
            pos[0].x = x + entry->pos_act.pm[0];
            pos[0].y = y + entry->pos_act.pm[1];
            pos[1].x = x + entry->pos_act.pm[2];
            pos[1].y = y + entry->pos_act.pm[3];
            itfMiscDrawSpritePrim(pos, &entry->uv_act, &color, z, entry->tex[1], 0, tag);
        }
    }
    if (handle->stat & 2) {
        color.components[3] = (s32)((u32)handle->ctr_deact * (u32)alpha) >> 7;
        entry = &handle->pelem[handle->deact];
        pos[0].x = x + entry->pos_act.pm[0];
        pos[0].y = y + entry->pos_act.pm[1];
        pos[1].x = x + entry->pos_act.pm[2];
        pos[1].y = y + entry->pos_act.pm[3];
        itfMiscDrawSpritePrim(pos, &entry->uv_act, &color, z, entry->tex[1], 0, tag);
    }
    itfMiscDrawAlphaPrim(tag, 1, 0);
    if (handle->stat & 1) {
        s32 timer = handle->ctr_act;

        color.components[3] = (s32)((u32)timer * (u32)alpha) >> 7;
        entry = &handle->pelem[handle->act];
        pos[0].x = x + entry->pos_act.pm[0] - timer * 2;
        pos[0].y = y + entry->pos_act.pm[1] - (timer >> 1);
        pos[1].x = x + entry->pos_act.pm[2] + timer * 2;
        pos[1].y = y + entry->pos_act.pm[3] + (timer >> 1);
        itfMiscDrawSpritePrim(pos, &entry->uv_act, &color, z, entry->tex[1], 0, tag);
    }
    if (handle->ctr < 0x40) {
        s32 timer = handle->ctr;

        spread = 0x40 - timer;
        color.components[3] = (s32)((u32)timer * (u32)alpha) >> 7;
        entry = &handle->pelem[handle->act];
        pos[0].x = x + entry->pos_act.pm[0] - spread * 2;
        pos[0].y = y + entry->pos_act.pm[1] - (spread >> 1);
        pos[1].x = x + entry->pos_act.pm[2] + spread * 2;
        pos[1].y = y + entry->pos_act.pm[3] + (spread >> 1);
        itfMiscDrawSpritePrim(pos, &entry->uv_act, &color, z, entry->tex[1], 0, tag);
    }
    itfMiscDrawAlphaPrim(tag, 0, 0);
}
#else
INCLUDE_ASM(const s32, "facility/fclRibbon", DrawElem);
#endif

INCLUDE_RODATA(const s32, "facility/fclRibbon", D_0053DA08);

