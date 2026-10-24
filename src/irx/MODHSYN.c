#include "MODHSYN.h"

/* MODHSYN.IRX, Sony's SPU2 synthesiser: it owns the voices and the envelopes,
 * and a .SMG's bank is loaded into it. See docs/irx-modules.md.
 *
 * The chunks still in assembly are the ones no C has been written for yet,
 * plus the import stubs and the import and export tables between them. */

INCLUDE_ASM("MODHSYN", modhsyn_e30);
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
INCLUDE_ASM("MODHSYN", hsynVoiceSetAttack);
INCLUDE_ASM("MODHSYN", hsynVoiceSetRelease);
INCLUDE_ASM("MODHSYN", hsynVoiceSetVolumeLeft);
INCLUDE_ASM("MODHSYN", hsynVoiceSetVolumeRight);
/* FUNC hsynVoiceFlagsGet */
int hsynVoiceFlagsGet(sceHSynChannel_t *chan, u8 semitones)
{
    chan->bendRange = semitones * 20;
}
/* ENDF hsynVoiceFlagsGet */
INCLUDE_ASM("MODHSYN", hsynVoiceFlagsSet);
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
INCLUDE_ASM("MODHSYN", modhsyn_e26);
INCLUDE_ASM("MODHSYN", hsynParamSlotResolve);
INCLUDE_ASM("MODHSYN", modhsyn_e10);
INCLUDE_ASM("MODHSYN", modhsyn_e11);
INCLUDE_ASM("MODHSYN", modhsyn_e12);
INCLUDE_ASM("MODHSYN", modhsyn_e13);
INCLUDE_ASM("MODHSYN", modhsyn_e14);
INCLUDE_ASM("MODHSYN", modhsyn_e9);
INCLUDE_ASM("MODHSYN", modhsyn_e15);
INCLUDE_ASM("MODHSYN", hsynEffectAttrRead);
INCLUDE_ASM("MODHSYN", hsynEffectAttrWrite);
INCLUDE_ASM("MODHSYN", modhsyn_e17);
INCLUDE_ASM("MODHSYN", modhsyn_e18);
INCLUDE_ASM("MODHSYN", modhsyn_e27);
INCLUDE_ASM("MODHSYN", hsynCmdReset);
INCLUDE_ASM("MODHSYN", hsynCmdProgramChange);
INCLUDE_ASM("MODHSYN", func_000021B8);
INCLUDE_ASM("MODHSYN", hsynCmdChannelPressure);
INCLUDE_ASM("MODHSYN", hsynCmdNoteOff);
INCLUDE_ASM("MODHSYN", hsynCmdNoteOn);
INCLUDE_ASM("MODHSYN", hsynCmdVoiceAlloc);
INCLUDE_ASM("MODHSYN", func_0000273C);
INCLUDE_ASM("MODHSYN", modhsyn_e19);
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
INCLUDE_ASM("MODHSYN", hsynSdSetParamPitch);
INCLUDE_ASM("MODHSYN", func_00004614);
INCLUDE_ASM("MODHSYN", hsynSdSetParamEnvelope);
INCLUDE_ASM("MODHSYN", hsynSdSetParamVolume);
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
INCLUDE_ASM("MODHSYN", hsynVoiceMaskSet);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocExport20);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocPathB);
/* FUNC hsynVoiceAllocPathBEntry */
int hsynVoiceAllocPathBEntry(int a, int b)
{
    return hsynVoiceAllocPathB(a + 0x18, b);
}
/* ENDF hsynVoiceAllocPathBEntry */
INCLUDE_ASM("MODHSYN", hsynVoiceAllocPathC);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocPathCEntry);
INCLUDE_ASM("MODHSYN", hsynVoiceReleaseExport21);
INCLUDE_ASM("MODHSYN", hsynVoiceReleaseOne);
INCLUDE_ASM("MODHSYN", hsynVoiceReleaseGroup);
INCLUDE_ASM("MODHSYN", func_000078D4);
INCLUDE_ASM("MODHSYN", func_00007F54);
INCLUDE_ASM("MODHSYN", func_00008350);
INCLUDE_ASM("MODHSYN", hsynVoiceMaskClearOne);
INCLUDE_ASM("MODHSYN", hsynModulationApply);
INCLUDE_ASM("MODHSYN", hsynModulationStep);
INCLUDE_ASM("MODHSYN", hsynPanApply);
INCLUDE_ASM("MODHSYN", hsynLfoStep);
INCLUDE_ASM("MODHSYN", D_00008914);
INCLUDE_ASM("MODHSYN", hsynUnrefParamGetA);
INCLUDE_ASM("MODHSYN", hsynUnrefParamGetB);
INCLUDE_ASM("MODHSYN", hsynUnrefParamSetA);
INCLUDE_ASM("MODHSYN", hsynUnrefParamSetB);
INCLUDE_ASM("MODHSYN", hsynUnrefParamSetC);
INCLUDE_ASM("MODHSYN", hsynUnrefParamSetD);
INCLUDE_ASM("MODHSYN", func_00008BB0);
INCLUDE_ASM("MODHSYN", func_00008BC8);
INCLUDE_ASM("MODHSYN", func_00008C1C);
INCLUDE_ASM("MODHSYN", func_00008C34);
INCLUDE_ASM("MODHSYN", func_00008C88);
INCLUDE_ASM("MODHSYN", func_00008CA0);
INCLUDE_ASM("MODHSYN", func_00008D00);
INCLUDE_ASM("MODHSYN", func_00008D54);
INCLUDE_ASM("MODHSYN", func_00008D6C);
INCLUDE_ASM("MODHSYN", func_00008DD4);
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
INCLUDE_ASM("MODHSYN", hsynTableEntryFill);
INCLUDE_ASM("MODHSYN", hsynUnrefTableSetA);
INCLUDE_ASM("MODHSYN", hsynUnrefTableSetB);
INCLUDE_ASM("MODHSYN", hsynUnrefTableSetC);
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
INCLUDE_ASM("MODHSYN", hsynSeqVarLenRead);
INCLUDE_ASM("MODHSYN", hsynSeqSkip);
INCLUDE_ASM("MODHSYN", hsynSeqSeek);
INCLUDE_ASM("MODHSYN", hsynSeqEmit);
INCLUDE_ASM("MODHSYN", hsynEffectDispatch);
INCLUDE_ASM("MODHSYN", hsynSeqStateReset);
INCLUDE_ASM("MODHSYN", hsynSeqInterpretA);
INCLUDE_ASM("MODHSYN", hsynSeqStart);
INCLUDE_ASM("MODHSYN", hsynSeqInterpretB);
INCLUDE_ASM("MODHSYN", D_00009AC4);
INCLUDE_ASM("MODHSYN", hsynSeqEmitTail);
INCLUDE_ASM("MODHSYN", D_00009AFC);
INCLUDE_ASM("MODHSYN", modhsyn_e16);
INCLUDE_ASM("MODHSYN", hsynSdLockAcquire);
INCLUDE_ASM("MODHSYN", hsynSdLockRelease);
INCLUDE_ASM("MODHSYN", hsynSdLockWait);
INCLUDE_ASM("MODHSYN", hsynSdInit);
INCLUDE_ASM("MODHSYN", hsynSdSetParam);
INCLUDE_ASM("MODHSYN", hsynSdGetParam);
INCLUDE_ASM("MODHSYN", hsynSdSetSwitch);
INCLUDE_ASM("MODHSYN", hsynSdGetSwitch);
INCLUDE_ASM("MODHSYN", hsynSdSetAddr);
INCLUDE_ASM("MODHSYN", hsynSdGetAddr);
INCLUDE_ASM("MODHSYN", hsynSdSetCoreAttr);
INCLUDE_ASM("MODHSYN", hsynSdNote2Pitch);
INCLUDE_ASM("MODHSYN", hsynSdPitch2Note);
INCLUDE_ASM("MODHSYN", hsynSdProcBatch);
INCLUDE_ASM("MODHSYN", hsynSdProcBatchEx);
INCLUDE_ASM("MODHSYN", hsynSdVoiceTrans);
INCLUDE_ASM("MODHSYN", hsynSdBlockTrans);
INCLUDE_ASM("MODHSYN", hsynSdVoiceTransStatus);
INCLUDE_ASM("MODHSYN", hsynSdBlockTransStatus);
INCLUDE_ASM("MODHSYN", hsynSdSetEffectAttr);
INCLUDE_ASM("MODHSYN", hsynSdGetEffectAttr);
INCLUDE_ASM("MODHSYN", hsynSdClearEffectWorkArea);
INCLUDE_ASM("MODHSYN", D_0000A5E4);
INCLUDE_ASM("MODHSYN", hsynEffectDepthSet);
INCLUDE_ASM("MODHSYN", hsynCoreStateGet);
INCLUDE_ASM("MODHSYN", hsynPitchBendApply);
INCLUDE_ASM("MODHSYN", D_0000A754);
INCLUDE_ASM("MODHSYN", hsynEffectFeedbackSet);
INCLUDE_ASM("MODHSYN", hsynCoreStateSet);
INCLUDE_ASM("MODHSYN", func_0000A810);
INCLUDE_ASM("MODHSYN", D_0000A92C);
INCLUDE_ASM("MODHSYN", func_0000A930);
INCLUDE_ASM("MODHSYN", func_0000A9FC);
INCLUDE_ASM("MODHSYN", hsynChannelStateGet);
INCLUDE_ASM("MODHSYN", func_0000AB90);
INCLUDE_ASM("MODHSYN", D_0000B000);
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
