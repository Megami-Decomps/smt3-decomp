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
extern char datDebugStockItemFmt[];

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

extern char datDebugUnitIndexFmt[];
extern char datDebugEmptyLabel[];
extern char datDebugIdFmt[];
extern char datDebugPartyMark[];
extern char datDebugLevelFmt[];
extern char datDebugHpMpFmt[];
extern char datDebugBadStatusFmt[];
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
extern char datDebugKotowariFmt[];
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

extern char datDebugKotowariYosuga[], datDebugKotowariMusubi[], datDebugKotowariSizima[];

const char *const datDebugKotowariLabels[3] = { datDebugKotowariYosuga, datDebugKotowariMusubi, datDebugKotowariSizima };

const char datDebugKotowariTitle[16] = "KOTOWARI EDIT";

const char datDebugFreeCameraText[16] = "FREE CAMERA";

const char datDebugGameCameraText[16] = "GAME CAMERA";

const char datDebugDeadScriptName[16] = "DEADSCRIPT";

const char datDebugInvisibleName[16] = "INVISIBLE";

const char datDebugNegoResultName[16] = "NEGORESULT";

char datDebugStockItemFmt[8] = "%d";

char nbUnitBadStatusNameNone[8] = "------";

char nbUnitBadStatusNameDead[8] = "DEAD";

char nbUnitBadStatusNameStone[8] = "STONE";

char nbUnitBadStatusNameFly[8] = "FLY";

char nbUnitBadStatusNamePara[8] = "PARA";

char nbUnitBadStatusNameCharm[8] = "CHARM";

char nbUnitBadStatusNamePoison[8] = "POISON";

char nbUnitBadStatusNameClose[8] = "CLOSE";

char nbUnitBadStatusNameBind[8] = "BIND";

char nbUnitBadStatusNamePanic[8] = "PANIC";

char nbUnitBadStatusNameSleep[8] = "SLEEP";

char nbUnitBadStatusNameFreeze[8] = "FREEZE";

char nbUnitBadStatusNameShock[8] = "SHOCK";

char datDebugUnitIndexFmt[8] = "[%2d] ";

char datDebugEmptyLabel[8] = "EMPTY";

char datDebugIdFmt[8] = "%03X ";

char datDebugPartyMark[8] = "P";

char datDebugLevelFmt[8] = "%3d";

char datDebugHpMpFmt[8] = "%3d/%3d";

char datDebugBadStatusFmt[4] = "%s";

s32 datDebugMenuCursor = 0;

char datDebugKotowariYosuga[8] = "YOSUGA";

char datDebugKotowariMusubi[8] = "MUSUBI";

char datDebugKotowariSizima[8] = "SIZIMA";

char datDebugKotowariFmt[8] = "%s  %3d";

char *nbDebugFreeCameraName = datDebugFreeCameraText;

char *nbDebugGameCameraName = datDebugGameCameraText;

s32 D_00550F80 = 1;

char D_00550F88[8] = "%s";

char nbDebugPhaseNameEnd[8] = "END";

char nbDebugPhaseNameAto[8] = "ATO";

char nbDebugPhaseNameCheck3[8] = "CHECK3";

char nbDebugPhaseNameCheck1[8] = "CHECK1";

char nbDebugPhaseNameAutotar[8] = "AUTOTAR";

char nbDebugPhaseNameAuto[8] = "AUTO";

char nbDebugPhaseNameReturn[8] = "RETURN";

char nbDebugPhaseNameEscape[8] = "ESCAPE";

char nbDebugPhaseNameWait[8] = "WAIT";

char nbDebugPhaseNameBadwait[8] = "BADWAIT";

char nbDebugPhaseNameNego[8] = "NEGO";

char nbDebugPhaseNameSkill[8] = "SKILL";

char nbDebugPhaseNameReady[8] = "READY";

char nbDebugPhaseNameStksel[8] = "STKSEL";

char nbDebugPhaseNameTarsel[8] = "TARSEL";

char nbDebugPhaseNameBad[8] = "BAD";

char nbDebugPhaseNameAiscr[8] = "AISCR";

char nbDebugPhaseNameAi[8] = "AI";

char nbDebugPhaseNameComm[8] = "COMM";

char nbDebugPhaseNameNon[8] = "NON";

char nbDebugActionNameDeads[8] = "DEADS";

char nbDebugActionNameCnter[8] = "CNTER";

char nbDebugActionNameKoke[8] = "KOKE";

char nbDebugActionNameBinjo[8] = "BINJO";

char nbDebugActionNameInoti[8] = "INOTI";

char nbDebugActionNameNorm[8] = "NORM";

char nbDebugTurnFmt[8] = "Turn:%d";

char nbDebugEnemyName[8] = "ENEMY";

char nbDebugPlayerName[8] = "PLAYER";

char D_00551078[8] = ".";

char nbDebugExpFmt[8] = "EXP :%d";

char nbDebugMakaFmt[8] = "MAKA:%d";

char nbDebugSummFmt[8] = "SUMM:%d";

char D_00551098[8] = "P>";

char nbDebugCharFmt[8] = "%c";

char D_005510A8[8] = "F>";

char D_005510B0[8] = "O>";

char nbMainProcessName[8] = "nb_main";

char nbMaeProcessName[8] = "nb_mae";

char nbAtoProcessName[8] = "nb_ato";

u32 nbResultTimer = 0;

s32 nbResultTobiwarpLabel = 0;

s32 nbResultState = 0;

s32 nbResultBestScore = 0;

s32 nbResultStep = 0;

u32 nbResultCounter = 0;

s32 D_005510E8 = 0;

char nbResultFmt2d[8] = "%2d";

char nbResultFmt6d[4] = "%6d";

s32 nbTobiwarpLabelIdx = 0;

f32 D_00551100 = 0.0f;

sdfTexHandle_t *nbResultTex1 = NULL;

sdfTexHandle_t *nbResultTex2 = NULL;

sdfTexHandle_t *nbResultTexHandle = NULL;

