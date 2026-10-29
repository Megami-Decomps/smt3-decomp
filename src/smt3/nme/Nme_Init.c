#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "nme.h"
#include "newdata.h"


extern void sdfTexFree(sdfTexHandle_t *aHandle);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void dds3KillProcessName(const char *name, s32 shutdowntimer);

extern char D_00551690[];

extern char D_005487C8[];

extern char D_005487D8[];

extern s8 gProcessStat;

u8 nmeCharToIndex(u8 lead, u8 trail) {
    s32 value = (((s32)lead << 8) | trail) - 0x8080;
    s32 index = ((u32)(value & 0xFF00) >> 1) + (value & 0x7F);
    return (u8)(index - 0x4A);
}

extern const u8 D_005276F8[][2];

u32 nmeDecodeChar(s8 code)
{
    s32 index = code - 0x20;
    return ((u32)D_005276F8[index][0] << 8) | D_005276F8[index][1];
}

extern void frDelFontStyle(u8 styleNo);
extern void nmeBuildNameFont(s8 mode, nmeWork_t *work);
extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void frLoadFontData(s32 style, const char *filename);
extern void nmeInitNameRows(s8 target, nmeWork_t *work);
extern char D_00548768[];

#ifdef NON_MATCHING
extern void *memset(void *a, s32 b, u32 c);
#endif

void frAllocNameData(void)
{
    s8 target = 0;
    sdfMemHandle_t *handle = sdfMemAllocDirect(0x51C8);
    nmeWork_t *work = sdfMemLock(handle);

    memset(work, 0, 0x51C8);
    frLoadFontData(4, D_00548768);
    for (; target < 4; target++) {
        nmeInitNameRows(target, work);
        nmeBuildNameFont(target, work);
    }
    frDelFontStyle(4);
    sdfMemFreeDirect(handle);
}

extern u8 D_00552020[4];
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);
void nmeStartFadeOut(void)
{
    dds3FadeOut(D_00552020[0], D_00552020[1], D_00552020[2], 0x1E);
}

s32 nmeGetInputMode(nmeWork_t *work) {
    s32 inputPos;

    if (work->inputMode < 2) {
        if (work->inputMode >= 0) {
            inputPos = work->inputPos;
            return inputPos / 8 != 0;
        }
    }
    return 2;
}

s32 nmeGetMaxNameLen(s32 mode) {
    return 8;
}

s8 nmeGetModeFlag(nmeWork_t *work) {
    return nmeGetMaxNameLen(nmeGetInputMode(work));
}

extern const char *D_005276C8[];

#ifdef NON_MATCHING
void nmeInitNameRow(s8 mode, s32 rowValue, void *data)
{
    nmeWork_t *work = data;
    s8 row = rowValue;
    s32 maxLength;
    const char *name;
    u8 (*characters)[2];
    s8 count;
    s32 i = 0;

    mode *= 3;
    maxLength = nmeGetMaxNameLen(row);

    if (maxLength > 0) {
        do {
            work->inputChars[row][i][0] = 0x80;
            work->inputChars[row][i][1] = 0x80;
            i++;
        } while (i < maxLength);
    }
    count = 0;
    name = D_005276C8[mode + row];
    characters = work->inputChars[row];
    while (*name != 0) {
        if (*name & 0x80) {
            (*characters)[0] = *name++;
            (*characters)[1] = *name;
        } else {
            u32 code = nmeDecodeChar(*name);
            (*characters)[1] = code;
            (*characters)[0] = code >> 8;
        }
        name++;
        characters++;
        count++;
    }
    work->inputRowEnabled[row] = count;
}
#else
INCLUDE_ASM(const s32, "nme/Nme_Init", nmeInitNameRow);
#endif

extern void nmeInitNameRow(s8 mode, s32 row, void *data);
void nmeInitNameRows(s8 mode, nmeWork_t *work) {
    nmeInitNameRow(mode, 0, work);
    nmeInitNameRow(mode, 1, work);
    nmeInitNameRow(mode, 2, work);
}

s32 nmeGetRowCount(s8 mode)
{
    switch (mode) {
    case 0:
        return 0xF;
    case 1:
        return 0xF;
    case 2:
        return 0x10;
    case 3:
        return 0xF;
    default:
        return 0;
    }
}

#ifdef NON_MATCHING
extern void cmpInitCursorLayout(void *cursor, s32 a, s32 b, s32 c, s32 d, s32 mode);
extern void cmpInitCursorRange(void *cursor, u8 count, u8 visible, s32 layout);
extern void nmeInitNameCursor(s8 mode, nmeWork_t *work);
#endif

void nmeInitNameBuffer(s8 mode, nmeWork_t *work) {
    work->inputMode = mode;
    work->inputPos = 0;
    work->primaryCursorActive = 0;
    work->alternateCursorActive = 0;
    work->unk1B = 0;
    nmeInitNameCursor(2, work);
    switch (mode) {
    case 0:
    case 1:
        cmpInitCursorRange(&work->modeCursor, 4, 4, 0);
        return cmpInitCursorLayout(&work->modeCursor, 0, 0, 0, 0, 0);
    case 2:
        cmpInitCursorRange(&work->modeCursor, 3, 3, 0);
        cmpInitCursorLayout(&work->modeCursor, 0, 0, 0, 0, 0);
        break;
    }
}


extern const char *D_005276C4 __attribute__((section(".data")));
extern u32 strlen(const char *string);
extern void cmpInitCursorRange(void *cursor, u8 count, u8 visible, s32 layout);
extern void cmpInitCursorLayout(void *cursor, s32 a, s32 b, s32 c, s32 d, s32 mode);

void nmeInitNameCursor(s8 mode, nmeWork_t *work)
{
    u16 inputCount = 0;
    u8 rowCount = nmeGetRowCount(mode);

    switch (mode) {
    case 0:
        inputCount = 6;
        break;
    case 1:
        inputCount = 6;
        break;
    case 2:
        inputCount = 6;
        break;
    case 3: {
        u32 length = strlen(D_005276C4);
        work->alternateCursorActive = 0;
        inputCount = length / 32;
        break;
    }
    }
    cmpInitCursorRange(&work->inputCursor, inputCount, 6, 0);
    cmpInitCursorLayout(&work->inputCursor, 0, 0, 0, 0, 0);
    cmpInitCursorRange(&work->nameCursor, rowCount, rowCount, 0);
    cmpInitCursorLayout(&work->nameCursor, 0, 0, 0, 0, 0);
}

extern void cmpResetCursorPos(s32 value, cmpCursorInfo_t *cursor);

void nmeInitCursor(s8 mode, nmeWork_t *work)
{
    cmpCursorInfo_t *cursor = &work->cursor;
    s8 row;

    cmpInitCursorRange(cursor, 4, 4, 0);
    cmpInitCursorLayout(cursor, 0, 0, 0, 0, 0);
    row = work->inputCursor.unk_24;
    if (row < 6) {
        if (row >= 3) {
            return cmpResetCursorPos(row - 2, cursor);
        }
    }
    cmpResetCursorPos(0, cursor);
    /* Qualified cc1probe --jump: this reset follows the nonsibling call in
     * .01.sibling and .11.cfg, then disappears in .13.life (verify: OK).
     * It retains the native jal/epilogue under the CONTRIBUTING exception. */
    row = 0;
}


extern char D_00548780[];
extern char D_00548798[];
extern char D_005487B0[];
extern char *nmeCharTexFile[4];
extern char *D_005276B0 __attribute__((section(".data")));
extern s32 fclLoadTexture(char *filename);
extern s32 evtBitCheck(s32 bit);

void nmeTexLoad(s8 mode, nmeWork_t *work)
{
    work->textures[0] = (sdfTexHandle_t *)fclLoadTexture(D_00548780);
    work->textures[1] = (sdfTexHandle_t *)fclLoadTexture(D_00548798);
    work->textures[2] = (sdfTexHandle_t *)fclLoadTexture(D_005487B0);
    if (evtBitCheck(0x8B0) != 0 && mode == 0) {
        work->textures[3] = (sdfTexHandle_t *)fclLoadTexture(D_005276B0);
    } else {
        work->textures[3] = (sdfTexHandle_t *)fclLoadTexture(nmeCharTexFile[mode]);
    }
}

void nmeFreeNameTextures(nmeWork_t *work) {
    s32 textureIndex;
    for (textureIndex = 0; textureIndex < 4; textureIndex++) {
        sdfTexFree(work->textures[textureIndex]);
    }
}

extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void cmpInitSeqInfo(void *work, s32 parent);
extern void nmeInitNameBuffer(s8 mode, nmeWork_t *work);
extern void nmeInitCursor(s8 mode, nmeWork_t *work);
extern void nmeTexLoad(s8 mode, nmeWork_t *work);
extern void frLoadFontData(s32 style, const char *filename);
extern char D_00548768[];

nmeWork_t *nmeLoadFont(s8 target, s8 mode, s32 parent)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(0x51C8);
    nmeWork_t *work = sdfMemLock(handle);

    memset(work, 0, 0x51C8);
    work->memHandle = handle;
    cmpInitSeqInfo(work, parent);
    work->sequence.current = 0;
    work->entryMode = mode;
    work->sequence.change = 1;
    work->targetNo = target;
    work->sequence.last = 0;
    work->inputRowEnabled[0] = 0;
    work->inputRowEnabled[1] = 0;
    work->inputRowEnabled[2] = 0;
    nmeInitNameBuffer(0, work);
    nmeInitCursor(4, work);
    nmeInitNameRows(target, work);
    frLoadFontData(4, D_00548768);
    nmeTexLoad(target, work);
    return work;
}

#ifdef NON_MATCHING
extern dds3GlobalWork_t *volatile nmeFontGlobalWork asm("DDS3_GBWK");

typedef struct {
    u32 packedRows[24][3];
} nmeNameGlyphImage_t;

typedef nmeNameGlyphImage_t nmeNameGlyphImages_t[16];

extern u8 *frDecodeFontImage(u8 style, s32 fontNo);
extern void sdfChipFreeDirect(void *ptr);

void nmeBuildNameFont(s8 target, nmeWork_t *work)
{
    volatile nmeWork_t *live = work;
    s8 row;
    s8 glyphCodeIndex = 0;
    s32 bitmapIndex = 0;

    for (row = 0; row < 2; row++) {
        s32 index;
        dds3GlobalWork_t *global = nmeFontGlobalWork;
        u8 count = live->inputRowEnabled[row];
        global->unk657C[(s32)target * 2 + row] = count;
        if (work->inputRowEnabled[row] <= 0) {
            continue;
        }
        index = 0;
        do {
            u8 lead = work->inputChars[row][index][0];
            u8 trail = work->inputChars[row][index][1];
            u16 code = (u16)(((u16)lead << 8) | trail);
            s32 fontNo;
            u16 decoded;
            u8 *source;
            nmeNameGlyphImage_t *destination;

            if (code > 0x8080) {
                decoded = nmeCharToIndex(lead, trail);
            } else if (code == 0x8080) {
                decoded = 0x20;
            } else {
                decoded = code;
            }
            nmeFontGlobalWork->unk657C[8 + (s32)target * 0x20 + (s32)glyphCodeIndex * 2] = decoded >> 8;
            nmeFontGlobalWork->unk657C[9 + (s32)target * 0x20 + (s32)glyphCodeIndex * 2] = decoded;
            glyphCodeIndex = (s8)(glyphCodeIndex + 1);
            if (decoded < 0x80) {
                fontNo = decoded - 0x20;
            } else {
                s32 value = decoded - 0x8080;
                fontNo = ((value & 0xFF00) >> 1) + (value & 0x7F);
            }
            source = frDecodeFontImage(4, fontNo);
            destination = &((nmeNameGlyphImages_t *)nmeFontGlobalWork->nameData)[target][bitmapIndex];
            *destination = *(nmeNameGlyphImage_t *)source;
            sdfChipFreeDirect(source);
            bitmapIndex++;
            index++;
        } while (work->inputRowEnabled[row] > index);
    }

    {
        s32 index;
        s8 count = work->inputRowEnabled[2];
        u8 byteCount = live->inputRowEnabled[2];
        if (count > 0) {
            index = 0;
            do {
                u8 lead = work->inputChars[2][index][0];
                u8 trail = work->inputChars[2][index][1];
                u16 code = (u16)(((u16)lead << 8) | trail);
                u16 decoded;

                if (code > 0x8080) {
                    decoded = nmeCharToIndex(lead, trail);
                } else if (code == 0x8080) {
                    decoded = 0x20;
                } else {
                    decoded = code;
                }
                nmeFontGlobalWork->unk657C[0x88 + index * 2] = decoded >> 8;
                nmeFontGlobalWork->unk657C[0x89 + index * 2] = decoded;
                index++;
                count = work->inputRowEnabled[2];
                byteCount = live->inputRowEnabled[2];
            } while (index < count);
            nmeFontGlobalWork->unk657C[0xD8] = byteCount;
        }
    }
}
#else
INCLUDE_ASM(const s32, "nme/Nme_Init", nmeBuildNameFont);
#endif

extern u32 dds3KernelDebugSignOrg;
extern u32 dds3KernelDebugSign;
extern void dds3FadeClear(void);

void nmeDestroyName(dds3ProcessID_t *processId) {
    if (nmeProcessId != NULL) {
        nmeBuildNameFont(nmeProcessId->targetNo, nmeProcessId);
        nmeFreeNameTextures(nmeProcessId);
        frDelFontStyle(4);
        sdfMemFreeDirect(nmeProcessId->memHandle);
        nmeProcessId = NULL;
        gProcessStat = 2;
        dds3KernelDebugSign = dds3KernelDebugSignOrg;
        dds3FadeClear();
    }
}

#ifdef NON_MATCHING
/* 69 of 71 words match; white RGB store order differs. See docs/nonmatching/nme/Nme_Init.md. */
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority,
                                      s32 startTimer, s32 shutdownTimer,
                                      dds3ProcessFunc_t process,
                                      dds3ProcessFunc_t shutdown,
                                      void *workdata);
extern s32 nmeCalc(dds3ProcessID_t *processId);
extern s32 nmeDraw(dds3ProcessID_t *processId);
extern s32 nmeUpdate(dds3ProcessID_t *processId);
extern void dds3FadeIn(s32 red, s32 green, s32 blue, s32 frames);

void nmeStartProcesses(s8 target, s8 mode, s32 unused)
{
    if (nmeProcessId != NULL) {
        return;
    }
    nmeProcessId = nmeLoadFont(target, mode, 0);
    dds3StartProcess(D_00551690, 0x406, 1, 1, nmeCalc, NULL, nmeProcessId);
    dds3StartProcess(D_005487C8, 0x2B16, 1, 1, nmeDraw, NULL, nmeProcessId);
    dds3StartProcess(D_005487D8, 0x5212, 1, 1, nmeUpdate,
                     (dds3ProcessFunc_t)nmeDestroyName,
                     nmeProcessId);
    gProcessStat = 1;
    dds3KernelDebugSignOrg = dds3KernelDebugSign;
    dds3KernelDebugSign = 0;
    if (evtBitCheck(0x12) == 0) {
        D_00552020[0] = D_00552020[1] = D_00552020[2] = -1;
    } else {
        D_00552020[0] = 0;
        D_00552020[1] = 0;
        D_00552020[2] = 0;
    }
    dds3FadeIn(D_00552020[0], D_00552020[1], D_00552020[2], 0x1E);
}
#else
INCLUDE_ASM(const s32, "nme/Nme_Init", nmeStartProcesses);
#endif

void nmeNameProcessEnd(void) {
    dds3KillProcessName(D_00551690, 1);
    dds3KillProcessName(D_005487C8, 1);
    dds3KillProcessName(D_005487D8, 1);
}

s32 nmeCheckProcessStat(void) {
    s32 state = gProcessStat;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        gProcessStat = 0;
    }
    return 0;
}

void func_00298678(void)
{
    if (nmeProcessId != NULL) {
        nmeProcessId->sequence.flag |= 0x10;
    }
}

extern s32 scrGetIntPara(s32 index);
extern void scrSetIntReturnValue(s32 value);
extern void nmeStartProcesses(s8 target, s8 mode, s32 unused);

s32 nme_298698(void)
{
    s32 done = 0;

    switch (gProcessStat) {
    case 0: {
        s8 target = scrGetIntPara(0);
        s8 mode = scrGetIntPara(1);
        nmeStartProcesses(target, mode, 0);
        break;
    }
    case 1:
        break;
    case 2:
        gProcessStat = 0;
        done = 1;
        break;
    }
    scrSetIntReturnValue(!done);
    return done;
}

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_00548768);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_00548780);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_00548798);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_005487B0);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_005487C8);

INCLUDE_RODATA(const s32, "nme/Nme_Init", D_005487D8);

INCLUDE_SDATA(const s32, "nme/Nme_Init", nmeProcessId);

INCLUDE_SDATA(const s32, "nme/Nme_Init", gProcessStat);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_0055165D);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551660);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551668);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551670);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551678);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551680);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551688);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551690);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_00551698);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_005516A0);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_005516A8);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_005516B0);

INCLUDE_SDATA(const s32, "nme/Nme_Init", D_005516B8);

INCLUDE_SDATA(const s32, "nme/Nme_Init", mccb);

