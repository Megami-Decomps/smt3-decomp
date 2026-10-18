#include "common.h"
typedef struct frFontGlyph {
    /* 0x00 */ u8 ctx;
    /* 0x01 */ u8 unk01[7];
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 advance;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 cellh;
    /* 0x14 */ union {
        s32 zbuf;
        u8 setup[4];
    } v;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ struct frFontGlyph *firstChild;
    /* 0x20 */ struct frFontGlyph *lastChild;
    /* 0x24 */ struct frFontGlyph *prev;
    /* 0x28 */ struct frFontGlyph *next;
} frFontGlyph_t;

typedef struct itfMesEntryBlock {
    /* 0x00 */ u8 unk00[0x12];
    /* 0x12 */ u8 color[4];
} itfMesEntryBlock_t;

typedef struct itfMesCurCtr {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s16 phase;
    /* 0x04 */ s16 alpha;
    /* 0x06 */ s16 timer;
    /* 0x08 */ s32 frames;
} itfMesCurCtr_t;

void sdfMemFreeDirect(s32 handle);


typedef struct itfMesEntry {
    /* 0x00 */ u32 *items;
    /* 0x04 */ s32 unk04;
} itfMesEntry_t;

typedef struct itfMesSub {
    /* 0x00 */ u32 magic;
    /* 0x04 */ u8 unk04[0xC];
    /* 0x10 */ s32 fixupOffset;
    /* 0x14 */ s32 fixupBytes;
    /* 0x18 */ s32 entryCount;
    /* 0x1C */ u8 relocated;
    /* 0x1D */ u8 unk1D[3];
    /* 0x20 */ itfMesEntry_t entries[1];
} itfMesSub_t;

typedef struct itfMesTable {
    /* 0x00 */ u8 unk00[0x18];
    /* 0x18 */ s16 count;
    /* 0x1A */ u8 unk1A[2];
    /* 0x1C */ void *items[1];
} itfMesTable_t;

typedef struct itfMesBlk40 {
    /* 0x00 */ u8 unk00[0xC];
    /* 0x0C */ u32 panelmask;
    /* 0x10 */ s16 selphase;
    /* 0x12 */ s16 selectsave;
    /* 0x14 */ s16 selectno;
} itfMesBlk40_t;

typedef struct itfMesRec {
    /* 0x00 */ u32 flags;
    /* 0x04 */ itfMesSub_t *sub;
    /* 0x08 */ u8 unk08[0x18];
    /* 0x20 */ u16 selidx;
    /* 0x22 */ u8 unk22[0x12];
    /* 0x34 */ s8 phase;
    /* 0x35 */ u8 unk35[0xB];
    /* 0x40 */ itfMesBlk40_t blk40;
} itfMesRec_t;

typedef struct itfMesInst {
    /* 0x00 */ itfMesRec_t *rec;
    /* 0x04 */ s32 unk04[4];
} itfMesInst_t;

extern itfMesInst_t pinst[];

s32 scrGetMessageHandle(void);
s32 scrGetIntPara(s32 index);
void fldPanelSet(s32 type);
void sdfAddressResolver(s32 *dst, s32 base, u8 *table, s32 len);
void frSetVariableRequest(s32 slot, s32 value);
itfMesEntry_t *GetTypeHeader2(itfMesSub_t *sub);

void itfMesMngSetSelect(s32 window, s32 count);
void itfMesMngMarkBitStat(s32 window, u32 mask);
void itfMesMngClearBitStat(s32 window, u32 mask);
void itfMesMngSetVar(s32 window, s32 slot, s32 value, s32 selector);
void itfMesMngSetMesPosition(s32 window, s32 x, s32 y);
void itfMesMngChangeWindowType(s32 window, s32 type, s32 alpha);
void itfMesMngSetSelectDefKey(s32 window, s32 first, s32 second);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngMessage);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetOffsetY1);

s8 itfMesMngGetPhase(s32 window)
{
    return pinst[window].rec->phase;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetPhase);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSelect);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetSelPhase);

void itfMesMngSetSelPhase(s32 window, s16 phase)
{
    pinst[window].rec->blk40.selphase = phase;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetSelectNo);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngMaskSelect);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngCloseWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngMsgWndCls);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngOpenSelectWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngCloseSelectWindow);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetMesPosDefault);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngInitialize);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRelease);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestMessage);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestMessageRelease);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestSelect);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngRequestSelectRelease);

void itfMesMngSetMaskSelect(s32 window, u32 mask)
{
    pinst[window].rec->blk40.panelmask = mask;
}

void itfMesMngSetSelect(s32 window, s32 count)
{
    itfMesBlk40_t *blk = &pinst[window].rec->blk40;
    u32 mask = blk->panelmask;
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
    blk->selectsave = n;
    blk->selectno = n;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSpeakerPosition);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSpeakerPositionOffset);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetMesPosition);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetMesPositionOffset);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSelPosition);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSelPositionOffset);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetPriority);

u32 itfMesMngGetStat(s32 window)
{
    return pinst[window].rec->flags;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetStat);

void itfMesMngMarkBitStat(s32 window, u32 mask)
{
    itfMesRec_t *rec = pinst[window].rec;

    rec->flags |= mask & 0xFFFF0000;
}

void itfMesMngClearBitStat(s32 window, u32 mask)
{
    itfMesRec_t *rec = pinst[window].rec;

    rec->flags &= ~mask | 0xFFFF;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetSelect);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetMesPage);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetMesCurrentPage);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetVar);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetVarLocalize);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetVar);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetVarImmediate);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetVarImmediate2);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetResourcePtr);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetDataPtrImmediate);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngChangeWindowType);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetSelectDefKey);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetWidth);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetDefaultSpeed);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetKutiCallback);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngInitializeManager);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngSetBitStatManager);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngClearBitStatManager);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngGetStatManager);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00535B40);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00535B50);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00535B60);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00535B70);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngDebugProc);

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesMngDebugShutdown);

INCLUDE_ASM(const s32, "interface/itfMesManager", DestructFlow);

void ResolveResourceAddress(itfMesSub_t *sub)
{
    s32 *payload = (s32 *)sub->entries;

    if (sub->relocated == 0) {
        sdfAddressResolver(payload, (s32)payload, (u8 *)sub + sub->fixupOffset, sub->fixupBytes);
        sub->relocated = 1;
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", CheckMessageBinary);

itfMesEntry_t *GetTypeHeader(itfMesRec_t *mes, s32 index)
{
    itfMesEntry_t *entries = mes->sub->entries;

    return &entries[index];
}

itfMesEntry_t *GetTypeHeader2(itfMesSub_t *sub)
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

INCLUDE_ASM(const s32, "interface/itfMesManager", SetVar);

void CommitVarTable(s32 *table)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        frSetVariableRequest(i, table[i]);
    }
}

u8 *GetSpeakerName(itfMesRec_t *mes)
{
    u32 *items = GetTypeHeader2(mes->sub)->items;

    return (u8 *)items[mes->selidx];
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

INCLUDE_ASM(const s32, "interface/itfMesManager", UnchainFontHandleUnit);

frFontGlyph_t *GetLastLinkFontHandle(frFontGlyph_t *g)
{
    while (g->prev != NULL) {
        g = g->prev;
    }
    return g;
}

void GetLastMessagePara(frFontGlyph_t *g, itfMesEntryBlock_t *blk)
{
    frFontGlyph_t *shade = g->lastChild;
    u8 ctx = g->ctx;

    blk->color[3] = ctx >> 1;
    blk->color[0] = shade->v.setup[1];
    blk->color[1] = shade->v.setup[0];
    blk->color[2] = shade->v.setup[2];
}

s32 CountLineMessage(frFontGlyph_t *last, frFontGlyph_t *first)
{
    s16 h = first->cellh;

    return ((first->y - last->y) >> 3) / h + 1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", ChangeFontPosition);

void ChangeZBuffer(frFontGlyph_t *g, s32 z)
{
    while (g != NULL) {
        g->v.zbuf = z;
        g = g->prev;
    }
}

void ChangeFontColor(frFontGlyph_t *g, s32 row, s32 rows, s32 value)
{
    s32 skip = rows - row - 1;
    s32 y = g->y;
    s32 cur = y;
    frFontGlyph_t *c;
    u8 flag;

    while (skip > 0) {
        while (y == cur) {
            g = g->prev;
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
            c->v.setup[0] = flag;
        }
        g = g->prev;
    } while (g != NULL && y == g->y);
}

void ChangeFontColorAll(frFontGlyph_t *g, u8 flag)
{
    frFontGlyph_t *c;

    for (; g != NULL; g = g->prev) {
        for (c = g->firstChild; c != NULL; c = c->next) {
            c->v.setup[0] = flag;
        }
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", ChangeFontAlpha);

s32 GetMessageWidth(frFontGlyph_t *g)
{
    s32 max = 0;

    while (g != NULL) {
        s32 y = g->y;
        s32 w = 0;

        do {
            w += g->advance;
            g = g->prev;
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

INCLUDE_ASM(const s32, "interface/itfMesManager", OpenCheckSpeakerWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeSpeaker);

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeMessage);

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeSelect);

INCLUDE_ASM(const s32, "interface/itfMesManager", InitializeWindow);

void InitializeVariable(s32 *slots)
{
    s32 n = 0x1F;
    s32 *p = &slots[31];

    do {
        n--;
        *p = 0;
        p--;
    } while (n >= 0);
}

void InitializeCurCtr(itfMesCurCtr_t *c, s32 preserve)
{
    if (preserve == 0) {
        c->kind = 0;
    }
    c->alpha = 0x40;
    c->phase = 0;
    c->timer = 0;
    c->frames = 0;
}

void ForceSetCurCtr(itfMesCurCtr_t *c, s16 phase, s16 alpha, s16 timer)
{
    c->phase = phase;
    c->alpha = alpha;
    c->timer = timer;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", ReleaseAllWindow);

void ReleaseAllVariable(s32 *slots)
{
    s32 n = 0x1F;

    do {
        if (slots[0] != 0) {
            sdfMemFreeDirect(slots[32]);
            slots[0] = 0;
        }
        n--;
        slots++;
    } while (n >= 0);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", ConvertZenkakuNumber);

INCLUDE_ASM(const s32, "interface/itfMesManager", ResetWindowPara);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcMesFlow);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcMessage);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcSelect);

INCLUDE_ASM(const s32, "interface/itfMesManager", SelectItem);

INCLUDE_ASM(const s32, "interface/itfMesManager", SelectItemMove);

INCLUDE_ASM(const s32, "interface/itfMesManager", SelectItemDefKey);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcCurCtr);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMesFlow);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawWindow);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawSelectCursor);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawRetPageCursor);

INCLUDE_ASM(const s32, "interface/itfMesManager", CalcMsgMng_Process);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMsgMng_Process);

INCLUDE_ASM(const s32, "interface/itfMesManager", DrawMsgMng_Shutdown);

INCLUDE_ASM(const s32, "interface/itfMesManager", DebugDrawWindowLine);

INCLUDE_ASM(const s32, "interface/itfMesManager", DebugMoveWinVert);

INCLUDE_ASM(const s32, "interface/itfMesManager", DebugMoveWinAlpha);

INCLUDE_ASM(const s32, "interface/itfMesManager", DebugChangeInstance);
INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4C0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4C8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4D8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4E0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4E8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4F0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F4F8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F500);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_0054F508);

