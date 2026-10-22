#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "newdata.h"

extern s32 scrGetIntPara(s32 n);

extern void scrSetIntReturnValue(s32 i);

extern void datAddItem(s32 id, s32 num);

extern void datAddHearts(s32 id);

extern void datAddMaka(s32 addmaka);

extern void datAddMakaPar(s32 addmaka);

extern s32 datCheckMaka(s32 maka);

extern void datPartyDamage(s32 mode);

extern void datPartyKaifuku(void);

extern s32 datGetUniqueDevilId(s32 uid);

extern s32 datCheckStockFull(void);

extern s32 datGetPlayerParam(s32 id);

extern void datAddPlayerParam(s32 id, s32 add);

extern s32 datGetNakamaParam(s32 did, s32 id);

extern s32 datGetPlayerLND(void);

extern s32 datGetPlayerLevel(void);

extern void datDeadNakama(s32 did);

extern s32 datBossRashGetRec(s32 eid, s32 type);

extern void datRemoveHearts(void);

extern void datEquipHearts(s32 HeartsID);

extern s32 p2sdPlaySE(s32 id, s32 volume, s32 pan);

extern s32 datSearchDevilStock(s32 id);

extern s32 datAddDevil(s32 id, s32 sw);

extern s32 datAddUniqueDevil(s32 id, s32 sw, s32 uid);

extern s32 datSearchUniqueDevilStock(s32 uid);

extern s32 datDelDevil(s32 id);

extern void datCopyDevilWork(s32 id, datUnitWork_t *w);

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
