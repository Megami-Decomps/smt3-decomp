#include "MODHSYN.h"

extern u8 hsynLibraryTable[];

/* MODHSYN.IRX, Sony's SPU2 synthesiser: it owns the voices and the envelopes,
 * and a .SMG's bank is loaded into it. See docs/irx-modules.md.
 *
 * The chunks still in assembly are the ones no C has been written for yet,
 * plus the import stubs and the import and export tables between them. */

/* FUNC modhsyn_e30 */
int modhsyn_e30()
{
    s32 state;
    s32 res;

    intrman_CpuSuspendIntr(&state);
    res = loadcore_ReleaseLibraryEntries(hsynLibraryTable);
    intrman_CpuResumeIntr(state);
    return res != 0;
}
/* ENDF modhsyn_e30 */
INCLUDE_ASM("MODHSYN", D_00000044);
INCLUDE_ASM("MODHSYN", func_00000050);
INCLUDE_ASM("MODHSYN", modhsyn_e7);
INCLUDE_ASM("MODHSYN", D_000002D4);
INCLUDE_ASM("MODHSYN", hsynLoadBank);
INCLUDE_ASM("MODHSYN", modhsyn_sceHSyn_Load);
INCLUDE_ASM("MODHSYN", modhsyn_e28);
INCLUDE_ASM("MODHSYN", D_00000714);
INCLUDE_ASM("MODHSYN", hsynResetState);
INCLUDE_ASM("MODHSYN", hsynVoiceParamReset);
INCLUDE_ASM("MODHSYN", hsynVoiceSetPitch);
INCLUDE_ASM("MODHSYN", hsynVoiceEnvelopeApply);
/* FUNC hsynVoiceSetAttack */
void hsynVoiceSetAttack(sceHSynChannel_t *chan, u8 val)
{
    u8 n;

    n = (val << 7) / 127;
    if (chan->vol != n) {
        chan->vol = n;
        hsynVoiceEnvelopeApply(chan, 0);
    }
}
/* ENDF hsynVoiceSetAttack */
/* FUNC hsynVoiceSetRelease */
void hsynVoiceSetRelease(sceHSynChannel_t *chan, u8 val)
{
    u8 n;

    n = (val << 7) / 127;
    if (chan->pan != n) {
        chan->pan = n;
        hsynVoiceEnvelopeApply(chan, 1);
    }
}
/* ENDF hsynVoiceSetRelease */
INCLUDE_ASM("MODHSYN", hsynVoiceSetVolumeLeft);
INCLUDE_ASM("MODHSYN", hsynVoiceSetVolumeRight);
/* FUNC hsynVoiceFlagsGet */
int hsynVoiceFlagsGet(sceHSynChannel_t *chan, u8 semitones)
{
    chan->bendRange = semitones * 20;
}
/* ENDF hsynVoiceFlagsGet */
/* FUNC hsynVoiceFlagsSet */
void hsynVoiceFlagsSet(sceHSynChannel_t *chan, u8 on)
{
    if (on != 0) {
        if (chan->priority == 0xFF) {
            chan->priority = 0x80;
            chan->unk34 = 0x80;
        }
    } else {
        chan->priority = 0xFF;
    }
}
/* ENDF hsynVoiceFlagsSet */
/* FUNC hsynVoiceIndexGet */
void hsynVoiceIndexGet(sceHSynChannel_t *chan, u8 val)
{
    chan->unk34 = val;
}
/* ENDF hsynVoiceIndexGet */
INCLUDE_ASM("MODHSYN", func_00000C58);
INCLUDE_ASM("MODHSYN", func_00000D24);
INCLUDE_ASM("MODHSYN", func_00000E40);
INCLUDE_ASM("MODHSYN", hsynVoiceSetPitchDirect);
INCLUDE_ASM("MODHSYN", D_0000106C);
INCLUDE_ASM("MODHSYN", hsynVoiceApplyAll);
INCLUDE_ASM("MODHSYN", hsynVoiceEventDispatch);
INCLUDE_ASM("MODHSYN", hsynVoiceUpdatePass);
/* FUNC hsynVoiceRefreshHook */
void hsynVoiceRefreshHook(void)
{
}
/* ENDF hsynVoiceRefreshHook */
INCLUDE_ASM("MODHSYN", hsynVoiceDirectPass);
INCLUDE_ASM("MODHSYN", hsynVoiceRefresh);
INCLUDE_ASM("MODHSYN", hsynServiceTick);
INCLUDE_ASM("MODHSYN", func_000017EC);
INCLUDE_ASM("MODHSYN", modhsyn_e8);
/* FUNC modhsyn_e26 */
int modhsyn_e26(u32 *mask)
{
    mask[0] = hsynVoiceMask[0];
    mask[1] = hsynVoiceMask[1];
    return 0;
}
/* ENDF modhsyn_e26 */
#ifdef NON_MATCHING
/* Three registers apart: retail keeps the unit index in the dead slotNo
 * register, gcc reuses the one holding slotNo * 2. */
/* FUNC hsynParamSlotResolve */
int hsynParamSlotResolve(sceHSynWork_t *work, int slotNo, sceHSynSlot_t **pslot, u16 **pval)
{
    sceHSynTbl_t *tbl;
    sceHSynSlot_t *slot;
    u32 unit;

    if (work != NULL) {
        tbl = work->tbl;
        /* nEntryWord counts eight-byte units, so a slot owns units 2n and
         * 2n+1 and its sceHSynSlot_t pointer is the odd one's second word. */
        unit = slotNo * 2 + 1;
        if (unit < tbl->nEntryWord) {
            slot = (sceHSynSlot_t *)((void **)tbl->entry)[unit * 2 + 1];
            *pslot = slot;
            *pval = &slot->unk14;
            return 1;
        }
    }
    return 0;
}
/* ENDF hsynParamSlotResolve */
#else
INCLUDE_ASM("MODHSYN", hsynParamSlotResolve);
#endif
/* FUNC modhsyn_e10 */
int modhsyn_e10(sceHSynWork_t *work, int slotNo, s16 val)
{
    sceHSynSlot_t *slot;
    u16 *p;

    if (hsynParamSlotResolve(work, slotNo, &slot, &p) == 0) {
        return -1;
    }
    *p = val;
    return 0;
}
/* ENDF modhsyn_e10 */
/* FUNC modhsyn_e11 */
int modhsyn_e11(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    u16 *p;

    if (hsynParamSlotResolve(work, slotNo, &slot, &p) == 0) {
        return 0;
    }
    return *p;
}
/* ENDF modhsyn_e11 */
INCLUDE_ASM("MODHSYN", modhsyn_e12);
INCLUDE_ASM("MODHSYN", modhsyn_e13);
INCLUDE_ASM("MODHSYN", modhsyn_e14);
INCLUDE_ASM("MODHSYN", modhsyn_e9);
/* FUNC modhsyn_e15 */
int modhsyn_e15(sceHSynStat_t *stat)
{
    hsynStat = stat;
    return 0;
}
/* ENDF modhsyn_e15 */
INCLUDE_ASM("MODHSYN", hsynEffectAttrRead);
INCLUDE_ASM("MODHSYN", hsynEffectAttrWrite);
INCLUDE_ASM("MODHSYN", modhsyn_e17);
/* FUNC modhsyn_e18 */
int modhsyn_e18(int mode)
{
    switch (mode) {
    case 0:
        hsynEnabled = 1;
        break;
    case 1:
        hsynEnabled = 0;
        break;
    default:
        return -1;
    }
    return 0;
}
/* ENDF modhsyn_e18 */
/* FUNC modhsyn_e27 */
int modhsyn_e27(void)
{
    return (hsynEnabled ^ 1) != 0;
}
/* ENDF modhsyn_e27 */
/* FUNC hsynCmdReset */
int hsynCmdReset(u8 **pp)
{
    u32 acc;
    u8 b;

    acc = 0;
    do {
        b = *(*pp)++;
        acc = (acc << 7) + (b & 0x7F);
    } while (b & 0x80);
    return acc;
}
/* ENDF hsynCmdReset */
INCLUDE_ASM("MODHSYN", hsynCmdProgramChange);
INCLUDE_ASM("MODHSYN", func_000021B8);
INCLUDE_ASM("MODHSYN", hsynCmdChannelPressure);
INCLUDE_ASM("MODHSYN", hsynCmdNoteOff);
INCLUDE_ASM("MODHSYN", hsynCmdNoteOn);
INCLUDE_ASM("MODHSYN", hsynCmdVoiceAlloc);
INCLUDE_ASM("MODHSYN", func_0000273C);
/* FUNC modhsyn_e19 */
int modhsyn_e19(u8 val)
{
    if (val >= 0x31) {
        return -1;
    }
    hsynModDisable = val;
    return 0;
}
/* ENDF modhsyn_e19 */
INCLUDE_ASM("MODHSYN", modhsyn_e20);
INCLUDE_ASM("MODHSYN", modhsyn_e21);
INCLUDE_ASM("MODHSYN", modhsyn_e22);
INCLUDE_ASM("MODHSYN", modhsyn_e25);
INCLUDE_ASM("MODHSYN", modhsyn_e23);
INCLUDE_ASM("MODHSYN", modhsyn_e24);
INCLUDE_ASM("MODHSYN", hsynInitCores);
/* FUNC hsynCoreIndexGet */
void hsynCoreIndexGet(sceHSynVoice_t *v, sceHSynVoice_t *at)
{
    sceHSynVoice_t *p;

    v->prev = at;
    p = at->next;
    v->next = p;
    at->next = v;
    p->prev = v;
}
/* ENDF hsynCoreIndexGet */
/* FUNC hsynCoreIndexSet */
void hsynCoreIndexSet(sceHSynVoice_t *v, sceHSynVoice_t *at)
{
    sceHSynVoice_t *p;

    v->next = at;
    p = at->prev;
    v->prev = p;
    at->prev = v;
    p->next = v;
}
/* ENDF hsynCoreIndexSet */
/* FUNC hsynVoiceMaskGet */
void hsynVoiceMaskGet(sceHSynVoice_t *v)
{
    v->next->prev = v->prev;
    v->prev->next = v->next;
}
/* ENDF hsynVoiceMaskGet */
/* FUNC hsynVoiceSlotGet */
void hsynVoiceSlotGet(sceHSynVoice_t *v, sceHSynVoice_t **head)
{
    sceHSynVoice_t *n;

    /* head points at the chNext/chPrev pair of the list's sentinel. */
    v->chPrev = (sceHSynVoice_t *)((u8 *)head - 8);
    n = head[0];
    v->chNext = n;
    head[0] = v;
    n->chPrev = v;
}
/* ENDF hsynVoiceSlotGet */
INCLUDE_ASM("MODHSYN", D_00003734);
/* FUNC hsynVoiceSlotSet */
void hsynVoiceSlotSet(sceHSynVoice_t *v)
{
    v->chNext->chPrev = v->chPrev;
    v->chPrev->chNext = v->chNext;
}
/* ENDF hsynVoiceSlotSet */
INCLUDE_ASM("MODHSYN", hsynServiceVoices);
/* FUNC hsynEnvRateLookup */
s16 hsynEnvRateLookup(s16 delta, s32 base, s32 bias)
{
    s32 v;

    v = base + bias + delta;
    if (v > 0x7FFF) {
        v = 0x7FFF;
    } else if (v < -0x8000) {
        v = -0x8000;
    }
    return v;
}
/* ENDF hsynEnvRateLookup */
/* FUNC hsynEnvLevelLookup */
s8 hsynEnvLevelLookup(s8 delta, s32 base, s32 bias)
{
    s32 v;

    v = base + bias + delta;
    if (v >= 0x80) {
        v = 0x7F;
    } else if (v < -0x80) {
        v = -0x80;
    }
    return v;
}
/* ENDF hsynEnvLevelLookup */
INCLUDE_ASM("MODHSYN", hsynAdjustFieldByKey);
INCLUDE_ASM("MODHSYN", hsynBuildAdsr1);
INCLUDE_ASM("MODHSYN", hsynBuildAdsr2);
INCLUDE_ASM("MODHSYN", hsynVoiceUpdatePathA);
INCLUDE_ASM("MODHSYN", hsynVoiceKeyOnPathA);
INCLUDE_ASM("MODHSYN", hsynVoicePitchCommit);
/* FUNC hsynSdSetParamPitch */
void hsynSdSetParamPitch(sceHSynVoice_t *v, s16 bend)
{
    sceSmgProgKey_t *key;
    u16 depth;

    key = v->key;
    if (key == NULL) {
        return;
    }
    if (bend >= 0) {
        depth = key->unk08;
    } else {
        depth = key->unk06;
    }
    v->unk34 = depth * bend / 8192;
}
/* ENDF hsynSdSetParamPitch */
INCLUDE_ASM("MODHSYN", func_00004614);
INCLUDE_ASM("MODHSYN", hsynSdSetParamEnvelope);
/* FUNC hsynSdSetParamVolume */
s32 hsynSdSetParamVolume(sceHSynVoice_t *v, s8 scale, u8 a, u8 b)
{
    sceHSynSlot_t *slot;

    slot = v->chan->slot;
    return hsynVolumeCurveLookup(v->vel, a, b, slot->unk10, slot->unk0C) * scale
        / 65536;
}
/* ENDF hsynSdSetParamVolume */
INCLUDE_ASM("MODHSYN", hsynSdSetParamVolumeLeft);
INCLUDE_ASM("MODHSYN", hsynSdSetParamVolumeRight);
INCLUDE_ASM("MODHSYN", hsynSdSetParamAdsr);
INCLUDE_ASM("MODHSYN", func_00004AE4);
INCLUDE_ASM("MODHSYN", hsynSdSetParamBend);
INCLUDE_ASM("MODHSYN", func_00004D90);
INCLUDE_ASM("MODHSYN", hsynVoiceKeyOff);
INCLUDE_ASM("MODHSYN", hsynVoiceStateClear);
INCLUDE_ASM("MODHSYN", hsynVoiceUpdatePathB);
INCLUDE_ASM("MODHSYN", hsynVoiceKeyOnPathB);
INCLUDE_ASM("MODHSYN", hsynEnvAttackStep);
INCLUDE_ASM("MODHSYN", hsynEnvDecayStep);
INCLUDE_ASM("MODHSYN", func_00005AA0);
INCLUDE_ASM("MODHSYN", func_00005B4C);
INCLUDE_ASM("MODHSYN", D_00005BDC);
INCLUDE_ASM("MODHSYN", hsynSpu2SettleDelay);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocScan);
INCLUDE_ASM("MODHSYN", hsynVoiceMaskUpdate);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocByPriority);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocByChannel);
INCLUDE_ASM("MODHSYN", hsynVoiceStop);
INCLUDE_ASM("MODHSYN", hsynVoiceRelease);
INCLUDE_ASM("MODHSYN", hsynVoiceStealScan);
INCLUDE_ASM("MODHSYN", hsynVoiceEngineStep);
INCLUDE_ASM("MODHSYN", hsynVoiceMaskClear);
INCLUDE_ASM("MODHSYN", hsynVoiceEnvelopeAdvance);
INCLUDE_ASM("MODHSYN", hsynVoiceVolumeAdvance);
INCLUDE_ASM("MODHSYN", hsynVoiceRetrigger);
INCLUDE_ASM("MODHSYN", hsynLfoApply);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocForCmd);
/* FUNC hsynVoiceMaskSet */
int hsynVoiceMaskSet(sceHSynVoice_t *v)
{
    return hsynVoiceMaskUpdate(v);
}
/* ENDF hsynVoiceMaskSet */
INCLUDE_ASM("MODHSYN", hsynVoiceAllocExport20);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocPathB);
/* FUNC hsynVoiceAllocPathBEntry */
int hsynVoiceAllocPathBEntry(int a, int b)
{
    return hsynVoiceAllocPathB(a + 0x18, b);
}
/* ENDF hsynVoiceAllocPathBEntry */
INCLUDE_ASM("MODHSYN", hsynVoiceAllocPathC);
/* FUNC hsynVoiceAllocPathCEntry */
int hsynVoiceAllocPathCEntry(int a, int b, int c, int d)
{
    return hsynVoiceAllocPathC(a + 0x18, b, c, d);
}
/* ENDF hsynVoiceAllocPathCEntry */
INCLUDE_ASM("MODHSYN", hsynVoiceReleaseExport21);
/* FUNC hsynVoiceReleaseOne */
int hsynVoiceReleaseOne(sceHSynVoice_t *v)
{
    return hsynVoiceRelease(v);
}
/* ENDF hsynVoiceReleaseOne */
INCLUDE_ASM("MODHSYN", hsynVoiceReleaseGroup);
INCLUDE_ASM("MODHSYN", func_000078D4);
INCLUDE_ASM("MODHSYN", func_00007F54);
INCLUDE_ASM("MODHSYN", func_00008350);
/* FUNC hsynVoiceMaskClearOne */
sceHSynVoice_t *hsynVoiceMaskClearOne(u8 allocHint)
{
    return hsynVoiceMaskClear(allocHint);
}
/* ENDF hsynVoiceMaskClearOne */
INCLUDE_ASM("MODHSYN", hsynModulationApply);
/* FUNC hsynModulationStep */
int hsynModulationStep(u16 a, u8 b)
{
    u8 limit;
    int on;
    int done;

    on = 1;
    done = 0;
    limit = hsynModDisable;
    if (limit == 0) {
        on = 0;
    } else if (hsynRelCount >= limit) {
        done = 1;
    }
    return hsynModulationApply(a, b, on, done);
}
/* ENDF hsynModulationStep */
INCLUDE_ASM("MODHSYN", hsynPanApply);
INCLUDE_ASM("MODHSYN", hsynLfoStep);
INCLUDE_ASM("MODHSYN", D_00008914);
/* FUNC hsynUnrefParamGetA */
int hsynUnrefParamGetA(u8 a, u8 b)
{
    return ((a - b) << 16) / 126;
}
/* ENDF hsynUnrefParamGetA */
/* FUNC hsynUnrefParamGetB */
int hsynUnrefParamGetB(u8 a, u8 b)
{
    return ((b - a) << 16) / 126;
}
/* ENDF hsynUnrefParamGetB */
INCLUDE_ASM("MODHSYN", hsynUnrefParamSetA);
INCLUDE_ASM("MODHSYN", hsynUnrefParamSetB);
INCLUDE_ASM("MODHSYN", hsynUnrefParamSetC);
INCLUDE_ASM("MODHSYN", hsynUnrefParamSetD);
INCLUDE_ASM("MODHSYN", func_00008BB0);
INCLUDE_ASM("MODHSYN", func_00008C1C);
INCLUDE_ASM("MODHSYN", func_00008C88);
INCLUDE_ASM("MODHSYN", func_00008D54);
INCLUDE_ASM("MODHSYN", hsynVolumeCurveLookup);
/* FUNC hsynUnrefStubA */
u8 hsynUnrefStubA(s32 v)
{
    return v;
}
/* ENDF hsynUnrefStubA */
/* FUNC hsynUnrefStubB */
u8 hsynUnrefStubB(u8 v)
{
    return 0x80 - v;
}
/* ENDF hsynUnrefStubB */
/* FUNC hsynTableEntryFill */
int hsynTableEntryFill(u8 x)
{
    u32 v;
    int r;

    r = 1;
    v = x;
    v = v * v / 127;
    if (v != 0) {
        r = v & 0xFF;
    }
    return r;
}
/* ENDF hsynTableEntryFill */
/* FUNC hsynUnrefTableSetA */
u8 hsynUnrefTableSetA(u8 x)
{
    u32 r;

    r = hsynTableEntryFill(x);
    return 0x80 - r;
}
/* ENDF hsynUnrefTableSetA */
/* FUNC hsynUnrefTableSetB */
u8 hsynUnrefTableSetB(u8 x)
{
    int c;

    c = 0x80;
    return c - hsynTableEntryFill(c - x);
}
/* ENDF hsynUnrefTableSetB */
/* FUNC hsynUnrefTableSetC */
u8 hsynUnrefTableSetC(u8 x)
{
    return hsynTableEntryFill(0x80 - x);
}
/* ENDF hsynUnrefTableSetC */
INCLUDE_ASM("MODHSYN", hsynVolumeScale);
/* FUNC hsynSeqByteRead */
int hsynSeqByteRead(sceHSynEnv_t *env)
{
    return (s16)((env->unk1C - 0x8000) >> 1);
}
/* ENDF hsynSeqByteRead */
/* FUNC hsynSeqWordRead */
int hsynSeqWordRead(sceHSynEnv_t *env)
{
    u32 t;

    t = 0xFFFF - env->unk1C;
    return (s16)((t - 0x8000) >> 1);
}
/* ENDF hsynSeqWordRead */
/* FUNC hsynSeqVarLenRead */
int hsynSeqVarLenRead(sceHSynEnv_t *env)
{
    int v;

    v = env->unk1C;
    if (v > 0xBFFF) {
        v -= 0x10000;
    } else if (v >= 0x4001) {
        v = 0x8000 - v;
    }
    return (s16)v;
}
/* ENDF hsynSeqVarLenRead */
/* FUNC hsynSeqSkip */
int hsynSeqSkip(sceHSynEnv_t *env)
{
    int v;

    v = -0x4000;
    if ((int)env->unk1C > 0x7FFF) {
        v = 0x4000;
    }
    return v;
}
/* ENDF hsynSeqSkip */
INCLUDE_ASM("MODHSYN", hsynSeqSeek);
/* FUNC hsynSeqEmit */
int hsynSeqEmit(sceHSynEnv_t *env, int sel)
{
    int v;

    if (sel != 0) {
        v = (s16)hsynSeqEmitTail() >> 2;
    } else {
        v = env->level;
    }
    return v;
}
/* ENDF hsynSeqEmit */
INCLUDE_ASM("MODHSYN", hsynEffectDispatch);
INCLUDE_ASM("MODHSYN", hsynSeqStateReset);
INCLUDE_ASM("MODHSYN", hsynSeqInterpretA);
INCLUDE_ASM("MODHSYN", hsynSeqStart);
INCLUDE_ASM("MODHSYN", hsynSeqInterpretB);
INCLUDE_ASM("MODHSYN", D_00009AC4);
/* FUNC hsynSeqEmitTail */
int hsynSeqEmitTail()
{
    int seed;

    seed = hsynRandSeed * 0x41C64E6D + 0x3039;
    hsynRandSeed = seed;
    return seed >> 16;
}
/* ENDF hsynSeqEmitTail */
INCLUDE_ASM("MODHSYN", D_00009AFC);
#ifdef NON_MATCHING
/* One nop long: gas will not move the second half of the `sw w, sym` macro
 * into the branch delay slot, where the retail assembler put it. */
/* FUNC modhsyn_e16 */
int modhsyn_e16(sceHSynSdLog_t *w)
{
    hsynSdWorkArea = w;
    if (w != NULL) {
        w->index = 0;
        w->unk04 = 0;
    }
    return 0;
}
/* ENDF modhsyn_e16 */
#else
INCLUDE_ASM("MODHSYN", modhsyn_e16);
#endif
/* FUNC hsynSdLockAcquire */
/* The work area is a ring of 28-byte call records; +0 holds the opcode with
 * bit 31 set while the call is outstanding, +4 its result. */
sceHSynSdLogEnt_t *hsynSdLockAcquire()
{
    sceHSynSdLog_t *w;
    u32 i;
    sceHSynSdLogEnt_t *e;

    w = hsynSdWorkArea;
    if (w == NULL) {
        return NULL;
    }
    i = w->index;
    e = &w->entry[i];
    w->index = i + 1;
    if (i + 1 >= w->size) {
        w->index = 0;
    }
    return e;
}
/* ENDF hsynSdLockAcquire */
/* FUNC hsynSdLockRelease */
void hsynSdLockRelease(sceHSynSdLogEnt_t *e, int op, u32 a0, u32 a1, u32 a2, u32 a3, u32 a4)
{
    if (e != NULL) {
        e->op = op | 0x80000000;
        e->arg[0] = a0;
        e->arg[1] = a1;
        e->arg[2] = a2;
        e->arg[3] = a3;
        e->arg[4] = a4;
    }
}
/* ENDF hsynSdLockRelease */
/* FUNC hsynSdLockWait */
void hsynSdLockWait(sceHSynSdLogEnt_t *e, int result)
{
    if (e != NULL) {
        e->result = result;
        e->op &= 0x7FFFFFFF;
    }
}
/* ENDF hsynSdLockWait */
/* FUNC hsynSdInit */
int hsynSdInit(u32 flag)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 0, flag, 0, 0, 0, 0);
    ret = libsd_sceSdInit(flag);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdInit */
/* FUNC hsynSdSetParam */
int hsynSdSetParam(u16 entry, u16 value)
{
    sceHSynSdLogEnt_t *e;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 1, entry, value, 0, 0, 0);
    libsd_sceSdSetParam(entry, value);
    hsynSdLockWait(e, 0);
}
/* ENDF hsynSdSetParam */
/* FUNC hsynSdGetParam */
int hsynSdGetParam(u16 entry)
{
    sceHSynSdLogEnt_t *e;
    u16 ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 2, entry, 0, 0, 0, 0);
    ret = libsd_sceSdGetParam(entry);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdGetParam */
/* FUNC hsynSdSetSwitch */
int hsynSdSetSwitch(u16 entry, u32 value)
{
    sceHSynSdLogEnt_t *e;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 3, entry, value, 0, 0, 0);
    libsd_sceSdSetSwitch(entry, value);
    hsynSdLockWait(e, 0);
}
/* ENDF hsynSdSetSwitch */
/* FUNC hsynSdGetSwitch */
u32 hsynSdGetSwitch(u16 entry)
{
    sceHSynSdLogEnt_t *e;
    u32 ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 4, entry, 0, 0, 0, 0);
    ret = libsd_sceSdGetSwitch(entry);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdGetSwitch */
/* FUNC hsynSdSetAddr */
int hsynSdSetAddr(u16 entry, u32 value)
{
    sceHSynSdLogEnt_t *e;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 5, entry, value, 0, 0, 0);
    libsd_sceSdSetAddr(entry, value);
    hsynSdLockWait(e, 0);
}
/* ENDF hsynSdSetAddr */
/* FUNC hsynSdGetAddr */
u32 hsynSdGetAddr(u16 entry)
{
    sceHSynSdLogEnt_t *e;
    u32 ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 6, entry, 0, 0, 0, 0);
    ret = libsd_sceSdGetAddr(entry);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdGetAddr */
/* FUNC hsynSdSetCoreAttr */
int hsynSdSetCoreAttr(u16 entry, u16 value)
{
    sceHSynSdLogEnt_t *e;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 7, entry, value, 0, 0, 0);
    libsd_sceSdSetCoreAttr(entry, value);
    hsynSdLockWait(e, 0);
}
/* ENDF hsynSdSetCoreAttr */
/* FUNC hsynSdNote2Pitch */
int hsynSdNote2Pitch(u16 center, u16 fine, u16 note, s16 bend)
{
    sceHSynSdLogEnt_t *e;
    u16 ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 9, center, fine, note, bend, 0);
    ret = libsd_sceSdNote2Pitch(center, fine, note, bend);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdNote2Pitch */
/* FUNC hsynSdPitch2Note */
int hsynSdPitch2Note(u16 center, u16 fine, u16 pitch)
{
    sceHSynSdLogEnt_t *e;
    u16 ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 10, center, fine, pitch, 0, 0);
    ret = libsd_sceSdPitch2Note(center, fine, pitch);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdPitch2Note */
/* FUNC hsynSdProcBatch */
int hsynSdProcBatch(u32 *cmds, u32 *rets, u32 count)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 11, (u32)cmds, (u32)rets, count, 0, 0);
    ret = libsd_sceSdProcBatch(cmds, rets, count);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdProcBatch */
/* FUNC hsynSdProcBatchEx */
int hsynSdProcBatchEx(u32 *cmds, u32 *rets, u32 count, u32 flags)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 12, (u32)cmds, (u32)rets, count, flags, 0);
    ret = libsd_sceSdProcBatchEx(cmds, rets, count, flags);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdProcBatchEx */
/* FUNC hsynSdVoiceTrans */
int hsynSdVoiceTrans(s16 chan, u16 mode, u8 *iopAddr, u32 spuAddr, u32 size)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 13, chan, mode, (u32)iopAddr, spuAddr, size);
    ret = libsd_sceSdVoiceTrans(chan, mode, iopAddr, spuAddr, size);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdVoiceTrans */
/* FUNC hsynSdBlockTrans */
int hsynSdBlockTrans(s16 chan, u16 mode, u8 *iopAddr, u32 size)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 14, chan, mode, (u32)iopAddr, size, 0);
    ret = libsd_sceSdBlockTrans(chan, mode, iopAddr, size);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdBlockTrans */
/* FUNC hsynSdVoiceTransStatus */
int hsynSdVoiceTransStatus(s16 chan, s16 flag)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 15, chan, flag, 0, 0, 0);
    ret = libsd_sceSdVoiceTransStatus(chan, flag);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdVoiceTransStatus */
/* FUNC hsynSdBlockTransStatus */
int hsynSdBlockTransStatus(s16 chan, s16 flag)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 16, chan, flag, 0, 0, 0);
    ret = libsd_sceSdBlockTransStatus(chan, flag);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdBlockTransStatus */
/* FUNC hsynSdSetEffectAttr */
int hsynSdSetEffectAttr(int core, sceSdEffectAttr_t *attr)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 19, core, (u32)attr, 0, 0, 0);
    ret = libsd_sceSdSetEffectAttr(core, attr);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdSetEffectAttr */
/* FUNC hsynSdGetEffectAttr */
void hsynSdGetEffectAttr(int core, sceSdEffectAttr_t *attr)
{
    sceHSynSdLogEnt_t *e;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 20, core, (u32)attr, 0, 0, 0);
    libsd_sceSdGetEffectAttr(core, attr);
    hsynSdLockWait(e, 0);
}
/* ENDF hsynSdGetEffectAttr */
/* FUNC hsynSdClearEffectWorkArea */
int hsynSdClearEffectWorkArea(int core, int channel, int effectMode)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 21, core, channel, effectMode, 0, 0);
    ret = libsd_sceSdClearEffectWorkArea(core, channel, effectMode);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdClearEffectWorkArea */
INCLUDE_ASM("MODHSYN", D_0000A5E4);
INCLUDE_ASM("MODHSYN", hsynEffectDepthSet);
/* FUNC hsynCoreStateGet */
int hsynCoreStateGet(sceHSynVoice_t *v)
{
    v->lfoA.unk00 = 0;
    v->lfoA.unk04 = 0;
    v->lfoA.unk02 = 0;
    v->lfoA.unk06 = 0;
    v->lfoA.unk08 = 0;
    v->lfoA.unk0C = 0;
    v->lfoA.unk10 = 0;
    return 0;
}
/* ENDF hsynCoreStateGet */
INCLUDE_ASM("MODHSYN", hsynPitchBendApply);
INCLUDE_ASM("MODHSYN", D_0000A754);
INCLUDE_ASM("MODHSYN", hsynEffectFeedbackSet);
/* FUNC hsynCoreStateSet */
int hsynCoreStateSet(sceHSynVoice_t *v)
{
    v->lfoB.unk00 = 0;
    v->lfoB.unk04 = 0;
    v->lfoB.unk02 = 0;
    v->lfoB.unk06 = 0;
    v->lfoB.unk08 = 0;
    v->lfoB.unk0C = 0;
    v->lfoB.unk10 = 0;
    return 0;
}
/* ENDF hsynCoreStateSet */
INCLUDE_ASM("MODHSYN", func_0000A810);
INCLUDE_ASM("MODHSYN", D_0000A92C);
INCLUDE_ASM("MODHSYN", func_0000A930);
INCLUDE_ASM("MODHSYN", func_0000A9FC);
/* FUNC hsynChannelStateGet */
int hsynChannelStateGet(sceHSynVoice_t *v)
{
    *(u32 *)&v->param[0] = 0;
    *(u32 *)&v->param[4] = -1;
    *(u32 *)&v->param[8] = 0;
    *(u32 *)&v->param[12] = 0;
    *(u32 *)&v->param[16] = 0;
    *(u32 *)&v->param[20] = 0;
    *(u32 *)&v->param[24] = 0;
    *(u32 *)&v->param[28] = 0;
    *(u32 *)&v->param[32] = 0;
    *(u32 *)&v->param[36] = 0;
    *(u32 *)&v->param[40] = 0;
    *(u32 *)&v->param[44] = 0x80;
    *(u32 *)&v->param[48] = 0x80;
    *(u32 *)&v->param[52] = 1;
    return 0;
}
/* ENDF hsynChannelStateGet */
INCLUDE_ASM("MODHSYN", func_0000AB90);
INCLUDE_ASM("MODHSYN", hsynLibraryTable);
/* FUNC hsynRetnStub */
void hsynRetnStub(void)
{
}
/* ENDF hsynRetnStub */
INCLUDE_ASM("MODHSYN", D_0000B094);
INCLUDE_ASM("MODHSYN", libsd_sceSdInit);
INCLUDE_ASM("MODHSYN", libsd_sceSdSetParam);
INCLUDE_ASM("MODHSYN", libsd_sceSdGetParam);
INCLUDE_ASM("MODHSYN", libsd_sceSdSetSwitch);
INCLUDE_ASM("MODHSYN", libsd_sceSdGetSwitch);
INCLUDE_ASM("MODHSYN", libsd_sceSdSetAddr);
INCLUDE_ASM("MODHSYN", libsd_sceSdGetAddr);
INCLUDE_ASM("MODHSYN", libsd_sceSdSetCoreAttr);
INCLUDE_ASM("MODHSYN", libsd_sceSdNote2Pitch);
INCLUDE_ASM("MODHSYN", libsd_sceSdPitch2Note);
INCLUDE_ASM("MODHSYN", libsd_sceSdProcBatch);
INCLUDE_ASM("MODHSYN", libsd_sceSdProcBatchEx);
INCLUDE_ASM("MODHSYN", libsd_sceSdVoiceTrans);
INCLUDE_ASM("MODHSYN", libsd_sceSdBlockTrans);
INCLUDE_ASM("MODHSYN", libsd_sceSdVoiceTransStatus);
INCLUDE_ASM("MODHSYN", libsd_sceSdBlockTransStatus);
INCLUDE_ASM("MODHSYN", libsd_sceSdSetEffectAttr);
INCLUDE_ASM("MODHSYN", libsd_sceSdGetEffectAttr);
INCLUDE_ASM("MODHSYN", libsd_sceSdClearEffectWorkArea);
INCLUDE_ASM("MODHSYN", D_0000B14C);
INCLUDE_ASM("MODHSYN", intrman_CpuSuspendIntr);
INCLUDE_ASM("MODHSYN", intrman_CpuResumeIntr);
INCLUDE_ASM("MODHSYN", D_0000B178);
INCLUDE_ASM("MODHSYN", loadcore_ReleaseLibraryEntries);
INCLUDE_ASM("MODHSYN", D_0000B19C);
INCLUDE_ASM("MODHSYN", thbase_DelayThread);
INCLUDE_ASM("MODHSYN", D_0000B1C0);
INCLUDE_ASM("MODHSYN", thbase_USecToSysClock);
INCLUDE_ASM("MODHSYN", func_0000B1E4);
INCLUDE_ASM("MODHSYN", D_0000B1EC);
