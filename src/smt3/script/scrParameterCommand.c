#include "common.h"
#include "basic.h"
#include "kernel.h"


extern sdfGrOt_t sdfGrOtTable[];
extern s8 dds3PadButtonState[];
extern void *sdfDevPrintf(s32 x, s32 y, s32 z, s32 flags, const char *format, ...);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void *sdfGrAllocPacketBuff(u32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern dds3Basic_t *dds3GetActiveScene(void);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern s32 dds3SceneBasicExistBasic(dds3Basic_t *scene, dds3Basic_t *basic);
extern void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 width, s32 height);
extern s32 evtSceneDispProcess(void *id);
extern s32 DispBaseData(dds3Basic_t *basic, s32 x, s32 y, sdfDrawTag_t *list);

extern s32 scrGetIntPara(s32 parameterIndex);
extern void scrSetIntReturnValue(s32 value);
extern void datAddItem(s32 itemId, s32 amount);
extern void datAddHearts(s32 amount);
extern void datAddMaka(s32 amount);
extern void datAddMakaPar(s32 percent);
extern s32 datCheckMaka(s32 amount);
extern void datPartyDamage(s32 percent);
extern void datPartyKaifuku(void);
extern s32 datGetUniqueDevilId(s32 uniqueId);
extern s32 datCheckStockFull(void);
extern s32 datGetPlayerParam(s32 parameter);
extern void datAddPlayerParam(s32 parameter, s32 amount);
extern s32 datGetNakamaParam(s32 devilId, s32 parameter);
extern s32 datGetPlayerLND(void);
extern s32 datGetPlayerLevel(void);
extern void datDeadNakama(s32 devilId);
extern s32 datBossRashGetRec(s32 encounterId, s32 record);

s32 prm_ITEM_PLUS(void)
{
    s32 itemId = scrGetIntPara(0);
    s32 amount = scrGetIntPara(1);

    datAddItem(itemId, amount);
    return 1;
}

s32 prm_DH_PLUS(void)
{
    datAddHearts(scrGetIntPara(0));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_DH_EQUIP);

s32 prm_MAKA_PLUS(void)
{
    datAddMaka(scrGetIntPara(0));
    return 1;
}

s32 prm_MAKA_PLUS_PER(void)
{
    datAddMakaPar(scrGetIntPara(0));
    return 1;
}

s32 prm_MAKA_CHECK(void)
{
    if (datCheckMaka(scrGetIntPara(0)) != 0) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

s32 prm_PARTY_DAMAGE(void)
{
    datPartyDamage(scrGetIntPara(0));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_PARTY_KAIFUKU);

s32 prm_PARTY_KAIFUKU2(void)
{
    datPartyKaifuku();
    return 1;
}

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_DV_PLUS);

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_DV_CHECK);

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_UDV_LEAVE);

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_UDV_CHECK);

s32 prm_UDV_GETID(void)
{
    scrSetIntReturnValue(datGetUniqueDevilId(scrGetIntPara(0)));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_UDV_DELETE_OK);

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_UDV_DELETE_NG);

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_STOCK_PLUS);

s32 prm_STOCK_FULL_CHECK(void)
{
    if (datCheckStockFull() != 0) {
        scrSetIntReturnValue(1);
    } else {
        scrSetIntReturnValue(0);
    }
    return 1;
}

s32 prm_PLAYER_GET_PARAM(void)
{
    scrSetIntReturnValue(datGetPlayerParam(scrGetIntPara(0)));
    return 1;
}

s32 prm_PLAYER_ADD_PARAM(void)
{
    s32 parameter = scrGetIntPara(0);
    s32 amount = scrGetIntPara(1);

    datAddPlayerParam(parameter, amount);
    return 1;
}

s32 prm_NAKAMA_GET_PARAM(void)
{
    s32 devilId = scrGetIntPara(0);
    s32 parameter = scrGetIntPara(1);

    scrSetIntReturnValue(datGetNakamaParam(devilId, parameter));
    return 1;
}

s32 prm_PLAYER_GET_LND(void)
{
    scrSetIntReturnValue(datGetPlayerLND());
    return 1;
}

s32 prm_PLAYER_GET_LEVEL(void)
{
    scrSetIntReturnValue(datGetPlayerLevel());
    return 1;
}

s32 prm_NAKAMA_DEAD(void)
{
    datDeadNakama(scrGetIntPara(0));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_NAKAMA_CHK_PIKUKO);

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_PIKUKO_GETID);

s32 prm_BOSS_PRESS_REC(void)
{
    s32 encounterId = scrGetIntPara(0);
    s32 record = scrGetIntPara(1);

    scrSetIntReturnValue(datBossRashGetRec(encounterId, record));
    return 1;
}

INCLUDE_ASM(const s32, "script/scrParameterCommand", prm_DV_COPY_PARAM);

INCLUDE_ASM(const s32, "script/scrParameterCommand", DispBaseData);

u32 DispObjectBaseData(dds3Basic_t *worldNode, s32 x, s32 y, sdfDrawTag_t *list) {
    dds3ObjectBaseControl_t *handle;
    void *command;

    handle = dds3GetBaseControlWorkData(worldNode);
    command = sdfDevPrintf((x * 3 << 6) + 0x7000,
                                           (y * 3 << 5) + 0x7900,
                                           0xFEFFFF,
                                           0,
                                           "FLAG  : 0x%08X",
                                           handle->flag);
    sdfDrawTagLink(list, (u32)command);
    return 1;
}

s32 dds3DebugBasic(dds3ProcessID_t *task) {
    s32 width;
    dds3Basic_t *node;
    sdfGrOt_t *surface;
    sdfDrawTag_t *list;
    sdfDrawTag_t *spriteList;

    if (dds3GetActiveScene() == NULL) {
        return -1;
    }
    node = (dds3Basic_t *)dds3GetProcessData(task);
    if (dds3SceneBasicExistBasic(dds3GetActiveScene(), (dds3Basic_t *)node) == 0) {
        return (s32)evtSceneDispProcess;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    width = DispBaseData(node, 3, 0xA, list) + 0xB;
    if (((u8 *)&node->type)[3] < 0xA) {
        if (((u8 *)&node->type)[3] >= 4) {
            width += DispObjectBaseData(node, 3, width, list);
        }
    }
    spriteList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(spriteList);
    dds3ConsPanelTagLink(spriteList, 0x24, 0x72, 0x23, width - 9);
    surface = &sdfGrOtTable[0x51];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, spriteList);
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
    if (dds3PadButtonState[0x33] < 0) {
        return (s32)evtSceneDispProcess;
    }
    return 0;
}

INCLUDE_ASM(const s32, "script/scrParameterCommand", dds3AllocBasic);
INCLUDE_SDATA(const s32, "script/scrParameterCommand", D_0054E9A8);

INCLUDE_SDATA(const s32, "script/scrParameterCommand", D_0054E9B0);

INCLUDE_SDATA(const s32, "script/scrParameterCommand", D_0054E9B8);

