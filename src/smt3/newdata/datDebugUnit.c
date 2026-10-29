#include "common.h"
#include "nb.h"
#include "effBattle.h"

const char D_005431A8[] = "STOCKCNT :%d";
const char D_005431B8[] = "STOCKLIST:";

extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void *sdfDevPrintf(s32 x, s32 y, s32 z, s32 attr, const char *format, ...);
extern const char D_00550EB0[];

typedef struct {
    u8 reserved0000[0x1914];
    u8 stock[16];
    s32 stockCount;
} datDebugStockWorkView_t;

void DispList(s32 listX, s32 listY, sdfDrawTag_t *tag)
{
    s32 x = listX;
    s32 y = listY;
    s32 count = ((datDebugStockWorkView_t *)DDS3_GBWK)->stockCount;
    s32 i;

    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7000 + x * 0xC0,
                                           0x7900 + y * 0x60,
                                           0xFEFFFF, 0, D_005431A8, count));
    y++;
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7000 + x * 0xC0,
                                           0x7900 + y * 0x60,
                                           0xFEFFFF, 0, D_005431B8));
    y++;

    for (i = 0; i < 16; i++) {
        u8 item = ((datDebugStockWorkView_t *)DDS3_GBWK)->stock[i];
        s32 attr = i < count ? 5 : 0xE;

        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7000 + x * 0xC0,
                                               0x7900 + y * 0x60,
                                               0xFEFFFF, attr, D_00550EB0, item));
        item = ((datDebugStockWorkView_t *)DDS3_GBWK)->stock[i];
        if (item >= 10) {
            x += 3;
        } else {
            x += 2;
        }
    }
}

INCLUDE_ASM(const s32, "newdata/datDebugUnit", DispUnit1);

extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void *sdfDevPrintf(s32 x, s32 y, s32 z, s32 attr, const char *format, ...);
extern void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);
extern void DispList(s32 x, s32 y, sdfDrawTag_t *tag);
extern void DispUnit1(s32 unit, s32 mode, s32 entry, sdfDrawTag_t *tag);
extern sdfGrOt_t D_002F6938;
const char D_005431C8[] = "     ID     LV  HP       MP     STAT";

s32 datDebugUnitProcess(dds3ProcessID_t *id)
{
    sdfDrawTag_t *unitList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTag_t *panelList;
    s32 i;
    s32 entry;

    sdfDrawTagInit(unitList);
    DispList(2, 10, unitList);
    sdfDrawTagLink(unitList, (u32)sdfDevPrintf(0x7180, 0x7E40, 0xFEFFFF, 4, D_005431C8));

    for (i = 0, entry = 15; i < 16; i++, entry++) {
        DispUnit1(i, 2, entry, unitList);
    }

    entry++;
    panelList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(panelList);
    dds3ConsPanelTagLink(panelList, 0x18, 0x72, 0x25, entry - 9);
    D_002F6938.regFunc(&D_002F6938, panelList);
    D_002F6938.regFunc(&D_002F6938, unitList);
    return 0;
}

extern const char *const D_005431F0[];
extern const char D_00543200[];
extern const char D_00550F70[];
extern s32 D_00550F54;
extern s8 dds3PadButtonState[];
extern s32 dds3SelectMenu(s32 pad, s32 listsx, s32 listsy, s32 winsx, s32 winsy,
                           s32 *basex, s32 *basey, s32 *x, s32 *y);
extern void datAddKotowariPoint(s32 type, s32 point);

s32 datDebugDrawSelectMenu(void)
{
    const char *labels[3];
    sdfDrawTag_t *listTag;
    sdfDrawTag_t *panelTag;
    s32 result;
    s32 i;
    s32 x;
    s32 y;

    memcpy(labels, D_005431F0, sizeof(labels));
    listTag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(listTag);
    sdfDrawTagLink(listTag, (u32)sdfDevPrintf(0x82C0, 0x7AE0, 0xFEFFFF, 0xE, D_00543200));

    for (i = 1, x = 27, y = 0x7B40; i < 4; i++, y += 0x60) {
        const char *name = labels[i - 1];
        s32 attr = 0;
        if (D_00550F54 == i - 1) {
            attr = 4;
        }
        sdfDrawTagLink(listTag,
                       (u32)sdfDevPrintf(0x7000 + x * 0xC0, y, 0xFEFFFF, attr, D_00550F70,
                                         name, DDS3_GBWK->kotowari[i]));
    }

    panelTag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(panelTag);
    dds3ConsPanelTagLink(panelTag, 0x12C, 0x36, 0xF, 5);
    D_002F6938.regFunc(&D_002F6938, panelTag);
    D_002F6938.regFunc(&D_002F6938, listTag);

    result = dds3SelectMenu(1, 1, 3, 1, 3, NULL, NULL, NULL, &D_00550F54);
    if (result == -1) {
        return -1;
    }
    if (dds3PadButtonState[0x34] & 2) {
        datAddKotowariPoint(D_00550F54 + 1, -1);
    }
    if (dds3PadButtonState[0x35] & 2) {
        datAddKotowariPoint(D_00550F54 + 1, 1);
    }
    if (dds3PadButtonState[0x38] & 2) {
        datAddKotowariPoint(D_00550F54 + 1, -10);
    }
    if (dds3PadButtonState[0x3A] & 2) {
        datAddKotowariPoint(D_00550F54 + 1, 10);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_005431F0);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_00543200);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_00543230);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_00543240);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_00543250);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EB0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EB8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EC0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EC8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550ED0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550ED8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EE0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EE8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EF0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EF8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F00);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F08);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F10);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F18);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F20);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F28);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F30);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F38);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F40);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F48);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F50);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F54);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F58);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F60);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F68);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F70);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F78);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F7C);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F80);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F88);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F90);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F98);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FA0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FA8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FB0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FB8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FC0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FC8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FD0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FD8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FE0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FE8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FF0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FF8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551000);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551008);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551010);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551018);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551020);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551028);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551030);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551038);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551040);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551048);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551050);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551058);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551060);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551068);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551070);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551078);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551080);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551088);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551090);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551098);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510A0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510A8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510B0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510B8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510C0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510C8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultTimer);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultTobiwarpLabel);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultState);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultBestScore);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultStep);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultCounter);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510E8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510F0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510F8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbTobiwarpLabelIdx);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551100);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultTex1);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultTex2);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultTexHandle);

