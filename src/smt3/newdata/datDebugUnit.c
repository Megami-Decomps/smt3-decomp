#include "common.h"
#include "nb.h"
#include "effBattle.h"

/* Declared here because no header carries these. */
extern void *memcpy(void *a, const void *b, size_t c);

const char datDebugStockCountFmt[] = "STOCKCNT :%d";
const char datDebugStockListLabel[] = "STOCKLIST:";

extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, addr_t packet);
extern void *sdfDevPrintf(s32 x, s32 y, s32 z, s32 attr, const char *format, ...);
extern const char datDebugStockItemFmt[];

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

    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + x * 0xC0,
                                           0x7900 + y * 0x60,
                                           0xFEFFFF, 0, datDebugStockCountFmt, count)));
    y++;
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + x * 0xC0,
                                           0x7900 + y * 0x60,
                                           0xFEFFFF, 0, datDebugStockListLabel)));
    y++;

    for (i = 0; i < 16; i++) {
        u8 item = ((datDebugStockWorkView_t *)DDS3_GBWK)->stock[i];
        s32 attr = i < count ? 5 : 0xE;

        sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + x * 0xC0,
                                               0x7900 + y * 0x60,
                                               0xFEFFFF, attr, datDebugStockItemFmt, item)));
        item = ((datDebugStockWorkView_t *)DDS3_GBWK)->stock[i];
        if (item >= 10) {
            x += 3;
        } else {
            x += 2;
        }
    }
}

extern const char datDebugUnitIndexFmt[];
extern const char datDebugEmptyLabel[];
extern const char datDebugIdFmt[];
extern const char datDebugPartyMark[];
extern const char datDebugLevelFmt[];
extern const char datDebugHpMpFmt[];
extern const char datDebugBadStatusFmt[];
extern const char *nbUnitBadStatusNames[13];

void DispUnit1(s32 unit, s32 x, s32 y, sdfDrawTag_t *tag)
{
    datUnitWork_t *work = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + unit;
    s32 column = x;
    s32 baseY = 0x7900 + y * 0x60;
    s32 badstatus;
    s32 badStatusIndex;

    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + column * 0xC0, baseY,
                                           0xFEFFFF, 0, datDebugUnitIndexFmt, unit)));
    column += 5;
    if ((work->flag & 1) == 0) {
        sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + column * 0xC0, baseY,
                                               0xFEFFFF, 0xE, datDebugEmptyLabel)));
        return;
    }

    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + column * 0xC0, baseY,
                                           0xFEFFFF, 0, datDebugIdFmt, work->id)));
    column += 4;
    if (work->flag & 2) {
        sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + column * 0xC0, baseY,
                                               0xFEFFFF, 5, datDebugPartyMark)));
    }
    column += 2;
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + column * 0xC0, baseY,
                                           0xFEFFFF, 0, datDebugLevelFmt, work->level)));
    column += 4;
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + column * 0xC0, baseY,
                                           0xFEFFFF, 0, datDebugHpMpFmt,
                                           work->hp, work->maxhp)));
    column += 8;
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + column * 0xC0, baseY,
                                           0xFEFFFF, 0, datDebugHpMpFmt,
                                           work->mp, work->maxmp)));
    column += 8;

    badstatus = work->badstatus;
    for (badStatusIndex = 0; badStatusIndex < 12; badStatusIndex++) {
        if ((badstatus >> badStatusIndex) & 1) {
            break;
        }
    }
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(0x7000 + column * 0xC0,
                                           0x7900 + y * 0x60,
                                           0xFEFFFF, 0, datDebugBadStatusFmt,
                                           nbUnitBadStatusNames[badStatusIndex])));
}

extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, addr_t packet);
extern void *sdfDevPrintf(s32 x, s32 y, s32 z, s32 attr, const char *format, ...);
extern void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);
extern void DispList(s32 x, s32 y, sdfDrawTag_t *tag);
extern void DispUnit1(s32 unit, s32 x, s32 y, sdfDrawTag_t *tag);
extern sdfGrOt_t sdfGrOtTable[];
SYMBOL_AT(sdfGrOt_t, D_002F6938, sdfGrOtTable, 0xA20);
const char datDebugUnitHeader[] = "     ID     LV  HP       MP     STAT";

s32 datDebugUnitProcess(dds3ProcessID_t *id)
{
    sdfDrawTag_t *unitList = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTag_t *panelList;
    s32 i;
    s32 entry;

    sdfDrawTagInit(unitList);
    DispList(2, 10, unitList);
    sdfDrawTagLink(unitList, EE_ADDR(sdfDevPrintf(0x7180, 0x7E40, 0xFEFFFF, 4, datDebugUnitHeader)));

    for (i = 0, entry = 15; i < 16; i++, entry++) {
        DispUnit1(i, 2, entry, unitList);
    }

    entry++;
    panelList = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(panelList);
    dds3ConsPanelTagLink(panelList, 0x18, 0x72, 0x25, entry - 9);
    D_002F6938.regFunc(&D_002F6938, panelList);
    D_002F6938.regFunc(&D_002F6938, unitList);
    return 0;
}

extern const char *const datDebugKotowariLabels[];
extern const char datDebugKotowariTitle[];
extern const char datDebugKotowariFmt[];
extern s32 datDebugMenuCursor;
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

    memcpy(labels, datDebugKotowariLabels, sizeof(labels));
    listTag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(listTag);
    sdfDrawTagLink(listTag, EE_ADDR(sdfDevPrintf(0x82C0, 0x7AE0, 0xFEFFFF, 0xE, datDebugKotowariTitle)));

    for (i = 1, x = 27, y = 0x7B40; i < 4; i++, y += 0x60) {
        const char *name = labels[i - 1];
        s32 attr = 0;
        if (datDebugMenuCursor == i - 1) {
            attr = 4;
        }
        sdfDrawTagLink(listTag,
                       EE_ADDR(sdfDevPrintf(0x7000 + x * 0xC0, y, 0xFEFFFF, attr, datDebugKotowariFmt,
                                         name, DDS3_GBWK->kotowari[i])));
    }

    panelTag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(panelTag);
    dds3ConsPanelTagLink(panelTag, 0x12C, 0x36, 0xF, 5);
    D_002F6938.regFunc(&D_002F6938, panelTag);
    D_002F6938.regFunc(&D_002F6938, listTag);

    result = dds3SelectMenu(1, 1, 3, 1, 3, NULL, NULL, NULL, &datDebugMenuCursor);
    if (result == -1) {
        return -1;
    }
    if (dds3PadButtonState[0x34] & 2) {
        datAddKotowariPoint(datDebugMenuCursor + 1, -1);
    }
    if (dds3PadButtonState[0x35] & 2) {
        datAddKotowariPoint(datDebugMenuCursor + 1, 1);
    }
    if (dds3PadButtonState[0x38] & 2) {
        datAddKotowariPoint(datDebugMenuCursor + 1, -10);
    }
    if (dds3PadButtonState[0x3A] & 2) {
        datAddKotowariPoint(datDebugMenuCursor + 1, 10);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", datDebugKotowariLabels);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", datDebugKotowariTitle);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_00543230);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_00543240);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_00543250);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugStockItemFmt);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNameNone);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNameDead);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNameStone);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNameFly);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNamePara);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNameCharm);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNamePoison);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNameClose);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNameBind);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNamePanic);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNameSleep);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNameFreeze);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbUnitBadStatusNameShock);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugUnitIndexFmt);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugEmptyLabel);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugIdFmt);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugPartyMark);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugLevelFmt);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugHpMpFmt);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugBadStatusFmt);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugMenuCursor);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugKotowariYosuga);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugKotowariMusubi);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugKotowariSizima);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", datDebugKotowariFmt);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F78);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbDebugGameCameraName);

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

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbMainProcessName);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbMaeProcessName);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbAtoProcessName);

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

