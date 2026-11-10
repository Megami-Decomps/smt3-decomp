#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "nme.h"
#include "newdata.h"


extern void sdfTexFree(sdfTexHandle_t *aHandle);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void dds3KillProcessName(const char *name, s32 shutdowntimer);

extern char nmeCalcProcessName[];

extern const char nmeDrawProcessName[];

extern const char nmeUpdateProcessName[];

extern s8 gProcessStat;

u8 nmeCharToIndex(u8 lead, u8 trail) {
    s32 value = (((s32)lead << 8) | trail) - 0x8080;
    s32 index = ((u32)(value & 0xFF00) >> 1) + (value & 0x7F);
    return (u8)(index - 0x4A);
}

extern const u8 nmeAsciiCodeTable[][2];

u32 nmeDecodeChar(s8 code)
{
    s32 index = code - 0x20;
    return ((u32)nmeAsciiCodeTable[index][0] << 8) | nmeAsciiCodeTable[index][1];
}

extern void frDelFontStyle(s32 styleNo);
extern void nmeBuildNameFont(s8 mode, nmeWork_t *work);
extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void frLoadFontData(s32 style, const char *filename);
extern void nmeInitNameRows(s8 target, nmeWork_t *work);
extern const char nmeFontPath[];

extern void *memset(void *a, s32 b, size_t c);

void frAllocNameData(void)
{
    s8 target = 0;
    sdfMemHandle_t *handle = sdfMemAllocDirect(0x51C8);
    nmeWork_t *work = sdfMemLock(handle);

    memset(work, 0, 0x51C8);
    frLoadFontData(4, nmeFontPath);
    for (; target < 4; target++) {
        nmeInitNameRows(target, work);
        nmeBuildNameFont(target, work);
    }
    frDelFontStyle(4);
    sdfMemFreeDirect(handle);
}

extern u8 nmeFadeColor[4];
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);
void nmeStartFadeOut(void)
{
    dds3FadeOut(nmeFadeColor[0], nmeFadeColor[1], nmeFadeColor[2], 0x1E);
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

extern const char *nmeDefaultNames[];

/* NON_MATCHING. Behaviour checked against retail: 3 PCSX2 call vectors give the same return and memory,
 * seq41, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
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
    name = nmeDefaultNames[mode + row];
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

extern void cmpInitCursorLayout(void *cursor, s32 a, s32 b, s32 c, s32 d, s32 mode);
extern void cmpInitCursorRange(void *cursor, u8 count, u8 visible, saddr_t layout);
extern void nmeInitNameCursor(s8 mode, nmeWork_t *work);

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


extern const char *nmeNameListRows __attribute__((section(".data")));
extern u32 strlen(const char *string);
extern void cmpInitCursorRange(void *cursor, u8 count, u8 visible, saddr_t layout);
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
        u32 length = strlen(nmeNameListRows);
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


extern const char nmeTexFile01[];
extern const char nmeTexFile02[];
extern const char nmeTexFile03[];
extern char *nmeCharTexFile[4];
extern char *nmeAltCharTexFile __attribute__((section(".data")));
extern s32 fclLoadTexture(char *filename);
extern s32 evtBitCheck(s32 bit);

void nmeTexLoad(s8 mode, nmeWork_t *work)
{
    work->textures[0] = EE_ADDR_AS(sdfTexHandle_t *, fclLoadTexture(nmeTexFile01));
    work->textures[1] = EE_ADDR_AS(sdfTexHandle_t *, fclLoadTexture(nmeTexFile02));
    work->textures[2] = EE_ADDR_AS(sdfTexHandle_t *, fclLoadTexture(nmeTexFile03));
    if (evtBitCheck(0x8B0) != 0 && mode == 0) {
        work->textures[3] = EE_ADDR_AS(sdfTexHandle_t *, fclLoadTexture(nmeAltCharTexFile));
    } else {
        work->textures[3] = EE_ADDR_AS(sdfTexHandle_t *, fclLoadTexture(nmeCharTexFile[mode]));
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
extern const char nmeFontPath[];

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
    frLoadFontData(4, nmeFontPath);
    nmeTexLoad(target, work);
    return work;
}

/* NON_MATCHING. Behaviour checked against retail: 1 PCSX2 call vector give the same return and memory,
 * seq41, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
#ifdef NON_MATCHING
extern dds3GlobalWork_t *volatile nmeFontGlobalWork FAKEMATCH_ASM_NAME("DDS3_GBWK");

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
        global->nameCode[(s32)target * 2 + row] = count;
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
            nmeFontGlobalWork->nameCode[8 + (s32)target * 0x20 + (s32)glyphCodeIndex * 2] = decoded >> 8;
            nmeFontGlobalWork->nameCode[9 + (s32)target * 0x20 + (s32)glyphCodeIndex * 2] = decoded;
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
                nmeFontGlobalWork->nameCode[0x88 + index * 2] = decoded >> 8;
                nmeFontGlobalWork->nameCode[0x89 + index * 2] = decoded;
                index++;
                count = work->inputRowEnabled[2];
                byteCount = live->inputRowEnabled[2];
            } while (index < count);
            nmeFontGlobalWork->nameCode[0xD8] = byteCount;
        }
    }
}
#else
INCLUDE_ASM(const s32, "nme/Nme_Init", nmeBuildNameFont);
#endif

extern u32 dds3KernelDebugSignOrg;
extern u32 dds3KernelDebugSign;
extern void dds3FadeClear(void);

s32 nmeDestroyName(dds3ProcessID_t *processId) {
    if (nmeProcessId != NULL) {
        nmeBuildNameFont(nmeProcessId->targetNo, nmeProcessId);
        nmeFreeNameTextures(nmeProcessId);
        frDelFontStyle(4);
        sdfMemFreeDirect(nmeProcessId->memHandle);
        nmeProcessId = NULL;
        gProcessStat = 2;
        dds3KernelDebugSign = dds3KernelDebugSignOrg;
        dds3FadeClear();
        /* KillProcess discards the shutdown result. The supported GNU EE bare
         * return keeps the native tail call; host compilers reject it. */
#ifdef NON_MATCHING
        return 0;
#else
        return;
#endif
    }
}

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
    dds3StartProcess(nmeCalcProcessName, 0x406, 1, 1, nmeCalc, NULL, nmeProcessId);
    dds3StartProcess(nmeDrawProcessName, 0x2B16, 1, 1, nmeDraw, NULL, nmeProcessId);
    dds3StartProcess(nmeUpdateProcessName, 0x5212, 1, 1, nmeUpdate,
                     nmeDestroyName,
                     nmeProcessId);
    gProcessStat = 1;
    dds3KernelDebugSignOrg = dds3KernelDebugSign;
    dds3KernelDebugSign = 0;
    {
        s32 event = evtBitCheck(0x12);
        /* Localized EE compiler constraints on the real fade arguments keep
         * the shared RGB base and frame value through their native consumers.
         * They reconstruct allocation/scheduling, not original source spelling. */
        FAKEMATCH_ASM_REG(s32, frames, "$7") = 30;
        FAKEMATCH_ASM_REG(u8 *, color, "$5") = nmeFadeColor;
        FAKEMATCH_ASM_LIVE_PAIR(frames, color);
        if (event == 0) {
            s32 white = -1;
            color[2] = white;
            /* Keep the live white value at the blue/red store boundary. */
            FAKEMATCH_ASM_LIVE(white);
            color[0] = color[1] = white;
        } else {
            color[0] = 0;
            color[1] = 0;
            /* The initialized green byte remains live until the fade reload.
             * This input-only constraint emits no access and claims no write. */
            FAKEMATCH_ASM_READ_MEM(color[1]);
            color[2] = 0;
        }
        dds3FadeIn(color[0], color[1], color[2], frames);
    }
}


void nmeNameProcessEnd(void) {
    dds3KillProcessName(nmeCalcProcessName, 1);
    dds3KillProcessName(nmeDrawProcessName, 1);
    dds3KillProcessName(nmeUpdateProcessName, 1);
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

const char nmeFontPath[24] = "/font/name_ent.fnt";

const char nmeTexFile01[24] = "/name/tmx/name_01.tmx";

const char nmeTexFile02[24] = "/name/tmx/name_02.tmx";

const char nmeTexFile03[24] = "/name/tmx/name_03.tmx";

const char nmeDrawProcessName[16] = "name_draw";

const char nmeUpdateProcessName[16] = "name_update";

const char nmeConfirmQuestion[16] = "Is this okay\?";

nmeWork_t *nmeProcessId = NULL;

s8 gProcessStat = 0;


char nmeDefaultNameChiaki[8] = "Chiaki";

char nmeDefaultNameIsamu[8] = "Isamu";

char nmeDefaultNameNitta[8] = "Nitta";

char nmeDefaultNameYuko[8] = "Yuko";

char nmeDefaultNameTakao[8] = "Takao";

char nmeEmptyName[8] = "";

char nmeCalcProcessName[8] = "name";

s8 nmeNameTextTemplate[8] = { '0', '0' };

char nmeConfirmYes[8] = "Yes";

char nmeConfirmNo[8] = "No";

s8 nmeCursorSkip[8] = { 1, 1 };

s8 nmeMoveTextTemplate[4] = { '0', '0' };

u32 mccb = 0;

