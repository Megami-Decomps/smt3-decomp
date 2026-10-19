#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "newdata.h"


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
extern void datRemoveHearts(void);
extern void datEquipHearts(s32 heartsId);
extern s32 p2sdPlaySE(s32 id, s32 volume, s32 pan);
extern s32 datSearchDevilStock(s32 devilId);
extern s32 datAddDevil(s32 devilId, s32 kind);
extern s32 datAddUniqueDevil(s32 devilId, s32 kind, s32 uniqueId);
extern s32 datSearchUniqueDevilStock(s32 uniqueId);
extern s32 datDelDevil(s32 devilId);
extern void datCopyDevilWork(s32 slot, datUnitWork_t *work);
extern void *sdfChipAllocDirect(u32 size);
extern void dds3DeleteBasic(dds3Basic_t *basic);
extern dds3BasicFunction_t *dds3BasicFuncTablePtr[];
extern s32 sdfFloatToULong(f32 value);
extern char D_0052F6B8[];
extern char D_0052F6C8[];
extern char D_0052F6D8[];
extern char D_0052F6F0[];
extern char D_0052F708[];
extern char D_0054E9A8[];
extern char D_0054E9B0[];
extern char D_0054E9B8[];

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

s32 prm_DH_EQUIP(void)
{
    s32 heartsId = scrGetIntPara(0);

    if (heartsId == 0) {
        datRemoveHearts();
    } else {
        datEquipHearts(heartsId);
    }
    return 1;
}

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

s32 prm_PARTY_KAIFUKU(void)
{
    datPartyKaifuku();
    p2sdPlaySE(0x10, 0x7F, 0x3F);
    return 1;
}

s32 prm_PARTY_KAIFUKU2(void)
{
    datPartyKaifuku();
    return 1;
}

s32 prm_DV_PLUS(void)
{
    s32 devilId = scrGetIntPara(2);
    s32 kind = scrGetIntPara(1);

    if (devilId == 0) {
        devilId = scrGetIntPara(0);
        if (datSearchDevilStock(devilId) == -1) {
            if (kind == 2) {
                datAddDevil(devilId | 0x80000000, 1);
                return 1;
            } else {
                datAddDevil(devilId, kind);
                return 1;
            }
        }
    } else {
        datAddUniqueDevil(scrGetIntPara(0), kind, devilId);
    }
    return 1;
}

s32 prm_DV_CHECK(void)
{
    if (datSearchDevilStock(scrGetIntPara(0)) == -1) {
        scrSetIntReturnValue(0);
    } else {
        scrSetIntReturnValue(1);
    }
    return 1;
}

s32 prm_UDV_LEAVE(void)
{
    s32 index = datSearchUniqueDevilStock(scrGetIntPara(0));

    if (index != -1) {
        datDelDevil(DDS3_GBWK->akuma[index].devilId);
    }
    return 1;
}

s32 prm_UDV_CHECK(void)
{
    if (datSearchUniqueDevilStock(scrGetIntPara(0)) == -1) {
        scrSetIntReturnValue(0);
    } else {
        scrSetIntReturnValue(1);
    }
    return 1;
}

s32 prm_UDV_GETID(void)
{
    scrSetIntReturnValue(datGetUniqueDevilId(scrGetIntPara(0)));
    return 1;
}

s32 prm_UDV_DELETE_OK(void)
{
    s32 index = datSearchUniqueDevilStock(scrGetIntPara(0));

    if (index != -1) {
        DDS3_GBWK->akuma[index].flag &= ~0x800;
    }
    return 1;
}

s32 prm_UDV_DELETE_NG(void)
{
    s32 index = datSearchUniqueDevilStock(scrGetIntPara(0));

    if (index != -1) {
        DDS3_GBWK->akuma[index].flag |= 0x800;
    }
    return 1;
}

s32 prm_STOCK_PLUS(void)
{
    s32 amount = scrGetIntPara(0);
    s32 total = DDS3_GBWK->akuma[0].unk00 + amount;

    if (total > 12) {
        total = 12;
    }
    DDS3_GBWK->akuma[0].unk00 = total;
    return 1;
}

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

s32 prm_NAKAMA_CHK_PIKUKO(void)
{
    s32 i;
    s32 devilId;

    for (i = 1; i < 16; i++) {
        if ((DDS3_GBWK->akuma[i].flag & 1) != 0) {
            devilId = DDS3_GBWK->akuma[i].devilId;
            if (devilId == scrGetIntPara(0)) {
                if ((DDS3_GBWK->akuma[i].flag & 0x1000) != 0) {
                    scrSetIntReturnValue(1);
                    return 1;
                }
            }
        }
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 prm_PIKUKO_GETID(void)
{
    s32 i;

    for (i = 1; i < 16; i++) {
        if ((DDS3_GBWK->akuma[i].flag & 1) != 0) {
            if ((DDS3_GBWK->akuma[i].flag & 0x1000) != 0) {
                scrSetIntReturnValue(DDS3_GBWK->akuma[i].devilId);
                return 1;
            }
        }
    }
    scrSetIntReturnValue(0);
    return 1;
}

s32 prm_BOSS_PRESS_REC(void)
{
    s32 encounterId = scrGetIntPara(0);
    s32 record = scrGetIntPara(1);

    scrSetIntReturnValue(datBossRashGetRec(encounterId, record));
    return 1;
}

s32 prm_DV_COPY_PARAM(void)
{
    s32 devilId = scrGetIntPara(0);
    s32 slot = scrGetIntPara(1);
    s32 i;

    for (i = 1; i < 16; i++) {
        if ((DDS3_GBWK->akuma[i].flag & 1) != 0) {
            if (DDS3_GBWK->akuma[i].devilId == devilId) {
                datCopyDevilWork(slot, (datUnitWork_t *)&DDS3_GBWK->akuma[i].flag);
                return 1;
            }
        }
    }
    return 1;
}

/* NON_MATCHING: 170 of 190 words differ, one word short; retail spills the
 * label column and keeps the value column, and this keeps the label column. */
#ifdef NON_MATCHING
s32 DispBaseData(dds3Basic_t *basic, s32 x, s32 y, sdfDrawTag_t *list)
{
    s32 valueX = ((x + 7) * 3 << 6) + 0x7000;
    s32 labelX = (x * 3 << 6) + 0x7000;
    dds3PoseSet_t *pose;
    void *command;
    char *format;
    s32 v0;
    s32 v1;
    s32 v2;

    command = sdfDevPrintf(labelX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0, D_0052F6B8, basic->serialID);
    sdfDrawTagLink(list, (u32)command);
    y++;
    command = sdfDevPrintf(labelX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0, D_0052F6C8, basic->name);
    sdfDrawTagLink(list, (u32)command);
    y++;
    pose = basic->poseset;
    if (pose == NULL) {
        return 2;
    }
    y++;
    command = sdfDevPrintf(labelX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0, D_0054E9A8);
    sdfDrawTagLink(list, (u32)command);
    format = D_0052F6D8;
    v0 = sdfFloatToULong(pose->pos.x);
    v1 = sdfFloatToULong(pose->pos.y);
    v2 = sdfFloatToULong(pose->pos.z);
    command = sdfDevPrintf(valueX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0xE, format, v0, v1, v2);
    sdfDrawTagLink(list, (u32)command);
    y += 2;
    command = sdfDevPrintf(labelX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0, D_0054E9B0);
    sdfDrawTagLink(list, (u32)command);
    format = D_0052F6F0;
    v0 = sdfFloatToULong(pose->rot.x);
    v1 = sdfFloatToULong(pose->rot.y);
    v2 = sdfFloatToULong(pose->rot.z);
    command = sdfDevPrintf(valueX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0xE, format, v0, v1, v2,
                           sdfFloatToULong(pose->rot.w));
    sdfDrawTagLink(list, (u32)command);
    y += 2;
    command = sdfDevPrintf(labelX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0, D_0054E9B8);
    sdfDrawTagLink(list, (u32)command);
    format = D_0052F708;
    v0 = sdfFloatToULong(pose->scl.x);
    v1 = sdfFloatToULong(pose->scl.y);
    v2 = sdfFloatToULong(pose->scl.z);
    command = sdfDevPrintf(valueX, (y * 3 << 5) + 0x7900, 0xFEFFFF, 0xE, format, v0, v1, v2);
    sdfDrawTagLink(list, (u32)command);
    return 8;
}
#else
INCLUDE_ASM(const s32, "script/scrParameterCommand", DispBaseData);
#endif

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

dds3Basic_t *dds3AllocBasic(u32 type)
{
    dds3Basic_t *basic;
    dds3BasicFunction_t *funcs;

    if (type >= 0x12) {
        return NULL;
    }
    basic = sdfChipAllocDirect(sizeof(dds3Basic_t));
    if (basic == NULL) {
        return NULL;
    }
    basic->type = type << 24;
    funcs = dds3BasicFuncTablePtr[type];
    basic->color = 0x80808080;
    basic->flag = 0;
    basic->serialID = 0;
    basic->name = NULL;
    basic->basicfunction = funcs;
    *(u32 *)&basic->staticdata = 0;
    *(u32 *)&basic->workdata = 0;
    basic->poseset = NULL;
    basic->next = NULL;
    basic->prev = NULL;
    basic->worknext = NULL;
    basic->workprev = NULL;
    basic->workpointer = NULL;
    if (funcs != NULL) {
        if (funcs->constructor != NULL) {
            if (((s32 (*)(dds3Basic_t *))funcs->constructor)(basic) != 1) {
                dds3DeleteBasic(basic);
                return NULL;
            }
        }
    }
    return basic;
}
INCLUDE_SDATA(const s32, "script/scrParameterCommand", D_0054E9A8);

INCLUDE_SDATA(const s32, "script/scrParameterCommand", D_0054E9B0);

INCLUDE_SDATA(const s32, "script/scrParameterCommand", D_0054E9B8);

