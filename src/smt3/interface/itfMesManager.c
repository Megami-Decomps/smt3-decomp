#include "interface.h"
#include "kernel.h"

typedef struct itfMesTable_t itfMesTable_t;
typedef struct itfMesEntry_t itfMesEntry_t;
typedef struct itfMesResource_t itfMesResource_t;
typedef struct itfMesSpeaker_t itfMesSpeaker_t;
typedef struct itfMesMessage_t itfMesMessage_t;
typedef struct itfMesOption_t itfMesOption_t;
typedef struct itfMesSelect_t itfMesSelect_t;
typedef struct itfMesWindow_t itfMesWindow_t;
typedef struct itfMesVariable_t itfMesVariable_t;
typedef struct itfMesCursor_t itfMesCursor_t;
typedef struct itfMesWork_t itfMesWork_t;
typedef struct itfMesLink_t itfMesLink_t;
typedef struct itfMesLinkControl_t itfMesLinkControl_t;
typedef struct itfMesManager_t itfMesManager_t;
typedef struct itfMesDebug_t itfMesDebug_t;
typedef struct itfPadState_t itfPadState_t;

/* Message tables contain relocated encoded-text addresses. */
struct itfMesTable_t {
    u8 unk0[0x18];
    s16 count;
    s16 bitCount;
    u32 items[1];
};

struct itfMesEntry_t {
    /* Relocated item-address table; address 1 marks a branch entry. */
    u32 *itemList;
    itfMesTable_t *table;
};

struct itfMesResource_t {
    u8 unk0[8];
    u32 magic;
    u8 unkC[4];
    s32 fixupTableOffset;
    s32 fixupTableBytes;
    u32 entryCount;
    u8 relocated;
    u8 unk1D[3];
    itfMesEntry_t entries[1];
};

struct itfMesSpeaker_t {
    u32 x;
    u32 y;
    frFontHandle_t *glyphChain;
    u16 selectedIndex;
    u8 pad0E[2];
};

struct itfMesMessage_t {
    u32 x;
    s32 y;
    itfMesTable_t *table;
    frFontHandle_t *glyphChain;
    s8 textState;
    u8 textInterfaceMask; /* 0x11: result of itfTestTextInterfaceMask(3). */
    u8 color[4];
    s16 lineCount;
    s16 itemIndex;
    s16 tableCount;
};

struct itfMesOption_t {
    s16 id;
    s16 value;
};

struct itfMesSelect_t {
    u32 x;
    u32 y;
    frFontHandle_t *glyphChain;
    u32 panelValue;
    s16 selectionState;
    s16 selectedIndex;
    s16 savedIndex;
    s16 rowCount;
    s16 glyphColorIntensity;
    s16 optionCount;
    itfMesOption_t options[15];
};

/* The speaker frame and message panel occupy +0x9C/+0xA0. */
struct itfMesWindow_t {
    itfPanel_t *frame;
    itfPanel_t *sprite;
    s32 bounds[4];
    s32 red;
    s32 green;
    s32 blue;
    s32 fadeLimit;
};

struct itfMesVariable_t {
    u32 addresses[0x20];
    sdfMemHandle_t *handles[0x20];
};

/* Native 0xC-byte fade record shared by message windows and sound UI state. */
struct itfMesCursor_t {
    u8 kind;
    u8 pad1;
    s16 phase;
    s16 alpha;
    s16 timer;
    u32 frames; /* 0x08: entry advances count frames before triangle skip. */
};

struct itfMesWork_t {
    u32 flags;
    itfMesResource_t *sub;
    u8 *temporaryFontEntry; /* 0x08: relocated font resource bytes from the MSG1 trailing entry. */
    s32 renderValue;
    s16 drawSurfaceIndex;
    s16 panelMode;
    itfMesSpeaker_t speaker;
    itfMesMessage_t message;
    itfMesSelect_t selection;
    u8 unk98[4];
    itfMesWindow_t window;
    itfMesVariable_t variables;
    itfMesCursor_t fade;
    u32 callbackAddress;
};

/* The message manager owns 128 native 0x14-byte link records. */
struct itfMesLink_t {
    struct itfMesLink_t *previous;
    struct itfMesLink_t *next;
    s32 index;
    itfMesWork_t *state;
    sdfMemHandle_t *resourceHandle;
};

struct itfMesLinkControl_t {
    itfMesLink_t *activeHead;
    itfMesLink_t *activeTail;
    itfMesLink_t *firstFree;
    itfMesLink_t *lastFree;
};

struct itfMesManager_t {
    u32 activeWindowCount;
    sdfTexHandle_t *windowTexture;
    u32 unk8;
    u16 flags;
    u16 unkE;
    itfMesLinkControl_t pool;
    itfMesLink_t nodes[0x80];
};

/* State of the interactive message-layout inspector. */
struct itfMesDebug_t {
    s32 selectedItem;
    s32 mode;
    itfMesLink_t *window;
};

/* Named primary/secondary buttons are ports 0/1 of input bank 1. */
struct itfPadState_t {
    u8 pad00[0x20];
    union {
        s8 buttons[0x20];
        struct {
            u8 pad20;
            s8 confirm;
            s8 primaryTriangle;
            s8 primaryCancel;
            s8 alternatePrev; /* +0x24: battle panels use as an alternate previous input. */
            s8 alternateNext; /* +0x25: battle panels use as an alternate next input. */
            s8 prev;
            s8 next;
            u8 pad28[9];
            s8 secondaryCross;
            s8 secondaryTriangle;
            s8 secondaryCancel;
            s8 coarseDown;
            s8 coarseUp;
            s8 secondaryUp;
            s8 secondaryDown;
            s8 fineDown;
            u8 pad39;
            s8 fineUp;
            u8 pad3B[5];
        };
    };
};

typedef struct { itfMesWork_t *mes; u8 unk04[0x10]; } itfMesInstance_t;
typedef void (*itfProcessShutdown_t)(dds3ProcessID_t *task);

s32 itfMesMngMessage(void);

extern u32 scrGetMessageHandle(void);

extern itfMesInstance_t pinst[];

extern s32 scrGetIntPara(s32);

extern s32 itfMesMngRequestMessage(s32 window, s32 entryIndex, s32 itemIndex);

void itfMesMngSetPhase(s32 msgid, s8 phase);

s32 itfMesMngSelect(void);

extern void itfMesMngRequestSelect(s32 id, s32 select);

extern void scrSetIntReturnValue(s32 i);

void itfMesMngRequestSelectRelease(s32 id);

s16 itfMesMngGetSelPhase(s32 msgid);

s16 itfMesMngGetSelectNo(s32 msgid);

s32 itfMesMngMaskSelect(void);

void itfMesMngSetMaskSelect(s32 id, u32 maskpattern);

s32 itfMesMngCloseWindow(void);

#define ITF_MES_LOWER_STATUS_PAIR_MASK 0x300

#define ITF_MES_LOWER_STATUS_FIRST_BIT 0x100

#define ITF_MES_LOWER_STATUS_SECOND_BIT 0x200

void itfMesMngMsgWndCls(s32 msgid);

extern void fldPanelSet(s32 mode);

#define ITF_MES_UPPER_STATUS_PAIR_MASK 0x3000

void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker);

#define ITF_MES_CLEAR_SCRIPT_PANEL_BIT 0xFFDFFFFF

u32 itfMesMngOpenSelectWindow(void);

u32 itfMesMngCloseSelectWindow(void);

s32 itfMesMngSetMesPosDefault(void);

void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);

#define ITF_MES_DEFAULT_TEXT_X 0x4B0

#define ITF_MES_DEFAULT_TEXT_Y 0xA88

s32 itfMesMngInitialize(itfMesResource_t *sub);

extern itfMesLink_t *itfMiscLinkTake(itfMesLinkControl_t *pstack);

extern itfMesManager_t itfMesMngManager;

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern u32 sdfMemLock(sdfMemHandle_t *aHandle);

itfMesResource_t *itfMesMngSetResourcePtr(s32 id, itfMesResource_t *presource);

void InitializeSpeaker(itfMesSpeaker_t *pspeaker, s32 defaultpos);

void InitializeMessage(itfMesMessage_t *pmessage, s32 defaultpos);

extern void InitializeSelect(struct itfMesSelect_t *pselect);

void InitializeWindow(itfMesWindow_t *pwindow);

void InitializeVariable(itfMesVariable_t *pvariable);

void InitializeCurCtr(itfMesCursor_t *pcurctr, s32 isdisp);

void itfMesMngRelease(s32 id);

void DestructFlow(s32 id);

s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);

#define ITF_MES_ENTRY_STATE_MASK 7

#define ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK 0xFFFDFFFF

#define ITF_MES_OPTION_STATE_MASK 0x38

#define ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK 0xFFFBFFFF

void itfMesMngSetSpeakerPosition(s32 id, s32 x, s32 y);

void ChangeFontPosition(frFontHandle_t *pfont, s32 dist_x, s32 dist_y);

void itfMesMngSetSpeakerPositionOffset(s32 id, s32 x, s32 y);

void itfMesMngSetMesPositionOffset(s32 id, s32 x, s32 y);

void itfMesMngSetSelPosition(s32 id, s32 x, s32 y);

void itfMesMngSetSelPositionOffset(s32 id, s32 x, s32 y);

void itfMesMngSetPriority(s32 id, s32 z_buf);

void ChangeZBuffer(frFontHandle_t *pfont, s32 z);

void itfMesMngSetStat(s32 id, u32 flag);

#define ITF_MES_HIGH_FLAGS_MASK 0xffff0000

s16 itfMesMngGetSelect(s32 id);

s16 itfMesMngGetMesPage(s32 id);

s16 itfMesMngGetMesCurrentPage(s32 id);

u8 *itfMesMngGetVar(s32 id, s32 index);

void itfMesMngSetVarImmediate(s32 id, u32 index, const char *pstr);

void SetVar(itfMesWork_t *pinst, s32 index, const char *pstr, s32 length);

void itfMesMngSetVarImmediate2(s32 id, s32 index, const char *pstr, s32 length);

void ResolveResourceAddress(itfMesResource_t *pdata);

itfMesEntry_t *GetTypeHeader2(itfMesResource_t *pfileheader);

#define ITF_MES_MAGIC_MSG1 0x3147534d

u8 *itfMesMngGetDataPtrImmediate(s32 id, s32 message, s32 page);

u8 *GetMessagePagePtr(itfMesTable_t *table, s32 index);

itfMesEntry_t *GetTypeHeader(itfMesWork_t *pinst, s32 index);

void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha);

#define ITF_MES_CONTENT_STATUS_MASK 0x3300

extern void ResetWindowPara(itfMesWindow_t *pwin, s32 type, s32 alpha);

void itfPnlRelease(itfPanel_t *handle);

s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index);

s32 itfMesMngGetWidth(s32 id, s32 message, s32 page);

void CommitVarTable(s32 *pvar);

void frSetExtDic(u8 *pExtFontData);

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);

s32 GetMessageWidth(frFontHandle_t *pfont);

void itfMesMngSetDefaultSpeed(s32 id, u8 speed);

void itfMesMngSetKutiCallback(s32 id, void (*func)(void));

void itfMesMngInitializeManager(void);

extern sdfTexHandle_t *itfMiscLoadTexture(const char *filename);

extern void itfMiscLinkInitialize(itfMesLinkControl_t *pool, itfMesLink_t *nodes, s32 count, s32 stride);

#define ITF_MES_WINDOW_CAPACITY 0x80

#define ITF_MES_POOL_NODE_BYTES 0x14

extern dds3ProcessID_t *dds3StartProcess(const char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                         s32 v3, dds3ProcessFunc_t drawExit,
                         itfProcessShutdown_t deleteExit, u32 p);

s32 CalcMsgMng_Process(void);

void DrawMsgMng_Shutdown(void);

s32 DrawMsgMng_Process(dds3ProcessID_t *id);

void itfMesMngSetBitStatManager(u32 flags);

void itfMesMngClearBitStatManager(u32 flags);

u16 itfMesMngGetStatManager(void);

s32 itfMesMngDebugProc(void);

extern itfMesDebug_t D_00326700;

itfMesLink_t *DebugChangeInstance(itfMesLink_t *pflow);

extern itfPadState_t dds3PadButtonState;

void DebugMoveWinVert(itfMesWindow_t *pwin, s32 flag);

void DebugMoveWinAlpha(itfMesWindow_t *pwin);

sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

extern void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y,
                        s32 sx, s32 sy);

extern char D_0054F4E8[];

extern char *D_0054F4D0[2];

extern char D_0054F4F0[];

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);

extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr,
                                  const char *aStr, ...);

extern char *D_00326710[];

extern char D_0054F4F8[];

extern sdfGrOt_t sdfGrOtTable[];

void itfMesMngDebugShutdown(void);

/* Slot record reached through the pool base, one 0x14 bytes per window. */
extern itfMesLink_t D_0056BE80[];

extern void ReleaseAllWindow(struct itfMesWork_t *pinst);

void ReleaseAllVariable(itfMesVariable_t *pvariable);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void itfMiscLinkGive(itfMesLink_t *pelem, itfMesLinkControl_t *pstack);

u32 CheckMessageBinary(itfMesResource_t *pdata);

#define ITF_MES_MAGIC_MSG0 0x3047534d

extern u32 strlen(const char *text);

#define ITF_MES_STRING_COPY_ROUND_BIAS 4

#define ITF_MES_COPY_ALIGN_MASK 3

extern void *memcpy(void *a, const void *b, u32 c);

#define ITF_MES_BINARY_COPY_ROUND_BIAS 5

extern void *memset(void *a, s32 b, u32 c);

frFontHandle_t *UnchainFontHandleUnit(frFontHandle_t *pfont, s32 index, s32 lines);

void ChangeFontAlpha(frFontHandle_t *pfont, u32 alpha);

#define ITF_MES_COLOR_BYTE_CLEAR_MASK 0xffffff00

void OpenCheckSpeakerWindow(itfMesWork_t *pinst);

void OpenMessageWindow(itfMesWork_t *pinst);

void ReadytoMessage(itfMesWork_t *panel);

extern itfPanel_t *itfPnlRegist2(s32 type, u32 texhandle);

void itfPnlSetPosition(itfPanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);

void itfPnlSetColor(itfPanel_t *handle, s32 r, s32 g, s32 b, s32 a);

void CalcMesFlow(itfMesWork_t *pinst);

extern void CalcWindow(itfMesWork_t *pinst);

extern void CalcMessage(itfMesWork_t *pinst);

extern void CalcSelect(itfMesWork_t *);

void CalcCurCtr(itfMesWork_t *pinst);

s32 SelectItem(itfMesWork_t *pinst);

void SelectItemMove(itfMesSelect_t *psel, s32 direct);

extern void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

s32 SelectItemDefKey(itfMesSelect_t *psel);

void ChangeFontColor(frFontHandle_t *pfont, s32 index, s32 lines, s32 color);

extern u32 dds3NullDrawProc(void);

void DrawMesFlow(itfMesWork_t *pinst);

void DrawWindow(itfMesWork_t *pinst);

void DrawRetPageCursor(itfMesWork_t *pinst);

void DrawSelectCursor(itfMesWork_t *pinst);

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

void DebugDrawWindowLine(itfPanel_t *meswin);

extern s32 sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void itfMiscDrawLineStripPrim(void *, void *, u8 *, u8 *, s32, u32, sdfDrawTag_t *);

extern s32 D_00326728[8];


extern u8 D_00326768[];

extern u8 D_00326770[];

extern void itfMiscDrawSpritePlanePrim(void *vertices, void *colors, s32 tail, s32 flag, sdfDrawTag_t *command);

extern void itfMiscDrawRectanglePrim(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, s32 tail, sdfDrawTag_t *command);

extern void itfMiscDrawSpritePrim2(void *vertices, void *uv, void *colors, s32 tail, s32 flag, sdfDrawTag_t *command);

extern s32 D_00326778[4];

extern s32 D_00326788[4];

extern void itfMiscDrawSpritePrim(void *vertexData, void *uvData, void *colorData, s32 tail, sdfTexHandle_t *texture, s32 flag, sdfDrawTag_t *command);

extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *command, s32 index, s32 flag);

extern s32 D_00326798[];

extern u8 D_0054F500[5];

extern u8 D_0054F508[5];

void itfPnlSetPositionOffset(itfPanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);

void sdfAddressResolver(s32 *aDataBody, s32 aOffset, u8 *aAddrInfoPtr, s32 aAddrInfoLeng);
void frSetVariableRequest(s32 VarNo, s32 pStr);
void itfMesMngSetSelect(s32 id, s32 selno);
void itfMesMngMarkBitStat(s32 id, u32 flag);
void itfMesMngClearBitStat(s32 id, u32 flag);
void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type);
/* Poll/start an entry: one ends the command, zero keeps waiting.
   A negative signed status is consumed by clearing it; positive status keeps waiting. */
s32 itfMesMngMessage(void) {
    s32 window = scrGetMessageHandle();
    itfMesWork_t *record;
    itfMesMessage_t *entryState;
    s32 entryIndex;
    s8 status;

    if (window < 0) {
        return 1;
    }
    record = pinst[window].mes;
    entryIndex = scrGetIntPara(0);
    entryState = &record->message;
    status = entryState->textState;
    if (status == 0) {
        if (itfMesMngRequestMessage(window, entryIndex, 0) == 0) {
            return 1;
        }
    } else if (status < 0) {
        entryState->textState = 0;
        return 1;
    }
    return 0;
}

void itfMesMngSetOffsetY1(s32 msgid, s32 y1)
{
    itfPnlSetPositionOffset(pinst[msgid].mes->window.sprite, 0, y1, 0, 0, 0);
}

s8 itfMesMngGetPhase(s32 msgid)
{
    return pinst[msgid].mes->message.textState;
}

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 msgid, s8 phase) {
    pinst[msgid].mes->message.textState = phase;
}

s32 itfMesMngSelect(void) {
    s32 window;
    s32 entry;
    itfMesWork_t *record;
    itfMesSelect_t *options;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    record = pinst[window].mes;
    entry = scrGetIntPara(0);
    options = &record->selection;
    if (options->selectionState == 0) {
        itfMesMngRequestSelect(window, entry);
    } else if (options->selectionState < 0) {
        options->selectionState = 0;
        scrSetIntReturnValue(options->selectedIndex);
        itfMesMngRequestSelectRelease(window);
        return 1;
    }
    return 0;
}

/* Return the option-block signed status word, not the separate entry status byte. */
s16 itfMesMngGetSelPhase(s32 msgid) {
    return pinst[msgid].mes->selection.selectionState;
}

void itfMesMngSetSelPhase(s32 msgid, s16 phase)
{
    pinst[msgid].mes->selection.selectionState = phase;
}

/* Return the same signed word forwarded as the script result when an option completes. */
s16 itfMesMngGetSelectNo(s32 msgid) {
    return pinst[msgid].mes->selection.selectedIndex;
}

/* Store the script's panel mask; a negative script window is a no-op. Return 1. */
s32 itfMesMngMaskSelect(void) {
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngSetMaskSelect(window, scrGetIntPara(0));
    return 1;
}

s32 itfMesMng_SEL_SELNO(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngSetSelect(window, scrGetIntPara(0));
    return 1;
}

s32 itfMesMngOpenWindow(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngMarkBitStat(window, 0x200000);
    fldPanelSet(2);
    return 1;
}

/* Finish single-bit states of the lower status pair; return whether its original
 * value was zero. Finishing a nonzero state therefore still returns 0. */
s32 itfMesMngCloseWindow(void) {
    s32 window = scrGetMessageHandle();
    itfMesWork_t *mes;
    u32 scriptStatus;

    if (window < 0) {
        return 1;
    }
    mes = pinst[window].mes;
    scriptStatus = mes->flags & ITF_MES_LOWER_STATUS_PAIR_MASK;
    if (scriptStatus == ITF_MES_LOWER_STATUS_FIRST_BIT || scriptStatus == ITF_MES_LOWER_STATUS_SECOND_BIT) {
        itfMesMngMsgWndCls(window);
        fldPanelSet(3);
    }
    return scriptStatus < 1;
}

/* Mark each originally nonzero status pair with both bits, then clean/reset
 * content and clear the script panel bit. The status pairs are set, not cleared. */
void itfMesMngMsgWndCls(s32 msgid) {
    itfMesWork_t *mes = pinst[msgid].mes;
    u32 previousFlags = mes->flags;

    if (previousFlags & ITF_MES_LOWER_STATUS_PAIR_MASK) {
        mes->flags = previousFlags | ITF_MES_LOWER_STATUS_PAIR_MASK;
    }
    if (previousFlags & ITF_MES_UPPER_STATUS_PAIR_MASK) {
        mes->flags = mes->flags | ITF_MES_UPPER_STATUS_PAIR_MASK;
    }
    itfMesMngRequestMessageRelease(msgid, 1);
    itfMesMngRequestSelectRelease(msgid);
    mes->flags &= ITF_MES_CLEAR_SCRIPT_PANEL_BIT;
}

u32 itfMesMngOpenSelectWindow(void) {
    return 1;
}

u32 itfMesMngCloseSelectWindow(void) {
    return 1;
}

s32 itfMesMngSetVarScr(void)
{
    s32 window = scrGetMessageHandle();
    s32 slot;
    s32 value;
    s32 selector;

    if (window < 0) {
        return 1;
    }
    slot = scrGetIntPara(0);
    value = scrGetIntPara(1);
    selector = scrGetIntPara(2);
    itfMesMngSetVar(window, slot, value, selector);
    return 1;
}

s32 itfMesMngIgnoreConfig(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    if (scrGetIntPara(0)) {
        itfMesMngMarkBitStat(window, 0x400000);
    } else {
        itfMesMngClearBitStat(window, 0x400000);
    }
    return 1;
}

s32 itfMesMngSetMesPositionScript(void)
{
    s32 window = scrGetMessageHandle();
    s32 x;
    s32 y;

    if (window < 0) {
        return 1;
    }
    x = scrGetIntPara(0);
    y = scrGetIntPara(1);
    itfMesMngSetMesPosition(window, x << 4, y << 3);
    return 1;
}

/* Move selected-entry text to the fixed default position; return 1. */
s32 itfMesMngSetMesPosDefault(void) {
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngSetMesPosition(window, ITF_MES_DEFAULT_TEXT_X, ITF_MES_DEFAULT_TEXT_Y);
    return 1;
}

s32 itfMesMngValidateNext(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    if (scrGetIntPara(0)) {
        itfMesMngClearBitStat(window, 0x800000);
        itfMesMngClearBitStat(window, 0x100000);
    } else {
        itfMesMngMarkBitStat(window, 0x800000);
        itfMesMngMarkBitStat(window, 0x100000);
    }
    return 1;
}

s32 itfMesMngWindowType(void)
{
    s32 window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngChangeWindowType(window, scrGetIntPara(0), 0);
    return 1;
}

s32 itfMesMngWindowTypeAlpha(void)
{
    s32 window = scrGetMessageHandle();
    s32 type;
    s32 alpha;

    if (window < 0) {
        return 1;
    }
    type = scrGetIntPara(0);
    alpha = scrGetIntPara(1);
    itfMesMngChangeWindowType(window, type, alpha);
    return 1;
}

s32 itfMesMngSelectDefKey(void)
{
    s32 window = scrGetMessageHandle();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = scrGetIntPara(0);
    second = scrGetIntPara(1);
    itfMesMngSetSelectDefKey(window, first, second);
    return 1;
}

s32 itfMesMngInitialize(itfMesResource_t *sub) {
    itfMesLink_t *node = itfMiscLinkTake(&itfMesMngManager.pool);
    s32 window = node->index;
    itfMesWork_t *mes;
    sdfMemHandle_t *handle;

    handle = sdfMemAllocDirect(sizeof(itfMesWork_t));
    node->resourceHandle = handle;
    mes = (itfMesWork_t *)sdfMemLock(handle);
    node->state = mes;
    mes->sub = NULL;
    itfMesMngSetResourcePtr(window, sub);
    mes->flags = 0;
    mes->drawSurfaceIndex = 0x47;
    mes->renderValue = 0xFFFFF0;
    mes->panelMode = 0;
    mes->callbackAddress = 0;
    InitializeSpeaker(&mes->speaker, 1);
    InitializeMessage(&mes->message, 1);
    InitializeSelect(&mes->selection);
    InitializeWindow(&mes->window);
    InitializeVariable(&mes->variables);
    InitializeCurCtr(&mes->fade, 0);
    itfMesMngManager.activeWindowCount++;
    return window;
}

/* Destroy an installed nonnegative window slot; no upper-bound validation. */
void itfMesMngRelease(s32 id) {
    if (id >= 0 && pinst[id].mes != NULL) {
        DestructFlow(id);
    }
}


/* Called above its definition, where ee-gcc assumed int. */
u8 *GetSpeakerName(itfMesWork_t *pinst);

s32 itfMesMngRequestMessage(s32 window, s32 entryIndex, s32 itemIndex) {
    itfMesWork_t *panel = pinst[window].mes;
    itfMesMessage_t *mes = &panel->message;
    itfMesSpeaker_t *spk;
    itfMesTable_t *table;
    u32 flags;
    u32 mode;

    table = GetTypeHeader(panel, entryIndex)->table;
    mes->itemIndex = itemIndex;
    mes->table = table;
    mes->tableCount = table->count;
    if (table->count == 0) {
        printf("itfMesMngRequestMessage() escape 1\n");
        return 0;
    }
    CommitVarTable((s32 *)panel->variables.addresses);
    if (panel->temporaryFontEntry != NULL) {
        frSetExtDic(panel->temporaryFontEntry);
    }
    ReadytoMessage(panel);
    spk = &panel->speaker;
    if (spk->selectedIndex != (u16)table->bitCount) {
        u16 nameId = table->bitCount;

        if (spk->glyphChain != NULL) {
            frDestroyFontRequestAfter(spk->glyphChain);
            spk->glyphChain = NULL;
        }
        spk->selectedIndex = nameId;
        if (nameId != 0xFFFF) {
            if (panel->window.frame != NULL) {
                itfPnlRelease(panel->window.frame);
                panel->window.frame = NULL;
            }
            if (nameId & 0x8000) {
                spk->glyphChain = MessageRequestEx(spk->x, spk->y, 0, 3, 0, 8, itfMesMngGetVar(window, nameId & 0x7FFF), NULL);
            } else {
                spk->glyphChain = MessageRequestEx(spk->x, spk->y, 0, 3, 0, 8, GetSpeakerName(panel), NULL);
            }
            frSetDrawPosition(spk->glyphChain, spk->x, spk->y);
            frSetDrawSpeed(spk->glyphChain, 0xA);
            ChangeZBuffer(spk->glyphChain, panel->renderValue);
        }
    }
    OpenCheckSpeakerWindow(panel);
    flags = panel->flags;
    if (flags & 0x200000) {
        mode = flags & 0x300;
        if (mode == 0 || mode == 0x300) {
            OpenMessageWindow(panel);
            panel->flags = (panel->flags & ~7) | 2;
        } else {
            panel->flags = (flags & ~7) | 3;
        }
    } else {
        panel->flags = (flags & ~7) | 3;
    }
    mes->textState = 1;
    return 1;
}

/* Release/reset selected-entry glyphs and their status; optionally do the same
 * for primary text. Negative window is a no-op; no other slot validation. */
void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker) {
    itfMesWork_t *mes;
    itfMesMessage_t *message;
    itfMesSpeaker_t *primaryTextBlock;

    if (id < 0) {
        return;
    }
    mes = pinst[id].mes;
    message = &mes->message;
    primaryTextBlock = &mes->speaker;
    if (message->glyphChain != NULL) {
        frDestroyFontRequestAfter(message->glyphChain);
        message->glyphChain = NULL;
    }
    InitializeMessage(message, 0);
    mes->flags &= ~ITF_MES_ENTRY_STATE_MASK;
    mes->flags &= ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK;
    if (cls_speaker == 0) {
        return;
    }
    if (primaryTextBlock->glyphChain != NULL) {
        frDestroyFontRequestAfter(primaryTextBlock->glyphChain);
        primaryTextBlock->glyphChain = NULL;
    }
    InitializeSpeaker(primaryTextBlock, 0);
}

extern s32 printf(const char *format, ...);


/* Called above its definition, where ee-gcc assumed int. */
frFontHandle_t *GetFontHandleUnit(u32 *list, s32 count, u32 mask, s32 x, s32 y, s32 depth);

void itfMesMngRequestSelect(s32 id, s32 select) {
    itfMesWork_t *panel = pinst[id].mes;
    itfMesSelect_t *sel = &panel->selection;
    itfMesEntry_t *entry;
    itfMesTable_t *table;
    s32 count;
    s32 y;
    frFontHandle_t *chain;

    entry = GetTypeHeader(panel, select);
    if (entry->itemList != (u32 *)1) {
        printf("itfMesMngRequestSelect() - Invalid Type : %d\n", entry->itemList);
    }
    if (sel->glyphChain != NULL) {
        frDestroyFontRequestAfter(sel->glyphChain);
        sel->glyphChain = NULL;
    }
    table = entry->table;
    count = GetNumSelectItem(table->bitCount, sel->panelValue);
    y = sel->y - count * 160;
    y += 0xB4;
    CommitVarTable((s32 *)panel->variables.addresses);
    if (panel->temporaryFontEntry != NULL) {
        frSetExtDic(panel->temporaryFontEntry);
    }
    chain = GetFontHandleUnit(&table->items[1], table->bitCount, sel->panelValue, sel->x, y, panel->renderValue);
    sel->selectionState = 1;
    sel->rowCount = count;
    sel->glyphChain = chain;
    panel->flags = (panel->flags & ~0x38) | 0x10;
}

/* Release option glyphs, clear option selection/state and invalidate its rank.
 * Other content and the window allocation remain installed. */
void itfMesMngRequestSelectRelease(s32 id) {
    itfMesWork_t *mes = pinst[id].mes;
    itfMesSelect_t *optionBlock = &mes->selection;

    if (optionBlock->glyphChain != NULL) {
        frDestroyFontRequestAfter(optionBlock->glyphChain);
        optionBlock->glyphChain = NULL;
    }
    optionBlock->panelValue = 0;
    optionBlock->selectionState = 0;
    optionBlock->rowCount = 0;
    optionBlock->selectedIndex = -1;
    mes->flags &= ~ITF_MES_OPTION_STATE_MASK;
    mes->flags &= ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK;
}

void itfMesMngSetMaskSelect(s32 id, u32 maskpattern)
{
    pinst[id].mes->selection.panelValue = maskpattern;
}

void itfMesMngSetSelect(s32 id, s32 selno)
{
    itfMesSelect_t *blk = &pinst[id].mes->selection;
    u32 mask = blk->panelValue;
    s32 bit = mask & 1;
    s32 n = bit == 0;
    s32 i = 0;

    if (selno > 0) {
        do {
            i++;
            mask >>= 1;
            bit = mask & 1;
            if (bit == 0) {
                n++;
            }
        } while (i < selno);
    }
    n = bit == 0 ? n - 1 : 0;
    blk->selectedIndex = n;
    blk->savedIndex = n;
}

/* Move primary text to an absolute position; an unchanged position is a no-op. */
void itfMesMngSetSpeakerPosition(s32 id, s32 x, s32 y) {
    itfMesSpeaker_t *primaryTextBlock = &pinst[id].mes->speaker;
    s32 delta[2];

    delta[0] = x - primaryTextBlock->x;
    delta[1] = y - primaryTextBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(primaryTextBlock->glyphChain, delta[0], delta[1]);
    primaryTextBlock->x = x;
    primaryTextBlock->y = y;
}

/* Translate primary text and update its cached position, including zero deltas. */
void itfMesMngSetSpeakerPositionOffset(s32 id, s32 x, s32 y) {
    itfMesSpeaker_t *primaryTextBlock = &pinst[id].mes->speaker;

    ChangeFontPosition(primaryTextBlock->glyphChain, x, y);
    primaryTextBlock->x += x;
    primaryTextBlock->y += y;
}

/* Move selected-entry text absolutely; an unchanged position is a no-op. */
void itfMesMngSetMesPosition(s32 id, s32 x, s32 y) {
    itfMesMessage_t *message = &pinst[id].mes->message;
    s32 delta[2];

    delta[0] = x - message->x;
    delta[1] = y - message->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(message->glyphChain, delta[0], delta[1]);
    message->x = x;
    message->y = y;
}

/* Translate selected-entry text and update its cached position. */
void itfMesMngSetMesPositionOffset(s32 id, s32 x, s32 y) {
    itfMesMessage_t *message = &pinst[id].mes->message;

    ChangeFontPosition(message->glyphChain, x, y);
    message->x += x;
    message->y += y;
}

/* Move option text absolutely; an unchanged position is a no-op. */
void itfMesMngSetSelPosition(s32 id, s32 x, s32 y) {
    itfMesSelect_t *optionBlock = &pinst[id].mes->selection;
    s32 delta[2];

    delta[0] = x - optionBlock->x;
    delta[1] = y - optionBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(optionBlock->glyphChain, delta[0], delta[1]);
    optionBlock->x = x;
    optionBlock->y = y;
}

/* Translate option text and update its cached position. */
void itfMesMngSetSelPositionOffset(s32 id, s32 x, s32 y) {
    itfMesSelect_t *optionBlock = &pinst[id].mes->selection;

    ChangeFontPosition(optionBlock->glyphChain, x, y);
    optionBlock->x += x;
    optionBlock->y += y;
}

/* Propagate a changed render value to all three glyph chains before caching it. */
void itfMesMngSetPriority(s32 id, s32 z_buf) {
    itfMesWork_t *mes = pinst[id].mes;

    if (mes->renderValue == z_buf) {
        return;
    }
    ChangeZBuffer(mes->speaker.glyphChain, z_buf);
    ChangeZBuffer(mes->message.glyphChain, z_buf);
    ChangeZBuffer(mes->selection.glyphChain, z_buf);
    mes->renderValue = z_buf;
}

u32 itfMesMngGetStat(s32 id)
{
    return pinst[id].mes->flags;
}

/* Replace only the high half, preserving the existing low-half cast. */
void itfMesMngSetStat(s32 id, u32 flag) {
    itfMesWork_t *mes = pinst[id].mes;

    mes->flags = (u32)(u16)mes->flags | (flag & ITF_MES_HIGH_FLAGS_MASK);
}

void itfMesMngMarkBitStat(s32 id, u32 flag)
{
    itfMesWork_t *rec = pinst[id].mes;

    rec->flags |= flag & 0xFFFF0000;
}

void itfMesMngClearBitStat(s32 id, u32 flag)
{
    itfMesWork_t *rec = pinst[id].mes;

    rec->flags &= ~flag | 0xFFFF;
}

/* Return the cached clear-bit rank; reset state uses -1. */
s16 itfMesMngGetSelect(s32 id) {
    return pinst[id].mes->selection.selectedIndex;
}

/* Return the selected table's cached item count, without another table lookup. */
s16 itfMesMngGetMesPage(s32 id) {
    return pinst[id].mes->message.tableCount;
}

/* Return the stored signed-halfword item index, without another table lookup. */
s16 itfMesMngGetMesCurrentPage(s32 id) {
    return pinst[id].mes->message.itemIndex;
}

/* Return the replacement text stored in a window slot. */
u8 *itfMesMngGetVar(s32 id, s32 index) {
    return (u8 *)pinst[id].mes->variables.addresses[index];
}

extern u8 *DDS3_GBWK;

/* NON_MATCHING: 1 of 34 words, 136 bytes against 172; gcc shares the three address inductions retail keeps apart. */
#ifdef NON_MATCHING
void itfMesMngGetVarLocalize(s32 id)
{
    char *out = (char *)((u8 *)pinst + 0x9F4);
    s32 lead = id * 20 + 0x6600;
    s32 tail = id * 20 + 0x6601;
    s32 count = id + 0x6650;
    s32 k = 0;

    if (((u8 *)DDS3_GBWK + 4)[count] != 0) {
        do {
            u8 code = ((u8 *)DDS3_GBWK + 4)[lead];

            if (code & 0x80) {
                *out = code;
                out++;
                *out = ((u8 *)DDS3_GBWK + 4)[tail];
            } else {
                *out = ((u8 *)DDS3_GBWK + 4)[tail];
            }
            out++;
            lead += 2;
            tail += 2;
            k++;
        } while (k < ((u8 *)DDS3_GBWK + 4)[count]);
    }
    *out = 0;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetVarLocalize);
#endif




extern s32 sprintf(char *buffer, const char *format, ...);

extern char D_0054F4C0[];

extern char *text1[];

extern char *text2[];

extern char *pStr[];

extern char *pstr[];

extern char *datExplainTable[];

extern char D_00311450[];

extern char D_00311850[];

void itfMesMngGetVarLocalize(s32 id);

/* Called above its definition, where ee-gcc assumed int. */
char *ConvertZenkakuNumber(s32 buf, char *num);

void itfMesMngSetVar(s32 id, s32 index, s32 num, s32 type) {
    switch (type) {
    case 0: {
        char buf[16];

        sprintf(buf, D_0054F4C0, num);
        itfMesMngSetVarImmediate(id, index, buf);
        break;
    }
    case 1:
        itfMesMngSetVarImmediate(id, index, text2[num]);
        break;
    case 8:
        itfMesMngSetVarImmediate(id, index, datExplainTable[num]);
        break;
    case 3:
        itfMesMngSetVarImmediate(id, index, pStr[num]);
        break;
    case 4: {
        char text[0x40];
        char *p;

        p = text;
        if (num >= 0x3E9) {
            *p = 0x52;
            text[1] = 0;
        } else {
            if (num < 0) {
                num = -num;
                *p = 0x42;
                p = &text[1];
            }
            p = ConvertZenkakuNumber(num, p);
            p[0] = 0x46;
            p[1] = 0;
        }
        itfMesMngSetVarImmediate(id, index, text);
        break;
    }
    case 5:
        itfMesMngSetVarImmediate(id, index, text1[num]);
        break;
    case 6:
        itfMesMngSetVarImmediate(id, index, &D_00311450[num * 32]);
        break;
    case 7:
        itfMesMngSetVarImmediate(id, index, &D_00311850[num * 32]);
        break;
    case 9:
        itfMesMngSetVarImmediate(id, index, pstr[num]);
        break;
    case 10: {
        char *buffer = (char *)((u8 *)pinst + 0x9F4);

        itfMesMngGetVarLocalize(num);
        itfMesMngSetVarImmediate2(id, index, buffer, strlen(buffer));
        break;
    }
    }
}

/* Replace a text slot with a copied NUL-terminated encoded string. */
void itfMesMngSetVarImmediate(s32 id, u32 index, const char *pstr) {
    SetVar(pinst[id].mes, index, pstr, 0);
}

/* Copy text into one of the window's message text slots. */
void itfMesMngSetVarImmediate2(s32 id, s32 index, const char *pstr, s32 length) {
    SetVar(pinst[id].mes, index, pstr, length);
}

/* Install/relocate a message resource and cache MSG1's trailing-entry word as
 * the temporary font entry. Return the previous resource without releasing it. */
itfMesResource_t *itfMesMngSetResourcePtr(s32 id, itfMesResource_t *presource) {
    itfMesWork_t *mes = pinst[id].mes;
    itfMesResource_t *previous = mes->sub;
    itfMesEntry_t *entry;
    u8 *temporaryFontEntry;

    mes->sub = presource;
    ResolveResourceAddress((itfMesResource_t *)presource);
    entry = GetTypeHeader2(presource);
    temporaryFontEntry = 0;
    if (presource->magic == ITF_MES_MAGIC_MSG1) {
        /* itfMesEntry_t is an invented type and the IDB has no counterpart,
         * so +8 past its two known words has no name to take. */
        temporaryFontEntry = *(u8 **)((u8 *)entry + 8);
    }
    mes->temporaryFontEntry = temporaryFontEntry;
    return previous;
}

/* Look up an entry-table item; only the item index has a bounds check. */
u8 *itfMesMngGetDataPtrImmediate(s32 id, s32 message, s32 page) {
    return GetMessagePagePtr(GetTypeHeader(pinst[id].mes, message)->table, page);
}

/* Refresh changed panel values only when both content-status pairs are clear.
 * Release the old primitive after applying the panel mode and fade limit. */
void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha) {
    itfMesWork_t *mes = pinst[id].mes;
    itfMesWindow_t *panelBlock = &mes->window;

    if ((mes->flags & ITF_MES_UPPER_STATUS_PAIR_MASK) == 0) {
        if (mes->panelMode != type || panelBlock->fadeLimit != alpha) {
            mes->panelMode = type;
            ResetWindowPara(panelBlock, type, alpha);
            if (panelBlock->sprite != NULL) {
                itfPnlRelease(panelBlock->sprite);
                panelBlock->sprite = NULL;
            }
        }
    }
}

/* Set or append the option with `id` in the window's option list; ids 1, 6, 7 and >= 16 are rejected. */
s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index) {
    itfMesSelect_t *optionBlock = &pinst[id].mes->selection;
    itfMesOption_t *option = NULL;
    s32 optionIndex;

    if ((u32)key >= 0x10) {
        return 0;
    }
    if (key == 1 || key == 6 || key == 7) {
        return 0;
    }
    for (optionIndex = 0; optionIndex < optionBlock->optionCount; optionIndex++) {
        if (optionBlock->options[optionIndex].id == key) {
            option = &optionBlock->options[optionIndex];
            break;
        }
    }
    if (option == NULL) {
        if (optionBlock->optionCount >= 15) {
            return 0;
        }
        option = &optionBlock->options[optionBlock->optionCount];
        optionBlock->optionCount++;
    }
    option->id = key;
    option->value = index;
    return 1;
}

/* Build and release a temporary glyph chain to measure the widest grouped row.
 * Empty tables and zero/missing encoded items return zero. */
s32 itfMesMngGetWidth(s32 id, s32 message, s32 page) {
    itfMesWork_t *mes = pinst[id].mes;
    itfMesTable_t *table;
    u8 *encodedText;
    frFontHandle_t *glyphChain;
    s32 textExtent;

    table = GetTypeHeader(mes, message)->table;
    if (table->count == 0) {
        return 0;
    }
    CommitVarTable((s32 *)mes->variables.addresses);
    if (mes->temporaryFontEntry != 0) {
        frSetExtDic(mes->temporaryFontEntry);
    }
    encodedText = GetMessagePagePtr(table, page);
    if (encodedText == 0) {
        return 0;
    }
    glyphChain = MessageRequest(0, 0, encodedText, 0);
    textExtent = GetMessageWidth(glyphChain);
    frDestroyFontRequestAfter(glyphChain);
    return textExtent;
}

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesMngSetDefaultSpeed(s32 id, u8 speed) {
    pinst[id].mes->message.color[3] = speed;
}

/* Store the optional callback address invoked without arguments during glyph building. */
void itfMesMngSetKutiCallback(s32 id, void (*func)(void)) {
    pinst[id].mes->callbackAddress = (u32)func;
}

/* Initialize the fixed window pool, load its texture and register the named
 * calculation/drawing tasks. Keep the existing task flags and callback casts. */
void itfMesMngInitializeManager(void) {
    itfMesLink_t *poolNode;

    itfMesMngManager.activeWindowCount = 0;
    itfMesMngManager.unk8 = 0;
    itfMesMngManager.flags = 0;
    itfMesMngManager.windowTexture = itfMiscLoadTexture("/itf/MESWIN.TMX");
    itfMiscLinkInitialize(&itfMesMngManager.pool, itfMesMngManager.nodes, ITF_MES_WINDOW_CAPACITY, ITF_MES_POOL_NODE_BYTES);
    for (poolNode = itfMesMngManager.pool.firstFree; poolNode != NULL; poolNode = poolNode->next) {
        poolNode->state = NULL;
    }
    dds3StartProcess("CalcMsgMng", 0x409, 0, 0, (dds3ProcessFunc_t)CalcMsgMng_Process, (itfProcessShutdown_t)DrawMsgMng_Shutdown, 0);
    dds3StartProcess("DrawMsgMng", 0x2B1A, 0, 0, DrawMsgMng_Process, (itfProcessShutdown_t)DrawMsgMng_Shutdown, 0);
}

/* Set manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesMngSetBitStatManager(u32 flags) {
    itfMesMngManager.flags |= flags;
}

/* Clear manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesMngClearBitStatManager(u32 flags) {
    itfMesMngManager.flags &= ~flags;
}

/* Read the manager's 16-bit flags, distinct from itfMessageFlags. */
u16 itfMesMngGetStatManager(void) {
    return itfMesMngManager.flags;
}

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00535B40);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00535B50);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00535B60);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00535B70);

s32 itfMesMngDebugProc(void) {
    itfMesWindow_t *panel;
    itfPanel_t *panelSprite;
    s32 *bounds;
    sdfDrawTag_t *packetList;
    s32 menuItemIndex;
    s32 menuItemY;
    const char *menuItemFormat;
    const char *selectionMarker;
    const char *coordinateFormat;
    sdfGrOt_t *packetOwner;

    if (D_00326700.window == NULL) {
        itfMesLink_t *window = DebugChangeInstance(NULL);

        D_00326700.window = window;
        if (window == NULL) {
            return 0;
        }
    }
    {
        itfMesDebug_t *debug = &D_00326700;

        panel = &debug->window->state->window;
        switch (debug->mode) {
        case 0:
            if (dds3PadButtonState.secondaryUp & 2) {
                if (--debug->selectedItem < 0) {
                    debug->selectedItem = 2;
                }
            } else if (dds3PadButtonState.secondaryDown & 2) {
                if (++debug->selectedItem >= 5) {
                    debug->selectedItem = 0;
                }
            }
            if (dds3PadButtonState.secondaryCross < 0) {
                itfMesDebug_t *confirmDebug = &D_00326700;

                switch (confirmDebug->selectedItem) {
                case 0:
                case 1:
                case 2:
                    confirmDebug->mode = 1;
                    break;
                case 3:
                    confirmDebug->mode = 2;
                    break;
                case 4:
                    confirmDebug->window = DebugChangeInstance(confirmDebug->window);
                    break;
                }
            } else if (dds3PadButtonState.secondaryCancel < 0) {
                return -1;
            }
            break;
        case 1:
            DebugMoveWinVert(panel, debug->selectedItem);
            if (dds3PadButtonState.secondaryCancel < 0) {
                debug->mode = 0;
            }
            break;
        case 2:
            DebugMoveWinAlpha((itfMesWindow_t *)panel);
            if (dds3PadButtonState.secondaryCancel < 0) {
                D_00326700.mode = 0;
            }
            break;
        }
    }

    packetList = dds3FontReqAllocDrawTag();
    dds3ConsPanelTagLink(packetList, 0x10, 0x10, 0x1E, 9);
    menuItemFormat = D_0054F4E8;
    menuItemY = 0x7A00;
    for (menuItemIndex = 0; menuItemIndex < 5; menuItemIndex++, menuItemY += 0x60) {
        if (D_00326700.selectedItem == menuItemIndex) {
            selectionMarker = D_0054F4D0[D_00326700.mode != 0];
        } else {
            selectionMarker = D_0054F4F0;
        }
        sdfDrawTagLink(packetList,
                        (u32)sdfDevPrintf(0x7180, menuItemY, 0xFFFFF0, 0,
                                                     menuItemFormat, selectionMarker, D_00326710[menuItemIndex]));
    }

    coordinateFormat = "( %3d, %3d )";
    panelSprite = panel->sprite;
    bounds = &panelSprite->left;
    sdfDrawTagLink(packetList,
                    (u32)sdfDevPrintf(0x7E00, 0x7A00, 0xFFFFF0, 0,
                                                 coordinateFormat, bounds[0] >> 4,
                                                 bounds[1] >> 3));
    sdfDrawTagLink(packetList,
                    (u32)sdfDevPrintf(0x7E00, 0x7A60, 0xFFFFF0, 0,
                                                 coordinateFormat, bounds[2] >> 4,
                                                 bounds[3] >> 3));
    sdfDrawTagLink(packetList,
                    (u32)sdfDevPrintf(0x7E00, 0x7B20, 0xFFFFF0, 0,
                                                 D_0054F4F8, panel->fadeLimit));
    bounds = D_00326700.window->state->window.bounds;
    sdfDrawTagLink(packetList,
                    (u32)sdfDevPrintf(0x7180, 0x7C40, 0xFFFFF0, 0,
                                                 "OFFSET : %3d, %3d - %3d, %3d",
                                                 bounds[0] >> 4,
                                                 bounds[1] >> 3,
                                                 bounds[2] >> 4,
                                                 bounds[3] >> 3));
    packetOwner = &sdfGrOtTable[83];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))packetOwner->regFunc)(packetOwner, packetList);
    return 0;
}

/* Reset the three otherwise unnamed message-state words. */
void itfMesMngDebugShutdown(void) {
    D_00326700.selectedItem = 0;
    D_00326700.mode = 0;
    D_00326700.window = NULL;
}

/* Tear down installed content/resources, return the pool record and decrement
 * the pool-array-relative counter. An empty record is a no-op. */
void DestructFlow(s32 id) {
    itfMesLink_t *windowRecord = &D_0056BE80[id];
    itfMesWork_t *mes = windowRecord->state;

    if (mes != NULL) {
        itfMesMngRequestMessageRelease(id, 1);
        itfMesMngRequestSelectRelease(id);
        ReleaseAllWindow(mes);
        ReleaseAllVariable(&mes->variables);
        mes->flags = 0;
        sdfMemFreeDirect(windowRecord->resourceHandle);
        windowRecord->state = NULL;
        itfMiscLinkGive(windowRecord, (itfMesLinkControl_t *)((u8 *)D_0056BE80 - 0x10));
        ((itfMesManager_t *)((u8 *)D_0056BE80 - 0x20))->activeWindowCount -= 1;
    }
}

void ResolveResourceAddress(itfMesResource_t *pdata)
{
    s32 *payload = (s32 *)pdata->entries;

    if (pdata->relocated == 0) {
        sdfAddressResolver(payload, (s32)payload, (u8 *)pdata + pdata->fixupTableOffset, pdata->fixupTableBytes);
        pdata->relocated = 1;
    }
}

/* Recognize MSG0/MSG1 magic only; this is not complete format validation. */
u32 CheckMessageBinary(itfMesResource_t *pdata) {
    u32 isMessageResource;

    isMessageResource = 0;
    if (pdata->magic == ITF_MES_MAGIC_MSG0 || pdata->magic == ITF_MES_MAGIC_MSG1) { /* "MSG0"/"MSG1" */
        isMessageResource = 1;
    }
    return isMessageResource;
}

itfMesEntry_t *GetTypeHeader(itfMesWork_t *pinst, s32 index)
{
    itfMesEntry_t *entries = pinst->sub->entries;

    return &entries[index];
}

itfMesEntry_t *GetTypeHeader2(itfMesResource_t *pfileheader)
{
    itfMesEntry_t *entries = pfileheader->entries;

    return &entries[pfileheader->entryCount];
}

u8 *GetMessagePagePtr(itfMesTable_t *table, s32 index)
{
    s32 count = table->count;

    if (index < 0 || index >= count) {
        return NULL;
    }
    return (u8 *)table->items[index];
}

/* Called above its definition, where ee-gcc assumed int. */
frFontHandle_t *GetLastLinkFontHandle(frFontHandle_t *pfont);

void ReadytoMessage(itfMesWork_t *panel) {
    itfMesMessage_t *mes = &panel->message;
    frFontHandle_t *chain;
    u32 result;

    if (mes->glyphChain != NULL) {
        frDestroyFontRequestAfter(mes->glyphChain);
        mes->glyphChain = NULL;
    }
    chain = MessageRequestEx(mes->x, mes->y, mes->color[0], mes->color[1], mes->color[2], mes->color[3],
                             GetMessagePagePtr(mes->table, mes->itemIndex), NULL);
    if (!(panel->flags & 0x400000)) {
        if ((itfMesMngManager.flags & 1) != 0) {
            RaiseMessageSpeedMax(chain);
        }
    }
    GetLastMessagePara(chain, mes);
    result = frGetMessageResult(3);
    mes->textInterfaceMask = result;
    if ((result & 2) != 0) {
        panel->flags |= 0x10000;
    } else {
        panel->flags &= 0xFFFEFFFF;
    }
    if (frGetMessageResult(4) != 0) {
        if (panel->callbackAddress != 0) {
            ((void (*)(void))panel->callbackAddress)();
        }
    }
    ChangeZBuffer(chain, panel->renderValue);
    mes->lineCount = CountLineMessage(GetLastLinkFontHandle(chain), chain);
    mes->glyphChain = chain;
}

/* Replace a retained text-slot allocation. Nonpositive byteCount copies the
 * padded string span; positive counts copy that many bytes into a zeroed block.
 * DDS1 string mode intentionally has no memset. Keep the K&R declaration form. */
void SetVar(mes, slotIndex, source, byteCount)
    itfMesWork_t *mes;
    s32 slotIndex;
    const char *source;
    s32 byteCount;
{
    itfMesVariable_t *slots = &mes->variables;
    u32 *textAddress = &slots->addresses[slotIndex];
    s32 allocationBytes;

    if (*textAddress != 0) {
        sdfMemFreeDirect(slots->handles[slotIndex]);
        *textAddress = 0;
    }
    if (byteCount <= 0) {
        allocationBytes = (strlen(source) + ITF_MES_STRING_COPY_ROUND_BIAS) & ~ITF_MES_COPY_ALIGN_MASK;
        slots->handles[slotIndex] = sdfMemAllocDirect(allocationBytes);
        *textAddress = sdfMemLock(slots->handles[slotIndex]);
        /* String mode copies the padded span, rather than only strlen + 1. */
        memcpy((void *)*textAddress, source, allocationBytes);
        return;
    }
    allocationBytes = (byteCount + ITF_MES_BINARY_COPY_ROUND_BIAS) & ~ITF_MES_COPY_ALIGN_MASK;
    slots->handles[slotIndex] = sdfMemAllocDirect(allocationBytes);
    *textAddress = sdfMemLock(slots->handles[slotIndex]);
    memset((void *)*textAddress, 0, allocationBytes);
    memcpy((void *)*textAddress, source, byteCount);
}

void CommitVarTable(s32 *pvar)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        frSetVariableRequest(i, pvar[i]);
    }
}

u8 *GetSpeakerName(itfMesWork_t *pinst)
{
    u32 *items = GetTypeHeader2(pinst->sub)->itemList;

    return (u8 *)items[pinst->speaker.selectedIndex];
}

s32 GetNumSelectItem(s32 allitem, u32 mask)
{
    s32 n = 0;

    while (allitem > 0) {
        u32 bit = mask & 1;

        mask >>= 1;
        allitem--;
        if (bit == 0) {
            n++;
        }
    }
    return n;
}

frFontHandle_t *GetFontHandleUnit(u32 *list, s32 count, u32 mask, s32 x, s32 y, s32 depth)
{
    frFontHandle_t *chain = NULL;
    s32 i;

    for (i = 0; i < count; i++, list++) {
        if ((mask & 1) != 0) {
            mask >>= 1;
            continue;
        }
        chain = MessageRequestEx(x, y, 0, 0, 0, 0x80, (u8 *)*list, chain);
        y += (s16)chain->parentDimensionsOrRenderWord.parentDimensions.cellHeight * 8;
        y += 0xA;
        mask >>= 1;
    }
    if (chain != NULL) {
        ChangeZBuffer(chain, depth);
        ChangeFontAlpha(chain, 0x80);
    }
    return chain;
}

/* Discard (to - from - 1) preceding rows when positive, retain the next
 * complete y-group and queue everything after it. Requires non-NULL input;
 * return NULL if discarding preceding rows exhausts the chain. */
frFontHandle_t *UnchainFontHandleUnit(frFontHandle_t *pfont, s32 index, s32 lines) {
    s32 rowsToDiscard = lines - index - 1;
    s32 rowY = pfont->y;
    frFontHandle_t *nextNode;
    frFontHandle_t *rowHead;
    frFontHandle_t *rowTail;

    while (rowsToDiscard > 0) {
        while (rowY == pfont->y) {
            nextNode = pfont->previous;
            pfont->next = NULL;
            pfont->chainHead = pfont;
            pfont->previous = NULL;
            frDestroyFontRequestAfter(pfont);
            pfont = nextNode;
            if (pfont == NULL) {
                return NULL;
            }
        }
        rowsToDiscard--;
        rowY = pfont->y;
    }
    rowHead = pfont;
    do {
        rowTail = pfont;
        pfont = pfont->previous;
    } while (pfont != NULL && rowY == pfont->y);
    while (pfont != NULL) {
        nextNode = pfont->previous;
        pfont->next = NULL;
        pfont->chainHead = pfont;
        pfont->previous = NULL;
        frDestroyFontRequestAfter(pfont);
        pfont = nextNode;
    }
    rowHead->next = NULL;
    rowTail->previous = NULL;
    for (pfont = rowHead; pfont != NULL; pfont = pfont->previous) {
        /* Despite its current name, this link points to the retained row tail. */
        pfont->chainHead = rowTail;
    }
    return rowHead;
}

frFontHandle_t *GetLastLinkFontHandle(frFontHandle_t *pfont)
{
    while (pfont->previous != NULL) {
        pfont = pfont->previous;
    }
    return pfont;
}

void GetLastMessagePara(frFontHandle_t *pfont, itfMesMessage_t *pmessage)
{
    frTextHandle_t *shade = pfont->lastChild;
    u8 ctx = pfont->glyphCodeOrContext.byteRoles.encodedContextByte;

    pmessage->color[3] = ctx >> 1;
    pmessage->color[0] = shade->renderValueOrSetupOrShade.setupBytes.fontIndex;
    pmessage->color[1] = shade->renderValueOrSetupOrShade.setupBytes.firstOption;
    pmessage->color[2] = shade->renderValueOrSetupOrShade.setupBytes.secondOption;
}

s32 CountLineMessage(frFontHandle_t *pfont_top, frFontHandle_t *pfont)
{
    s16 h = pfont->parentDimensionsOrRenderWord.parentDimensions.cellHeight;

    return ((pfont->y - pfont_top->y) >> 3) / h + 1;
}

/* Translate every linked node in text units; a NULL chain is a no-op. */
void ChangeFontPosition(frFontHandle_t *pfont, s32 dist_x, s32 dist_y) {
    if (pfont == NULL) {
        return;
    }
    do {
        pfont->x += dist_x;
        pfont->y += dist_y;
        pfont = pfont->previous;
    } while (pfont != NULL);
}

void ChangeZBuffer(frFontHandle_t *pfont, s32 z)
{
    while (pfont != NULL) {
        pfont->renderValueOrSetupOrShade.renderValue = z;
        pfont = pfont->previous;
    }
}

void ChangeFontColor(frFontHandle_t *pfont, s32 index, s32 lines, s32 color)
{
    s32 skip = lines - index - 1;
    s32 y = pfont->y;
    s32 cur = y;
    frTextHandle_t *c;
    u8 flag;

    while (skip > 0) {
        while (y == cur) {
            pfont = pfont->previous;
            if (pfont == NULL) {
                return;
            }
            cur = pfont->y;
        }
        skip--;
        y = cur;
    }
    flag = color;
    do {
        for (c = pfont->firstChild; c != NULL; c = c->next) {
            c->renderValueOrSetupOrShade.setupBytes.firstOption = flag;
        }
        pfont = pfont->previous;
    } while (pfont != NULL && y == pfont->y);
}

void ChangeFontColorAll(frFontHandle_t *pfont, u8 color)
{
    frTextHandle_t *c;

    for (; pfont != NULL; pfont = pfont->previous) {
        for (c = pfont->firstChild; c != NULL; c = c->next) {
            c->renderValueOrSetupOrShade.setupBytes.firstOption = color;
        }
    }
}

/* Clear each child's low color byte, then OR the unmasked input word.
 * High input bits can therefore also change the upper bytes. */
void ChangeFontAlpha(frFontHandle_t *pfont, u32 alpha) {
    frTextHandle_t *child;

    for (; pfont != NULL; pfont = pfont->previous) {
        for (child = pfont->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = child->parentDimensionsOrRenderWord.renderWord & ITF_MES_COLOR_BYTE_CLEAR_MASK | alpha;
        }
    }
}

s32 GetMessageWidth(frFontHandle_t *pfont)
{
    s32 max = 0;

    while (pfont != NULL) {
        s32 y = pfont->y;
        s32 w = 0;

        do {
            w += pfont->advance;
            pfont = pfont->previous;
        } while (pfont != NULL && y == pfont->y);
        if (w > max) {
            max = w;
        }
    }
    return max << 4;
}

s32 GetMaskedIndex(s32 index, u32 mask)
{
    s32 i = 0;

    while (i < 0x20) {
        if ((mask & 1) == 0) {
            if (--index < 0) {
                break;
            }
        }
        i++;
        mask >>= 1;
    }
    return i;
}

void RaiseMessageSpeedMax(frFontHandle_t *pfont)
{
    while (pfont != NULL) {
        if (pfont->firstChild->renderValueOrSetupOrShade.setupBytes.secondOption == 0) {
            frSetDrawSpeed(pfont, 0x80);
        }
        pfont = pfont->previous;
    }
}

void RaiseMessageSpeedMaxAll(frFontHandle_t *pfont)
{
    while (pfont != NULL) {
        frSetDrawSpeed(pfont, 0x80);
        pfont = pfont->previous;
    }
}

void OpenMessageWindow(itfMesWork_t *pinst) {
    itfMesWindow_t *place = &pinst->window;
    itfMesMessage_t *mes = &pinst->message;

    if (place->sprite == NULL) {
        if (pinst->panelMode != 2) {
            place->sprite = itfPnlRegist2(6, (u32)itfMesMngManager.windowTexture);
        } else {
            place->sprite = itfPnlRegist2(8, (u32)itfMesMngManager.windowTexture);
        }
    }
    itfPnlSetPosition(place->sprite, mes->x + place->bounds[0], mes->y + place->bounds[1],
                      mes->x + place->bounds[2], mes->y + place->bounds[3], pinst->renderValue);
    itfPnlSetColor(place->sprite, place->red, place->green, place->blue, 0);
    pinst->flags = (pinst->flags & ~0x300) | 0x100;
}

void OpenCheckSpeakerWindow(itfMesWork_t *pinst) {
    itfMesSpeaker_t *origin = &pinst->speaker;
    itfMesWindow_t *place = &pinst->window;
    if (origin->glyphChain != NULL && !(pinst->flags & 0x10000)) {
        if (place->frame == NULL) {
            s32 width = origin->glyphChain->advance * 16;
            place->frame = itfPnlRegist2(7, (u32)itfMesMngManager.windowTexture);
            itfPnlSetPosition(place->frame, origin->x - 0x2D0, origin->y - 0x68, origin->x + width + 0x2D0, origin->y + 0xE8, pinst->renderValue);
            itfPnlSetColor(place->frame, 0x7F, 0x7F, 0x7F, 0);
        }
        pinst->flags = (pinst->flags & ~0x3000) | 0x1000;
    } else if (place->frame != 0) {
        pinst->flags |= 0x3000;
    }
}

void InitializeSpeaker(itfMesSpeaker_t *pspeaker, s32 defaultpos) {
    if (defaultpos != 0) {
        pspeaker->x = 0x430;
        pspeaker->y = 0x9B0;
    }
    pspeaker->glyphChain = 0;
    pspeaker->selectedIndex = 0xffff;
}

void InitializeMessage(itfMesMessage_t *pmessage, s32 defaultpos) {
    if (defaultpos != 0) {
        pmessage->x = 0x4B0;
        pmessage->y = 0xA88;
    }
    pmessage->glyphChain = NULL;
    pmessage->textState = 0;
    pmessage->textInterfaceMask = 0;
    pmessage->lineCount = 0;
    pmessage->itemIndex = 0;
    pmessage->tableCount = 0;
    pmessage->color[0] = 0;
    pmessage->color[1] = 0;
    pmessage->color[2] = 0;
    pmessage->color[3] = 0x80;
    pmessage->table = NULL;
}

void InitializeSelect(itfMesSelect_t *pselect)
{
    pselect->x = 0x560;
    pselect->y = 0xBC8;
    pselect->glyphChain = NULL;
    pselect->panelValue = 0;
    pselect->selectionState = 0;
    pselect->selectedIndex = -1;
    pselect->savedIndex = -1;
    pselect->rowCount = 0;
    pselect->glyphColorIntensity = 0;
    pselect->optionCount = 0;
}

void InitializeWindow(itfMesWindow_t *pwindow)
{
    pwindow->frame = NULL;
    pwindow->sprite = NULL;
    ResetWindowPara(pwindow, 0, 0);
}

void InitializeVariable(itfMesVariable_t *pvariable)
{
    s32 n = 0x1F;
    u32 *p = &pvariable->addresses[31];

    do {
        n--;
        *p = 0;
        p--;
    } while (n >= 0);
}

void InitializeCurCtr(itfMesCursor_t *pcurctr, s32 isdisp)
{
    if (isdisp == 0) {
        pcurctr->kind = 0;
    }
    pcurctr->alpha = 0x40;
    pcurctr->phase = 0;
    pcurctr->timer = 0;
    pcurctr->frames = 0;
}

void ForceSetCurCtr(itfMesCursor_t *pcurctr, s16 phase, s16 ctr0, s16 ctr1)
{
    pcurctr->phase = phase;
    pcurctr->alpha = ctr0;
    pcurctr->timer = ctr1;
}

void ReleaseAllWindow(itfMesWork_t *pinst) {
    itfMesWindow_t *place = &pinst->window;

    if (place->frame != NULL) {
        itfPnlRelease(place->frame);
        place->frame = NULL;
    }
    if (place->sprite != NULL) {
        itfPnlRelease(place->sprite);
        place->sprite = NULL;
    }
    pinst->flags &= ~0xF00;
}

void ReleaseAllVariable(itfMesVariable_t *pvariable)
{
    s32 remaining = 0x1F;

    do {
        s32 index = 0x1F - remaining;

        if (pvariable->addresses[index] != 0) {
            sdfMemFreeDirect(pvariable->handles[index]);
            pvariable->addresses[index] = 0;
        }
        remaining--;
    } while (remaining >= 0);
}

char *ConvertZenkakuNumber(s32 buf, char *num) {
    s32 digits[10];
    s32 count = 0;
    s32 radix = 10;
    s32 *p = digits;
    s32 i;

    do {
        *p = buf % radix;
        buf = buf / radix;
        count++;
        p++;
    } while (buf > 0 && count < 10);

    for (i = count - 1; i >= 0; i--) {
        *num = (u8)digits[i] + 0x30;
        num++;
    }
    *num = 0;
    return num;
}

void ResetWindowPara(itfMesWindow_t *pwin, s32 type, s32 alpha)
{
    switch (type) {
    default:
        pwin->bounds[0] = -0x1390;
        pwin->bounds[1] = -0x3D8;
        pwin->bounds[2] = 0x2A30;
        pwin->bounds[3] = 0x590;
        pwin->red = 0x7F;
        pwin->green = 0x7F;
        pwin->blue = 0x7F;
        pwin->fadeLimit = (alpha > 0) ? alpha : 0x66;
        break;
    case 1:
        pwin->bounds[0] = -0x1DB0;
        pwin->bounds[1] = -0x2F8;
        pwin->bounds[2] = 0x2C30;
        pwin->bounds[3] = 0x548;
        pwin->red = 0x7F;
        pwin->green = 0x7F;
        pwin->blue = 0x7F;
        pwin->fadeLimit = (alpha > 0) ? alpha : 0x57;
        break;
    case 2:
        pwin->bounds[0] = -0x1390;
        pwin->bounds[1] = -0x208;
        pwin->bounds[2] = 0x2A30;
        pwin->bounds[3] = 0x410;
        pwin->red = 0x7F;
        pwin->green = 0x7F;
        pwin->blue = 0x7F;
        pwin->fadeLimit = (alpha > 0) ? alpha : 0x87;
        break;
    }
}

void CalcMesFlow(itfMesWork_t *pinst) {
    CalcWindow(pinst);
    CalcMessage(pinst);
    CalcSelect(pinst);
    CalcCurCtr(pinst);
}

void CalcWindow(itfMesWork_t *pinst) {
    itfMesWindow_t *place = &pinst->window;
    itfPanel_t *sprite = place->sprite;
    itfPanel_t *frame;
    u32 mode;

    mode = pinst->flags & 0x300;
    switch (mode) {
    case 0x100:
        sprite->alpha += 0x18;
        if (sprite->alpha >= place->fadeLimit) {
            sprite->alpha = place->fadeLimit;
            pinst->flags = (pinst->flags & ~0x307) | 0x203;
        }
        break;
    case 0x300:
        sprite->alpha -= 8;
        if (sprite->alpha <= 0) {
            sprite->alpha = 0;
            pinst->flags &= ~0x300;
        }
        break;
    }
    frame = place->frame;
    mode = pinst->flags & 0x3000;
    switch (mode) {
    case 0x1000:
        frame->alpha += 0x20;
        if (frame->alpha >= 0xC8) {
            frame->alpha = 0xC8;
            pinst->flags = (pinst->flags & ~0x3000) | 0x2000;
        }
        break;
    case 0x3000:
        frame->alpha -= 0x20;
        if (frame->alpha <= 0) {
            frame->alpha = 0;
            pinst->flags &= ~0x3000;
            itfPnlRelease(frame);
            place->frame = NULL;
        }
        break;
    }
}

void CalcMessage(itfMesWork_t *pinst) {
    itfMesMessage_t *mes = &pinst->message;
    u32 flags;

    if (mes->textState != 1) {
        return;
    }
    flags = pinst->flags;
    if ((flags & 7) == 4) {
        pinst->fade.frames++;
        if (flags & 0x800000) {
            mes->textState = -1;
            return;
        }
        if ((flags & 0x100000) == 0) {
            pinst->fade.kind = 1;
        }
        if (dds3PadButtonState.confirm >= 0 && dds3PadButtonState.primaryCancel >= 0 &&
            (dds3PadButtonState.primaryTriangle == 0 || pinst->fade.frames < 5)) {
            if (mes->textInterfaceMask != 0) {
                return;
            }
        }
        if (mes->textInterfaceMask != 0) {
            p2sdPlaySE(9, 0x7F, 0x3F);
        }
        InitializeCurCtr(&pinst->fade, 0);
        mes->itemIndex++;
        if (mes->itemIndex >= mes->tableCount) {
            mes->textState = -1;
            return;
        }
        CommitVarTable((s32 *)pinst->variables.addresses);
        if (pinst->temporaryFontEntry != NULL) {
            frSetExtDic(pinst->temporaryFontEntry);
        }
        ReadytoMessage(pinst);
        OpenCheckSpeakerWindow(pinst);
        pinst->flags = (pinst->flags & ~7) | 3;
    } else if (dds3PadButtonState.confirm < 0 || dds3PadButtonState.primaryCancel < 0 ||
               dds3PadButtonState.primaryTriangle != 0) {
        if (!(flags & 0x400000)) {
            RaiseMessageSpeedMaxAll(mes->glyphChain);
            if (pinst->speaker.glyphChain != NULL) {
                frSetDrawSpeed(pinst->speaker.glyphChain, 0x80);
            }
        }
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcSelect);

s32 SelectItem(itfMesWork_t *pinst) {
    itfMesSelect_t *sel = &pinst->selection;
    s32 dir = 0;
    s32 index;
    if (dds3PadButtonState.prev & 2) {
        if (sel->selectedIndex != 0) {
            dir = -1;
        } else if (dds3PadButtonState.prev < 0) {
            dir = -1;
        }
    } else if (dds3PadButtonState.next & 2) {
        if (sel->selectedIndex != sel->rowCount - 1 || dds3PadButtonState.next < 0) {
            dir = 1;
        }
    }
    if (dir != 0) {
        SelectItemMove(sel, dir);
        InitializeCurCtr(&pinst->fade, 1);
    }
    if (dds3PadButtonState.confirm < 0) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        return 1;
    }
    if (sel->optionCount > 0 && (index = SelectItemDefKey(sel)) >= 0) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        if (index != sel->selectedIndex) {
            ChangeFontColor(sel->glyphChain, sel->selectedIndex, sel->rowCount, 0);
            ChangeFontColor(sel->glyphChain, index, sel->rowCount, 1);
            sel->selectedIndex = index;
            sel->savedIndex = index;
        }
        return 1;
    }
    return 0;
}

void SelectItemMove(itfMesSelect_t *psel, s32 direct) {
    s32 cur = psel->selectedIndex;
    u16 maxv;
    ChangeFontColor(psel->glyphChain, cur, psel->rowCount, 0);
    if (direct < 0) {
        cur = cur - 1;
        if (cur < 0) {
            cur = psel->rowCount - 1;
        }
        maxv = psel->rowCount;
    } else {
        cur = cur + 1;
        if (cur >= psel->rowCount) {
            cur = 0;
        }
        maxv = psel->rowCount;
    }
    ChangeFontColor(psel->glyphChain, cur, (s16)maxv, 1);
    psel->selectedIndex = cur;
    psel->savedIndex = cur;
    p2sdPlaySE(1, 0x7F, 0x3F);
}

s32 SelectItemDefKey(itfMesSelect_t *psel) {
    s32 i;

    for (i = 0; i < psel->optionCount; i++) {
        itfMesOption_t *option = &psel->options[i];

        if (dds3PadButtonState.buttons[option->id] < 0) {
            s32 prefixLength = option->value;
            s32 rank = 0;
            u32 mask = psel->panelValue;

            if (prefixLength > 0) {
                s32 remaining = prefixLength;
                do {
                    if ((mask & 1) == 0) {
                        rank++;
                    }
                    mask >>= 1;
                } while (--remaining != 0);
            }
            if ((mask & 1) == 0) {
                return rank;
            }
        }
    }
    return -1;
}

void CalcCurCtr(itfMesWork_t *pinst) {
    itfMesCursor_t *fade = &pinst->fade;
    s32 minimumAlpha;
    if (fade->kind != 0) {
        if (fade->timer > 0) {
            fade->timer -= 8;
        }
        switch (fade->phase) {
        case 0:
            fade->alpha += 8;
            if (fade->alpha >= 0xFF) {
                fade->phase = 1;
                fade->alpha = 0xFF;
                fade->timer = 0x80;
            }
            break;
        case 1:
            fade->alpha -= 8;
            minimumAlpha = (fade->kind & 1) ? 0x20 : 0x40;
            if (fade->alpha <= minimumAlpha) {
                fade->alpha = minimumAlpha;
                fade->phase = 0;
            }
            break;
        }
    }
}

/* NON_MATCHING: 37 of 99 words, exact size; two guards emit bnel where retail has bnez and everything after shifts. */
#ifdef NON_MATCHING
void DrawMesFlow(itfMesWork_t *pinst) {
    u32 flags = pinst->flags;
    itfMesSelect_t *sel;
    s16 index;

    itfMesMngManager.flags &= ~2;
    DrawWindow(pinst);
    if (!(flags & 0x10000)) {
        if (pinst->speaker.glyphChain != NULL) {
            frDrawFont(pinst->speaker.glyphChain);
        }
    }
    if (!(flags & 0x20000)) {
        if ((flags & 7) >= 3) {
            if (frDrawFont(pinst->message.glyphChain) > 0) {
                if ((pinst->flags & 7) != 4) {
                    pinst->fade.frames = 0;
                    pinst->flags = (pinst->flags & ~7) | 4;
                }
            }
        }
    }
    if (!(flags & 0x40000)) {
        if ((flags & 0x38) >= 0x18) {
            if (frDrawFontEx(pinst->selection.glyphChain, 1) > 0) {
                if ((flags & 0x38) == 0x18) {
                    sel = &pinst->selection;
                    index = sel->selectedIndex;
                    if (sel->selectedIndex == -1) {
                        sel->selectedIndex = 0;
                        sel->savedIndex = 0;
                        index = 0;
                    }
                    ChangeFontColor(sel->glyphChain, index, sel->rowCount, 1);
                    pinst->fade.kind = 2;
                    pinst->flags = (pinst->flags & ~0x38) | 0x20;
                }
            }
        }
    }
    if ((pinst->fade.kind & 1) != 0) {
        DrawRetPageCursor(pinst);
    }
    if ((pinst->fade.kind & 2) != 0) {
        DrawSelectCursor(pinst);
    }
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMesFlow);
#endif


void DrawWindow(itfMesWork_t *pinst) {
    itfMesWindow_t *place = &pinst->window;
    u32 flags = pinst->flags;

    if (flags & 0x80000) {
        return;
    }
    if ((flags & 0x300) >= 0x100) {
        itfPnlDraw(place->sprite, &sdfGrOtTable[pinst->drawSurfaceIndex]);
        itfMesMngManager.flags |= 2;
        if (place->frame != NULL) {
            itfPnlDraw(place->frame, &sdfGrOtTable[pinst->drawSurfaceIndex]);
        }
    }
    if (D_00326700.window != NULL && D_00326700.window->state == pinst && place != NULL) {
        DebugDrawWindowLine(place->sprite);
    }
}

/* NON_MATCHING: 128 of 191 words, exact size; the three stack arrays' store and constant order is scheduled differently. */
#ifdef NON_MATCHING
void DrawSelectCursor(itfMesWork_t *pinst) {
    s32 uv[4];
    s32 color[4];
    s32 rect[4];
    itfMesCursor_t *fade = &pinst->fade;
    itfMesSelect_t *sel = &pinst->selection;
    sdfDrawTag_t *list;
    sdfGrOt_t *packetOwner;
    s32 x;
    s32 y;
    s32 timer;
    s32 d;
    s32 index;
    s32 rowOfs;

    index = sel->savedIndex;
    if (index == -1) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    x = sel->x;
    rowOfs = sel->rowCount * 160 - 0xC0;
    y = sel->y - rowOfs;
    y += index * 168;
    color[0] = 0x57;
    color[1] = 0;
    color[2] = 0x10;
    color[3] = 0x26;
    rect[1] = y - 8;
    rect[0] = x - 0xF0;
    rect[2] = D_00326728[0];
    rect[3] = y + 0xA8;
    itfMiscDrawSpritePlanePrim(rect, color, pinst->renderValue, 0, list);
    D_00326728[1] = y - 8;
    D_00326728[3] = y - 8;
    D_00326728[5] = y + 0xA8;
    D_00326728[7] = y + 0xA8;
    itfMiscDrawRectanglePrim(D_00326728, "W", D_00326768, D_00326770, pinst->renderValue, list);
    uv[0] = 0x150;
    uv[1] = 0x2F0;
    uv[2] = 0x1B0;
    uv[3] = 0x3F0;
    color[0] = 0x80;
    color[1] = 0x80;
    color[2] = 0x80;
    color[3] = 0x26;
    rect[0] = x - 0x150;
    rect[2] = x - 0xF0;
    rect[3] = y + 0x88;
    rect[1] = y + 8;
    itfMiscDrawSpritePrim(rect, uv, color, pinst->renderValue, itfMesMngManager.windowTexture, 0, list);
    uv[0] = 0x260;
    uv[1] = 0x2F0;
    uv[3] = 0x3F0;
    uv[2] = 0x360;
    color[3] = fade->alpha;
    rect[2] = rect[0] + 0x100;
    rect[3] = rect[1] + 0x80;
    itfMiscDrawSpritePrim2(rect, uv, color, pinst->renderValue, 0, list);
    timer = fade->timer;
    if (timer > 0) {
        d = 0x80 - timer;
        rect[0] -= d;
        rect[1] -= d >> 1;
        rect[2] += d * 2;
        rect[3] += d >> 1;
        color[3] = timer;
        itfMiscDrawAlphaPrim(list, 1, 0);
        itfMiscDrawSpritePrim2(rect, uv, color, pinst->renderValue, 0, list);
        itfMiscDrawAlphaPrim(list, 0, 0);
    }
    packetOwner = &sdfGrOtTable[pinst->drawSurfaceIndex];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))packetOwner->regFunc)(packetOwner, list);
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", DrawSelectCursor);
#endif


void DrawRetPageCursor(itfMesWork_t *pinst) {
    s32 rect[4];
    itfMesCursor_t *fade = &pinst->fade;
    sdfDrawTag_t *list;
    sdfGrOt_t *packetOwner;
    s32 timer;
    s32 d;

    rect[0] = 0x1A50;
    rect[1] = 0xCA0;
    rect[2] = 0x1B60;
    rect[3] = 0xD28;
    D_00326788[3] = fade->alpha;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    itfMiscDrawSpritePrim(rect, D_00326778, D_00326788, pinst->renderValue, itfMesMngManager.windowTexture, 0, list);
    timer = fade->timer;
    if (timer > 0) {
        d = 0x80 - timer;
        rect[0] -= d * 2;
        rect[1] -= d;
        rect[2] += d * 2;
        rect[3] += d;
        D_00326788[3] = timer;
        itfMiscDrawAlphaPrim(list, 1, 0);
        itfMiscDrawSpritePrim(rect, D_00326778, D_00326788, pinst->renderValue, itfMesMngManager.windowTexture, 0, list);
        itfMiscDrawAlphaPrim(list, 0, 0);
    }
    packetOwner = &sdfGrOtTable[pinst->drawSurfaceIndex];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))packetOwner->regFunc)(packetOwner, list);
}

s32 CalcMsgMng_Process(void) {
    itfMesLink_t *node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        CalcMesFlow(node->state);
        node = node->next;
    }
    return 0;
}

/* NON_MATCHING: 24 of 28 words, exact size; bnel against retail's bne on the DrawMesFlow loop, waiting on that callee. */
#ifdef NON_MATCHING
s32 DrawMsgMng_Process(dds3ProcessID_t *id) {
    itfMesLink_t *node;

    if (dds3NullDrawProc() != 0) {
        return 0;
    }
    node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        itfMesLink_t *next = node->next;
        DrawMesFlow(node->state);
        node = next;
    }
    itfMesMngManager.unk8++;
    return 0;
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMsgMng_Process);
#endif

void DrawMsgMng_Shutdown(void) {
    itfMesLink_t *node = itfMesMngManager.pool.activeHead;
    s32 window;
    while (node != 0) {
        window = node->index;
        node = node->next;
        DestructFlow(window);
    }
    sdfTexFreeDelayed(itfMesMngManager.windowTexture);
    itfMesMngManager.windowTexture = NULL;
}

void DebugDrawWindowLine(itfPanel_t *meswin) {
    s32 vertices[4][2] = {
        {meswin->left, meswin->top},
        {meswin->right, meswin->top},
        {meswin->right, meswin->bottom},
        {meswin->left, meswin->bottom}
    };
    sdfDrawTag_t *list;
    sdfGrOt_t *packetOwner;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    itfMiscDrawLineStripPrim(vertices, D_00326798, D_0054F500, D_0054F508, 5, 0xFFFFFF, list);
    packetOwner = &sdfGrOtTable[81];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))packetOwner->regFunc)(packetOwner, list);
}

void DebugMoveWinVert(itfMesWindow_t *pwin, s32 flag) {
    itfPanel_t *sprite = pwin->sprite;
    s32 *bounds;
    s32 dx;
    s32 dy;

    if (sprite != NULL) {
        bounds = pwin->bounds;
        if (dds3PadButtonState.coarseDown & 2) {
            dx = -16;
        } else {
            dx = ((u8)dds3PadButtonState.coarseUp << 3) & 0x10;
        }
        if (dds3PadButtonState.secondaryUp & 2) {
            dy = -8;
        } else {
            dy = ((u8)dds3PadButtonState.secondaryDown << 2) & 8;
        }
        if (dds3PadButtonState.secondaryCross != 0) {
            dx *= 8;
            dy *= 8;
        }
        if (dx != 0 || dy != 0) {
            switch (flag) {
            case 0:
                itfPnlSetPositionOffset(sprite, dx, dy, 0, 0, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                break;
            case 1:
                itfPnlSetPositionOffset(sprite, 0, 0, dx, dy, 0);
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            case 2:
                itfPnlSetPositionOffset(sprite, dx, dy, dx, dy, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            }
        }
    }
}

/* Step the stepper's index by pad input: one per press, ten with the fast modifier held; mirror it into the target. */
void DebugMoveWinAlpha(itfMesWindow_t *pwin) {
    s32 step;

    if (dds3PadButtonState.coarseDown & 2) {
        step = -1;
    } else {
        step = (dds3PadButtonState.coarseUp & 2) > 0;
    }
    if (dds3PadButtonState.secondaryUp & 2) {
        step = -1;
    } else if (dds3PadButtonState.secondaryDown & 2) {
        step = 1;
    }
    if (dds3PadButtonState.secondaryCross != 0) {
        step *= 10;
    }
    if (step != 0) {
        s32 index = (pwin->fadeLimit + step) & 0xFF;

        pwin->fadeLimit = index;
        if (pwin->sprite != NULL) {
            pwin->sprite->alpha = index;
        }
    }
}

itfMesLink_t *DebugChangeInstance(itfMesLink_t *pflow) {
    s32 index = 0;
    s32 count = itfMesMngManager.activeWindowCount;

    if (count <= 0) {
        return NULL;
    }
    for (;;) {
        if (pflow != NULL) {
            pflow = pflow->previous;
        }
        if (pflow == NULL) {
            pflow = itfMesMngManager.pool.activeTail;
        }
        if ((pflow->state)->window.sprite != NULL) {
            break;
        }
        index++;
        if (count < index) {
            pflow = NULL;
            break;
        }
    }
    return pflow;
}
INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4C0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4C8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4D0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4D8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4E0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4E8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4F0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4F8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F500);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F508);

