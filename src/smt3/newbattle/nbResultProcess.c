#include "common.h"
#include "newbattle.h"
#include "nb.h"
#include "nbResultWork.h"
#include "effBattle.h"
#include "font.h"
#include "sdfFrame.h"

extern u32 dds3GetKernelCount(void);

extern s32 nbResultBestScore;

extern u32 nbResultTimer;
extern u32 nbResultCounter;
extern u8 rstCalcLevelFromExp(u8 index, u32 exp);
extern void nbResultCountEntries(s32 x, s32 y);
extern void p2sdPlaySE(s32 no, s32 volume, s32 pan);

extern void fldSkyBattle(void);

extern TMX_HEADER_t *nbResultTmxData1, *nbResultTmxData2, *nbResultTmxData3;
extern sdfTexHandle_t *nbResultTex1, *nbResultTex2, *nbResultTexHandle;
extern sdfTexHandle_t *sdfTexSet(TMX_HEADER_t *texture);
extern s32 nbResultTobiwarpLabel;
extern s32 nbResultState;
extern s32 D_005510E8;
extern s32 GetAddExp(s32 index, s32 cap);
extern void nbResultLoadTextures(void);

/* The clear loop walks from the row start; the fill loop keeps a cursor
 * at resultadd, the last word of the same 52-byte score row. */
SYMBOL_AT_REGION(nbResultScore_t, resultScoreFillBase, nbResultScoreTable, 0);
#define SCORE_AT(type, field) \
    (*EE_ADDR_AS(type *, out + (addr_t)((size_t)&((nbResultScore_t *)0)->field - \
                                      (size_t)&((nbResultScore_t *)0)->resultadd)))

void nbResultLoadTextures(void)
{
    FAKEMATCH_ASM_REG(s32, count, "$18");
    s32 i;
    s32 div;
    addr_t out;
    dds3GlobalWork_t *global;
    FAKEMATCH_ASM_REG(s32, one, "$20");
    nbResultCaption_t *caption;
    nbResultScore_t *clear;

    nbResultTex1 = sdfTexSet(nbResultTmxData1);
    nbResultTex2 = sdfTexSet(nbResultTmxData2);
    {
        sdfTexHandle_t *handle = sdfTexSet(nbResultTmxData3);
        nbResultTimer = 0;
        nbResultTexHandle = handle;
    }
    nbResultTobiwarpLabel = 0;
    clear = nbResultScoreTable;

    nbResultState = 0;

    count = 3;
    do {
        count--;
        clear->useflg = 0;
        clear->exp_cnt = 0;
        clear->l_lv = 0;
        clear->lvupflg = 0;
        clear->unitidx = 0;
        clear->unit = NULL;
        clear->lvupanmflg = 0;
        clear->lvupanmcnt = 0;
        clear->lvuplvanmcnt = 0;
        clear->ofsx = 0;
        clear->stopcnt = 0;
        clear++;
    } while (count >= 0);
    global = DDS3_GBWK;
    count = 0;
    {
        /* Refresh the stock snapshot only after GetAddExp and only if there
         * is space for another score. Rejected entries still reread stockCount. */
        FAKEMATCH_ASM_REG(dds3GlobalWork_t *, view, "$5") = global;
        i = 0;
        if ((s32)view->stockCount > 0) {
            {
                nbResultScore_t *rows = resultScoreFillBase;
                div = 45;
                out = EE_ADDR(&rows->resultadd);
            }
            do {
                FAKEMATCH_ASM_REG(unsigned char *, adjusted, "$4") = (unsigned char *)view + 4;
                FAKEMATCH_ASM_REG(unsigned char *, stock, "$3") = EE_ADDR_AS(unsigned char *, (addr_t)i + EE_ADDR(adjusted));
                FAKEMATCH_ASM_REG(u32, index, "$10");
                FAKEMATCH_ASM_REG(u32, stride, "$6");
                unsigned char *earlyRow;
                datUnitWork_t *unit;
                u32 flags;

                index = stock[((size_t)&((dds3GlobalWork_t *)0)->stockOrder[0]) - 4];
                FAKEMATCH_ASM_LIVE(index);
                {
                    /* 236 bytes per real stock unit: ((16*n - n)*4 - n)*4.
                     * The intermediates share v0 before the final stride. */
                    u32 scaled = ((u32)index << 4) - index;
                    FAKEMATCH_ASM_LIVE(scaled);
                    scaled = (scaled << 2) - index;
                    stride = scaled << 2;
                }
                earlyRow = EE_ADDR_AS(unsigned char *, (addr_t)stride + EE_ADDR(adjusted));
                flags = *(u32 *)(earlyRow + ((size_t)&((dds3GlobalWork_t *)0)->unitwork[0].flag) - 4);
                FAKEMATCH_ASM_LIVE_WITH_INPUT(i, flags);
                i++;
                if (!(flags & 1)) {
                    goto next;
                }
                {
                    s32 skew = 0xc0;
                    s32 addExp;
                    s32 quotient;
                    FAKEMATCH_ASM_REG(u32, passIndex, "$4");
                    FAKEMATCH_ASM_REG(s32, targetCap, "$5");
                    if (!(flags & 2)) {
                        goto next;
                    }
                    FAKEMATCH_ASM_LIVE(skew);
                    unit = (datUnitWork_t *)((unsigned char *)view + stride +
                                           (size_t)&((dds3GlobalWork_t *)0)->unitwork[0]);

                    one = 1;
                    {
                        s32 exp;
                        u16 level;
                        u32 fields;
                        FAKEMATCH_ASM_REG(unsigned char *, levelBase, "$9");
                        FAKEMATCH_ASM_REG(unsigned char *, expBase, "$3");
                        fields = stride + ((size_t)&((dds3GlobalWork_t *)0)->unitwork[0].level) - 8;
                        /* Co-schedule the real field displacement and the first
                         * score value without extending either past its use. */
                        FAKEMATCH_ASM_LIVE_PAIR(fields, one);
                        levelBase = (unsigned char *)view + fields;
                        /* Level uses the old view in a1 before that register
                         * takes the cap; the EXP address uses the adjusted view. */
                        targetCap = D_005510E8;
                        FAKEMATCH_ASM_LIVE_WITH_INPUT(adjusted, targetCap);

                        SCORE_AT(s32, useflg) = one;
                        expBase = adjusted + fields;
                        /* Hold the actual level address through the independent EXP
                         * address and unit calculations before the ordered reads. */
                        FAKEMATCH_ASM_LIVE_WITH_INPUT(levelBase, expBase);
                        FAKEMATCH_ASM_LIVE_WITH_INPUT(levelBase, unit);
                        FAKEMATCH_ASM_LIVE_WITH_MEM(levelBase, SCORE_AT(s32, useflg));
                        level = *(u16 *)(levelBase + 8);
                        FAKEMATCH_ASM_LIVE_WITH_INPUT(expBase, level);
                        exp = *(s32 *)expBase;
                        passIndex = index;
                        FAKEMATCH_ASM_LIVE_WITH_INPUT_PAIR(unit, exp, passIndex);
                        SCORE_AT(datUnitWork_t *, unit) = unit;
                        FAKEMATCH_ASM_LIVE_WITH_MEM(exp, SCORE_AT(datUnitWork_t *, unit));
                        SCORE_AT(s32, exp_cnt) = exp;
                        FAKEMATCH_ASM_LIVE_WITH_MEM(level, SCORE_AT(s32, exp_cnt));
                        SCORE_AT(s32, l_lv) = level;
                    }
                    FAKEMATCH_ASM_LIVE_WITH_MEM(skew, SCORE_AT(s32, l_lv));
                    SCORE_AT(s32, ofsx) = skew;
                    SCORE_AT(s32, lvupflg) = 0;
                    FAKEMATCH_ASM_LIVE_AFTER_MEM_PAIR(index, SCORE_AT(s32, lvupflg), SCORE_AT(s32, ofsx));
                    SCORE_AT(s32, unitidx) = index;
                    SCORE_AT(s32, lvupanmflg) = 0;
                    SCORE_AT(s32, lvupanmcnt) = 0;
                    SCORE_AT(s32, lvuplvanmcnt) = 0;
                    FAKEMATCH_ASM_LIVE_PAIR(div, index);
                    FAKEMATCH_ASM_LIVE(targetCap);
                    addExp = GetAddExp(passIndex, targetCap);
                    *EE_ADDR_AS(s32 *, out - sizeof(s32)) = addExp;
                    quotient = addExp / div;
                    *(s32 *)EE_ADDR_PTR(out) = quotient;
                    if (addExp % div > 0) {
                        *(s32 *)EE_ADDR_PTR(out) = quotient + 1;
                    }
                    if (*(s32 *)EE_ADDR_PTR(out) <= 0) {
                        *(s32 *)EE_ADDR_PTR(out) = one;
                    }
                    count++;
                    out += sizeof(nbResultScore_t);
                    if (count >= 4) {
                        break;
                    }
                    global = DDS3_GBWK;
                }
next:
                view = global;
            } while (i < (s32)view->stockCount);
        }
    }
    caption = nbResultCaptions;
    {
        FAKEMATCH_ASM_REG(s32, cap, "$10") = 63;
        do {
            cap--;
            caption->flag = 0;
            caption->x = 0;
            caption->y = 0;
            caption->life = 0;
            caption++;
        } while (cap >= 0);
    }
}
#undef SCORE_AT

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
        if (nbResultCaptions[i].flag == 0) {
            nbResultCaptions[i].flag = 1;
            nbResultCaptions[i].x = x;
            nbResultCaptions[i].y = y;
            nbResultCaptions[i].life = 20;
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

void nbResultInitLabels(void)
{
    s32 i;
    s32 age;
    s32 alpha;
    s32 height;
    s32 y;
    s32 x;
    u32 fade;

    nbPnl_setOt(0x48);
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    for (i = 0; i < 64; i++) {
        if (nbResultCaptions[i].flag != 0) {
            nbResultCaptions[i].life -= SDF_FRAME_DUE();
            if (nbResultCaptions[i].life <= 0) {
                nbResultCaptions[i].flag = 0;
            } else {
                x = nbResultCaptions[i].x;
                y = nbResultCaptions[i].y;
                age = 20 - nbResultCaptions[i].life;
                y -= age;
                alpha = 0x80;
                height = 0x16;
                fade = ((u32)age - 10u) * 12u;
                if (age >= 11) {
                    alpha -= (s32)fade;
                    if (alpha < 0) {
                        alpha = 0;
                    }
                }
                if (age < 4) {
                    height = age * 5;
                    y += (0x16 - height) / 2;
                }
                nbPnlSprite(x, y, 0x5C, height, 0xA0, 0x68,
                            0x5F, 0x16, ((u32)alpha << 24) | 0x808080,
                            nbResultTex1);
            }
        }
    }
    nbPnl_setOt(0x47);
}

extern char *text2[];
extern s32 rstCalcExpFromLevel(u8 index, s32 level);
extern const char nbResultFmt2d[], nbResultFmt6d[];
extern frFontHandle_t *nbResultFontHandle;
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
#include "sdk_libc.h"

#ifdef NON_MATCHING
/* 109 of 649 words match at 649 against the native 651 words; first difference
   at +0x8. Block structure, frame size and every block's addressing match;
   retail spills the scoreIndex parameter to 0x20 and keeps a table-address
   pseudo in a callee-saved register across the first three blocks, where this
   draft gives scoreIndex that register and rematerialises the address. The
   allocation priorities decide it: cc1probe --alloc (verify: OK) puts
   scoreIndex at refs=14 live=402 (priority 0.1045) just ahead of the entry
   block's row-address pseudo at refs=4 live=77 (0.1039), so it takes the last
   callee-saved register and everything after it renumbers. The two-word
   shortfall is the two address rematerialisations that pseudo would save.
   Same family as the gcse availability accident in docs/levers/when-to-stop.md. */
void nbResultDrawPC1(s32 scoreIndex, s32 x, s32 y, s32 frame,
                     datUnitWork_t *work, s32 unitId, nbMainProcessData_t *data)
{
    char buf[16];
    s32 alpha;
    s32 drawX;
    s32 level;

    s32 count;
    s32 expAtLevel;
    s32 expNextLevel;
    s32 exp;
    u32 expCnt;
    u32 maxExp;
    s32 width;
    u32 spriteColor;
    u32 drawColor;
    u32 fontColor;
    u32 baseColor = 0x808080;
    u32 rewardColor = 0xA09DC300;

    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    if (nbResultScoreTable[scoreIndex].stopcnt > 0) {
        nbResultScoreTable[scoreIndex].stopcnt -= SDF_FRAME_DUE();
    }
    if (frame > 0) {
        if (frame < 8) {
            alpha = frame * 16;
        } else {
            alpha = 0x80;
        }
        drawX = nbResultScoreTable[scoreIndex].ofsx * 0x46 / 100;
        SDF_FRAME_WHEN(nbResultScoreTable[scoreIndex].ofsx = drawX);
        drawX = SDF_FRAME_APPROACH_DRAW_DUE(&nbResultScoreTable[scoreIndex].ofsx, drawX, 0.0f);
        spriteColor = ((u32)alpha << 24) | baseColor;
        nbPnlSprite(x + drawX, y, 0x25, 0x22, 0xBF, 1, 0x25, 0x22, spriteColor, nbResultTex1);
        nbPnlSprite(x + drawX + 0x25, y, 0x11F, 0x22, 0xE3, 1, 1, 0x22, spriteColor, nbResultTex1);
        nbPnlSprite(x + drawX + 0x144, y, 0x78, 0x22, 0x6A, 0x2E, 0x78, 0x22, spriteColor, nbResultTex1);
    }
    if (frame >= 13) {
        if (nbResultTobiwarpLabel > 0 && nbResultScoreTable[scoreIndex].stopcnt == 0 && SDF_FRAME_DUE()) {
            expCnt = nbResultScoreTable[scoreIndex].exp_cnt +
                     nbResultScoreTable[scoreIndex].resultadd;
            nbResultScoreTable[scoreIndex].exp_cnt = expCnt;
            maxExp = work->exp + nbResultScoreTable[scoreIndex].addexp;
            if (expCnt >= maxExp) {
                nbResultScoreTable[scoreIndex].exp_cnt = maxExp;
            } else {
                nbResultCounter |= 1;
            }
        }
        if (nbResultScoreTable[scoreIndex].lvupanmflg != 0) {
            count = nbResultScoreTable[scoreIndex].lvupanmcnt + SDF_FRAME_DUE();
            nbResultScoreTable[scoreIndex].lvupanmcnt = count;
            if (count >= 16) {
                nbResultScoreTable[scoreIndex].lvupanmflg = 0;
                nbResultScoreTable[scoreIndex].lvupanmcnt = 0;
            }
        }
        expAtLevel = rstCalcExpFromLevel(unitId, nbResultScoreTable[scoreIndex].l_lv);
        expNextLevel = rstCalcExpFromLevel(unitId, nbResultScoreTable[scoreIndex].l_lv + 1);
        exp = nbResultScoreTable[scoreIndex].exp_cnt;
        level = rstCalcLevelFromExp(unitId, exp);
        if (nbResultScoreTable[scoreIndex].l_lv < level) {
            nbResultScoreTable[scoreIndex].lvupflg = 1;
            nbResultScoreTable[scoreIndex].l_lv = level;
            nbResultScoreTable[scoreIndex].stopcnt = 0xF;
            nbResultScoreTable[scoreIndex].lvupanmflg = 1;
            nbResultCounter |= 2;
            nbResultScoreTable[scoreIndex].lvupanmcnt = 0;
            nbResultCountEntries(x + 0xFE, y + 0x13);
        }
        if (frame < 0x22) {
            alpha = frame * 6 - 0x48;
            if (alpha > 0x80) {
                alpha = 0x80;
            }
        } else {
            alpha = 0x80;
        }
        spriteColor = ((u32)alpha << 24) | baseColor;
        nbPnlSprite(x + 0x17, y + 0x18, 8, 7, 0x98, 0x23, 8, 7, spriteColor, nbResultTex1);
        nbPnlSprite(x + 0x1F, y + 0x18, 0xAA, 7, 0x9F, 0x23, 1, 7, spriteColor, nbResultTex1);
        nbPnlSprite(x + 0xC9, y + 0x18, 8, 7, 0xA1, 0x23, 8, 7, spriteColor, nbResultTex1);
        if (nbResultScoreTable[scoreIndex].lvupflg != 0) {
            nbPnlSprite(x + 0x20, y + 8, 0x34, 0x1E, 0x9F, 1, 0x34, 0x1E, 0x80808080,
                        nbResultTex2);
            count = nbResultScoreTable[scoreIndex].lvuplvanmcnt + 1;
            nbResultScoreTable[scoreIndex].lvuplvanmcnt = count;
            if (count >= 9) {
                nbResultScoreTable[scoreIndex].lvuplvanmcnt = 8;
            }
            alpha = 0x80;
        }
        drawColor = ((u32)alpha << 24) | baseColor;
        nbPnlSprite(x + 0x27, y + 0x11, 0x12, 0xC, 0xAC, 0x1E, 0x12, 0xC, drawColor,
                    nbResultTex1);
        fontColor = rewardColor | alpha;
        level = nbResultScoreTable[scoreIndex].l_lv;
        sprintf(buf, nbResultFmt2d, level < 100 ? level : 99);
        nbResultFontHandle = (frFontHandle_t *)frwGetSmallNumFontFRQ((x + 0x3C) << 4, (y + 0x10) << 3,
                                                            0, fontColor, buf, NULL);
        frDrawFontEx(nbResultFontHandle, 1);
        frDestroyFontRequestAfter(nbResultFontHandle);
        nbPnlSprite(x + 0xCA, y, 0x9B, 0x1F, 1, 0x51, 0x9B, 0x1F, drawColor, nbResultTex1);
        nbPnlSprite(x + 0xD6, y + 8, 0x27, 0x18, 0x70, 0x12, 0x27, 0x18, drawColor,
                    nbResultTex1);
        if (nbResultScoreTable[scoreIndex].lvupanmflg != 0) {
            if (nbResultScoreTable[scoreIndex].lvupanmcnt >= 6) {
                alpha = (nbResultScoreTable[scoreIndex].lvupanmcnt * 128 - 640) / 10;
                sprintf(buf, nbResultFmt6d, expNextLevel - exp);
                fontColor = 0xA09DC380;
                nbResultFontHandle = (frFontHandle_t *)frwGetSmallNumFontFRQ(
                    (x + 0x11D) << 4, (y + 7) << 3, 0, rewardColor | alpha, buf, NULL);
                alpha = 0x80;
                frDrawFontEx(nbResultFontHandle, 1);
                frDestroyFontRequestAfter(nbResultFontHandle);
            }
        } else {
            sprintf(buf, nbResultFmt6d, expNextLevel - exp);
            nbResultFontHandle = (frFontHandle_t *)frwGetSmallNumFontFRQ(
                (x + 0x11D) << 4, (y + 7) << 3, 0, fontColor, buf, NULL);
            frDrawFontEx(nbResultFontHandle, 1);
            frDestroyFontRequestAfter(nbResultFontHandle);
        }
        if (nbResultScoreTable[scoreIndex].lvupanmflg != 0) {
            count = nbResultScoreTable[scoreIndex].lvupanmcnt;
            if (count < 2) {
                width = 0xE - count * 7;
                spriteColor = ((u32)alpha << 24) | 0x808080;
                nbPnlSprite(x + 0xF9, y - width / 2 + 0x18, 0x68, width, 1, 0x2E, 0x68, 0xE,
                            spriteColor, nbResultTex1);
            } else if (count >= 0xB) {
                width = (count * 0xE - 0x8C) / 5;
                spriteColor = ((u32)alpha << 24) | 0x808080;
                nbPnlSprite(x + 0xF9, y - width / 2 + 0x18, 0x68, width, 1, 0x3D, 0x68, 0xE,
                            spriteColor, nbResultTex1);
            }
        } else {
            spriteColor = ((u32)alpha << 24) | 0x808080;
            nbPnlSprite(x + 0xF9, y + 0x11, 0x68, 0xE, 1, 0x3D, 0x68, 0xE, spriteColor,
                        nbResultTex1);
            width = (exp - expAtLevel) * 0x68 / (expNextLevel - expAtLevel);
            if (width < 0) {
                width = 0;
            }
            if (width > 0x68) {
                width = 0x68;
            }
            if (expAtLevel < exp) {
                if (width < 2) {
                    width = 2;
                }
            }
            if (exp < expNextLevel) {
                if (width >= 0x68) {
                    width = 0x67;
                }
            }
            nbPnlSprite(x + 0xF9, y + 0x11, width, 0xE, 1, 0x2E, width, 0xE, spriteColor,
                        nbResultTex1);
        }
        cmpSetSkillListRows(0x14);
        if (work->flag & 4) {
            nbResultFontHandle = frGetCNameFRQ(0, 1, 0, 0);
            frSetDrawPosition(nbResultFontHandle, (x + 0x59) << 4, (y + 0xC) << 3);
            frSetFontColor(nbResultFontHandle, fontColor);
            frDrawFont(nbResultFontHandle);
            frDestroyFontRequestAfter(nbResultFontHandle);
        } else {
            nbResultFontHandle = (frFontHandle_t *)frwGetSmallFontFRQ(
                (x + 0x59) << 4, (y + 0xC) << 3, 0, fontColor, text2[work->id], NULL);
            frDrawFont(nbResultFontHandle);
            frDestroyFontRequestAfter(nbResultFontHandle);
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
        nbResultBestScore += SDF_FRAME_DUE();
    }
    return nbResultBestScore;
}

extern s32 nbTobiwarpLabelIdx;
extern f32 D_00551100;
extern frFontRequest_t *nbResultFontRequest;
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
/* 788 of 926 words match at the native 3704-byte size; first difference at
   +0x3AC. The ordinary casts already emit the four native CVT.W.S words
   (function 0x24); decompals objdump calls those bits trunc.w.s. Check raw
   opcodes rather than that mnemonic. Remaining differences include saved-
   register assignment and scheduling in the item-text loop and tail blocks.
   The inherited volatile cast in the item-collect loop reproduces retail's
   second lbu of the same byte and was worth about 30 words in earlier
   experiments, but volatile is not a legitimate match device, so that loop
   needs a real explanation before this function could land. */
void nbFindTobiwarpLabel(nbMainProcessData_t *data)
{
    char numberText[16];
    u32 itemId[3];
    u32 itemCount[3];
    s32 i;
    s32 itemTotal;
    s32 haveItem = 0;
    u32 rewardColor = 0xA09DC300;
    u32 color;
    s32 step;
    s32 alpha;
    s32 packed;
    u32 half;
    s32 scoreY;
    s32 resultState;
    s32 spread;
    frFontRequest_t *font;
    s32 ty1;
    s32 ty2;
    s32 px;
    s32 py;

    D_00551100 = D_00551100 + SDF_FRAME_STEP(0.099999995f);
    nbResultTimer += SDF_FRAME_DUE();
    nbPnl_setOt(0x47);
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);

    if (((s32)nbResultTimer) > 0) {
        if (((s32)nbResultTimer) < 10) {
            step = (s32)nbResultTimer;
        } else {
            step = 10;
        }
        alpha = (step * 0x80) / 10;
        packed = (alpha << 16) | alpha | (alpha << 8) | 0x80000000;
        nbPnlSprite(0, 0, 0x200, 0x1C0, 0, 0, 0x100, 0x100,
                    packed, nbResultTexHandle);
        alpha = (step * 112) / 10;
        packed = (alpha << 16) | alpha | (alpha << 8) | 0x10000000;
        nbPnlSprite(-2, -2, 0x204, 0x1C4, 0, 0, 0x100, 0x100,
                    packed, nbResultTexHandle);
        alpha = (step * 144) / 10;
        packed = (alpha << 16) | alpha | (alpha << 8) | 0x10000000;
        nbPnlSprite(-4, -4, 0x208, 0x1C8, 0, 0, 0x100, 0x100,
                    packed, nbResultTexHandle);
        alpha = step * 16;
        packed = (alpha << 16) | alpha | (alpha << 8) | 0x10000000;
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
        half = alpha | 0x80000000;
        packed = half | ((alpha << 16) | (alpha << 8));
        nbPnl_rsetAlpha(1);
        nbPnlSprite(0, 0x30, 0x9C, 0x2C, 1, 0x25, 0x9C, 0x2C,
                    packed, nbResultTex2);
        alpha = step * 128 / 15;
        nbPnl_rsetAlpha(0);
        nbPnlSprite(21, 58, 99, 21, 1, 1, 99, 21,
                    0x80808080, nbResultTex1);
        px = 21;
        py = 58;
        if (alpha > 0) {
            nbPnl_rsetAlpha(1);
            spread = step / 4;
            nbPnlSprite(px - spread, py - spread, 99 + spread * 2, 21 + spread * 2,
                        1, 1, 99, 21, ((u32)alpha << 24) | 0xC0C0C0, nbResultTex1);
            spread = step / 2;
            nbPnlSprite(px - spread, py - spread, 99 + spread * 2, 21 + spread * 2,
                        1, 1, 99, 21, ((u32)alpha << 24) | 0x909090, nbResultTex1);
            spread = step * 3 / 4;
            nbPnlSprite(px - spread, py - spread, 99 + spread * 2, 21 + spread * 2,
                        1, 1, 99, 21, ((u32)alpha << 24) | 0x606060, nbResultTex1);
            nbPnlSprite(px - step, py - step, 99 + step * 2, 21 + step * 2,
                        1, 1, 99, 21, ((u32)alpha << 24) | 0x303030, nbResultTex1);
        }
    }
    nbPnl_rsetAlpha(0);
    itemId[0] = 0;
    itemId[1] = 0;
    itemId[2] = 0;
    itemCount[0] = 0;
    itemCount[1] = 0;
    itemCount[2] = 0;
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
        ty1 = 0x338;
        ty2 = 0x348;
        for (i = 0; i < itemTotal; i++) {
            u32 id = itemId[i];
            if (id != 0) {
                font = frwGetSmallFontFRQ(0x1210, ty1,
                                          0, (s32)0xA09DC380, pStr[id], NULL);
                nbResultFontRequest = font;
                frDrawFont((frFontHandle_t *)font);
                frDestroyFontRequestAfter((frFontHandle_t *)nbResultFontRequest);
                nbPnlSprite(0x197, 0x6B + i * 0x1A, 0x0B, 0x0B,
                            0xAC, 0x12, 0x0B, 0x0B, 0x80808080,
                            nbResultTex1);
                sprintf(numberText, nbResultFmt2d, itemCount[i]);
                font = frwGetSmallNumFontFRQ(0x1AC0, ty2,
                                             0, 0xA09DC380, numberText, NULL);
                nbResultFontRequest = font;
                frDrawFont((frFontHandle_t *)font);
                frDestroyFontRequestAfter((frFontHandle_t *)nbResultFontRequest);
            }
            ty2 += 0xD0;
            ty1 += 0xD0;
        }
        cmpSetSkillListRows(-1);
        }
    }

    if (((s32)nbResultTimer) >= 24) {
        s32 phase = ((s32)nbResultTimer) < 31 ? ((s32)nbResultTimer) - 23 : 8;
        s32 y = (s32)(sdfSin(D_00551100 + D_00551100) * 16.0f);
        half = (0x70 + y) | ((u32)phase << 28);
            packed = half | (((0x70 + y) << 16) | ((0x70 + y) << 8));
        nbPnlSprite(0x27, 0x5E, 0x37, 0x23, 0x21, 1, 0x37, 0x23,
                    packed, nbResultTex2);
        y = (s32)(sdfSin((D_00551100 + D_00551100) + 0.099999995f) * 16.0f);
        half = (0x70 + y) | ((u32)phase << 28);
            packed = half | (((0x70 + y) << 16) | ((0x70 + y) << 8));
        nbPnlSprite(0x3F, 0x80, 0x1F, 0x1F, 1, 1, 0x1F, 0x1F,
                    packed, nbResultTex2);
        if (haveItem != 0) {
            y = (s32)(sdfSin((D_00551100 + D_00551100) + 0.19999999f) * 16.0f);
            half = (0x70 + y) | ((u32)phase << 28);
            packed = half | (((0x70 + y) << 16) | ((0x70 + y) << 8));
            nbPnlSprite(0xD1, 0x5E, 0x45, 0x23, 0x59, 1, 0x45, 0x23,
                        packed, nbResultTex2);
        }
    }

    if (((s32)nbResultTimer) >= 29) {
        scoreY = ((s32)nbResultTimer) < 43 ? ((s32)nbResultTimer) - 28 : 15;
        color = rewardColor | ((scoreY * 128) / 15);
        sprintf(numberText, nbResultFmt6d, nbGetKakutokuExp(data));
        font = frwGetSmallNumFontFRQ(0x7A0, 0x348, 0,
                                     color,
                                     numberText, 0);
        nbResultFontRequest = font;
        frDrawFontEx((frFontHandle_t *)font, 1);
        frDestroyFontRequestAfter((frFontHandle_t *)nbResultFontRequest);
        sprintf(numberText, nbResultFmt6d, nbGetKakutokuMaka(data));
        font = frwGetSmallNumFontFRQ(0x7A0, 0x448, 0,
                                     color,
                                     numberText, 0);
        nbResultFontRequest = font;
        frDrawFontEx((frFontHandle_t *)font, 1);
        frDestroyFontRequestAfter((frFontHandle_t *)nbResultFontRequest);
    }

    nbResultCounter = 0;
    if (((s32)nbResultTimer) >= 21) {
        nbResultTobiwarpLabel = ((s32)nbResultTimer) - 50;
        scoreY = 0xD0;
        for (i = 0; i < 4; i++) {
            nbResultScore_t *entry = &nbResultScoreTable[i];
            if (entry->useflg != 0) {
                nbResultDrawPC1(i, 0x3F, scoreY,
                                ((s32)nbResultTimer) - i * 6 - 20,
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
        if (((s32)nbResultTimer) < 10) {
            step = (s32)nbResultTimer;
        } else {
            step = 10;
        }
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
        step = resultState < SDF_FRAMES(16) ? (s32)SDF_FRAME_STEP(resultState * 8) : 0x80;
        alpha = resultState < SDF_FRAMES(16) ? 0x80 - (s32)SDF_FRAME_STEP(resultState * 8) : 0;
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
extern const char nbResultDoneMsg[];

/* Qualified reorg traces: the preceding C reward routine removes $2 from
 * the fall-through live set, preserving the final plain loading-state branch. */
#ifdef NON_MATCHING

#endif

s32 nbResultProcess2(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    if (dds3GetProcessRunTime(id) == SDF_FRAMES(3)) {
        dds3FadeIn2(10);
    }
    if (dds3GetProcessRunTime(id) >= SDF_FRAMES(16)) {
        if (dds3GetActiveScene() != NULL) {
            nbResultUpdateUnits();
        }
    }
    data = nbGetMainProcessData();
    if (dds3GetProcessRunTime(id) == SDF_FRAMES(20)) {
        nbResultStep = fldLoadNextFile_BtlRet();
    }
    if (nbResultStep != 0) {
        fldLoadNextTask();
        if (fldLoadNextGetStatus() == 0) {
            nbResultStep = 0;
            printf(nbResultDoneMsg);
        }
    }
    if (dds3GetProcessRunTime(id) >= SDF_FRAMES(21)) {
        if (dds3PadButtonState[0x21] != 0 || dds3PadButtonState[0x23] != 0) {
            nbResultInitTimer(data);
        }
        if (nbResultFindBestScore(data) >= 15) {
            if (dds3PadButtonState[0x21] != 0 || dds3PadButtonState[0x23] != 0) {
                if (nbResultState == 0) {
                    nbResultState = SDF_FRAMES(1);
                }
            }
        }
    }
    nbFindTobiwarpLabel(data);
    if (nbResultState > 0) {
        if (nbResultState < SDF_FRAMES(100)) {
            nbResultState++;
        }
        if (nbResultState >= SDF_FRAMES(16) && nbResultStep == 0) {
            nbResultAddExp();
            nbBgmFadeOut();
            if (data->battletime >= SDF_FRAMES(0x367)) {
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

const char nbResultDoneMsg[16] = "pac1 done\n";

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

const char nbStageLightName[16] = "BTL_DEF_LIGHT";

const sdfVector4Unaligned_t nbNiidaCameraPos = { 0.0f, -100.0f, -600.0f, 0.0f };

const char nbNiidaCameraText[16] = "NIIDA CAMERA";

const sdfVector4Unaligned_t nbEventCameraPos = { 0.0f, -100.0f, -600.0f, 0.0f };

const char nbEventCameraText[16] = "EVENT CAMERA";

const f32 nbStageCameraDefaultPos[4] = { 213.0f, -276.669983f, -636.839966f, 0.0f };

const f32 nbStageCameraDefaultRot[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

const char nbStageProcessName[16] = "nb_stage";

const char nbStageDrawProcessName[24] = "nb_draw_stage";

