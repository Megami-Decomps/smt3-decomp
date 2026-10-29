#include "interface.h"
#include "kernel.h"

typedef struct itfMesBinMesHeader_t itfMesBinMesHeader_t;
typedef struct itfMesBinTypeHeader_t itfMesBinTypeHeader_t;
typedef struct itfMesResource_t itfMesResource_t;
typedef struct Speaker_tag Speaker_tag;
typedef struct message_tag message_tag;
typedef struct defkey_tag defkey_tag;
typedef struct select_tag select_tag;
typedef struct window_tag window_tag;
typedef struct variable_tag variable_tag;
typedef struct curctr_tag curctr_tag;
typedef struct instanceMes_tag instanceMes_tag;
typedef struct itfMesLink_t itfMesLink_t;
typedef struct itfMesLinkControl_t itfMesLinkControl_t;
typedef struct itfMesManager_t itfMesManager_t;
typedef struct debug_tag debug_tag;
typedef struct itfPadState_t itfPadState_t;

/* Message tables contain relocated encoded-text addresses. */
struct itfMesBinMesHeader_t {
    u8 label[0x18];
    s16 page;
    s16 speaker;
    u32 addr[1];
};

struct itfMesBinTypeHeader_t {
    /* Relocated item-address ptr; address 1 marks a branch entry. */
    u32 *type;
    itfMesBinMesHeader_t *ptr;
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
    itfMesBinTypeHeader_t entries[1];
};

struct Speaker_tag {
    u32 x;
    u32 y;
    frFontHandle_t *pfont;
    u16 num;
    u8 pad0E[2];
};

struct message_tag {
    u32 x;
    s32 y;
    itfMesBinMesHeader_t *table;
    frFontHandle_t *pfont;
    s8 phase;
    u8 textInterfaceMask; /* 0x11: result of itfTestTextInterfaceMask(3). */
    u8 color[4];
    s16 lines;
    s16 page;
    s16 maxpage;
};

struct defkey_tag {
    s16 key;
    s16 index;
};

struct select_tag {
    u32 x;
    u32 y;
    frFontHandle_t *pfont;
    u32 mask;
    s16 phase;
    s16 select;
    s16 select_imm;
    s16 lines;
    s16 glyphColorIntensity;
    s16 numdef;
    defkey_tag defkey[15];
};

/* The speaker frame and message panel occupy +0x9C/+0xA0. */
struct window_tag {
    itfpanel_t *spk;
    itfpanel_t *mes;
    s32 mes_offset[4];
    s32 red;
    s32 green;
    s32 blue;
    s32 fadeLimit;
};

struct variable_tag {
    u32 pptr[0x20];
    sdfMemHandle_t *pmh[0x20];
};

/* Native 0xC-byte fade record shared by message windows and sound UI state. */
struct curctr_tag {
    u8 isdraw;
    u8 pad1;
    s16 phase;
    s16 ctr0;
    s16 ctr1;
    u32 ctr_key; /* 0x08: entry advances count ctr_key before triangle skip. */
};

struct instanceMes_tag {
    u32 stat;
    itfMesResource_t *pdata;
    u8 *pext; /* 0x08: relocated font resource bytes from the MSG1 trailing entry. */
    s32 z;
    s16 otindex;
    s16 type;
    Speaker_tag speaker;
    message_tag message;
    select_tag select;
    u8 unk98[4];
    window_tag window;
    variable_tag variable;
    curctr_tag curctr;
    u32 func_kuti;
};

/* The message manager owns 128 native 0x14-byte link records. */
struct itfMesLink_t {
    struct itfMesLink_t *previous;
    struct itfMesLink_t *next;
    s32 index;
    instanceMes_tag *state;
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
    u32 ctr;
    u16 flags;
    u16 unkE;
    itfMesLinkControl_t pool;
    itfMesLink_t nodes[0x80];
};

/* State of the interactive message-layout inspector. */
struct debug_tag {
    s32 cursor;
    s32 mode;
    itfMesLink_t *pmesflow;
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

typedef struct { instanceMes_tag *mes; u8 unk04[0x10]; } itfMesInstance_t;
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

void InitializeSpeaker(Speaker_tag *pspeaker, s32 defaultpos);

void InitializeMessage(message_tag *pmessage, s32 defaultpos);

extern void InitializeSelect(struct select_tag *pselect);

void InitializeWindow(window_tag *pwindow);

void InitializeVariable(variable_tag *pvariable);

void InitializeCurCtr(curctr_tag *pcurctr, s32 isdisp);

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

void SetVar(instanceMes_tag *pinst, s32 index, const char *pstr, s32 length);

void itfMesMngSetVarImmediate2(s32 id, s32 index, const char *pstr, s32 length);

void ResolveResourceAddress(itfMesResource_t *pdata);

itfMesBinTypeHeader_t *GetTypeHeader2(itfMesResource_t *pfileheader);

#define ITF_MES_MAGIC_MSG1 0x3147534d

u8 *itfMesMngGetDataPtrImmediate(s32 id, s32 message, s32 page);

u8 *GetMessagePagePtr(itfMesBinMesHeader_t *table, s32 index);

itfMesBinTypeHeader_t *GetTypeHeader(instanceMes_tag *pinst, s32 index);

void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha);

#define ITF_MES_CONTENT_STATUS_MASK 0x3300

extern void ResetWindowPara(window_tag *pwin, s32 type, s32 alpha);

void itfPnlRelease(itfpanel_t *handle);

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

extern debug_tag D_00326700;

itfMesLink_t *DebugChangeInstance(itfMesLink_t *pflow);

extern itfPadState_t dds3PadButtonState;

void DebugMoveWinVert(window_tag *pwin, s32 flag);

void DebugMoveWinAlpha(window_tag *pwin);

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

extern void ReleaseAllWindow(struct instanceMes_tag *pinst);

void ReleaseAllVariable(variable_tag *pvariable);

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

void OpenCheckSpeakerWindow(instanceMes_tag *pinst);

void OpenMessageWindow(instanceMes_tag *pinst);

void ReadytoMessage(instanceMes_tag *panel);

extern itfpanel_t *itfPnlRegist2(s32 type, u32 texhandle);

void itfPnlSetPosition(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);

void itfPnlSetColor(itfpanel_t *handle, s32 r, s32 g, s32 b, s32 a);

void CalcMesFlow(instanceMes_tag *pinst);

extern void CalcWindow(instanceMes_tag *pinst);

extern void CalcMessage(instanceMes_tag *pinst);

extern void CalcSelect(instanceMes_tag *);

void CalcCurCtr(instanceMes_tag *pinst);

s32 SelectItem(instanceMes_tag *pinst);

void SelectItemMove(select_tag *psel, s32 direct);

extern void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

s32 SelectItemDefKey(select_tag *psel);

void ChangeFontColor(frFontHandle_t *pfont, s32 index, s32 lines, s32 color);

extern u32 dds3NullDrawProc(void);

void DrawMesFlow(instanceMes_tag *pinst);

void DrawWindow(instanceMes_tag *pinst);

void DrawRetPageCursor(instanceMes_tag *pinst);

void DrawSelectCursor(instanceMes_tag *pinst);

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

void DebugDrawWindowLine(itfpanel_t *meswin);

extern s32 sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void itfMiscDrawLineStripPrim(void *, void *, u8 *, u8 *, s32, u32, sdfDrawTag_t *);

extern s32 D_00326728[8];


extern u8 D_00326768[];

extern u8 D_00326770[];

extern void itfMiscDrawSpritePlanePrim(void *pos, void *color, s32 z, s32 context, sdfDrawTag_t *pdrawtag);

extern void itfMiscDrawRectanglePrim(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, s32 tail, sdfDrawTag_t *command);

extern void itfMiscDrawSpritePrim2(void *pos, void *uv, void *color, s32 z, s32 context, sdfDrawTag_t *pdrawtag);

extern s32 D_00326778[4];

extern s32 D_00326788[4];

extern void itfMiscDrawSpritePrim(void *pos, void *uv, void *color, s32 z, sdfTexHandle_t *texhandle, s32 context, sdfDrawTag_t *pdrawtag);

extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context);

extern s32 D_00326798[];

extern u8 D_0054F500[5];

extern u8 D_0054F508[5];

void itfPnlSetPositionOffset(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);

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
    instanceMes_tag *record;
    message_tag *entryState;
    s32 entryIndex;
    s8 status;

    if (window < 0) {
        return 1;
    }
    record = pinst[window].mes;
    entryIndex = scrGetIntPara(0);
    entryState = &record->message;
    status = entryState->phase;
    if (status == 0) {
        if (itfMesMngRequestMessage(window, entryIndex, 0) == 0) {
            return 1;
        }
    } else if (status < 0) {
        entryState->phase = 0;
        return 1;
    }
    return 0;
}

void itfMesMngSetOffsetY1(s32 msgid, s32 y1)
{
    itfPnlSetPositionOffset(pinst[msgid].mes->window.mes, 0, y1, 0, 0, 0);
}

s8 itfMesMngGetPhase(s32 msgid)
{
    return pinst[msgid].mes->message.phase;
}

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 msgid, s8 phase) {
    pinst[msgid].mes->message.phase = phase;
}

s32 itfMesMngSelect(void) {
    s32 window;
    s32 entry;
    instanceMes_tag *record;
    select_tag *options;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    record = pinst[window].mes;
    entry = scrGetIntPara(0);
    options = &record->select;
    if (options->phase == 0) {
        itfMesMngRequestSelect(window, entry);
    } else if (options->phase < 0) {
        options->phase = 0;
        scrSetIntReturnValue(options->select);
        itfMesMngRequestSelectRelease(window);
        return 1;
    }
    return 0;
}

/* Return the option-block signed status word, not the separate entry status byte. */
s16 itfMesMngGetSelPhase(s32 msgid) {
    return pinst[msgid].mes->select.phase;
}

void itfMesMngSetSelPhase(s32 msgid, s16 phase)
{
    pinst[msgid].mes->select.phase = phase;
}

/* Return the same signed word forwarded as the script result when an option completes. */
s16 itfMesMngGetSelectNo(s32 msgid) {
    return pinst[msgid].mes->select.select;
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
    instanceMes_tag *mes;
    u32 scriptStatus;

    if (window < 0) {
        return 1;
    }
    mes = pinst[window].mes;
    scriptStatus = mes->stat & ITF_MES_LOWER_STATUS_PAIR_MASK;
    if (scriptStatus == ITF_MES_LOWER_STATUS_FIRST_BIT || scriptStatus == ITF_MES_LOWER_STATUS_SECOND_BIT) {
        itfMesMngMsgWndCls(window);
        fldPanelSet(3);
    }
    return scriptStatus < 1;
}

/* Mark each originally nonzero status pair with both bits, then clean/reset
 * content and clear the script panel bit. The status pairs are set, not cleared. */
void itfMesMngMsgWndCls(s32 msgid) {
    instanceMes_tag *mes = pinst[msgid].mes;
    u32 previousFlags = mes->stat;

    if (previousFlags & ITF_MES_LOWER_STATUS_PAIR_MASK) {
        mes->stat = previousFlags | ITF_MES_LOWER_STATUS_PAIR_MASK;
    }
    if (previousFlags & ITF_MES_UPPER_STATUS_PAIR_MASK) {
        mes->stat = mes->stat | ITF_MES_UPPER_STATUS_PAIR_MASK;
    }
    itfMesMngRequestMessageRelease(msgid, 1);
    itfMesMngRequestSelectRelease(msgid);
    mes->stat &= ITF_MES_CLEAR_SCRIPT_PANEL_BIT;
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
    instanceMes_tag *mes;
    sdfMemHandle_t *handle;

    handle = sdfMemAllocDirect(sizeof(instanceMes_tag));
    node->resourceHandle = handle;
    mes = (instanceMes_tag *)sdfMemLock(handle);
    node->state = mes;
    mes->pdata = NULL;
    itfMesMngSetResourcePtr(window, sub);
    mes->stat = 0;
    mes->otindex = 0x47;
    mes->z = 0xFFFFF0;
    mes->type = 0;
    mes->func_kuti = 0;
    InitializeSpeaker(&mes->speaker, 1);
    InitializeMessage(&mes->message, 1);
    InitializeSelect(&mes->select);
    InitializeWindow(&mes->window);
    InitializeVariable(&mes->variable);
    InitializeCurCtr(&mes->curctr, 0);
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
u8 *GetSpeakerName(instanceMes_tag *pinst);

s32 itfMesMngRequestMessage(s32 window, s32 entryIndex, s32 itemIndex) {
    instanceMes_tag *panel = pinst[window].mes;
    message_tag *mes = &panel->message;
    Speaker_tag *spk;
    itfMesBinMesHeader_t *table;
    u32 flags;
    u32 mode;

    table = GetTypeHeader(panel, entryIndex)->ptr;
    mes->page = itemIndex;
    mes->table = table;
    mes->maxpage = table->page;
    if (table->page == 0) {
        printf("itfMesMngRequestMessage() escape 1\n");
        return 0;
    }
    CommitVarTable((s32 *)panel->variable.pptr);
    if (panel->pext != NULL) {
        frSetExtDic(panel->pext);
    }
    ReadytoMessage(panel);
    spk = &panel->speaker;
    if (spk->num != (u16)table->speaker) {
        u16 nameId = table->speaker;

        if (spk->pfont != NULL) {
            frDestroyFontRequestAfter(spk->pfont);
            spk->pfont = NULL;
        }
        spk->num = nameId;
        if (nameId != 0xFFFF) {
            if (panel->window.spk != NULL) {
                itfPnlRelease(panel->window.spk);
                panel->window.spk = NULL;
            }
            if (nameId & 0x8000) {
                spk->pfont = MessageRequestEx(spk->x, spk->y, 0, 3, 0, 8, itfMesMngGetVar(window, nameId & 0x7FFF), NULL);
            } else {
                spk->pfont = MessageRequestEx(spk->x, spk->y, 0, 3, 0, 8, GetSpeakerName(panel), NULL);
            }
            frSetDrawPosition(spk->pfont, spk->x, spk->y);
            frSetDrawSpeed(spk->pfont, 0xA);
            ChangeZBuffer(spk->pfont, panel->z);
        }
    }
    OpenCheckSpeakerWindow(panel);
    flags = panel->stat;
    if (flags & 0x200000) {
        mode = flags & 0x300;
        if (mode == 0 || mode == 0x300) {
            OpenMessageWindow(panel);
            panel->stat = (panel->stat & ~7) | 2;
        } else {
            panel->stat = (flags & ~7) | 3;
        }
    } else {
        panel->stat = (flags & ~7) | 3;
    }
    mes->phase = 1;
    return 1;
}

/* Release/reset selected-entry glyphs and their status; optionally do the same
 * for primary text. Negative window is a no-op; no other slot validation. */
void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker) {
    instanceMes_tag *mes;
    message_tag *message;
    Speaker_tag *primaryTextBlock;

    if (id < 0) {
        return;
    }
    mes = pinst[id].mes;
    message = &mes->message;
    primaryTextBlock = &mes->speaker;
    if (message->pfont != NULL) {
        frDestroyFontRequestAfter(message->pfont);
        message->pfont = NULL;
    }
    InitializeMessage(message, 0);
    mes->stat &= ~ITF_MES_ENTRY_STATE_MASK;
    mes->stat &= ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK;
    if (cls_speaker == 0) {
        return;
    }
    if (primaryTextBlock->pfont != NULL) {
        frDestroyFontRequestAfter(primaryTextBlock->pfont);
        primaryTextBlock->pfont = NULL;
    }
    InitializeSpeaker(primaryTextBlock, 0);
}

extern s32 printf(const char *format, ...);


/* Called above its definition, where ee-gcc assumed int. */
frFontHandle_t *GetFontHandleUnit(u32 *list, s32 count, u32 mask, s32 x, s32 y, s32 depth);

void itfMesMngRequestSelect(s32 id, s32 select) {
    instanceMes_tag *panel = pinst[id].mes;
    select_tag *sel = &panel->select;
    itfMesBinTypeHeader_t *entry;
    itfMesBinMesHeader_t *table;
    s32 count;
    s32 y;
    frFontHandle_t *chain;

    entry = GetTypeHeader(panel, select);
    if (entry->type != (u32 *)1) {
        printf("itfMesMngRequestSelect() - Invalid Type : %d\n", entry->type);
    }
    if (sel->pfont != NULL) {
        frDestroyFontRequestAfter(sel->pfont);
        sel->pfont = NULL;
    }
    table = entry->ptr;
    count = GetNumSelectItem(table->speaker, sel->mask);
    y = sel->y - count * 160;
    y += 0xB4;
    CommitVarTable((s32 *)panel->variable.pptr);
    if (panel->pext != NULL) {
        frSetExtDic(panel->pext);
    }
    chain = GetFontHandleUnit(&table->addr[1], table->speaker, sel->mask, sel->x, y, panel->z);
    sel->phase = 1;
    sel->lines = count;
    sel->pfont = chain;
    panel->stat = (panel->stat & ~0x38) | 0x10;
}

/* Release option glyphs, clear option selection/state and invalidate its rank.
 * Other content and the window allocation remain installed. */
void itfMesMngRequestSelectRelease(s32 id) {
    instanceMes_tag *mes = pinst[id].mes;
    select_tag *optionBlock = &mes->select;

    if (optionBlock->pfont != NULL) {
        frDestroyFontRequestAfter(optionBlock->pfont);
        optionBlock->pfont = NULL;
    }
    optionBlock->mask = 0;
    optionBlock->phase = 0;
    optionBlock->lines = 0;
    optionBlock->select = -1;
    mes->stat &= ~ITF_MES_OPTION_STATE_MASK;
    mes->stat &= ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK;
}

void itfMesMngSetMaskSelect(s32 id, u32 maskpattern)
{
    pinst[id].mes->select.mask = maskpattern;
}

void itfMesMngSetSelect(s32 id, s32 selno)
{
    select_tag *blk = &pinst[id].mes->select;
    u32 mask = blk->mask;
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
    blk->select = n;
    blk->select_imm = n;
}

/* Move primary text to an absolute position; an unchanged position is a no-op. */
void itfMesMngSetSpeakerPosition(s32 id, s32 x, s32 y) {
    Speaker_tag *primaryTextBlock = &pinst[id].mes->speaker;
    s32 delta[2];

    delta[0] = x - primaryTextBlock->x;
    delta[1] = y - primaryTextBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(primaryTextBlock->pfont, delta[0], delta[1]);
    primaryTextBlock->x = x;
    primaryTextBlock->y = y;
}

/* Translate primary text and update its cached position, including zero deltas. */
void itfMesMngSetSpeakerPositionOffset(s32 id, s32 x, s32 y) {
    Speaker_tag *primaryTextBlock = &pinst[id].mes->speaker;

    ChangeFontPosition(primaryTextBlock->pfont, x, y);
    primaryTextBlock->x += x;
    primaryTextBlock->y += y;
}

/* Move selected-entry text absolutely; an unchanged position is a no-op. */
void itfMesMngSetMesPosition(s32 id, s32 x, s32 y) {
    message_tag *message = &pinst[id].mes->message;
    s32 delta[2];

    delta[0] = x - message->x;
    delta[1] = y - message->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(message->pfont, delta[0], delta[1]);
    message->x = x;
    message->y = y;
}

/* Translate selected-entry text and update its cached position. */
void itfMesMngSetMesPositionOffset(s32 id, s32 x, s32 y) {
    message_tag *message = &pinst[id].mes->message;

    ChangeFontPosition(message->pfont, x, y);
    message->x += x;
    message->y += y;
}

/* Move option text absolutely; an unchanged position is a no-op. */
void itfMesMngSetSelPosition(s32 id, s32 x, s32 y) {
    select_tag *optionBlock = &pinst[id].mes->select;
    s32 delta[2];

    delta[0] = x - optionBlock->x;
    delta[1] = y - optionBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(optionBlock->pfont, delta[0], delta[1]);
    optionBlock->x = x;
    optionBlock->y = y;
}

/* Translate option text and update its cached position. */
void itfMesMngSetSelPositionOffset(s32 id, s32 x, s32 y) {
    select_tag *optionBlock = &pinst[id].mes->select;

    ChangeFontPosition(optionBlock->pfont, x, y);
    optionBlock->x += x;
    optionBlock->y += y;
}

/* Propagate a changed render value to all three glyph chains before caching it. */
void itfMesMngSetPriority(s32 id, s32 z_buf) {
    instanceMes_tag *mes = pinst[id].mes;

    if (mes->z == z_buf) {
        return;
    }
    ChangeZBuffer(mes->speaker.pfont, z_buf);
    ChangeZBuffer(mes->message.pfont, z_buf);
    ChangeZBuffer(mes->select.pfont, z_buf);
    mes->z = z_buf;
}

u32 itfMesMngGetStat(s32 id)
{
    return pinst[id].mes->stat;
}

/* Replace only the high half, preserving the existing low-half cast. */
void itfMesMngSetStat(s32 id, u32 flag) {
    instanceMes_tag *mes = pinst[id].mes;

    mes->stat = (u32)(u16)mes->stat | (flag & ITF_MES_HIGH_FLAGS_MASK);
}

void itfMesMngMarkBitStat(s32 id, u32 flag)
{
    instanceMes_tag *rec = pinst[id].mes;

    rec->stat |= flag & 0xFFFF0000;
}

void itfMesMngClearBitStat(s32 id, u32 flag)
{
    instanceMes_tag *rec = pinst[id].mes;

    rec->stat &= ~flag | 0xFFFF;
}

/* Return the cached clear-bit rank; reset state uses -1. */
s16 itfMesMngGetSelect(s32 id) {
    return pinst[id].mes->select.select;
}

/* Return the selected table's cached item count, without another table lookup. */
s16 itfMesMngGetMesPage(s32 id) {
    return pinst[id].mes->message.maxpage;
}

/* Return the stored signed-halfword item index, without another table lookup. */
s16 itfMesMngGetMesCurrentPage(s32 id) {
    return pinst[id].mes->message.page;
}

/* Return the replacement text stored in a window slot. */
u8 *itfMesMngGetVar(s32 id, s32 index) {
    return (u8 *)pinst[id].mes->variable.pptr[index];
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
    instanceMes_tag *mes = pinst[id].mes;
    itfMesResource_t *previous = mes->pdata;
    itfMesBinTypeHeader_t *entry;
    u8 *temporaryFontEntry;

    mes->pdata = presource;
    ResolveResourceAddress((itfMesResource_t *)presource);
    entry = GetTypeHeader2(presource);
    temporaryFontEntry = 0;
    if (presource->magic == ITF_MES_MAGIC_MSG1) {
        /* itfMesBinTypeHeader_t is an invented type and the IDB has no counterpart,
         * so +8 past its two known words has no name to take. */
        temporaryFontEntry = *(u8 **)((u8 *)entry + 8);
    }
    mes->pext = temporaryFontEntry;
    return previous;
}

/* Look up an entry-table item; only the item index has a bounds check. */
u8 *itfMesMngGetDataPtrImmediate(s32 id, s32 message, s32 page) {
    return GetMessagePagePtr(GetTypeHeader(pinst[id].mes, message)->ptr, page);
}

/* Refresh changed panel values only when both content-status pairs are clear.
 * Release the old primitive after applying the panel mode and fade limit. */
void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha) {
    instanceMes_tag *mes = pinst[id].mes;
    window_tag *panelBlock = &mes->window;

    if ((mes->stat & ITF_MES_UPPER_STATUS_PAIR_MASK) == 0) {
        if (mes->type != type || panelBlock->fadeLimit != alpha) {
            mes->type = type;
            ResetWindowPara(panelBlock, type, alpha);
            if (panelBlock->mes != NULL) {
                itfPnlRelease(panelBlock->mes);
                panelBlock->mes = NULL;
            }
        }
    }
}

/* Set or append the option with `id` in the window's option list; ids 1, 6, 7 and >= 16 are rejected. */
s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index) {
    select_tag *optionBlock = &pinst[id].mes->select;
    defkey_tag *option = NULL;
    s32 optionIndex;

    if ((u32)key >= 0x10) {
        return 0;
    }
    if (key == 1 || key == 6 || key == 7) {
        return 0;
    }
    for (optionIndex = 0; optionIndex < optionBlock->numdef; optionIndex++) {
        if (optionBlock->defkey[optionIndex].key == key) {
            option = &optionBlock->defkey[optionIndex];
            break;
        }
    }
    if (option == NULL) {
        if (optionBlock->numdef >= 15) {
            return 0;
        }
        option = &optionBlock->defkey[optionBlock->numdef];
        optionBlock->numdef++;
    }
    option->key = key;
    option->index = index;
    return 1;
}

/* Build and release a temporary glyph chain to measure the widest grouped row.
 * Empty tables and zero/missing encoded items return zero. */
s32 itfMesMngGetWidth(s32 id, s32 message, s32 page) {
    instanceMes_tag *mes = pinst[id].mes;
    itfMesBinMesHeader_t *table;
    u8 *encodedText;
    frFontHandle_t *glyphChain;
    s32 textExtent;

    table = GetTypeHeader(mes, message)->ptr;
    if (table->page == 0) {
        return 0;
    }
    CommitVarTable((s32 *)mes->variable.pptr);
    if (mes->pext != 0) {
        frSetExtDic(mes->pext);
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
    pinst[id].mes->func_kuti = (u32)func;
}

/* Initialize the fixed window pool, load its texture and register the named
 * calculation/drawing tasks. Keep the existing task flags and callback casts. */
void itfMesMngInitializeManager(void) {
    itfMesLink_t *poolNode;

    itfMesMngManager.activeWindowCount = 0;
    itfMesMngManager.ctr = 0;
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
    window_tag *panel;
    itfpanel_t *panelSprite;
    s32 *bounds;
    sdfDrawTag_t *packetList;
    s32 menuItemIndex;
    s32 menuItemY;
    const char *menuItemFormat;
    const char *selectionMarker;
    const char *coordinateFormat;
    sdfGrOt_t *packetOwner;

    if (D_00326700.pmesflow == NULL) {
        itfMesLink_t *window = DebugChangeInstance(NULL);

        D_00326700.pmesflow = window;
        if (window == NULL) {
            return 0;
        }
    }
    {
        debug_tag *debug = &D_00326700;

        panel = &debug->pmesflow->state->window;
        switch (debug->mode) {
        case 0:
            if (dds3PadButtonState.secondaryUp & 2) {
                if (--debug->cursor < 0) {
                    debug->cursor = 2;
                }
            } else if (dds3PadButtonState.secondaryDown & 2) {
                if (++debug->cursor >= 5) {
                    debug->cursor = 0;
                }
            }
            if (dds3PadButtonState.secondaryCross < 0) {
                debug_tag *confirmDebug = &D_00326700;

                switch (confirmDebug->cursor) {
                case 0:
                case 1:
                case 2:
                    confirmDebug->mode = 1;
                    break;
                case 3:
                    confirmDebug->mode = 2;
                    break;
                case 4:
                    confirmDebug->pmesflow = DebugChangeInstance(confirmDebug->pmesflow);
                    break;
                }
            } else if (dds3PadButtonState.secondaryCancel < 0) {
                return -1;
            }
            break;
        case 1:
            DebugMoveWinVert(panel, debug->cursor);
            if (dds3PadButtonState.secondaryCancel < 0) {
                debug->mode = 0;
            }
            break;
        case 2:
            DebugMoveWinAlpha((window_tag *)panel);
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
        if (D_00326700.cursor == menuItemIndex) {
            selectionMarker = D_0054F4D0[D_00326700.mode != 0];
        } else {
            selectionMarker = D_0054F4F0;
        }
        sdfDrawTagLink(packetList,
                        (u32)sdfDevPrintf(0x7180, menuItemY, 0xFFFFF0, 0,
                                                     menuItemFormat, selectionMarker, D_00326710[menuItemIndex]));
    }

    coordinateFormat = "( %3d, %3d )";
    panelSprite = panel->mes;
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
    bounds = D_00326700.pmesflow->state->window.mes_offset;
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
    D_00326700.cursor = 0;
    D_00326700.mode = 0;
    D_00326700.pmesflow = NULL;
}

/* Tear down installed content/resources, return the pool record and decrement
 * the pool-array-relative counter. An empty record is a no-op. */
void DestructFlow(s32 id) {
    itfMesLink_t *windowRecord = &D_0056BE80[id];
    instanceMes_tag *mes = windowRecord->state;

    if (mes != NULL) {
        itfMesMngRequestMessageRelease(id, 1);
        itfMesMngRequestSelectRelease(id);
        ReleaseAllWindow(mes);
        ReleaseAllVariable(&mes->variable);
        mes->stat = 0;
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

itfMesBinTypeHeader_t *GetTypeHeader(instanceMes_tag *pinst, s32 index)
{
    itfMesBinTypeHeader_t *entries = pinst->pdata->entries;

    return &entries[index];
}

itfMesBinTypeHeader_t *GetTypeHeader2(itfMesResource_t *pfileheader)
{
    itfMesBinTypeHeader_t *entries = pfileheader->entries;

    return &entries[pfileheader->entryCount];
}

u8 *GetMessagePagePtr(itfMesBinMesHeader_t *table, s32 index)
{
    s32 count = table->page;

    if (index < 0 || index >= count) {
        return NULL;
    }
    return (u8 *)table->addr[index];
}

/* Called above its definition, where ee-gcc assumed int. */
frFontHandle_t *GetLastLinkFontHandle(frFontHandle_t *pfont);

void ReadytoMessage(instanceMes_tag *panel) {
    message_tag *mes = &panel->message;
    frFontHandle_t *chain;
    u32 result;

    if (mes->pfont != NULL) {
        frDestroyFontRequestAfter(mes->pfont);
        mes->pfont = NULL;
    }
    chain = MessageRequestEx(mes->x, mes->y, mes->color[0], mes->color[1], mes->color[2], mes->color[3],
                             GetMessagePagePtr(mes->table, mes->page), NULL);
    if (!(panel->stat & 0x400000)) {
        if ((itfMesMngManager.flags & 1) != 0) {
            RaiseMessageSpeedMax(chain);
        }
    }
    GetLastMessagePara(chain, mes);
    result = frGetMessageResult(3);
    mes->textInterfaceMask = result;
    if ((result & 2) != 0) {
        panel->stat |= 0x10000;
    } else {
        panel->stat &= 0xFFFEFFFF;
    }
    if (frGetMessageResult(4) != 0) {
        if (panel->func_kuti != 0) {
            ((void (*)(void))panel->func_kuti)();
        }
    }
    ChangeZBuffer(chain, panel->z);
    mes->lines = CountLineMessage(GetLastLinkFontHandle(chain), chain);
    mes->pfont = chain;
}

/* Replace a retained text-slot allocation. Nonpositive byteCount copies the
 * padded string span; positive counts copy that many bytes into a zeroed block.
 * DDS1 string mode intentionally has no memset. Keep the K&R declaration form. */
void SetVar(mes, slotIndex, source, byteCount)
    instanceMes_tag *mes;
    s32 slotIndex;
    const char *source;
    s32 byteCount;
{
    variable_tag *slots = &mes->variable;
    u32 *textAddress = &slots->pptr[slotIndex];
    s32 allocationBytes;

    if (*textAddress != 0) {
        sdfMemFreeDirect(slots->pmh[slotIndex]);
        *textAddress = 0;
    }
    if (byteCount <= 0) {
        allocationBytes = (strlen(source) + ITF_MES_STRING_COPY_ROUND_BIAS) & ~ITF_MES_COPY_ALIGN_MASK;
        slots->pmh[slotIndex] = sdfMemAllocDirect(allocationBytes);
        *textAddress = sdfMemLock(slots->pmh[slotIndex]);
        /* String mode copies the padded span, rather than only strlen + 1. */
        memcpy((void *)*textAddress, source, allocationBytes);
        return;
    }
    allocationBytes = (byteCount + ITF_MES_BINARY_COPY_ROUND_BIAS) & ~ITF_MES_COPY_ALIGN_MASK;
    slots->pmh[slotIndex] = sdfMemAllocDirect(allocationBytes);
    *textAddress = sdfMemLock(slots->pmh[slotIndex]);
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

u8 *GetSpeakerName(instanceMes_tag *pinst)
{
    u32 *items = GetTypeHeader2(pinst->pdata)->type;

    return (u8 *)items[pinst->speaker.num];
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

void GetLastMessagePara(frFontHandle_t *pfont, message_tag *pmessage)
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

void OpenMessageWindow(instanceMes_tag *pinst) {
    window_tag *place = &pinst->window;
    message_tag *mes = &pinst->message;

    if (place->mes == NULL) {
        if (pinst->type != 2) {
            place->mes = itfPnlRegist2(6, (u32)itfMesMngManager.windowTexture);
        } else {
            place->mes = itfPnlRegist2(8, (u32)itfMesMngManager.windowTexture);
        }
    }
    itfPnlSetPosition(place->mes, mes->x + place->mes_offset[0], mes->y + place->mes_offset[1],
                      mes->x + place->mes_offset[2], mes->y + place->mes_offset[3], pinst->z);
    itfPnlSetColor(place->mes, place->red, place->green, place->blue, 0);
    pinst->stat = (pinst->stat & ~0x300) | 0x100;
}

void OpenCheckSpeakerWindow(instanceMes_tag *pinst) {
    Speaker_tag *origin = &pinst->speaker;
    window_tag *place = &pinst->window;
    if (origin->pfont != NULL && !(pinst->stat & 0x10000)) {
        if (place->spk == NULL) {
            s32 width = origin->pfont->advance * 16;
            place->spk = itfPnlRegist2(7, (u32)itfMesMngManager.windowTexture);
            itfPnlSetPosition(place->spk, origin->x - 0x2D0, origin->y - 0x68, origin->x + width + 0x2D0, origin->y + 0xE8, pinst->z);
            itfPnlSetColor(place->spk, 0x7F, 0x7F, 0x7F, 0);
        }
        pinst->stat = (pinst->stat & ~0x3000) | 0x1000;
    } else if (place->spk != 0) {
        pinst->stat |= 0x3000;
    }
}

void InitializeSpeaker(Speaker_tag *pspeaker, s32 defaultpos) {
    if (defaultpos != 0) {
        pspeaker->x = 0x430;
        pspeaker->y = 0x9B0;
    }
    pspeaker->pfont = 0;
    pspeaker->num = 0xffff;
}

void InitializeMessage(message_tag *pmessage, s32 defaultpos) {
    if (defaultpos != 0) {
        pmessage->x = 0x4B0;
        pmessage->y = 0xA88;
    }
    pmessage->pfont = NULL;
    pmessage->phase = 0;
    pmessage->textInterfaceMask = 0;
    pmessage->lines = 0;
    pmessage->page = 0;
    pmessage->maxpage = 0;
    pmessage->color[0] = 0;
    pmessage->color[1] = 0;
    pmessage->color[2] = 0;
    pmessage->color[3] = 0x80;
    pmessage->table = NULL;
}

void InitializeSelect(select_tag *pselect)
{
    pselect->x = 0x560;
    pselect->y = 0xBC8;
    pselect->pfont = NULL;
    pselect->mask = 0;
    pselect->phase = 0;
    pselect->select = -1;
    pselect->select_imm = -1;
    pselect->lines = 0;
    pselect->glyphColorIntensity = 0;
    pselect->numdef = 0;
}

void InitializeWindow(window_tag *pwindow)
{
    pwindow->spk = NULL;
    pwindow->mes = NULL;
    ResetWindowPara(pwindow, 0, 0);
}

void InitializeVariable(variable_tag *pvariable)
{
    s32 n = 0x1F;
    u32 *p = &pvariable->pptr[31];

    do {
        n--;
        *p = 0;
        p--;
    } while (n >= 0);
}

void InitializeCurCtr(curctr_tag *pcurctr, s32 isdisp)
{
    if (isdisp == 0) {
        pcurctr->isdraw = 0;
    }
    pcurctr->ctr0 = 0x40;
    pcurctr->phase = 0;
    pcurctr->ctr1 = 0;
    pcurctr->ctr_key = 0;
}

void ForceSetCurCtr(curctr_tag *pcurctr, s16 phase, s16 ctr0, s16 ctr1)
{
    pcurctr->phase = phase;
    pcurctr->ctr0 = ctr0;
    pcurctr->ctr1 = ctr1;
}

void ReleaseAllWindow(instanceMes_tag *pinst) {
    window_tag *place = &pinst->window;

    if (place->spk != NULL) {
        itfPnlRelease(place->spk);
        place->spk = NULL;
    }
    if (place->mes != NULL) {
        itfPnlRelease(place->mes);
        place->mes = NULL;
    }
    pinst->stat &= ~0xF00;
}

void ReleaseAllVariable(variable_tag *pvariable)
{
    s32 remaining = 0x1F;

    do {
        s32 index = 0x1F - remaining;

        if (pvariable->pptr[index] != 0) {
            sdfMemFreeDirect(pvariable->pmh[index]);
            pvariable->pptr[index] = 0;
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

void ResetWindowPara(window_tag *pwin, s32 type, s32 alpha)
{
    switch (type) {
    default:
        pwin->mes_offset[0] = -0x1390;
        pwin->mes_offset[1] = -0x3D8;
        pwin->mes_offset[2] = 0x2A30;
        pwin->mes_offset[3] = 0x590;
        pwin->red = 0x7F;
        pwin->green = 0x7F;
        pwin->blue = 0x7F;
        pwin->fadeLimit = (alpha > 0) ? alpha : 0x66;
        break;
    case 1:
        pwin->mes_offset[0] = -0x1DB0;
        pwin->mes_offset[1] = -0x2F8;
        pwin->mes_offset[2] = 0x2C30;
        pwin->mes_offset[3] = 0x548;
        pwin->red = 0x7F;
        pwin->green = 0x7F;
        pwin->blue = 0x7F;
        pwin->fadeLimit = (alpha > 0) ? alpha : 0x57;
        break;
    case 2:
        pwin->mes_offset[0] = -0x1390;
        pwin->mes_offset[1] = -0x208;
        pwin->mes_offset[2] = 0x2A30;
        pwin->mes_offset[3] = 0x410;
        pwin->red = 0x7F;
        pwin->green = 0x7F;
        pwin->blue = 0x7F;
        pwin->fadeLimit = (alpha > 0) ? alpha : 0x87;
        break;
    }
}

void CalcMesFlow(instanceMes_tag *pinst) {
    CalcWindow(pinst);
    CalcMessage(pinst);
    CalcSelect(pinst);
    CalcCurCtr(pinst);
}

void CalcWindow(instanceMes_tag *pinst) {
    window_tag *place = &pinst->window;
    itfpanel_t *sprite = place->mes;
    itfpanel_t *frame;
    u32 mode;

    mode = pinst->stat & 0x300;
    switch (mode) {
    case 0x100:
        sprite->alpha += 0x18;
        if (sprite->alpha >= place->fadeLimit) {
            sprite->alpha = place->fadeLimit;
            pinst->stat = (pinst->stat & ~0x307) | 0x203;
        }
        break;
    case 0x300:
        sprite->alpha -= 8;
        if (sprite->alpha <= 0) {
            sprite->alpha = 0;
            pinst->stat &= ~0x300;
        }
        break;
    }
    frame = place->spk;
    mode = pinst->stat & 0x3000;
    switch (mode) {
    case 0x1000:
        frame->alpha += 0x20;
        if (frame->alpha >= 0xC8) {
            frame->alpha = 0xC8;
            pinst->stat = (pinst->stat & ~0x3000) | 0x2000;
        }
        break;
    case 0x3000:
        frame->alpha -= 0x20;
        if (frame->alpha <= 0) {
            frame->alpha = 0;
            pinst->stat &= ~0x3000;
            itfPnlRelease(frame);
            place->spk = NULL;
        }
        break;
    }
}

void CalcMessage(instanceMes_tag *pinst) {
    message_tag *mes = &pinst->message;
    u32 flags;

    if (mes->phase != 1) {
        return;
    }
    flags = pinst->stat;
    if ((flags & 7) == 4) {
        pinst->curctr.ctr_key++;
        if (flags & 0x800000) {
            mes->phase = -1;
            return;
        }
        if ((flags & 0x100000) == 0) {
            pinst->curctr.isdraw = 1;
        }
        if (dds3PadButtonState.confirm >= 0 && dds3PadButtonState.primaryCancel >= 0 &&
            (dds3PadButtonState.primaryTriangle == 0 || pinst->curctr.ctr_key < 5)) {
            if (mes->textInterfaceMask != 0) {
                return;
            }
        }
        if (mes->textInterfaceMask != 0) {
            p2sdPlaySE(9, 0x7F, 0x3F);
        }
        InitializeCurCtr(&pinst->curctr, 0);
        mes->page++;
        if (mes->page >= mes->maxpage) {
            mes->phase = -1;
            return;
        }
        CommitVarTable((s32 *)pinst->variable.pptr);
        if (pinst->pext != NULL) {
            frSetExtDic(pinst->pext);
        }
        ReadytoMessage(pinst);
        OpenCheckSpeakerWindow(pinst);
        pinst->stat = (pinst->stat & ~7) | 3;
    } else if (dds3PadButtonState.confirm < 0 || dds3PadButtonState.primaryCancel < 0 ||
               dds3PadButtonState.primaryTriangle != 0) {
        if (!(flags & 0x400000)) {
            RaiseMessageSpeedMaxAll(mes->pfont);
            if (pinst->speaker.pfont != NULL) {
                frSetDrawSpeed(pinst->speaker.pfont, 0x80);
            }
        }
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcSelect);

void func_001D0518(instanceMes_tag *pinst, s32 y)
{
    Speaker_tag *speaker = &pinst->speaker;
    message_tag *message = &pinst->message;
    window_tag *window = &pinst->window;

    message->y += y;
    ChangeFontPosition(message->pfont, 0, y);
    if (window->mes != NULL) {
        itfPnlSetPositionOffset(window->mes, 0, y, 0, 0, 0);
    }
    if (window->spk != NULL) {
        itfPnlSetPositionOffset(window->spk, 0, y, 0, y, 0);
    }
    if (speaker->pfont != NULL) {
        speaker->y += y;
        ChangeFontPosition(speaker->pfont, 0, y);
    }
}

s32 SelectItem(instanceMes_tag *pinst) {
    select_tag *sel = &pinst->select;
    s32 dir = 0;
    s32 index;
    if (dds3PadButtonState.prev & 2) {
        if (sel->select != 0) {
            dir = -1;
        } else if (dds3PadButtonState.prev < 0) {
            dir = -1;
        }
    } else if (dds3PadButtonState.next & 2) {
        if (sel->select != sel->lines - 1 || dds3PadButtonState.next < 0) {
            dir = 1;
        }
    }
    if (dir != 0) {
        SelectItemMove(sel, dir);
        InitializeCurCtr(&pinst->curctr, 1);
    }
    if (dds3PadButtonState.confirm < 0) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        return 1;
    }
    if (sel->numdef > 0 && (index = SelectItemDefKey(sel)) >= 0) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        if (index != sel->select) {
            ChangeFontColor(sel->pfont, sel->select, sel->lines, 0);
            ChangeFontColor(sel->pfont, index, sel->lines, 1);
            sel->select = index;
            sel->select_imm = index;
        }
        return 1;
    }
    return 0;
}

void SelectItemMove(select_tag *psel, s32 direct) {
    s32 cur = psel->select;
    u16 maxv;
    ChangeFontColor(psel->pfont, cur, psel->lines, 0);
    if (direct < 0) {
        cur = cur - 1;
        if (cur < 0) {
            cur = psel->lines - 1;
        }
        maxv = psel->lines;
    } else {
        cur = cur + 1;
        if (cur >= psel->lines) {
            cur = 0;
        }
        maxv = psel->lines;
    }
    ChangeFontColor(psel->pfont, cur, (s16)maxv, 1);
    psel->select = cur;
    psel->select_imm = cur;
    p2sdPlaySE(1, 0x7F, 0x3F);
}

s32 SelectItemDefKey(select_tag *psel) {
    s32 i;

    for (i = 0; i < psel->numdef; i++) {
        defkey_tag *option = &psel->defkey[i];

        if (dds3PadButtonState.buttons[option->key] < 0) {
            s32 prefixLength = option->index;
            s32 rank = 0;
            u32 mask = psel->mask;

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

void CalcCurCtr(instanceMes_tag *pinst) {
    curctr_tag *fade = &pinst->curctr;
    s32 minimumAlpha;
    if (fade->isdraw != 0) {
        if (fade->ctr1 > 0) {
            fade->ctr1 -= 8;
        }
        switch (fade->phase) {
        case 0:
            fade->ctr0 += 8;
            if (fade->ctr0 >= 0xFF) {
                fade->phase = 1;
                fade->ctr0 = 0xFF;
                fade->ctr1 = 0x80;
            }
            break;
        case 1:
            fade->ctr0 -= 8;
            minimumAlpha = (fade->isdraw & 1) ? 0x20 : 0x40;
            if (fade->ctr0 <= minimumAlpha) {
                fade->ctr0 = minimumAlpha;
                fade->phase = 0;
            }
            break;
        }
    }
}

void DrawMesFlow(instanceMes_tag *pinst) {
    u32 flags = pinst->stat;
    select_tag *sel;
    frFontHandle_t *glyph;

    itfMesMngManager.flags &= ~2;
    DrawWindow(pinst);
    glyph = pinst->speaker.pfont;
    if (!(flags & 0x10000)) {
        if (glyph != NULL) {
            frDrawFont(glyph);
        }
    }
    glyph = pinst->message.pfont;
    if (!(flags & 0x20000)) {
        if ((flags & 7) >= 3) {
            if (frDrawFont(glyph) > 0) {
                if ((pinst->stat & 7) != 4) {
                    pinst->curctr.ctr_key = 0;
                    pinst->stat = (pinst->stat & ~7) | 4;
                }
            }
        }
    }
    glyph = pinst->select.pfont;
    if (!(flags & 0x40000)) {
        if ((flags & 0x38) >= 0x18) {
            if (frDrawFontEx(glyph, 1) > 0) {
                if ((flags & 0x38) == 0x18) {
                    sel = &pinst->select;
                    if (sel->select == -1) {
                        sel->select = 0;
                        sel->select_imm = 0;
                    }
                    ChangeFontColor(sel->pfont, sel->select, sel->lines, 1);
                    pinst->stat = (pinst->stat & ~0x38) | 0x20;
                    pinst->curctr.isdraw = 2;
                }
            }
        }
    }
    if ((pinst->curctr.isdraw & 1) != 0) {
        DrawRetPageCursor(pinst);
    }
    if ((pinst->curctr.isdraw & 2) != 0) {
        DrawSelectCursor(pinst);
    }
}

void DrawWindow(instanceMes_tag *pinst) {
    window_tag *place = &pinst->window;
    u32 flags = pinst->stat;

    if (flags & 0x80000) {
        return;
    }
    if ((flags & 0x300) >= 0x100) {
        itfPnlDraw(place->mes, &sdfGrOtTable[pinst->otindex]);
        itfMesMngManager.flags |= 2;
        if (place->spk != NULL) {
            itfPnlDraw(place->spk, &sdfGrOtTable[pinst->otindex]);
        }
    }
    if (D_00326700.pmesflow != NULL && D_00326700.pmesflow->state == pinst && place != NULL) {
        DebugDrawWindowLine(place->mes);
    }
}

/* NON_MATCHING: 128 of 191 words, exact size; the three stack arrays' store and constant order is scheduled differently. */
#ifdef NON_MATCHING
void DrawSelectCursor(instanceMes_tag *pinst) {
    s32 uv[4];
    s32 color[4];
    s32 rect[4];
    curctr_tag *fade = &pinst->curctr;
    select_tag *sel = &pinst->select;
    sdfDrawTag_t *list;
    sdfGrOt_t *packetOwner;
    s32 x;
    s32 y;
    s32 timer;
    s32 d;
    s32 index;
    s32 rowOfs;

    index = sel->select_imm;
    if (index == -1) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    x = sel->x;
    rowOfs = sel->lines * 160 - 0xC0;
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
    itfMiscDrawSpritePlanePrim(rect, color, pinst->z, 0, list);
    D_00326728[1] = y - 8;
    D_00326728[3] = y - 8;
    D_00326728[5] = y + 0xA8;
    D_00326728[7] = y + 0xA8;
    itfMiscDrawRectanglePrim(D_00326728, "W", D_00326768, D_00326770, pinst->z, list);
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
    itfMiscDrawSpritePrim(rect, uv, color, pinst->z, itfMesMngManager.windowTexture, 0, list);
    uv[0] = 0x260;
    uv[1] = 0x2F0;
    uv[3] = 0x3F0;
    uv[2] = 0x360;
    color[3] = fade->ctr0;
    rect[2] = rect[0] + 0x100;
    rect[3] = rect[1] + 0x80;
    itfMiscDrawSpritePrim2(rect, uv, color, pinst->z, 0, list);
    timer = fade->ctr1;
    if (timer > 0) {
        d = 0x80 - timer;
        rect[0] -= d;
        rect[1] -= d >> 1;
        rect[2] += d * 2;
        rect[3] += d >> 1;
        color[3] = timer;
        itfMiscDrawAlphaPrim(list, 1, 0);
        itfMiscDrawSpritePrim2(rect, uv, color, pinst->z, 0, list);
        itfMiscDrawAlphaPrim(list, 0, 0);
    }
    packetOwner = &sdfGrOtTable[pinst->otindex];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))packetOwner->regFunc)(packetOwner, list);
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", DrawSelectCursor);
#endif


void DrawRetPageCursor(instanceMes_tag *pinst) {
    s32 rect[4];
    curctr_tag *fade = &pinst->curctr;
    sdfDrawTag_t *list;
    sdfGrOt_t *packetOwner;
    s32 timer;
    s32 d;

    rect[0] = 0x1A50;
    rect[1] = 0xCA0;
    rect[2] = 0x1B60;
    rect[3] = 0xD28;
    D_00326788[3] = fade->ctr0;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    itfMiscDrawSpritePrim(rect, D_00326778, D_00326788, pinst->z, itfMesMngManager.windowTexture, 0, list);
    timer = fade->ctr1;
    if (timer > 0) {
        d = 0x80 - timer;
        rect[0] -= d * 2;
        rect[1] -= d;
        rect[2] += d * 2;
        rect[3] += d;
        D_00326788[3] = timer;
        itfMiscDrawAlphaPrim(list, 1, 0);
        itfMiscDrawSpritePrim(rect, D_00326778, D_00326788, pinst->z, itfMesMngManager.windowTexture, 0, list);
        itfMiscDrawAlphaPrim(list, 0, 0);
    }
    packetOwner = &sdfGrOtTable[pinst->otindex];
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

s32 DrawMsgMng_Process(dds3ProcessID_t *id) {
    itfMesLink_t *node;

    if (dds3NullDrawProc() != 0) {
        return 0;
    }
    node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        DrawMesFlow(node->state);
        node = node->next;
    }
    itfMesMngManager.ctr++;
    return 0;
}

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

void DebugDrawWindowLine(itfpanel_t *meswin) {
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

void DebugMoveWinVert(window_tag *pwin, s32 flag) {
    itfpanel_t *sprite = pwin->mes;
    s32 *bounds;
    s32 dx;
    s32 dy;

    if (sprite != NULL) {
        bounds = pwin->mes_offset;
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
void DebugMoveWinAlpha(window_tag *pwin) {
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
        if (pwin->mes != NULL) {
            pwin->mes->alpha = index;
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
        if ((pflow->state)->window.mes != NULL) {
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

