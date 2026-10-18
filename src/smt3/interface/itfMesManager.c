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

void itfMesMngSetPhase(s32 windowIndex, s8 status);

s32 itfMesMngSelect(void);

extern void itfMesMngRequestSelect(s32 window, s32 entryIndex);

extern void scrSetIntReturnValue(s32 retVal);

void itfMesMngRequestSelectRelease(s32 window);

s16 itfMesMngGetSelPhase(s32 windowIndex);

s16 itfMesMngGetSelectNo(s32 windowIndex);

s32 itfMesMngMaskSelect(void);

void itfMesMngSetMaskSelect(s32 window, u32 mask);

s32 itfMesMngCloseWindow(void);

#define ITF_MES_LOWER_STATUS_PAIR_MASK 0x300

#define ITF_MES_LOWER_STATUS_FIRST_BIT 0x100

#define ITF_MES_LOWER_STATUS_SECOND_BIT 0x200

void itfMesMngMsgWndCls(s32 window);

extern void fldPanelSet(s32 unused);

#define ITF_MES_UPPER_STATUS_PAIR_MASK 0x3000

void itfMesMngRequestMessageRelease(s32 window, s32 releasePrimaryBlock);

#define ITF_MES_CLEAR_SCRIPT_PANEL_BIT 0xFFDFFFFF

u32 itfMesMngOpenSelectWindow(void);

u32 itfMesMngCloseSelectWindow(void);

s32 itfMesMngSetMesPosDefault(void);

void itfMesMngSetMesPosition(s32 window, s32 x, s32 y);

#define ITF_MES_DEFAULT_TEXT_X 0x4B0

#define ITF_MES_DEFAULT_TEXT_Y 0xA88

s32 itfMesMngInitialize(itfMesResource_t *sub);

extern itfMesLink_t *itfMiscLinkTake(itfMesLinkControl_t *pool);

extern itfMesManager_t itfMesMngManager;

sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

extern u32 sdfMemLock(sdfMemHandle_t *allocation);

itfMesResource_t *itfMesMngSetResourcePtr(s32 window, itfMesResource_t *sub);

void InitializeSpeaker(itfMesSpeaker_t *cursor, s32 resetPosition);

void InitializeMessage(itfMesMessage_t *cur, s32 resetPos);

extern void InitializeSelect(struct itfMesSelect_t *cursor);

void InitializeWindow(itfMesWindow_t *window);

void InitializeVariable(itfMesVariable_t *slots);

void InitializeCurCtr(itfMesCursor_t *c, s32 preserve);

void itfMesMngRelease(s32 window);

void DestructFlow(s32 window);

s32 frDestroyFontRequestAfter(frFontHandle_t *glyph);

#define ITF_MES_ENTRY_STATE_MASK 7

#define ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK 0xFFFDFFFF

#define ITF_MES_OPTION_STATE_MASK 0x38

#define ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK 0xFFFBFFFF

void itfMesMngSetSpeakerPosition(s32 window, s32 x, s32 y);

void ChangeFontPosition(frFontHandle_t *glyph, s32 dx, s32 dy);

void itfMesMngSetSpeakerPositionOffset(s32 window, s32 dx, s32 dy);

void itfMesMngSetMesPositionOffset(s32 window, s32 dx, s32 dy);

void itfMesMngSetSelPosition(s32 window, s32 x, s32 y);

void itfMesMngSetSelPositionOffset(s32 window, s32 dx, s32 dy);

void itfMesMngSetPriority(s32 window, s32 renderValue);

void ChangeZBuffer(frFontHandle_t *g, s32 z);

void itfMesMngSetStat(s32 window, u32 highFlags);

#define ITF_MES_HIGH_FLAGS_MASK 0xffff0000

s16 itfMesMngGetSelect(s32 window);

s16 itfMesMngGetMesPage(s32 window);

s16 itfMesMngGetMesCurrentPage(s32 window);

u8 *itfMesMngGetVar(s32 window, s32 slotIndex);

void itfMesMngSetVarImmediate(s32 window, u32 slotIndex, const char *sourceText);

void SetVar(itfMesWork_t *mes, s32 slotIndex, const char *source, s32 byteCount);

void itfMesMngSetVarImmediate2(s32 window, s32 slotIndex, const char *source, s32 byteCount);

void ResolveResourceAddress(itfMesResource_t *sub);

itfMesEntry_t *GetTypeHeader2(itfMesResource_t *sub);

#define ITF_MES_MAGIC_MSG1 0x3147534d

u8 *itfMesMngGetDataPtrImmediate(s32 window, s32 entryIndex, s32 itemIndex);

u8 *GetMessagePagePtr(itfMesTable_t *table, s32 index);

itfMesEntry_t *GetTypeHeader(itfMesWork_t *mes, s32 index);

void itfMesMngChangeWindowType(s32 window, s32 panelMode, s32 fadeLimit);

#define ITF_MES_CONTENT_STATUS_MASK 0x3300

extern void ResetWindowPara(itfMesWindow_t *block, s32 panelMode, s32 fadeLimit);

void itfPnlRelease(itfPanel_t *sprite);

s32 itfMesMngSetSelectDefKey(s32 window, s32 id, s32 value);

s32 itfMesMngGetWidth(s32 window, s32 entryIndex, s32 itemIndex);

void CommitVarTable(s32 *table);

void frSetExtDic(u8 *fontData);

frFontHandle_t *MessageRequest(s32 x, s32 y, u8 *encodedText, frFontHandle_t *sub);

s32 GetMessageWidth(frFontHandle_t *g);

void itfMesMngSetDefaultSpeed(s32 window, u8 channelValue);

void itfMesMngSetKutiCallback(s32 window, void (*callback)(void));

void itfMesMngInitializeManager(void);

extern sdfTexHandle_t *itfMiscLoadTexture(const char *path);

extern void itfMiscLinkInitialize(itfMesLinkControl_t *pool, itfMesLink_t *nodes, s32 count, s32 stride);

#define ITF_MES_WINDOW_CAPACITY 0x80

#define ITF_MES_POOL_NODE_BYTES 0x14

extern dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         itfProcessShutdown_t destroy, u32 userValue);

s32 CalcMsgMng_Process(void);

void DrawMsgMng_Shutdown(void);

s32 DrawMsgMng_Process(dds3ProcessID_t *task);

void itfMesMngSetBitStatManager(u32 mask);

void itfMesMngClearBitStatManager(u32 mask);

u16 itfMesMngGetStatManager(void);

s32 itfMesMngDebugProc(void);

extern itfMesDebug_t D_00326700;

itfMesLink_t *DebugChangeInstance(itfMesLink_t *node);

extern itfPadState_t dds3PadButtonState;

void DebugMoveWinVert(itfMesWindow_t *panel, s32 boundsMode);

void DebugMoveWinAlpha(itfMesWindow_t *stepper);

sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

extern void dds3ConsPanelTagLink(sdfDrawTag_t *packetList, s32 column, s32 row,
                        s32 columnCount, s32 rowCount);

extern char D_0054F4E8[];

extern char *D_0054F4D0[2];

extern char D_0054F4F0[];

extern void sdfDrawTagLink(sdfDrawTag_t *list, u32 packetAddress);

extern void *sdfDevPrintf(s32 source, s32 end, s32 argument, s32 index,
                                  const char *format, ...);

extern char *D_00326710[];

extern char D_0054F4F8[];

extern sdfGrOt_t sdfGrOtTable[];

void itfMesMngDebugShutdown(void);

/* Slot record reached through the pool base, one 0x14 bytes per window. */
extern itfMesLink_t D_0056BE80[];

extern void ReleaseAllWindow(struct itfMesWork_t *effect);

void ReleaseAllVariable(itfMesVariable_t *slots);

extern void sdfMemFreeDirect(sdfMemHandle_t *allocation);

extern void itfMiscLinkGive(itfMesLink_t *node, itfMesLinkControl_t *pool);

u32 CheckMessageBinary(itfMesResource_t *resource);

#define ITF_MES_MAGIC_MSG0 0x3047534d

extern u32 strlen(const char *text);

#define ITF_MES_STRING_COPY_ROUND_BIAS 4

#define ITF_MES_COPY_ALIGN_MASK 3

extern void *memcpy(void *destination, const void *source, u32 size);

#define ITF_MES_BINARY_COPY_ROUND_BIAS 5

extern void *memset(void *destination, s32 value, u32 size);

frFontHandle_t *UnchainFontHandleUnit(frFontHandle_t *glyph, s32 from, s32 to);

void ChangeFontAlpha(frFontHandle_t *glyph, u32 color);

#define ITF_MES_COLOR_BYTE_CLEAR_MASK 0xffffff00

void OpenCheckSpeakerWindow(itfMesWork_t *panel);

extern itfPanel_t *itfPnlRegist2(s32 kind, u32 payload);

void itfPnlSetPosition(itfPanel_t *panel, s32 left, s32 top, s32 right, s32 bottom, s32 renderValue);

void itfPnlSetColor(itfPanel_t *panel, s32 redValue, s32 greenValue, s32 blueValue, s32 alphaValue);

void CalcMesFlow(itfMesWork_t *object);

extern void CalcWindow(itfMesWork_t *panel);

extern void CalcMessage(itfMesWork_t *mes);

extern void CalcSelect(itfMesWork_t *);

void CalcCurCtr(itfMesWork_t *panel);

s32 SelectItem(itfMesWork_t *panel);

void SelectItemMove(itfMesSelect_t *obj, s32 dir);

extern void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

s32 SelectItemDefKey(itfMesSelect_t *selection);

void ChangeFontColor(frFontHandle_t *g, s32 row, s32 rows, s32 value);

extern u32 dds3NullDrawProc(void);

void DrawMesFlow(itfMesWork_t *panel);

extern void sdfTexFreeDelayed(sdfTexHandle_t *texture);

void DebugDrawWindowLine(itfPanel_t *sprite);

extern s32 sdfGrAllocPacketBuff(s32 size);

extern void sdfDrawTagInit(sdfDrawTag_t *list);

extern void itfMiscDrawLineStripPrim(void *, void *, u8 *, u8 *, s32, u32, sdfDrawTag_t *);

extern s32 D_00326798[];

extern u8 D_0054F500[5];

extern u8 D_0054F508[5];

void itfPnlSetPositionOffset(itfPanel_t *panel, s32 leftDelta, s32 topDelta, s32 rightDelta, s32 bottomDelta, s32 renderValueDelta);

void sdfAddressResolver(s32 *dst, s32 base, u8 *table, s32 len);
void frSetVariableRequest(s32 slot, s32 value);
void itfMesMngSetSelect(s32 window, s32 count);
void itfMesMngMarkBitStat(s32 window, u32 mask);
void itfMesMngClearBitStat(s32 window, u32 mask);
void itfMesMngSetVar(s32 window, s32 slot, s32 value, s32 selector);
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

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetOffsetY1);

s8 itfMesMngGetPhase(s32 window)
{
    return pinst[window].mes->message.textState;
}

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 windowIndex, s8 status) {
    pinst[windowIndex].mes->message.textState = status;
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
s16 itfMesMngGetSelPhase(s32 windowIndex) {
    return pinst[windowIndex].mes->selection.selectionState;
}

void itfMesMngSetSelPhase(s32 window, s16 phase)
{
    pinst[window].mes->selection.selectionState = phase;
}

/* Return the same signed word forwarded as the script result when an option completes. */
s16 itfMesMngGetSelectNo(s32 windowIndex) {
    return pinst[windowIndex].mes->selection.selectedIndex;
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
void itfMesMngMsgWndCls(s32 window) {
    itfMesWork_t *mes = pinst[window].mes;
    u32 previousFlags = mes->flags;

    if (previousFlags & ITF_MES_LOWER_STATUS_PAIR_MASK) {
        mes->flags = previousFlags | ITF_MES_LOWER_STATUS_PAIR_MASK;
    }
    if (previousFlags & ITF_MES_UPPER_STATUS_PAIR_MASK) {
        mes->flags = mes->flags | ITF_MES_UPPER_STATUS_PAIR_MASK;
    }
    itfMesMngRequestMessageRelease(window, 1);
    itfMesMngRequestSelectRelease(window);
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
void itfMesMngRelease(s32 window) {
    if (window >= 0 && pinst[window].mes != NULL) {
        DestructFlow(window);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestMessage);

/* Release/reset selected-entry glyphs and their status; optionally do the same
 * for primary text. Negative window is a no-op; no other slot validation. */
void itfMesMngRequestMessageRelease(s32 window, s32 releasePrimaryBlock) {
    itfMesWork_t *mes;
    itfMesMessage_t *message;
    itfMesSpeaker_t *primaryTextBlock;

    if (window < 0) {
        return;
    }
    mes = pinst[window].mes;
    message = &mes->message;
    primaryTextBlock = &mes->speaker;
    if (message->glyphChain != NULL) {
        frDestroyFontRequestAfter(message->glyphChain);
        message->glyphChain = NULL;
    }
    InitializeMessage(message, 0);
    mes->flags &= ~ITF_MES_ENTRY_STATE_MASK;
    mes->flags &= ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK;
    if (releasePrimaryBlock == 0) {
        return;
    }
    if (primaryTextBlock->glyphChain != NULL) {
        frDestroyFontRequestAfter(primaryTextBlock->glyphChain);
        primaryTextBlock->glyphChain = NULL;
    }
    InitializeSpeaker(primaryTextBlock, 0);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestSelect);

/* Release option glyphs, clear option selection/state and invalidate its rank.
 * Other content and the window allocation remain installed. */
void itfMesMngRequestSelectRelease(s32 window) {
    itfMesWork_t *mes = pinst[window].mes;
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

void itfMesMngSetMaskSelect(s32 window, u32 mask)
{
    pinst[window].mes->selection.panelValue = mask;
}

void itfMesMngSetSelect(s32 window, s32 count)
{
    itfMesSelect_t *blk = &pinst[window].mes->selection;
    u32 mask = blk->panelValue;
    s32 bit = mask & 1;
    s32 n = bit == 0;
    s32 i = 0;

    if (count > 0) {
        do {
            i++;
            mask >>= 1;
            bit = mask & 1;
            if (bit == 0) {
                n++;
            }
        } while (i < count);
    }
    n = bit == 0 ? n - 1 : 0;
    blk->selectedIndex = n;
    blk->savedIndex = n;
}

/* Move primary text to an absolute position; an unchanged position is a no-op. */
void itfMesMngSetSpeakerPosition(s32 window, s32 x, s32 y) {
    itfMesSpeaker_t *primaryTextBlock = &pinst[window].mes->speaker;
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
void itfMesMngSetSpeakerPositionOffset(s32 window, s32 dx, s32 dy) {
    itfMesSpeaker_t *primaryTextBlock = &pinst[window].mes->speaker;

    ChangeFontPosition(primaryTextBlock->glyphChain, dx, dy);
    primaryTextBlock->x += dx;
    primaryTextBlock->y += dy;
}

/* Move selected-entry text absolutely; an unchanged position is a no-op. */
void itfMesMngSetMesPosition(s32 window, s32 x, s32 y) {
    itfMesMessage_t *message = &pinst[window].mes->message;
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
void itfMesMngSetMesPositionOffset(s32 window, s32 dx, s32 dy) {
    itfMesMessage_t *message = &pinst[window].mes->message;

    ChangeFontPosition(message->glyphChain, dx, dy);
    message->x += dx;
    message->y += dy;
}

/* Move option text absolutely; an unchanged position is a no-op. */
void itfMesMngSetSelPosition(s32 window, s32 x, s32 y) {
    itfMesSelect_t *optionBlock = &pinst[window].mes->selection;
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
void itfMesMngSetSelPositionOffset(s32 window, s32 dx, s32 dy) {
    itfMesSelect_t *optionBlock = &pinst[window].mes->selection;

    ChangeFontPosition(optionBlock->glyphChain, dx, dy);
    optionBlock->x += dx;
    optionBlock->y += dy;
}

/* Propagate a changed render value to all three glyph chains before caching it. */
void itfMesMngSetPriority(s32 window, s32 renderValue) {
    itfMesWork_t *mes = pinst[window].mes;

    if (mes->renderValue == renderValue) {
        return;
    }
    ChangeZBuffer(mes->speaker.glyphChain, renderValue);
    ChangeZBuffer(mes->message.glyphChain, renderValue);
    ChangeZBuffer(mes->selection.glyphChain, renderValue);
    mes->renderValue = renderValue;
}

u32 itfMesMngGetStat(s32 window)
{
    return pinst[window].mes->flags;
}

/* Replace only the high half, preserving the existing low-half cast. */
void itfMesMngSetStat(s32 window, u32 highFlags) {
    itfMesWork_t *mes = pinst[window].mes;

    mes->flags = (u32)(u16)mes->flags | (highFlags & ITF_MES_HIGH_FLAGS_MASK);
}

void itfMesMngMarkBitStat(s32 window, u32 mask)
{
    itfMesWork_t *rec = pinst[window].mes;

    rec->flags |= mask & 0xFFFF0000;
}

void itfMesMngClearBitStat(s32 window, u32 mask)
{
    itfMesWork_t *rec = pinst[window].mes;

    rec->flags &= ~mask | 0xFFFF;
}

/* Return the cached clear-bit rank; reset state uses -1. */
s16 itfMesMngGetSelect(s32 window) {
    return pinst[window].mes->selection.selectedIndex;
}

/* Return the selected table's cached item count, without another table lookup. */
s16 itfMesMngGetMesPage(s32 window) {
    return pinst[window].mes->message.tableCount;
}

/* Return the stored signed-halfword item index, without another table lookup. */
s16 itfMesMngGetMesCurrentPage(s32 window) {
    return pinst[window].mes->message.itemIndex;
}

/* Return the replacement text stored in a window slot. */
u8 *itfMesMngGetVar(s32 window, s32 slotIndex) {
    return (u8 *)pinst[window].mes->variables.addresses[slotIndex];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetVarLocalize);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetVar);

/* Replace a text slot with a copied NUL-terminated encoded string. */
void itfMesMngSetVarImmediate(s32 window, u32 slotIndex, const char *sourceText) {
    SetVar(pinst[window].mes, slotIndex, sourceText, 0);
}

/* Copy text into one of the window's message text slots. */
void itfMesMngSetVarImmediate2(s32 window, s32 slotIndex, const char *source, s32 byteCount) {
    SetVar(pinst[window].mes, slotIndex, source, byteCount);
}

/* Install/relocate a message resource and cache MSG1's trailing-entry word as
 * the temporary font entry. Return the previous resource without releasing it. */
itfMesResource_t *itfMesMngSetResourcePtr(s32 window, itfMesResource_t *sub) {
    itfMesWork_t *mes = pinst[window].mes;
    itfMesResource_t *previous = mes->sub;
    itfMesEntry_t *entry;
    u8 *temporaryFontEntry;

    mes->sub = sub;
    ResolveResourceAddress((itfMesResource_t *)sub);
    entry = GetTypeHeader2(sub);
    temporaryFontEntry = 0;
    if (sub->magic == ITF_MES_MAGIC_MSG1) {
        temporaryFontEntry = *(u8 **)((u8 *)entry + 8);
    }
    mes->temporaryFontEntry = temporaryFontEntry;
    return previous;
}

/* Look up an entry-table item; only the item index has a bounds check. */
u8 *itfMesMngGetDataPtrImmediate(s32 window, s32 entryIndex, s32 itemIndex) {
    return GetMessagePagePtr(GetTypeHeader(pinst[window].mes, entryIndex)->table, itemIndex);
}

/* Refresh changed panel values only when both content-status pairs are clear.
 * Release the old primitive after applying the panel mode and fade limit. */
void itfMesMngChangeWindowType(s32 window, s32 panelMode, s32 fadeLimit) {
    itfMesWork_t *mes = pinst[window].mes;
    itfMesWindow_t *panelBlock = &mes->window;

    if ((mes->flags & ITF_MES_UPPER_STATUS_PAIR_MASK) == 0) {
        if (mes->panelMode != panelMode || panelBlock->fadeLimit != fadeLimit) {
            mes->panelMode = panelMode;
            ResetWindowPara(panelBlock, panelMode, fadeLimit);
            if (panelBlock->sprite != NULL) {
                itfPnlRelease(panelBlock->sprite);
                panelBlock->sprite = NULL;
            }
        }
    }
}

/* Set or append the option with `id` in the window's option list; ids 1, 6, 7 and >= 16 are rejected. */
s32 itfMesMngSetSelectDefKey(s32 window, s32 id, s32 value) {
    itfMesSelect_t *optionBlock = &pinst[window].mes->selection;
    itfMesOption_t *option = NULL;
    s32 optionIndex;

    if ((u32)id >= 0x10) {
        return 0;
    }
    if (id == 1 || id == 6 || id == 7) {
        return 0;
    }
    for (optionIndex = 0; optionIndex < optionBlock->optionCount; optionIndex++) {
        if (optionBlock->options[optionIndex].id == id) {
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
    option->id = id;
    option->value = value;
    return 1;
}

/* Build and release a temporary glyph chain to measure the widest grouped row.
 * Empty tables and zero/missing encoded items return zero. */
s32 itfMesMngGetWidth(s32 window, s32 entryIndex, s32 itemIndex) {
    itfMesWork_t *mes = pinst[window].mes;
    itfMesTable_t *table;
    u8 *encodedText;
    frFontHandle_t *glyphChain;
    s32 textExtent;

    table = GetTypeHeader(mes, entryIndex)->table;
    if (table->count == 0) {
        return 0;
    }
    CommitVarTable((s32 *)mes->variables.addresses);
    if (mes->temporaryFontEntry != 0) {
        frSetExtDic(mes->temporaryFontEntry);
    }
    encodedText = GetMessagePagePtr(table, itemIndex);
    if (encodedText == 0) {
        return 0;
    }
    glyphChain = MessageRequest(0, 0, encodedText, 0);
    textExtent = GetMessageWidth(glyphChain);
    frDestroyFontRequestAfter(glyphChain);
    return textExtent;
}

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesMngSetDefaultSpeed(s32 window, u8 channelValue) {
    pinst[window].mes->message.color[3] = channelValue;
}

/* Store the optional callback address invoked without arguments during glyph building. */
void itfMesMngSetKutiCallback(s32 window, void (*callback)(void)) {
    pinst[window].mes->callbackAddress = (u32)callback;
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
void itfMesMngSetBitStatManager(u32 mask) {
    itfMesMngManager.flags |= mask;
}

/* Clear manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesMngClearBitStatManager(u32 mask) {
    itfMesMngManager.flags &= ~mask;
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
void DestructFlow(s32 window) {
    itfMesLink_t *windowRecord = &D_0056BE80[window];
    itfMesWork_t *mes = windowRecord->state;

    if (mes != NULL) {
        itfMesMngRequestMessageRelease(window, 1);
        itfMesMngRequestSelectRelease(window);
        ReleaseAllWindow(mes);
        ReleaseAllVariable(&mes->variables);
        mes->flags = 0;
        sdfMemFreeDirect(windowRecord->resourceHandle);
        windowRecord->state = NULL;
        itfMiscLinkGive(windowRecord, (itfMesLinkControl_t *)((u8 *)D_0056BE80 - 0x10));
        ((itfMesManager_t *)((u8 *)D_0056BE80 - 0x20))->activeWindowCount -= 1;
    }
}

void ResolveResourceAddress(itfMesResource_t *sub)
{
    s32 *payload = (s32 *)sub->entries;

    if (sub->relocated == 0) {
        sdfAddressResolver(payload, (s32)payload, (u8 *)sub + sub->fixupTableOffset, sub->fixupTableBytes);
        sub->relocated = 1;
    }
}

/* Recognize MSG0/MSG1 magic only; this is not complete format validation. */
u32 CheckMessageBinary(itfMesResource_t *resource) {
    u32 isMessageResource;

    isMessageResource = 0;
    if (resource->magic == ITF_MES_MAGIC_MSG0 || resource->magic == ITF_MES_MAGIC_MSG1) { /* "MSG0"/"MSG1" */
        isMessageResource = 1;
    }
    return isMessageResource;
}

itfMesEntry_t *GetTypeHeader(itfMesWork_t *mes, s32 index)
{
    itfMesEntry_t *entries = mes->sub->entries;

    return &entries[index];
}

itfMesEntry_t *GetTypeHeader2(itfMesResource_t *sub)
{
    itfMesEntry_t *entries = sub->entries;

    return &entries[sub->entryCount];
}

u8 *GetMessagePagePtr(itfMesTable_t *table, s32 index)
{
    s32 count = table->count;

    if (index < 0 || index >= count) {
        return NULL;
    }
    return (u8 *)table->items[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", ReadytoMessage);

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

void CommitVarTable(s32 *table)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        frSetVariableRequest(i, table[i]);
    }
}

u8 *GetSpeakerName(itfMesWork_t *mes)
{
    u32 *items = GetTypeHeader2(mes->sub)->itemList;

    return (u8 *)items[mes->speaker.selectedIndex];
}

s32 GetNumSelectItem(s32 count, u32 mask)
{
    s32 n = 0;

    while (count > 0) {
        u32 bit = mask & 1;

        mask >>= 1;
        count--;
        if (bit == 0) {
            n++;
        }
    }
    return n;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", GetFontHandleUnit);

/* Discard (to - from - 1) preceding rows when positive, retain the next
 * complete y-group and queue everything after it. Requires non-NULL input;
 * return NULL if discarding preceding rows exhausts the chain. */
frFontHandle_t *UnchainFontHandleUnit(frFontHandle_t *glyph, s32 from, s32 to) {
    s32 rowsToDiscard = to - from - 1;
    s32 rowY = glyph->y;
    frFontHandle_t *nextNode;
    frFontHandle_t *rowHead;
    frFontHandle_t *rowTail;

    while (rowsToDiscard > 0) {
        while (rowY == glyph->y) {
            nextNode = glyph->previous;
            glyph->next = NULL;
            glyph->chainHead = glyph;
            glyph->previous = NULL;
            frDestroyFontRequestAfter(glyph);
            glyph = nextNode;
            if (glyph == NULL) {
                return NULL;
            }
        }
        rowsToDiscard--;
        rowY = glyph->y;
    }
    rowHead = glyph;
    do {
        rowTail = glyph;
        glyph = glyph->previous;
    } while (glyph != NULL && rowY == glyph->y);
    while (glyph != NULL) {
        nextNode = glyph->previous;
        glyph->next = NULL;
        glyph->chainHead = glyph;
        glyph->previous = NULL;
        frDestroyFontRequestAfter(glyph);
        glyph = nextNode;
    }
    rowHead->next = NULL;
    rowTail->previous = NULL;
    for (glyph = rowHead; glyph != NULL; glyph = glyph->previous) {
        /* Despite its current name, this link points to the retained row tail. */
        glyph->chainHead = rowTail;
    }
    return rowHead;
}

frFontHandle_t *GetLastLinkFontHandle(frFontHandle_t *g)
{
    while (g->previous != NULL) {
        g = g->previous;
    }
    return g;
}

void GetLastMessagePara(frFontHandle_t *g, itfMesMessage_t *blk)
{
    frTextHandle_t *shade = g->lastChild;
    u8 ctx = g->glyphCodeOrContext.byteRoles.encodedContextByte;

    blk->color[3] = ctx >> 1;
    blk->color[0] = shade->renderValueOrSetupOrShade.setupBytes.fontIndex;
    blk->color[1] = shade->renderValueOrSetupOrShade.setupBytes.firstOption;
    blk->color[2] = shade->renderValueOrSetupOrShade.setupBytes.secondOption;
}

s32 CountLineMessage(frFontHandle_t *last, frFontHandle_t *first)
{
    s16 h = first->parentDimensionsOrRenderWord.parentDimensions.cellHeight;

    return ((first->y - last->y) >> 3) / h + 1;
}

/* Translate every linked node in text units; a NULL chain is a no-op. */
void ChangeFontPosition(frFontHandle_t *glyph, s32 dx, s32 dy) {
    if (glyph == NULL) {
        return;
    }
    do {
        glyph->x += dx;
        glyph->y += dy;
        glyph = glyph->previous;
    } while (glyph != NULL);
}

void ChangeZBuffer(frFontHandle_t *g, s32 z)
{
    while (g != NULL) {
        g->renderValueOrSetupOrShade.renderValue = z;
        g = g->previous;
    }
}

void ChangeFontColor(frFontHandle_t *g, s32 row, s32 rows, s32 value)
{
    s32 skip = rows - row - 1;
    s32 y = g->y;
    s32 cur = y;
    frTextHandle_t *c;
    u8 flag;

    while (skip > 0) {
        while (y == cur) {
            g = g->previous;
            if (g == NULL) {
                return;
            }
            cur = g->y;
        }
        skip--;
        y = cur;
    }
    flag = value;
    do {
        for (c = g->firstChild; c != NULL; c = c->next) {
            c->renderValueOrSetupOrShade.setupBytes.firstOption = flag;
        }
        g = g->previous;
    } while (g != NULL && y == g->y);
}

void ChangeFontColorAll(frFontHandle_t *g, u8 flag)
{
    frTextHandle_t *c;

    for (; g != NULL; g = g->previous) {
        for (c = g->firstChild; c != NULL; c = c->next) {
            c->renderValueOrSetupOrShade.setupBytes.firstOption = flag;
        }
    }
}

/* Clear each child's low color byte, then OR the unmasked input word.
 * High input bits can therefore also change the upper bytes. */
void ChangeFontAlpha(frFontHandle_t *glyph, u32 color) {
    frTextHandle_t *child;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (child = glyph->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = child->parentDimensionsOrRenderWord.renderWord & ITF_MES_COLOR_BYTE_CLEAR_MASK | color;
        }
    }
}

s32 GetMessageWidth(frFontHandle_t *g)
{
    s32 max = 0;

    while (g != NULL) {
        s32 y = g->y;
        s32 w = 0;

        do {
            w += g->advance;
            g = g->previous;
        } while (g != NULL && y == g->y);
        if (w > max) {
            max = w;
        }
    }
    return max << 4;
}

s32 GetMaskedIndex(s32 skip, u32 mask)
{
    s32 i = 0;

    while (i < 0x20) {
        if ((mask & 1) == 0) {
            if (--skip < 0) {
                break;
            }
        }
        i++;
        mask >>= 1;
    }
    return i;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", RaiseMessageSpeedMax);

INCLUDE_ASM(const s32, "interface/itfMesManager", RaiseMessageSpeedMaxAll);

INCLUDE_ASM(const s32, "interface/itfMesManager", OpenMessageWindow);

void OpenCheckSpeakerWindow(itfMesWork_t *panel) {
    itfMesSpeaker_t *origin = &panel->speaker;
    itfMesWindow_t *place = &panel->window;
    if (origin->glyphChain != NULL && !(panel->flags & 0x10000)) {
        if (place->frame == NULL) {
            s32 width = origin->glyphChain->advance * 16;
            place->frame = itfPnlRegist2(7, (u32)itfMesMngManager.windowTexture);
            itfPnlSetPosition(place->frame, origin->x - 0x2D0, origin->y - 0x68, origin->x + width + 0x2D0, origin->y + 0xE8, panel->renderValue);
            itfPnlSetColor(place->frame, 0x7F, 0x7F, 0x7F, 0);
        }
        panel->flags = (panel->flags & ~0x3000) | 0x1000;
    } else if (place->frame != 0) {
        panel->flags |= 0x3000;
    }
}

void InitializeSpeaker(itfMesSpeaker_t *cursor, s32 resetPosition) {
    if (resetPosition != 0) {
        cursor->x = 0x430;
        cursor->y = 0x9B0;
    }
    cursor->glyphChain = 0;
    cursor->selectedIndex = 0xffff;
}

void InitializeMessage(itfMesMessage_t *cur, s32 resetPos) {
    if (resetPos != 0) {
        cur->x = 0x4B0;
        cur->y = 0xA88;
    }
    cur->glyphChain = NULL;
    cur->textState = 0;
    cur->textInterfaceMask = 0;
    cur->lineCount = 0;
    cur->itemIndex = 0;
    cur->tableCount = 0;
    cur->color[0] = 0;
    cur->color[1] = 0;
    cur->color[2] = 0;
    cur->color[3] = 0x80;
    cur->table = NULL;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeSelect);

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeWindow);

void InitializeVariable(itfMesVariable_t *slots)
{
    s32 n = 0x1F;
    u32 *p = &slots->addresses[31];

    do {
        n--;
        *p = 0;
        p--;
    } while (n >= 0);
}

void InitializeCurCtr(itfMesCursor_t *c, s32 preserve)
{
    if (preserve == 0) {
        c->kind = 0;
    }
    c->alpha = 0x40;
    c->phase = 0;
    c->timer = 0;
    c->frames = 0;
}

void ForceSetCurCtr(itfMesCursor_t *c, s16 phase, s16 alpha, s16 timer)
{
    c->phase = phase;
    c->alpha = alpha;
    c->timer = timer;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", ReleaseAllWindow);

void ReleaseAllVariable(itfMesVariable_t *slots)
{
    s32 remaining = 0x1F;

    do {
        s32 index = 0x1F - remaining;

        if (slots->addresses[index] != 0) {
            sdfMemFreeDirect(slots->handles[index]);
            slots->addresses[index] = 0;
        }
        remaining--;
    } while (remaining >= 0);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", ConvertZenkakuNumber);

INCLUDE_ASM(const s32, "interface/itfMesManager", ResetWindowPara);

void CalcMesFlow(itfMesWork_t *object) {
    CalcWindow(object);
    CalcMessage(object);
    CalcSelect(object);
    CalcCurCtr(object);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcMessage);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcSelect);

s32 SelectItem(itfMesWork_t *panel) {
    itfMesSelect_t *sel = &panel->selection;
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
        InitializeCurCtr(&panel->fade, 1);
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

void SelectItemMove(itfMesSelect_t *obj, s32 dir) {
    s32 cur = obj->selectedIndex;
    u16 maxv;
    ChangeFontColor(obj->glyphChain, cur, obj->rowCount, 0);
    if (dir < 0) {
        cur = cur - 1;
        if (cur < 0) {
            cur = obj->rowCount - 1;
        }
        maxv = obj->rowCount;
    } else {
        cur = cur + 1;
        if (cur >= obj->rowCount) {
            cur = 0;
        }
        maxv = obj->rowCount;
    }
    ChangeFontColor(obj->glyphChain, cur, (s16)maxv, 1);
    obj->selectedIndex = cur;
    obj->savedIndex = cur;
    p2sdPlaySE(1, 0x7F, 0x3F);
}

s32 SelectItemDefKey(itfMesSelect_t *selection) {
    s32 i;

    for (i = 0; i < selection->optionCount; i++) {
        itfMesOption_t *option = &selection->options[i];

        if (dds3PadButtonState.buttons[option->id] < 0) {
            s32 prefixLength = option->value;
            s32 rank = 0;
            u32 mask = selection->panelValue;

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

void CalcCurCtr(itfMesWork_t *panel) {
    itfMesCursor_t *fade = &panel->fade;
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

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMesFlow);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawSelectCursor);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawRetPageCursor);

s32 CalcMsgMng_Process(void) {
    itfMesLink_t *node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        CalcMesFlow(node->state);
        node = node->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMsgMng_Process);

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

void DebugDrawWindowLine(itfPanel_t *sprite) {
    s32 vertices[4][2] = {
        {sprite->left, sprite->top},
        {sprite->right, sprite->top},
        {sprite->right, sprite->bottom},
        {sprite->left, sprite->bottom}
    };
    sdfDrawTag_t *list;
    sdfGrOt_t *packetOwner;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    itfMiscDrawLineStripPrim(vertices, D_00326798, D_0054F500, D_0054F508, 5, 0xFFFFFF, list);
    packetOwner = &sdfGrOtTable[81];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))packetOwner->regFunc)(packetOwner, list);
}

void DebugMoveWinVert(itfMesWindow_t *panel, s32 boundsMode) {
    itfPanel_t *sprite = panel->sprite;
    s32 *bounds;
    s32 dx;
    s32 dy;

    if (sprite != NULL) {
        bounds = panel->bounds;
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
            switch (boundsMode) {
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
void DebugMoveWinAlpha(itfMesWindow_t *stepper) {
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
        s32 index = (stepper->fadeLimit + step) & 0xFF;

        stepper->fadeLimit = index;
        if (stepper->sprite != NULL) {
            stepper->sprite->alpha = index;
        }
    }
}

itfMesLink_t *DebugChangeInstance(itfMesLink_t *node) {
    s32 index = 0;
    s32 count = itfMesMngManager.activeWindowCount;

    if (count <= 0) {
        return NULL;
    }
    for (;;) {
        if (node != NULL) {
            node = node->previous;
        }
        if (node == NULL) {
            node = itfMesMngManager.pool.activeTail;
        }
        if ((node->state)->window.sprite != NULL) {
            break;
        }
        index++;
        if (count < index) {
            node = NULL;
            break;
        }
    }
    return node;
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

