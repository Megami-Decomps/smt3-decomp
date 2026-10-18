#include "common.h"
#include "kernel.h"
#include "nb.h"


extern dds3ProcessID_t *dds3SearchProcessName(char *name);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern void *sdfChipAllocDirect(u32 size);
extern void nbPanelStockFadeOUT(void);
extern void nbPanelStockEnd(void);
extern char D_00544350[];

nbKoukaProcessData_t *nbGetKoukaProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(D_00544350));
}

s32 nbGetKoukaPacketCnt(void)
{
    nbKoukaProcessData_t *work = nbGetKoukaProcessData();

    return work->packetcnt;
}

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbGetUniqueIdKoukaPacketCnt);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbCountKoukaByType);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbLinkPacket);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbDelKoukaEntry);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbAddKoukaPacket);

INCLUDE_RODATA(const s32, "newbattle/nbKoukaProcess", D_00544350);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", DelKoukaPacket);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbDelKoukaPacket);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbDelAllUniqueIDKoukaPacket);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbDelAllKoukaPacketType);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbDelAllMojiNumEffectKoukaPacket);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbDelUnEventKoukaPacket);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbOnOffKoukaPacketFlag);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbCheckSePacket);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunBossEffectKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunHatudoEffectEndKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunSkillEffectKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbRunKoukaCase1);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunNumEffectKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunUnitColKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunMotionStopKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunMotionKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunDamageMotionKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbReturnKoukaUnit);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunSummonKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunSakeKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunBadEffKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunBadEffKaifukuKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunHitEffectKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", NumEffectClip);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbClipKoukaNum);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbDrawKoukaNum);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbApplyHpMp);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbRunKoukaCase12);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbApplyBadStat);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunBadMotKaifukuKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunSeKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunPcmKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunHitStopKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunAnalyzeKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunHelpKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunFormOnOffKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunFormFlagOnOffKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunFlyKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunDeadEventKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunFadeKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunSkillNameKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunHojoKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunFlareChargeKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunPadShockKouka);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", RunSkillColorKouka);

void RunStockPanelFadeOutKouka(void)
{
    nbPanelStockFadeOUT();
}

void RunStockPanelEndKouka(void)
{
    nbPanelStockEnd();
}

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbKoukaProcess1);

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbKoukaShutdown);

nbKoukaProcessData_t *InitKoukaProcessData(nbMainProcessData_t *battle)
{
    nbKoukaProcessData_t *work = sdfChipAllocDirect(sizeof(nbKoukaProcessData_t));

    work->data = battle;
    work->packetcnt = 0;
    work->first = NULL;
    work->last = NULL;
    return work;
}

INCLUDE_ASM(const s32, "newbattle/nbKoukaProcess", nbInitKoukaProcess);

INCLUDE_SDATA(const s32, "newbattle/nbKoukaProcess", D_00551138);

