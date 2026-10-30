#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "mselGlobal.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"
#include "field.h"

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

extern s32 fldWmapEnabled;

extern dds3ProcessID_t *mselShutDown(u32 mode);

extern const char D_0053DF08[];

extern void mselFreeResources(s32 arg);



extern mselGlobalWork_t *gGlobal;

s32 fclDestroyMessage(void);
s8 fclInitMessage(void *data);

s32 fclLoadTexture(char *pFileName);

extern void evtBitOn(s32 no);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern void *sdfMemGetPtr(sdfMemHandle_t *handle);
extern void *memset(void *ptr, s32 value, u32 size);
extern void *sdfChipAllocZeroDirect(s32 size);
extern void sdfChipFreeDirect(void *ptr);
extern u32 dds3GetRandIntA(void *state, u32 limit);
void mselCountSaves(void);
void Msel_TOverData_Clear(void);
void mselFindSaveSlot(s32 *first, s32 *second, u32 start, u32 end, s8 reverse);

void mselDevilChoice(void)
{
    mselSaveData_t *data = sdfMemGetPtr(gGlobal->unk_08);
    s32 count = data->count;
    s32 selected[10];
    s32 scores[10];
    s32 chosen[5];
    s32 i, j, remaining, chosenCount, target, index, poolCount;
    s32 *pool;

    memset(selected, -1, sizeof(selected));
    memset(scores, 0, sizeof(scores));
    memset(chosen, -1, sizeof(chosen));
    for (i = 0; i < count; i++) {
        u32 exp = data->records[i].exp;
        for (j = 0; j < 10; j++) {
            if (exp >= (u32)scores[j]) {
                mselFindSaveSlot(scores, selected, j + 1, 10, 0);
                scores[j] = exp;
                selected[j] = i;
                break;
            }
        }
    }
    remaining = 0;
    for (i = 0; i < 10; i++) {
        if (selected[i] >= 0) {
            remaining++;
        }
    }
    mselCountSaves();
    Msel_TOverData_Clear();
    chosenCount = 0;
    target = 5;
    for (i = 0; remaining > 0 && i < 3; i++) {
        index = dds3GetRandIntA(NULL, remaining);
        remaining--;
        gGlobal->tover[chosenCount] = selected[index];
        gGlobal->unk_60++;
        chosen[chosenCount++] = selected[index];
        mselFindSaveSlot(scores, selected, index + 1, 10, 1);
    }
    {
        s32 freshCount = data->count;
        if (freshCount < 5) target = freshCount;
    }
    poolCount = count - chosenCount;
    if (poolCount > 0) {
        pool = sdfChipAllocZeroDirect(poolCount * sizeof(*pool));
        if (chosenCount < target) {
            s32 filled = 0;
            for (i = 0; i < count; i++) {
                for (j = 0; j < chosenCount; j++) {
                    if (i == chosen[j]) {
                        break;
                    }
                }
                if (j >= chosenCount && j != 0) {
                    pool[filled++] = i;
                }
            }
        }
        remaining = poolCount;
        for (i = chosenCount; i < target; i++) {
            index = dds3GetRandIntA(NULL, poolCount);
            poolCount--;
            gGlobal->tover[i] = pool[index];
            gGlobal->unk_60++;
            chosen[i] = pool[index];
            mselFindSaveSlot(pool, NULL, index + 1, remaining, 1);
        }
        sdfChipFreeDirect(pool);
    }
    gGlobal->Z_RGBA = 0x64618700;
    gGlobal->Zalpha_cnt = 0;
}


void Msel_TOverData_Clear(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        gGlobal->tover[i] = -1;
    }
    gGlobal->unk_60 = 0;
}

void mselCountSaves(void)
{
    mselGlobalWork_t *work;
    s32 i;

    for (i = 0; i < 5; i++)
    {
        gGlobal->unk_94[i] = gGlobal->tover[i];
    }
    work = gGlobal;
    work->unk_90 = work->unk_60;
    if (work->unk_60 != 0)
    {
        work->unk_88 = 0;
        work->unk_8C = 0x64618780;
    }
}

void mselFindSaveSlot(s32 *first, s32 *second, u32 start, u32 end, s8 reverse)
{
    u32 i;

    if (reverse != 0) {
        i = start;
        if (start < end) {
            for (; i < end; i++) {
                if (first != NULL) {
                    first[i - 1] = first[i];
                }
                if (second != NULL) {
                    second[i - 1] = second[i];
                }
            }
        }
    } else {
        i = end - 1;
        if (i >= start) {
            do {
                if (first != NULL) {
                    first[i] = first[i - 1];
                }
                if (second != NULL) {
                    second[i] = second[i - 1];
                }
                i--;
            } while (i >= start);
        }
    }
}

extern char D_0053DEC0[];

extern char D_0053DED8[];

void mselLoadBackground(void)
{
    gGlobal->unk_40 = fclLoadTexture(D_0053DEC0);
    gGlobal->unk_44 = 0;
    gGlobal->unk_48 = fclLoadTexture(D_0053DED8);
}

extern void sdfTexFreeDelayed(s32 aHandle);

void mselFreeTextures(void)
{
    if (gGlobal->unk_40 != 0) {
        sdfTexFreeDelayed(gGlobal->unk_40);
        gGlobal->unk_40 = 0;
    }
    if (gGlobal->unk_44 != 0) {
        sdfTexFreeDelayed(gGlobal->unk_44);
        gGlobal->unk_44 = 0;
    }
    if (gGlobal->unk_48 != 0) {
        sdfTexFreeDelayed(gGlobal->unk_48);
        gGlobal->unk_48 = 0;
    }
}

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DEC0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DED8);

const char D_0053DEF0[] = "/title/mseldata.bmd";

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer, s32 shutdownTimer,
                                         dds3ProcessFunc_t processFunc, dds3ProcessFunc_t shutdownFunc, void *work);
extern s32 mselMain(dds3ProcessID_t *id);
extern u32 dds3BackColor;

dds3ProcessID_t *mselShutDown(u32 mode)
{
    sdfMemHandle_t *handle;
    u32 *work;
    s32 i;
    void *fileHandle;

    handle = sdfMemAllocDirect(0xB4);
    work = sdfMemLock(handle);
    gGlobal = (mselGlobalWork_t *)work;
    gGlobal->unk_00 = handle;
    gGlobal->unk_4C = 0;
    gGlobal->unk_50 = 0;
    gGlobal->cursor = 0;
    for (i = 0; i < 5; i++) {
        gGlobal->tover[i] = -1;
    }
    gGlobal->unk_60 = 0;
    mselLoadBackground();
    gGlobal->unk_A8 = mode;
    gGlobal->unk_04 = sdfMemAllocDirect(0x1A510);
    gGlobal->unk_08 = sdfMemAllocDirect(0x7E90);
    fileHandle = sdfDevLoadFileEx(D_0053DEF0, (u32 *)&gGlobal->unk_5C, NULL);
    gGlobal->unk_58 = fileHandle;
    fclInitMessage(gGlobal->unk_5C);
    dds3BackColor = 0x80000000;
    return dds3StartProcess((char *)D_0053DF08, 0x2B19, 0, 0, mselMain, NULL, NULL);
}

extern void mselFreeTextures(void);

extern void sdfMemFreeDelayedDirect(void *aHandle);

void mselFreeResources(s32 arg)
{
    mselFreeTextures();
    fclDestroyMessage();
    sdfMemFreeDelayedDirect(gGlobal->unk_04);
    sdfMemFreeDelayedDirect(gGlobal->unk_08);
    sdfMemFreeDelayedDirect(gGlobal->unk_58);
    sdfMemFreeDelayedDirect(gGlobal->unk_00);
    gGlobal = NULL;
}

const char D_0053DF08[] = "mselProc";

extern void *memcpy(void *dest, const void *src, u32 size);
extern s8 dds3PadButtonState[];
extern void rsetOtOfs(s32 offset);
extern void rsetAlpha(s32 alpha);
extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        u32 color, sdfTexHandle_t *texture);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern s32 dds3FadeCheck(void);
extern void dds3AdminiCallSequence(s32 sequence, void *data, u32 size, s32 mode);
extern s32 evtBitOff(s32 no);
extern s32 fclChkMessage(void);
extern s32 fclChkSelMessage(void);
extern s8 fclGetSelMessagePos(void);
extern void fclSetSelMessagePos(s8 pos);
extern s32 fclStartMessage(s32 message);
extern s32 fclStartSelMessage(s32 message);
extern s8 fclEndMessage(void);
extern void fclUpdateMessage(void);
extern s32 fclEncycRegist2(fclencyceelem_t *record, s32 mode);
extern void fmProcTrigSequence(s32 sequence);
extern s32 fmProcessChk(void);
extern u32 slGetExitMode(void);
extern s32 p2sdLoadCheckMidi(s32 id);
extern void p2sdLoadSMGFile(s32 id);
extern void p2sdPlayBGMFade(s32 id);
extern s32 p2sdStopBGM(s32 id);
extern void p2sdPlaySE(s32 id, s32 volume, s32 pan);
extern void mselDrawTitleAnim(void);
extern void mselDrawTitleAnim2(s32 animate);
extern void mselDrawBackground(void);
extern void mselUpdateCursor(void);
extern void mselStartDataSet(void);
extern void mselStartGame(void);

s32 mselMain(dds3ProcessID_t *id)
{
    s32 selection, count, digit, hours, seconds, i, sequence;
    mselSaveData_t *save;
    dds3GlobalWork_t *snapshot;
    sdfMemHandle_t *handle;
    mselGlobalWork_t *work;

    rsetOtOfs(0x48);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    switch (gGlobal->unk_4C) {
    case 0:
        dds3FadeOut(0, 0, 0, 1);
        if (p2sdLoadCheckMidi(0x710000) != 0) {
            gGlobal->unk_4C = 2;
            gGlobal->unk_50 = 0;
        } else {
            p2sdLoadSMGFile(0x710000);
            gGlobal->unk_50 = 0;
            gGlobal->unk_4C = (s32)((u32)gGlobal->unk_4C + 1);
        }
        break;
    case 1:
        if (p2sdLoadCheckMidi(0x710000) != 0) {
            gGlobal->unk_50 = 0;
            gGlobal->unk_4C = (s32)((u32)gGlobal->unk_4C + 1);
        }
        break;
    case 2:
        p2sdPlayBGMFade(0x710001);
        dds3FadeIn(0, 0, 0, 20);
        gGlobal->unk_50 = 0;
        gGlobal->unk_4C = (s32)((u32)gGlobal->unk_4C + 1);
        mselStartDataSet();
        break;
    case 3:
        mselDrawTitleAnim();
        if (dds3FadeCheck() == 0) {
            /* Keep the live cursor alias in a2 until its field read. Ordinary
             * allocation merges it with the mode/store base; this local EE
             * constraint preserves native scheduling, not original spelling. */
            register mselGlobalWork_t *cursorWork __asm__("$6");
            s32 titleMode;
            mselUpdateCursor();
            titleMode = gGlobal->unk_A8;
            cursorWork = gGlobal;
            if (titleMode == 0 && dds3PadButtonState[0x23] < 0) {
                gGlobal->unk_50 = 0;
                gGlobal->unk_4C = 22;
                dds3FadeOut(0, 0, 0, 20);
                p2sdStopBGM(0x710001);
                p2sdPlaySE(10, 127, 63);
            } else if (dds3PadButtonState[0x21] < 0 || dds3PadButtonState[0x2C] < 0) {
                if (cursorWork->cursor == 0) {
                    fldWmapEnabled = 0;
                    evtBitOn(0xAF2);
                } else {
                    fldWmapEnabled = 1;
                    evtBitOff(0xAF2);
                }
                p2sdPlaySE(8, 127, 63);
                gGlobal->unk_A8 = 1;
                gGlobal->unk_50 = 0;
                Msel_TOverData_Clear();
                p2sdStopBGM(0x710001);
                gGlobal->unk_4C = 16;
                dds3FadeOut(0, 0, 0, 20);
            }
        }
        break;
    case 15:
        mselDrawTitleAnim();
        goto startMessage;
    case 16:
        mselDrawTitleAnim();
        goto loadAttention;
    case 20:
        mselDrawTitleAnim2(0);
        /* fall through */
    case 17:
    startMessage:
        if (dds3FadeCheck() == 0) {
            dds3FadeIn(0, 0, 0, 20);
            fclStartMessage(0);
            gGlobal->unk_4C = 4;
        }
        break;
    case 21:
        mselDrawTitleAnim2(1);
        if (dds3FadeCheck() == 0) {
            gGlobal->unk_4C = 12;
            gGlobal->unk_50 = 0;
            dds3FadeIn(0, 0, 0, 90);
        }
        break;
    case 4:
        mselDrawBackground();
        if (fclChkMessage() == 0) {
            gGlobal->unk_4C = 5;
            gGlobal->unk_50 = 0;
            fclEndMessage();
            fclSetSelMessagePos(0);
            fclStartMessage(1);
            fclStartSelMessage(2);
        } else fclUpdateMessage();
        break;
    case 5:
        mselDrawBackground();
        if (fclChkSelMessage() != 0) {
            gGlobal->unk_4C = 6;
            gGlobal->unk_50 = 0;
        }
        fclUpdateMessage();
        break;
    case 6:
        mselDrawBackground();
        if (fclChkSelMessage() == 0) {
            selection = fclGetSelMessagePos();
            fclEndMessage();
            Msel_TOverData_Clear();
            if (selection == 0) {
                snapshot = sdfMemGetPtr(gGlobal->unk_04);
                *snapshot = *DDS3_GBWK;
                gGlobal->unk_4C = 18;
                gGlobal->unk_50 = 0;
                dds3FadeOut(0, 0, 0, 20);
            } else {
                p2sdStopBGM(0x710001);
                gGlobal->unk_4C = 19;
                dds3FadeOut(0, 0, 0, 20);
            }
        } else fclUpdateMessage();
        break;
    case 18:
        mselDrawBackground();
        if (dds3FadeCheck() == 0) {
            dds3FadeIn(0, 0, 0, 20);
            gGlobal->unk_4C = 7;
            gGlobal->unk_50 = 0;
            fmProcTrigSequence(4);
        }
        break;
    case 19:
        mselDrawBackground();
    loadAttention:
        if (dds3FadeCheck() == 0) {
            if (gGlobal->unk_40 != 0) {
                sdfTexFreeDelayed(gGlobal->unk_40);
                gGlobal->unk_40 = 0;
            }
            selection = fclLoadTexture("/title/att_font.tmx");
            gGlobal->unk_4C = 13;
            gGlobal->unk_44 = selection;
            gGlobal->unk_50 = 0;
            dds3FadeIn(0, 0, 0, 90);
        }
        break;
    case 7:
        if (fmProcessChk() == 0) {
            u32 exitMode = slGetExitMode();
            if (exitMode != 0) {
                mselSaveData_t *loadedSave;
                if (exitMode != 2) break;
                loadedSave = sdfMemGetPtr(gGlobal->unk_08);
                *loadedSave = DDS3_GBWK->encyclopedia;
                memcpy(&gGlobal->fileInfo, DDS3_GBWK, sizeof(GameFileInfo_t));
                handle = DDS3_GBWK->memHandle;
                snapshot = sdfMemGetPtr(gGlobal->unk_04);
                *DDS3_GBWK = *snapshot;
                DDS3_GBWK->memHandle = handle;
                mselDevilChoice();
                if (gGlobal->unk_60 == 0) {
                    dds3FadeIn(0, 0, 0, 90);
                    gGlobal->unk_4C = 12;
                    gGlobal->unk_50 = 0;
                } else {
                    dds3FadeIn(0, 0, 0, 15);
                    gGlobal->unk_4C = 8;
                    gGlobal->unk_50 = 0;
                    fclStartMessage(3);
                }
            } else {
                gGlobal->unk_4C = 17;
                gGlobal->unk_50 = 0;
                dds3FadeOut(0, 0, 0, 0);
            }
        }
        break;
    case 8:
        mselDrawTitleAnim2(0);
        if (fclChkMessage() == 0) {
            gGlobal->unk_4C = 9;
            gGlobal->unk_50 = 0;
            fclSetSelMessagePos(0);
            fclStartMessage(4);
            fclStartSelMessage(5);
        } else fclUpdateMessage();
        break;
    case 9:
        mselDrawTitleAnim2(0);
        if (fclChkSelMessage() != 0) {
            gGlobal->unk_4C = 10;
            gGlobal->unk_50 = 0;
        }
        fclUpdateMessage();
        break;
    case 10:
        mselDrawTitleAnim2(0);
        if (fclChkSelMessage() == 0) {
            selection = fclGetSelMessagePos();
            fclEndMessage();
            if (selection == 0) {
                gGlobal->unk_4C = 11;
                gGlobal->unk_50 = 0;
                fclStartMessage(6);
            } else if (selection == 1) {
                gGlobal->unk_4C = 8;
                gGlobal->unk_50 = 0;
                mselDevilChoice();
                fclStartMessage(3);
            } else {
                gGlobal->unk_4C = 20;
                gGlobal->unk_50 = 0;
                dds3FadeOut(0, 0, 0, 20);
            }
        } else fclUpdateMessage();
        break;
    case 11:
        mselDrawTitleAnim2(1);
        if (fclChkMessage() != 0) fclUpdateMessage();
        else {
            fclEndMessage();
            gGlobal->unk_4C = 21;
            gGlobal->unk_50 = 0;
            dds3FadeOut(0, 0, 0, 20);
        }
        break;
    case 12:
        evtBitOn(0x8B1);
        count = (s16)gGlobal->fileInfo.endcnt + 1;
        if (count <= 0) count = 1;
        if (count > 0) evtBitOn(0x8B6);
        if (count >= 2) evtBitOn(0x8B7);
        if (count >= 3) evtBitOn(0x8B8);
        if (count >= 4) evtBitOn(0x8B9);
        if (count >= 5) evtBitOn(0x8BA);
        count = gGlobal->fileInfo.PlayTime;
        if (count > 0x066FEBF8) count = 0x066FEBF8;
        /* The retail residue retains the divisors of a full h/m/s split.
         * As in the live save-preview family, compute that split before the
         * minute digit; ee-gcc removes the unused hours/seconds arithmetic
         * but retains divisor setup from its division guards. The live count
         * scalar is reused across completion-count, tick and digit phases. */
        hours = count / 108000;
        digit = (count % 108000) / 1800;
        seconds = (count % 1800) / 30;
        count = digit % 10;
        if (count == 0) evtBitOn(0x8B2);
        if (count == 8) evtBitOn(0x8B3);
        if (count == 4) evtBitOn(0x8B4);
        if (count == 5) evtBitOn(0x8B5);
        save = sdfMemGetPtr(gGlobal->unk_08);
        for (i = 0; i < 5; i++) {
            s32 entry = gGlobal->tover[i];
            if (entry != -1) fclEncycRegist2(&save->records[entry], 1);
        }
        p2sdStopBGM(0x710001);
        {
            /* Local v0 allocation only covers the pre-call handle reads.
             * Reload gGlobal after free; this is not a cross-call cache. */
            register mselGlobalWork_t *textureWork __asm__("$2") = gGlobal;
            if (textureWork->unk_40 != 0) {
                sdfTexFreeDelayed(textureWork->unk_40);
                gGlobal->unk_40 = 0;
            }
        }
        selection = fclLoadTexture("/title/att_font.tmx");
        gGlobal->unk_4C = 13;
        gGlobal->unk_44 = selection;
        gGlobal->unk_50 = 0;
        break;
    case 13: {
        s32 fadeCounter, opacity, nextCounter;
        rsetOtOfs(0x47);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        fadeCounter = gGlobal->unk_50;
        opacity = 128;
        if ((u32)fadeCounter - 240 < 91) {
            /* Preserve the live 128 endpoint in a0 for subtraction and clamp;
             * ordinary allocation reuses opacity and schedules div differently.
             * This target-specific aid emits no arithmetic instructions. */
            register s32 maximum __asm__("$4") = 128;
            opacity = maximum - (fadeCounter - 240) * 128 / 90;
            if (opacity < 0) opacity = 0;
            if (opacity > 128) opacity = maximum;
        }
        if (fadeCounter >= 330) opacity = 0;
        DrwSprite2D(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0,
                    ((u32)opacity << 24) | 0x808080, (sdfTexHandle_t *)gGlobal->unk_44);
        if (gGlobal->unk_50 == 330) dds3FadeOut(255, 255, 255, 60);
        work = gGlobal;
        nextCounter = work->unk_50;
        if (nextCounter >= 390) {
            work->unk_50 = 0;
            work->unk_4C = 14;
            mselStartGame();
        } else work->unk_50 = (s32)((u32)nextCounter + 1);
        break;
    }
    case 22: {
        s32 exitCounter;
        mselDrawTitleAnim();
        exitCounter = gGlobal->unk_50;
        if (exitCounter >= 21) {
            gGlobal->unk_50 = 0;
            gGlobal->unk_4C = 14;
            sequence = 3;
            dds3AdminiCallSequence(2, &sequence, sizeof(sequence), 0);
        } else gGlobal->unk_50 = (s32)((u32)exitCounter + 1);
        break;
    }
    }
    return 0;
}
s32 mselExit(u32 mode)
{
    mselShutDown(mode);
    return -1;
}

s32 mselInit(void)
{
    mselFreeResources(0);
    dds3KillProcessName((char *)D_0053DF08, 1);
    return 0;
}

s32 fldWmapIsEnabled(void)
{
    return fldWmapEnabled;
}

extern void dds3GlobalWorkUserInit2(s32 mode);

extern s32 dds3ConfigChanged(void);

extern void dds3ConfigSaveBackup(void);

extern void dds3ConfigRestoreBackup(void);

extern void evtBitOn(s32 no);

#ifdef NON_MATCHING
extern void dds3CreateGameData(s32 no);
#endif

void mselStartDataSet(void)
{
    if (gGlobal->unk_A8 == 0) {
        dds3CreateGameData(0);
    }
    if (DDS3_GBWK->transition != 0) {
        dds3GlobalWorkUserInit2(1);
    } else {
        dds3GlobalWorkUserInit2(0);
    }
    if (dds3ConfigChanged() == 0) {
        dds3ConfigSaveBackup();
    } else {
        dds3ConfigRestoreBackup();
    }
    evtBitOn(0xC0F);
}

extern fldDanteGunState_t fldRunDanteGunFlag;

extern void fldSetNextField(s32 a, s32 b);

extern void dds3AdminiCallSequence(s32 sequence, void *data, u32 datasize, s32 restore);

void mselStartGame(void)
{
    s32 field;

    fldRunDanteGunFlag.enabled = 1;
    field = 0x259;
    fldRunDanteGunFlag.counter = 0;
    fldSetNextField(0xB, 0x259);
    dds3AdminiCallSequence(7, &field, 4, 0);
}

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DF90);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DFA8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DFB8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DFC8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DFD8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DFF0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E008);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E020);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E030);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E040);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E050);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E060);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E078);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E088);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0A0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0B0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0C0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0E0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0F8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E108);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E118);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E130);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E148);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E158);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E168);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E178);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E190);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E1A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E1C0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E1D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E1E0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E1F0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E200);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E218);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E228);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E240);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E250);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E260);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E270);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E280);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E290);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E2A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E2C0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E2D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E2E0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E2F0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E308);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E318);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E328);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E338);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E348);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E360);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E370);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E380);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E390);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E3A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E3B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E3C8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E3D8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E3F0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E400);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E410);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E420);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E430);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E440);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E458);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E468);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E478);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E488);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E498);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E4A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E4B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E4C8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E4D8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E4E8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E500);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E510);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E520);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E530);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E540);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E550);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E560);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E570);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E580);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E590);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5A0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5B0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5C0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5E8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5F8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E608);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E618);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E630);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E648);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E660);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E678);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E690);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E6A0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E6B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E6C8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E6E0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E6F0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E700);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E710);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E720);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E730);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E740);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E750);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E760);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E770);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E780);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E798);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E7B0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E7C0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E7D8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E7E8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E7F8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E808);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E818);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E828);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E838);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E848);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E878);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E890);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E8A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E8B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E8D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E8E0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E8F8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E910);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E920);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E930);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E950);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E968);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E978);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E988);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E998);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E9A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E9B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E9D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E9E8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA00);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA10);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA20);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA30);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA48);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA60);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA78);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA88);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA98);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EAA8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EAB8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EAD0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EAE0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EAF0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB08);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB18);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB30);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB40);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB58);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB68);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB80);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB90);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBA0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBB0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBC0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBD0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBE0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBF0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC00);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC10);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC28);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC38);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC48);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC58);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC70);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC80);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC90);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ECA0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ECB0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ECC0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ECD0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ECE8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED00);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED18);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED30);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED48);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED60);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED78);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED90);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EDA8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EDC0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EDD8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EDF0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE08);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE20);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE38);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE50);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE68);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE80);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE98);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EEB0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EEC8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EEE0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EEF8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF10);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF28);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF40);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF58);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF70);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF88);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EFA0);

