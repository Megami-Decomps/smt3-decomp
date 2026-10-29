#include "common.h"
#include "newbattle.h"
#include "nb.h"
#include "effBattle.h"
#include "font.h"

/* Four native 0x34-byte rows; member names follow typeNbResultWork. */
typedef struct nbResultScore_t {
    s32 useflg;
    s32 exp_cnt;
    s32 l_lv;
    s32 lvupflg;
    s32 unitidx;
    datUnitWork_t *unit;
    s32 lvupanmflg;
    s32 lvupanmcnt;
    s32 lvuplvanmcnt;
    s32 ofsx;
    s32 stopcnt;
    s32 addexp;
    s32 resultadd;
} nbResultScore_t;

extern nbResultScore_t nbResultScoreTable[4];
extern s32 nbResultBestScore;

typedef struct nbResultCaption_t {
    s32 flag;
    s32 x;
    s32 y;
    s32 life;
} nbResultCaption_t;

extern nbResultCaption_t D_00574F50[64];
extern u32 nbResultTimer;
extern u32 nbResultCounter;
extern u8 rstCalcLevelFromExp(u8 index, u32 exp);
extern void nbResultCountEntries(s32 x, s32 y);
extern void p2sdPlaySE(s32 no, s32 volume, s32 pan);

extern void fldSkyBattle(void);

extern TMX_HEADER_t *D_00551FD0, *D_00551FD4, *D_00551FD8;
extern sdfTexHandle_t *nbResultTex1, *nbResultTex2, *nbResultTexHandle;
extern sdfTexHandle_t *sdfTexSet(TMX_HEADER_t *texture);
extern s32 nbResultTobiwarpLabel;
extern s32 nbResultState;
extern s32 D_005510E8;
extern s32 GetAddExp(s32 index, s32 cap);

#ifdef NON_MATCHING
void nbResultLoadTextures(void)
{
    volatile nbResultScore_t *score;
    datUnitWork_t *unit;
    volatile dds3GlobalWork_t *global;
    u32 index;
    u32 flags;
    s32 i;
    s32 count;
    s32 addExp;

    nbResultTex1 = sdfTexSet(D_00551FD0);
    nbResultTex2 = sdfTexSet(D_00551FD4);
    nbResultTexHandle = sdfTexSet(D_00551FD8);
    nbResultTimer = 0;
    nbResultTobiwarpLabel = 0;
    nbResultState = 0;
    for (i = 0; i < 4; i++) {
        score = &nbResultScoreTable[i];
        score->useflg = 0;
        score->exp_cnt = 0;
        score->l_lv = 0;
        score->lvupflg = 0;
        score->unitidx = 0;
        score->unit = NULL;
        score->lvupanmflg = 0;
        score->lvupanmcnt = 0;
        score->lvuplvanmcnt = 0;
        score->ofsx = 0;
        score->stopcnt = 0;
    }
    count = 0;
    i = 0;
    global = DDS3_GBWK;
    while (i < (s32)global->stockCount) {
        index = global->stockOrder[i++];
        unit = (datUnitWork_t *)&global->akuma[index].flag;
        flags = unit->flag;
        if ((flags & 1) && (flags & 2)) {
            score = &nbResultScoreTable[count];
            score->useflg = 1;
            score->unit = unit;
            score->exp_cnt = unit->exp;
            score->l_lv = unit->level;
            score->ofsx = 0xC0;
            score->lvupflg = 0;
            score->unitidx = index;
            score->lvupanmflg = 0;
            score->lvupanmcnt = 0;
            score->lvuplvanmcnt = 0;
            addExp = GetAddExp(index, D_005510E8);
            score->addexp = addExp;
            score->resultadd = addExp / 45;
            if (addExp % 45 > 0) {
                score->resultadd = addExp / 45 + 1;
            }
            if (score->resultadd <= 0) {
                score->resultadd = 1;
            }
            count++;
            if (count >= 4) {
                break;
            }
            global = DDS3_GBWK;
        }
    }
    for (i = 0; i < 64; i++) {
        D_00574F50[i].flag = 0;
        D_00574F50[i].x = 0;
        D_00574F50[i].y = 0;
        D_00574F50[i].life = 0;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultLoadTextures);
#endif

extern sdfTexHandle_t *nbResultTex1, *nbResultTex2, *nbResultTexHandle;
extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

void nbResultFreeTextures(void)
{
    sdfTexFreeDelayed(nbResultTex1);
    sdfTexFreeDelayed(nbResultTex2);
    sdfTexFreeDelayed(nbResultTexHandle);
    nbResultTex1 = NULL;
    nbResultTex2 = NULL;
    nbResultTexHandle = NULL;
}

void nbResultCountEntries(s32 x, s32 y)
{
    s32 i;

    for (i = 0; i < 64; i++) {
        if (D_00574F50[i].flag == 0) {
            D_00574F50[i].flag = 1;
            D_00574F50[i].x = x;
            D_00574F50[i].y = y;
            D_00574F50[i].life = 20;
            return;
        }
    }
}

extern void nbPnl_setOt(s32 otofs);
extern void nbPnl_rsetTest(s32 ate, s32 atst, s32 aref, s32 afail,
                           s32 date, s32 datm, s32 zte, s32 ztst);
extern void nbPnl_rsetAlpha(s32 mode);
extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v,
                        s32 uw, s32 vh, s32 rgba, sdfTexHandle_t *texture);

#ifdef NON_MATCHING
void nbResultInitLabels(void)
{
    nbResultCaption_t *caption;
    s32 i;
    s32 age;
    s32 alpha;
    s32 height;
    s32 y;

    nbPnl_setOt(0x48);
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    for (i = 0; i < 64; i++) {
        caption = &D_00574F50[i];
        if (caption->flag != 0) {
            caption->life--;
            if (caption->life <= 0) {
                caption->flag = 0;
            } else {
                age = 20 - caption->life;
                y = caption->y - age;
                alpha = 0x80;
                height = 0x16;
                if (age >= 11) {
                    alpha = 0x80 - (age - 10) * 12;
                    if (alpha < 0) {
                        alpha = 0;
                    }
                }
                if (age < 4) {
                    height = age * 5;
                    y += (0x16 - height) / 2;
                }
                nbPnlSprite(caption->x, y, 0x5C, height, 0xA0, 0x68,
                            0x5F, 0x16, ((u32)alpha << 24) | 0x808080,
                            nbResultTex1);
            }
        }
    }
    nbPnl_setOt(0x47);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultInitLabels);
#endif

extern char *text2[];
extern s32 rstCalcExpFromLevel(u8 index, s32 level);
extern const char D_005510F0[], D_005510F8[];
extern frFontHandle_t *D_00551FBC;
extern frFontHandle_t *frGetCNameFRQ(s8 target, s8 style, frFontHandle_t *chain, s32 firstOption);
extern void frSetDrawPosition(frFontHandle_t *font, u32 x, u32 y);
extern void frSetFontColor(frFontHandle_t *font, u32 color);
typedef struct frFontRequest_t frFontRequest_t;
extern frFontRequest_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                             const char *text, frFontRequest_t *previous);
extern frFontRequest_t *frwGetSmallFontFRQ(s32 x, s32 y, s32 priority, s32 color,
                                          const char *text, frFontRequest_t *previous);
extern s32 frDrawFontEx(frFontHandle_t *font, s8 mode);
extern s32 frDrawFont(frFontHandle_t *font);
extern s32 frDestroyFontRequestAfter(frFontHandle_t *font);
extern void cmpSetSkillListRows(s32 rows);
extern s32 sprintf(char *buffer, const char *format, ...);

#ifdef NON_MATCHING
void nbResultDrawPC1(s32 scoreIndex, s32 x, s32 y, s32 frame,
                     datUnitWork_t *work, s32 unitId, nbMainProcessData_t *data)
{
    volatile nbResultScore_t *entry = &nbResultScoreTable[scoreIndex];
    s32 alpha, stop, current, increment;
    s32 drawX;
    s32 level;
    s32 expAtLevel;
    s32 expNextLevel;
    s32 exp;
    u32 maxExp, counter;
    s32 width;
    u32 spriteColor;
    u32 drawColor;
    u32 fontColor;
    u32 labelColor;
    char buf[16];

    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    stop = entry->stopcnt;
    if (stop > 0) {
        entry->stopcnt = stop - 1;
    }
    if (frame > 0) {
        alpha = frame < 8 ? frame * 16 : 0x80;
        drawX = (s32)((u32)entry->ofsx * 0x46) / 100;
        entry->ofsx = drawX;
        spriteColor = 0x808080 | ((u32)alpha << 24);
        nbPnlSprite(x + drawX, y, 0x25, 0x22, 0xBF, 1, 0x25, 0x22, spriteColor, nbResultTex1);
        nbPnlSprite(x + drawX + 0x25, y, 0x11F, 0x22, 0xE3, 1, 1, 0x22, spriteColor, nbResultTex1);
        nbPnlSprite(x + drawX + 0x144, y, 0x78, 0x22, 0x6A, 0x2E, 0x78, 0x22, spriteColor, nbResultTex1);
    }
    if (frame >= 13) {
        if (nbResultTobiwarpLabel > 0 && entry->stopcnt == 0) {
            increment = entry->resultadd;
            current = (u32)entry->exp_cnt + (u32)increment;
            entry->exp_cnt = current;
            increment = entry->addexp;
            maxExp = work->exp + increment;
            if ((u32)current < maxExp) {
                nbResultCounter |= 1;
            } else {
                entry->exp_cnt = maxExp;
            }
        }
        if (entry->lvupanmflg != 0) {
            current = (u32)entry->lvupanmcnt + 1;
            entry->lvupanmcnt = current;
            if (current >= 16) {
                entry->lvupanmflg = 0;
                entry->lvupanmcnt = 0;
            }
        }
        expAtLevel = rstCalcExpFromLevel((u8)unitId, entry->l_lv);
        expNextLevel = rstCalcExpFromLevel((u8)unitId, (s32)((u32)entry->l_lv + 1));
        exp = entry->exp_cnt;
        level = rstCalcLevelFromExp((u8)unitId, exp);
        if (entry->l_lv < level) {
            counter = *(volatile u32 *)&nbResultCounter;
            entry->lvupflg = 1;
            entry->l_lv = level;
            entry->stopcnt = 0xF;
            entry->lvupanmflg = 1;
            *(volatile u32 *)&nbResultCounter = counter | 2;
            entry->lvupanmcnt = 0;
            nbResultCountEntries(x + 0xFE, y + 0x13);
        }
        alpha = frame < 0x22 ? frame * 6 - 0x48 : 0x80;
        if (alpha >= 0x81) {
            alpha = 0x80;
        }
        spriteColor = 0x808080 | ((u32)alpha << 24);
        nbPnlSprite(x + 0x17, y + 0x18, 8, 7, 0x98, 0x23, 8, 7, spriteColor, nbResultTex1);
        nbPnlSprite(x + 0x1F, y + 0x18, 0xAA, 7, 0x9F, 0x23, 1, 7, spriteColor, nbResultTex1);
        nbPnlSprite(x + 0xC9, y + 0x18, 8, 7, 0xA1, 0x23, 8, 7, spriteColor, nbResultTex1);
        drawX = y + 8;
        drawColor = spriteColor;
        if (entry->lvupflg != 0) {
            nbPnlSprite(x + 0x20, drawX, 0x34, 0x1E, 0x9F, 1, 0x34, 0x1E, 0x80808080, nbResultTex2);
            current = (u32)entry->lvuplvanmcnt + 1;
            entry->lvuplvanmcnt = current;
            if (current >= 9) {
                entry->lvuplvanmcnt = 8;
            }
            alpha = 0x80;
            drawColor = 0x80808080;
        }
        nbPnlSprite(x + 0x27, y + 0x11, 0x12, 0xC, 0xAC, 0x1E, 0x12, 0xC, drawColor, nbResultTex1);
        fontColor = 0xA09DC300 | alpha;
        current = entry->l_lv;
        sprintf(buf, D_005510F0, current < 0x64 ? current : 0x63);
        D_00551FBC = (frFontHandle_t *)frwGetSmallNumFontFRQ(
            (x + 0x3C) * 16, (y + 0x10) * 8, 0, fontColor, buf, NULL);
        frDrawFontEx(D_00551FBC, 1);
        frDestroyFontRequestAfter(D_00551FBC);
        nbPnlSprite(x + 0xCA, y, 0x9B, 0x1F, 1, 0x51, 0x9B, 0x1F, drawColor, nbResultTex1);
        nbPnlSprite(x + 0xD6, drawX, 0x27, 0x18, 0x70, 0x12, 0x27, 0x18, drawColor, nbResultTex1);
        if (entry->lvupanmflg != 0) {
            current = entry->lvupanmcnt;
            if (current >= 6) {
                labelColor = 0xA09DC300 | ((s32)(((u32)current << 7) - 0x280) / 10);
                sprintf(buf, D_005510F8, (s32)((u32)expNextLevel - (u32)exp));
                alpha = 0x80;
                fontColor = 0xA09DC380;
                D_00551FBC = (frFontHandle_t *)frwGetSmallNumFontFRQ((x + 0x11D) * 16, (y + 7) * 8, 0,
                    labelColor, buf, 0);
                frDrawFontEx(D_00551FBC, 1);
                frDestroyFontRequestAfter(D_00551FBC);
            }
        } else {
            sprintf(buf, D_005510F8, (s32)((u32)expNextLevel - (u32)exp));
            D_00551FBC = (frFontHandle_t *)frwGetSmallNumFontFRQ(
                (x + 0x11D) * 16, (y + 7) * 8, 0, fontColor, buf, NULL);
            frDrawFontEx(D_00551FBC, 1);
            frDestroyFontRequestAfter(D_00551FBC);
        }
        spriteColor = 0x808080 | ((u32)alpha << 24);
        if (entry->lvupanmflg != 0) {
            current = entry->lvupanmcnt;
            if (current < 2) {
                width = (s32)(0xE - (u32)current * 7);
                nbPnlSprite(x + 0xF9, y - width / 2 + 0x18, 0x68, width, 1, 0x2E, 0x68, 0xE,
                    spriteColor, nbResultTex1);
            } else if (current >= 0xB) {
                width = (s32)((u32)current * 0xE - 0x8C) / 5;
                nbPnlSprite(x + 0xF9, y - width / 2 + 0x18, 0x68, width, 1, 0x3D, 0x68, 0xE,
                    spriteColor, nbResultTex1);
            }
        } else {
            nbPnlSprite(x + 0xF9, y + 0x11, 0x68, 0xE, 1, 0x3D, 0x68, 0xE, spriteColor, nbResultTex1);
            width = (s32)(((u32)exp - (u32)expAtLevel) * 0x68) /
                    (s32)((u32)expNextLevel - (u32)expAtLevel);
            if (width < 0) width = 0;
            if (width >= 0x69) width = 0x68;
            if (expAtLevel < exp && width < 2) width = 2;
            if (exp < expNextLevel && width >= 0x68) width = 0x67;
            nbPnlSprite(x + 0xF9, y + 0x11, width, 0xE, 1, 0x2E, width, 0xE, spriteColor, nbResultTex1);
        }
        cmpSetSkillListRows(0x14);
        if (work->flag & 4) {
            D_00551FBC = frGetCNameFRQ(0, 1, 0, 0);
            frSetDrawPosition(D_00551FBC, (x + 0x59) * 16, (y + 0xC) * 8);
            frSetFontColor(D_00551FBC, fontColor);
            frDrawFont(D_00551FBC);
            frDestroyFontRequestAfter(D_00551FBC);
        } else {
            D_00551FBC = (frFontHandle_t *)frwGetSmallFontFRQ(
                (x + 0x59) * 16, (y + 0xC) * 8, 0, fontColor, text2[work->id], NULL);
            frDrawFont(D_00551FBC);
            frDestroyFontRequestAfter(D_00551FBC);
        }
        cmpSetSkillListRows(-1);
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultDrawPC1);
#endif

void nbResultInitTimer(nbMainProcessData_t *data)
{
    s32 i;
    s32 oldLevel;
    s32 index;

    nbResultTimer = 900;
    nbResultCounter = 0;
    for (i = 0; i < 4; i++) {
        if (nbResultScoreTable[i].useflg != 0) {
            index = (u8)nbResultScoreTable[i].unitidx;
            oldLevel = rstCalcLevelFromExp(index, nbResultScoreTable[i].exp_cnt);
            nbResultScoreTable[i].exp_cnt = nbResultScoreTable[i].unit->exp + nbResultScoreTable[i].addexp;
            nbResultScoreTable[i].l_lv = rstCalcLevelFromExp(index, nbResultScoreTable[i].exp_cnt);
            if (oldLevel < nbResultScoreTable[i].l_lv) {
                nbResultScoreTable[i].lvupflg = 1;
                nbResultCounter |= 2;
                nbResultCountEntries(317, 227 + i * 40);
            }
        }
    }
    if (nbResultCounter & 2) {
        p2sdPlaySE(17, 127, 63);
    }
    nbResultCounter = 0;
}

s32 nbResultFindBestScore(nbMainProcessData_t *data)
{
    nbResultScore_t *entry;
    s32 pending;
    s32 i;

    entry = nbResultScoreTable;
    pending = 0;
    for (i = 0; i < 4; i++, entry++) {
        if (entry->useflg != 0) {
            if (entry->exp_cnt < entry->unit->exp + entry->addexp) {
                pending = 1;
            }
        }
    }
    if (pending == 0 && nbResultBestScore < 60) {
        nbResultBestScore++;
    }
    return nbResultBestScore;
}

extern s32 nbTobiwarpLabelIdx;
extern f32 D_00551100;
extern frFontRequest_t *D_00551FC0;
extern char *pStr[];
extern void aDrawBoxG(s32 x, s32 y, s32 w, s32 h,
                      u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1,
                      u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3);
extern f32 sdfSin(f32 angle);
extern s32 nbGetKakutokuExp(nbMainProcessData_t *data);
extern s32 nbGetKakutokuMaka(nbMainProcessData_t *data);
extern void rsetOtOfs(s32 offset);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void rsetAlpha(s32 mode);

#ifdef NON_MATCHING
void nbFindTobiwarpLabel(nbMainProcessData_t *data)
{
    char numberText[16];
    u32 itemId[3];
    u32 itemCount[3];
    s32 i;
    s32 itemTotal;
    s32 haveItem = 0;
    u32 rewardColor = 0xA09DC300;
    s32 step;
    s32 alpha;
    s32 packed;
    s32 scoreY;
    s32 resultState;
    s32 spread;
    frFontRequest_t *font;

    D_00551100 = D_00551100 + 0.099999995f;
    nbResultTimer++;
    nbPnl_setOt(0x47);
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);

    if (((s32)nbResultTimer) > 0) {
        step = ((s32)nbResultTimer) < 10 ? ((s32)nbResultTimer) : 10;
        alpha = (step * 0x80) / 10;
        packed = 0x80000000 | alpha | (alpha << 8) | (alpha << 16);
        nbPnlSprite(0, 0, 0x200, 0x1C0, 0, 0, 0x100, 0x100,
                    packed, nbResultTexHandle);
        alpha = (step * 112) / 10;
        packed = 0x10000000 | alpha | (alpha << 8) | (alpha << 16);
        nbPnlSprite(-2, -2, 0x204, 0x1C4, 0, 0, 0x100, 0x100,
                    packed, nbResultTexHandle);
        alpha = (step * 144) / 10;
        packed = 0x10000000 | alpha | (alpha << 8) | (alpha << 16);
        nbPnlSprite(-4, -4, 0x208, 0x1C8, 0, 0, 0x100, 0x100,
                    packed, nbResultTexHandle);
        alpha = step * 16;
        packed = 0x10000000 | alpha | (alpha << 8) | (alpha << 16);
        nbPnlSprite(-6, -6, 0x20C, 0x1CC, 0, 0, 0x100, 0x100,
                    packed, nbResultTexHandle);
    }

    if (((s32)nbResultTimer) >= 9) {
        step = ((s32)nbResultTimer) < 23 ? 23 - ((s32)nbResultTimer) : 0;
        nbPnlSprite(0, 0x36, 0xFC, 0x1C, 2, 0x53, 0xFC, 0x1C,
                    0x80808080, nbResultTex2);
        nbPnlSprite(0xFC, 0x36, 0x104, 0x1C, 0xFD, 0x53, 1, 0x1C,
                    0x80808080, nbResultTex2);
        nbPnlSprite(0, 0x56, 0x13, 0x66, 0xE7, 1, 0x13, 0x66,
                    0x80808080, nbResultTex1);
        nbPnlSprite(0x13, 0x56, 0x1ED, 0x66, 0xF9, 1, 1, 0x66,
                    0x80808080, nbResultTex1);
        nbPnlSprite(0, 0xC0, 0xFC, 5, 2, 0x6F, 0xFC, 5,
                    0x80808080, nbResultTex2);
        nbPnlSprite(0xFC, 0xC0, 0x104, 5, 0xFD, 0x6F, 1, 5,
                    0x80808080, nbResultTex2);
        alpha = (s32)(sdfSin(D_00551100 * 1.3f) * 24.0f) + 0x70;
        packed = 0x80000000 | alpha | (alpha << 8) | (alpha << 16);
        nbPnl_rsetAlpha(1);
        nbPnlSprite(0, 0x30, 0x9C, 0x2C, 1, 0x25, 0x9C, 0x2C,
                    packed, nbResultTex2);
        alpha = step * 128 / 15;
        nbPnl_rsetAlpha(0);
        nbPnlSprite(21, 58, 99, 21, 1, 1, 99, 21,
                    0x80808080, nbResultTex1);
        if (alpha > 0) {
            nbPnl_rsetAlpha(1);
            packed = (u32)alpha << 24;
            spread = step / 4;
            nbPnlSprite(21 - spread, 58 - spread, 99 + spread * 2, 21 + spread * 2,
                        1, 1, 99, 21, packed | 0xC0C0C0, nbResultTex1);
            spread = step / 2;
            nbPnlSprite(21 - spread, 58 - spread, 99 + spread * 2, 21 + spread * 2,
                        1, 1, 99, 21, packed | 0x909090, nbResultTex1);
            spread = step * 3 / 4;
            nbPnlSprite(21 - spread, 58 - spread, 99 + spread * 2, 21 + spread * 2,
                        1, 1, 99, 21, packed | 0x606060, nbResultTex1);
            nbPnlSprite(21 - step, 58 - step, 99 + step * 2, 21 + step * 2,
                        1, 1, 99, 21, packed | 0x303030, nbResultTex1);
        }
    }
    nbPnl_rsetAlpha(0);
    itemId[0] = itemId[1] = itemId[2] = 0;
    itemCount[0] = itemCount[1] = itemCount[2] = 0;
    itemTotal = 0;
    for (i = 0; i < 3; i++) {
        u8 item = data->item[i];
        if (item != 0 && (u8)(item + 0xC0) >= 0x19) {
            haveItem = 1;
            itemId[itemTotal] = ((volatile nbMainProcessData_t *)data)->item[i];
            itemCount[itemTotal] = data->itemcnt[i];
            itemTotal++;
        }
    }

    if (((s32)nbResultTimer) >= 9) {
        step = ((s32)nbResultTimer) < 23 ? 23 - ((s32)nbResultTimer) : 0;
        alpha = 128 - step * 128 / 15;
        packed = ((u32)alpha << 24) | 0x808080;
        nbPnlSprite(0x5C, 0x64, 0x37, 0x16, 1, 0x17, 0x37, 0x16, packed, nbResultTex1);
        nbPnlSprite(0x93, 0x64, 0x36, 0x16, 0x39, 0x17, 0x36, 0x16, packed, nbResultTex1);
        nbPnlSprite(0x5C, 0x84, 0x37, 0x16, 1, 0x17, 0x37, 0x16, packed, nbResultTex1);
        nbPnlSprite(0x93, 0x84, 0x36, 0x16, 0x39, 0x17, 0x36, 0x16, packed, nbResultTex1);
        nbPnlSprite(0x30, 0x68, 0x27, 0x10, 0x97, 1, 0x27, 0x10, packed, nbResultTex1);
        nbPnlSprite(0x49, 0x88, 0x0E, 0x10, 0x9D, 0x12, 0x0E, 0x10, packed, nbResultTex1);
        if (haveItem != 0) {
            nbPnlSprite(0xDC, 0x68, 0x31, 0x10, 0x65, 1, 0x31, 0x12,
                        packed, nbResultTex1);
            for (i = 0; i < 3; i++) {
                nbPnlSprite(0x111, 0x64 + i * 0x1A, 0x37, 0x16,
                            1, 0x17, 0x37, 0x16, packed,
                            nbResultTex1);
                nbPnlSprite(0x148, 0x64 + i * 0x1A, 0x53, 0x16,
                            0x37, 0x17, 1, 0x16, packed,
                            nbResultTex1);
                nbPnlSprite(0x19B, 0x64 + i * 0x1A, 0x36, 0x16,
                            0x39, 0x17, 0x36, 0x16, packed,
                            nbResultTex1);
            }

        cmpSetSkillListRows(0x13);
        for (i = 0; i < itemTotal; i++) {
            u32 id = itemId[i];
            if (id == 0) { continue; }
            font = frwGetSmallFontFRQ(0x1210, 0x338 + i * 0xD0,
                                      0, (s32)0xA09DC380, pStr[id], NULL);
            D_00551FC0 = font;
            frDrawFont((frFontHandle_t *)font);
            frDestroyFontRequestAfter((frFontHandle_t *)D_00551FC0);
            nbPnlSprite(0x197, 0x6B + i * 0x1A, 0x0B, 0x0B,
                        0xAC, 0x12, 0x0B, 0x0B, 0x80808080,
                        nbResultTex1);
            sprintf(numberText, D_005510F0, itemCount[i]);
            font = frwGetSmallNumFontFRQ(0x1AC0, 0x348 + i * 0xD0,
                                         0, 0xA09DC380, numberText, NULL);
            D_00551FC0 = font;
            frDrawFont((frFontHandle_t *)font);
            frDestroyFontRequestAfter((frFontHandle_t *)D_00551FC0);
        }
        cmpSetSkillListRows(-1);
        }
    }

    if (((s32)nbResultTimer) >= 24) {
        s32 phase = ((s32)nbResultTimer) < 31 ? ((s32)nbResultTimer) - 23 : 8;
        s32 y = (s32)(sdfSin(D_00551100 + D_00551100) * 16.0f);
        packed = (((u32)phase << 28) | (0x70 + y) | ((0x70 + y) << 8) | ((0x70 + y) << 16));
        nbPnlSprite(0x27, 0x5E, 0x37, 0x23, 0x21, 1, 0x37, 0x23,
                    packed, nbResultTex2);
        y = (s32)(sdfSin((D_00551100 + D_00551100) + 0.099999995f) * 16.0f);
        packed = (((u32)phase << 28) | (0x70 + y) | ((0x70 + y) << 8) | ((0x70 + y) << 16));
        nbPnlSprite(0x3F, 0x80, 0x1F, 0x1F, 1, 1, 0x1F, 0x1F,
                    packed, nbResultTex2);
        if (haveItem != 0) {
            y = (s32)(sdfSin((D_00551100 + D_00551100) + 0.19999999f) * 16.0f);
            packed = (((u32)phase << 28) | (0x70 + y) | ((0x70 + y) << 8) | ((0x70 + y) << 16));
            nbPnlSprite(0xD1, 0x5E, 0x45, 0x23, 0x59, 1, 0x45, 0x23,
                        packed, nbResultTex2);
        }
    }

    if (((s32)nbResultTimer) >= 29) {
        scoreY = ((s32)nbResultTimer) < 43 ? ((s32)nbResultTimer) - 28 : 15;
        rewardColor |= (scoreY * 128) / 15;
        sprintf(numberText, D_005510F8, nbGetKakutokuExp(data));
        font = frwGetSmallNumFontFRQ(0x7A0, 0x348, 0,
                                     rewardColor,
                                     numberText, 0);
        D_00551FC0 = font;
        frDrawFontEx((frFontHandle_t *)font, 1);
        frDestroyFontRequestAfter((frFontHandle_t *)D_00551FC0);
        sprintf(numberText, D_005510F8, nbGetKakutokuMaka(data));
        font = frwGetSmallNumFontFRQ(0x7A0, 0x448, 0,
                                     rewardColor,
                                     numberText, 0);
        D_00551FC0 = font;
        frDrawFontEx((frFontHandle_t *)font, 1);
        frDestroyFontRequestAfter((frFontHandle_t *)D_00551FC0);
    }

    nbResultCounter = 0;
    if (((s32)nbResultTimer) >= 21) {
        nbResultTobiwarpLabel = ((s32)nbResultTimer) - 50;
        scoreY = 0xD0;
        for (i = 0; i < 4; i++) {
            nbResultScore_t *entry = &nbResultScoreTable[i];
            s32 use = entry->useflg;
            u32 frame = *(volatile u32 *)&nbResultTimer;
            if (use != 0) {
                nbResultDrawPC1(i, 0x3F, scoreY, (s32)(frame - 20 - (u32)i * 6),
                                entry->unit, entry->unitidx, data);
            }
            scoreY += 0x28;
        }
    }

    if ((nbResultCounter & 1) != 0) {
        p2sdPlaySE(5, 0x7F, 0x3F);
        nbTobiwarpLabelIdx++;
    }
    if ((nbResultCounter & 2) != 0) {
        p2sdPlaySE(0x11, 0x7F, 0x3F);
    }
    nbResultInitLabels();

    resultState = nbResultState;
    if (((s32)nbResultTimer) > 0) {
        step = ((s32)nbResultTimer) < 10 ? ((s32)nbResultTimer) : 10;
        scoreY = (step * 0x80) / 10;
        rsetOtOfs(0x47);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        rsetAlpha(1);
        alpha = ((scoreY * 0x50) / 100);
        aDrawBoxG(0, 0, 0x200, 0x96,
                  0, 0x2F, 0x4C, alpha,
                  0, 0x2F, 0x4C, 0,
                  0, 0x2F, 0x4C, 0,
                  0, 0x2F, 0x4C, 0);
        rsetAlpha(0);
        resultState = nbResultState;
    }

    if (resultState > 0) {
        nbPnl_setOt(0x48);
        nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        nbPnl_rsetAlpha(0);
        resultState = nbResultState;
        step = resultState < 16 ? resultState * 8 : 0x80;
        alpha = resultState < 16 ? 0x80 - resultState * 8 : 0;
        packed = ((u32)step << 24) | (alpha << 16) | (alpha << 8) | alpha;
        nbPnlSprite(0, 0, 0x200, 0x1C0, 0, 0, 0x100, 0x100,
                    packed, nbResultTexHandle);
        nbPnl_setOt(0x47);
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbFindTobiwarpLabel);
#endif

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 timer);

void ShutdownProcess(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    if (data->autoCommProcess != NULL) {
        dds3KillProcess(data->autoCommProcess, 1);
        data->autoCommProcess = NULL;
    }
    if (data->camera != NULL) {
        dds3KillProcess(data->camera, 1);
        data->camera = NULL;
    }
    if (data->EffectProcessID != NULL) {
        dds3KillProcess(data->EffectProcessID, 1);
        data->EffectProcessID = NULL;
    }
    if (data->UnitProcessID != NULL) {
        dds3KillProcess(data->UnitProcessID, 1);
        data->UnitProcessID = NULL;
    }
    if (data->helpProcess != NULL) {
        dds3KillProcess(data->helpProcess, 1);
        data->helpProcess = NULL;
    }
    if (data->panelProcess != NULL) {
        dds3KillProcess(data->panelProcess, 1);
        data->panelProcess = NULL;
    }
}



extern void pacFileHandleFree_(struct NB_FILEHANDLE_T *handle);
extern void evtFreeStage(void);

void nbResultUpdateUnits(void)
{
    nbMainProcessData_t *data;
    nbParty_t *party;
    s32 i;

    data = nbGetMainProcessData();
    for (i = 0; i < 32; i++) {
        if (data->form[i].stat & 1) {
            if (data->form[i].packh != NULL) {
                pacFileHandleFree_(data->form[i].packh);
                data->form[i].packh = NULL;
            }
        }
        data->form[i].stat = 0;
    }
    party = data->party;
    for (i = 0; i < 14; i++, party++) {
        party->flag |= 0x28;
    }
    evtFreeStage();
}


s32 GetAddExp(s32 index, s32 cap)
{
    s32 exp = rstCalcExpFromLevel((u8)index, 0x100) - 1;
    s32 current = DDS3_GBWK->akuma[index].exp;

    exp -= current;
    if (exp < 0) {
        exp = 0;
    }
    if (cap < exp) {
        exp = cap;
    }
    return exp;
}

extern s32 nbGetKakutokuMaka(nbMainProcessData_t *data);
extern s32 nbGetKakutokuExp(nbMainProcessData_t *data);
extern s32 datCheckSyojiSkill(datUnitWork_t *work, s32 skill);
extern void datAddMaka(s32 addmaka);
extern void datAddItem(s32 id, s32 num);
extern void datAddExp(datUnitWork_t *work, s32 exp);

void nbResultAddExp(void)
{
    nbMainProcessData_t *data;
    datUnitWork_t *unit;
    s32 i;
    s32 exp;
    s32 addExp;
    const datSpecialSkill_t *specialSkills;
    u32 flag;

    data = nbGetMainProcessData();
    for (i = 0; i < 4; i++) {
        if (data->item[i] != 0) {
            if (data->itemcnt[i] != 0) {
                datAddItem(data->item[i], data->itemcnt[i]);
            }
        }
    }

    datAddMaka(nbGetKakutokuMaka(data));

    /* Passive skill 0x162 follows the 0x120 normal-skill entries. */
    specialSkills = datSkillTable.special;
    for (i = 0; i < 16; i++) {
        exp = nbGetKakutokuExp(data);
        unit = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + i;
        flag = unit->flag;
        if ((flag & 1) != 0) {
            if ((flag & 2) == 0) {
                if (datCheckSyojiSkill(unit, 0x162) == 0) {
                    continue;
                }
                if (exp >= 1) {
                    exp /= specialSkills[0x162 - 0x120].n;
                }
            }
            if ((unit->badstatus & 0xFFF) != 0x800 && unit->hp != 0) {
                addExp = GetAddExp(i, exp);
                datAddExp(unit, addExp);
            }
        }
    }
}




extern u32 dds3GetProcessRunTime(dds3ProcessID_t *id);
extern void dds3FadeIn2(s32 frame);
extern struct dds3Basic_t *dds3GetActiveScene(void);
extern s32 fldLoadNextFile_BtlRet(void);
extern s32 fldLoadNextTask(void);
extern u32 fldLoadNextGetStatus(void);
extern s8 dds3PadButtonState[];
extern s32 nbResultStep;
extern s32 nbResultState;
extern void nbFindTobiwarpLabel(nbMainProcessData_t *data);
extern void nbBgmFadeOut(void);
extern void nbIncResultCounter(void);
extern char D_005435F8[];

/* Qualified reorg traces: the preceding C reward routine removes $2 from
 * the fall-through live set, preserving the final plain loading-state branch. */
#ifdef NON_MATCHING
extern s32 printf(const char *fmt, ...);
#endif

s32 nbResultProcess2(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    if (dds3GetProcessRunTime(id) == 3) {
        dds3FadeIn2(10);
    }
    if (dds3GetProcessRunTime(id) >= 16) {
        if (dds3GetActiveScene() != NULL) {
            nbResultUpdateUnits();
        }
    }
    data = nbGetMainProcessData();
    if (dds3GetProcessRunTime(id) == 20) {
        nbResultStep = fldLoadNextFile_BtlRet();
    }
    if (nbResultStep != 0) {
        fldLoadNextTask();
        if (fldLoadNextGetStatus() == 0) {
            nbResultStep = 0;
            printf(D_005435F8);
        }
    }
    if (dds3GetProcessRunTime(id) >= 21) {
        if (dds3PadButtonState[0x21] != 0 || dds3PadButtonState[0x23] != 0) {
            nbResultInitTimer(data);
        }
        if (nbResultFindBestScore(data) >= 15) {
            if (dds3PadButtonState[0x21] != 0 || dds3PadButtonState[0x23] != 0) {
                if (nbResultState == 0) {
                    nbResultState = 1;
                }
            }
        }
    }
    nbFindTobiwarpLabel(data);
    if (nbResultState > 0) {
        if (nbResultState < 100) {
            nbResultState++;
        }
        if (nbResultState >= 16 && nbResultStep == 0) {
            nbResultAddExp();
            nbBgmFadeOut();
            if (data->battletime >= 0x367) {
                nbIncResultCounter();
            }
            return -1;
        }
    }
    return 0;
}

extern void ShutdownProcess(void);
extern s32 nbGetKakutokuExp(nbMainProcessData_t *data);
extern s32 nbTobiwarpLabelIdx;
extern s32 D_005510E8;
extern s32 nbResultBestScore;

dds3ProcessFunc_t nb_result_Process(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    ShutdownProcess();
    D_005510E8 = nbGetKakutokuExp(data);
    nbResultLoadTextures();
    nbTobiwarpLabelIdx = 0;
    nbResultBestScore = 0;
    nbResultStep = 0;
    return nbResultProcess2;
}

extern void nbStageFilterAlp(s32 alpha);

void nb_result_Shutdown(void)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    data->ResultProcessID = NULL;
    nbResultFreeTextures();
    nbStageFilterAlp(0);
}

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 a, s32 b,
                                          dds3ProcessFunc_t process, dds3ProcessFunc_t shutdown,
                                          void *work);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_005435F8);

void nbInitResultProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *process = dds3StartProcess("nb_result", 0x3F9, 1, 1,
        (dds3ProcessFunc_t)nb_result_Process, (dds3ProcessFunc_t)nb_result_Shutdown, NULL);
    dds3LinkProcess(data->MainProcessID, process);
    data->ResultProcessID = process;
}

void nbLoadStageSky(void)
{
    fldSkyBattle();
}

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543618);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543628);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543648);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543668);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543678);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543688);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543698);

