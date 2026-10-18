#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

/* Screen pixel pair in the debug overlay's own 1/16 pixel grid. */
typedef struct
{
    s32 x;
    s32 y;
} EffDebugPoint_t;

extern sdfGrOt_t sdfGrOtTable[85];

extern void *sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(void *tag);
extern void sdfDrawTagLink(void *tag, void *prim);
extern void effScreenOffset(void);
extern void *dds3PrimFrameRect(s32 x, s32 y, s32 color, s32 width, s32 height, u32 mode0, u32 mode1);
extern void *dds3PrimLine(s32 x0, s32 y0, s32 color0, u32 mode0, s32 x1, s32 y1, s32 color1, u32 mode1, s32 z);

INCLUDE_ASM(const s32, "code_001AC0A8", dds3_1AC0A8);

INCLUDE_ASM(const s32, "code_001AC0A8", dds3_1AC138);

INCLUDE_ASM(const s32, "code_001AC0A8", func_001AC150);

INCLUDE_ASM(const s32, "code_001AC0A8", effPCPBezierEval);

INCLUDE_ASM(const s32, "code_001AC0A8", func_001AC3D8);

INCLUDE_ASM(const s32, "code_001AC0A8", func_001AC4C8);

INCLUDE_ASM(const s32, "code_001AC0A8", func_001AC4F0);

/* vu0 routine: project a world point and frame it */
void effDebugPointDraw(QuadF32 *pos)
{
    QuadF32 screen;
    EffDebugPoint_t point;
    void *tag;
    sdfGrOt_t *ot;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    VU0_LOAD_VF10(pos);
    effScreenOffset();
    VU0_STORE_VF10_TO(screen);
    point.x = (s32)screen.components[0] - 0x700;
    point.y = ((s32)screen.components[1] << 1) - 0xF20;
    sdfDrawTagLink(tag, dds3PrimFrameRect(point.x * 16 + 0x7000, point.y * 8 + 0x7900,
                                          0xFF0000, 0x20, 0x10, 0x60008080, 0x60008080));
    ot = &sdfGrOtTable[83];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

/* vu0 routine: project a world point and frame it in a given mode */
void effDebugPointDraw2(QuadF32 *pos, u32 mode)
{
    QuadF32 screen;
    EffDebugPoint_t point;
    void *tag;
    sdfGrOt_t *ot;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    VU0_LOAD_VF10(pos);
    effScreenOffset();
    VU0_STORE_VF10_TO(screen);
    point.x = (s32)screen.components[0] - 0x700;
    point.y = ((s32)screen.components[1] << 1) - 0xF20;
    sdfDrawTagLink(tag, dds3PrimFrameRect(point.x * 16 + 0x7000, point.y * 8 + 0x7900,
                                          0xFF0000, 0x20, 0x10, mode, mode));
    ot = &sdfGrOtTable[83];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

/* vu0 routine: project two world points and draw the line */
void effDebugLineDraw(QuadF32 *from, QuadF32 *to)
{
    QuadF32 screen[2];
    EffDebugPoint_t point[2];
    void *tag;
    sdfGrOt_t *ot;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    VU0_LOAD_VF10(from);
    effScreenOffset();
    VU0_STORE_VF10(&screen[0]);
    VU0_LOAD_VF10(to);
    effScreenOffset();
    VU0_STORE_VF10(&screen[1]);
    point[0].x = (s32)screen[0].components[0] - 0x700;
    point[0].y = ((s32)screen[0].components[1] << 1) - 0xF20;
    point[1].x = (s32)screen[1].components[0] - 0x700;
    point[1].y = ((s32)screen[1].components[1] << 1) - 0xF20;
    sdfDrawTagLink(tag, dds3PrimLine(point[0].x * 16 + 0x7000, point[0].y * 8 + 0x7900,
                                     0xFF0000, 0x60008080,
                                     point[1].x * 16 + 0x7000, point[1].y * 8 + 0x7900,
                                     0xFF0000, 0x60008080, 0));
    ot = &sdfGrOtTable[83];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

/* vu0 routine: project two world points and draw the line in a given mode */
void effDebugLineDraw2(QuadF32 *from, QuadF32 *to, u32 mode)
{
    QuadF32 screen[2];
    EffDebugPoint_t point[2];
    void *tag;
    sdfGrOt_t *ot;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    VU0_LOAD_VF10(from);
    effScreenOffset();
    VU0_STORE_VF10(&screen[0]);
    VU0_LOAD_VF10(to);
    effScreenOffset();
    VU0_STORE_VF10(&screen[1]);
    point[0].x = (s32)screen[0].components[0] - 0x700;
    point[0].y = ((s32)screen[0].components[1] << 1) - 0xF20;
    point[1].x = (s32)screen[1].components[0] - 0x700;
    point[1].y = ((s32)screen[1].components[1] << 1) - 0xF20;
    sdfDrawTagLink(tag, dds3PrimLine(point[0].x * 16 + 0x7000, point[0].y * 8 + 0x7900,
                                     0xFF0000, mode,
                                     point[1].x * 16 + 0x7000, point[1].y * 8 + 0x7900,
                                     0xFF0000, mode, 0));
    ot = &sdfGrOtTable[83];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

INCLUDE_ASM(const s32, "code_001AC0A8", effDebugStringDraw);

INCLUDE_ASM(const s32, "code_001AC0A8", effDebugWireframeSphereDraw);

INCLUDE_ASM(const s32, "code_001AC0A8", effDebugSelectCursorDraw);

INCLUDE_ASM(const s32, "code_001AC0A8", effDebugWireframeCylinderDraw);

INCLUDE_ASM(const s32, "code_001AC0A8", effDebugWindowDraw);

INCLUDE_SDATA(const s32, "code_001AC0A8", D_0054F270);

INCLUDE_SDATA(const s32, "code_001AC0A8", D_0054F274);

