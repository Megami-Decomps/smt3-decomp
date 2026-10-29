#include "common.h"
#include "newbattle.h"
#include "nb.h"
#include "ps2asm.h"
#include "basic.h"
#include "libsdf.h"
#include "effBattle.h"

extern dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void nbKillMainProcess(void);

extern void nbCameraPressProcessInit(void);

extern void evtBitOff(s32 no);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

extern void evtDisableMoonDraw(void);

extern void nbFreeDeadMoonTex(void);

extern void nbInitEventProcess(nbMainProcessData_t *data);

extern void nbDelUnEventKoukaPacket(void);

/* The player party's unit records in the global work. */
#define NB_PLAYERUNIT ((datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54))

/* The 38-byte encounter row; only the flag word this unit reads is named. */
typedef struct {
    u8 unk_00[0x4];
    u8 formtype;
    u8 unk_05[0x1B];
    u16 flag;
    u8 unk_22[0x4];
} nbEncountForm_t;



typedef struct nbActionStack_t {
    s32 type;
    s32 from;
    s32 to;
    s32 data;
} nbActionStack_t;

extern nbEncountForm_t nbEncountFormTable[];

extern void nbClearActionStack(void);

extern void nbSetupBattleUnits(nbMainProcessData_t *data, s32 encno);

extern void nbCameraSetState(s32 state);

extern void nbGroupFormDispOff(s32 side);

extern void nbInitEncProcess(nbMainProcessData_t *data);

extern void nbPanelPressGaugeStart(void);

extern s32 nbCheckActionStack(void);

extern void nbPushAction(s32 type, s32 from, s32 to, s32 d);

extern s32 nbPopAction(nbActionStack_t *act);

extern void nbUpdateFormation(nbMainProcessData_t *data);

extern void nbInitActionProcess(nbMainProcessData_t *data, s32 type, s32 from, s32 to, s32 d);

extern void nbCameraCRSetPlayerFormRequest(void);

extern void nbInitDebugProcess(nbMainProcessData_t *data);

extern s32 D_0054F0F4;

extern sdfMemHandle_t *sdfDevLoadFileEx(s32 path, void *destination, void *size);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern s32 printf(const char *format, ...);

extern char D_00543448[];

extern s32 CheckErrorActionStack(nbMainProcessData_t *data, s32 index);

extern void CheckSenseiOrBackAttack(nbMainProcessData_t *data);

extern void nbSetPlayerStartData(nbMainProcessData_t *data);

extern void nbInitUnitProcess(nbMainProcessData_t *data);

extern void nbInitKoukaProcess(nbMainProcessData_t *data);

extern void nbInitTarSelProcess(nbMainProcessData_t *data);

extern void nbInitMaeProcess(nbMainProcessData_t *data);

extern void nbInitEffectProcess(nbMainProcessData_t *data);

extern void nbInitCameraProcess(nbMainProcessData_t *data);

extern void nbInitPanelProcess(nbMainProcessData_t *data);

extern void nbInitHelpProcess(nbMainProcessData_t *data);

extern void nbInitAutoCommProcess(nbMainProcessData_t *data);

extern dds3ProcessID_t *nbInitMakaProcess(dds3ProcessID_t *parent);

extern void nbInitStageProcess(nbMainProcessData_t *data);

extern void nbPanelPressGaugeEncountHide(void);

extern void fileSyncAll(void);

extern void sdfChipFreeDirect(void *aPtr);

extern void *sdfChipAllocDirect(u32 aSize);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *memset(void *a, s32 b, u32 c);

extern void nbFreeAllFormResources(nbMainProcessData_t *data);

extern void nbFreeEventPack(nbMainProcessData_t *data);

extern void itfMesMngRelease(s32 id);

extern s32 itfMesMngInitialize(void *resource);

extern u8 itfMesEmbedded_3F8228[];

extern void nbFormatFormation(nbMainProcessData_t *data, s32 index);

extern void nbAllocMainProcessData(nbMainProcessData_t *data);

extern void SetDebugFlag(nbMainProcessData_t *data);

extern s32 nbCheckSensei(nbMainProcessData_t *data);

extern s32 nbCheckBackAttack(nbMainProcessData_t *data);

extern s32 evtBitCheck(s32 no);





extern char D_00543540[]; /* "encno = %d\n" */

extern void effBTLSystemDelete2(struct effBTLSysHandle_t *hEffBTLSys);

extern void nbGetEncountTableItem(nbMainProcessData_t *data);

extern void nbInitResultProcess(nbMainProcessData_t *data);

extern void nbBgmFadeOut(void);

extern char D_00543550[]; /* "formationtype = %d\n" */

extern char D_00543468[]; /* "game over !!\n" */

extern char D_00543478[]; /* "level up !!\n" */

extern char D_005510B8[]; /* "nb_main" */

extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);

extern s32 rstChkResultEntry(void);

extern void evtSetAgeOfMoon(s32 age);

extern void evtEnableMoonDraw(void);

extern void nbLoadDeadMoonTex(void);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern s32 nbMainProcess1(dds3ProcessID_t *id);

extern s32 nbCheckBadKaifuku(datUnitWork_t *work, s32 type);

extern u32 datGetMaxHp(datUnitWork_t *work);

extern u32 datGetMaxMp(datUnitWork_t *work);

extern void datSetAutoKaifukuSkill(datUnitWork_t *work, s32 skill);

extern void nbInitAtoProcess(nbMainProcessData_t *data);

extern void nbSetChkEndPhase(nbMainProcessData_t *data);

extern void nbSetChkTurnPhase(nbMainProcessData_t *data);

extern void nbSetPressMaePhase(nbMainProcessData_t *data);

extern s8 dds3PadButtonState[64];

extern u32 dds3GetRandIntA(void *state, u32 limit);

extern struct nbKoukaPacket_t *nbAddDeadPcmPacket(s32 startframe, s32 uniqueid,
                                                  nbFormation_t *form);

extern struct nbKoukaPacket_t *nbAddPadShockPacket(s32 startframe, s32 uniqueid, s32 mode, s32 pow,
                                                   s32 time);

extern s32 nbCheckBadStatusGameOver(nbMainProcessData_t *data);

extern s32 nbCheckRenzokuEncount(nbMainProcessData_t *data);

extern char D_00543458[];

extern void nbSetHojoCounter(s32 formindex, s32 type, s32 point);

extern void nbSetOrder(void);

/* The 60-byte devil record; only the flag word this unit reads is named. */
typedef struct {
    u32 flag;
    u8 race;
    u8 level;
    u16 hp;
    u16 maxhp;
    u16 mp;
    u16 maxmp;
    s16 aisyoid;
    s8 param[6];
    u16 skill[8];
    s16 keisyotype;
    u16 keisyoform;
    u16 dropmakka;
    u16 dropexp;
    u8 dropitem[2];
    u8 droppoint[2];
    u16 specialbit;
    u8 specialitem;
    u8 specialpoint;
    u8 hougyokupoint;
    u8 masekipoint;
    s8 attackattr;
    u8 attackcnt;
    u8 attackinterval;
    u8 reserve1;
} datDevilFormat_t;

extern datDevilFormat_t datDevilTable[];

extern void nbSetNextOrder(void);

extern void nbSetNextPhase(s32 phase);

extern s32 nbMakeBadKaifukuPacket(s32 startframe, s32 uniqueid, nbFormation_t *form);

extern void nbAddKagututiMoon(nbMainProcessData_t *data);

extern u32 dds3DebugSign;

extern void nbCheckPackLoad(nbMainProcessData_t *data);

extern s32 nbGetStageData(void);

extern void nbCameraPressProcessThrough(void);

extern void nbPanelPressTurnOmit(void);

extern void nbPanelPressTurnStart(void);

extern s32 nbCameraPressProcessGetFrame(void);

extern s32 nbCameraPressProcessCheckRun(void);

extern s32 nbCountKoukaByType(s32 type);

extern s32 nbGetActionSeq(dds3ProcessID_t *id);

extern s32 p2sdGetPcmState(void);

extern s32 nbGetJyokyoMes(nbMainProcessData_t *data, s32 jyokyomes);

extern void nbDispMsgHelp(s32 msgid, s32 no, s32 msgtype);

extern void nbCountActiveUnits(nbMainProcessData_t *data);

extern void nbDebugMainProcess(nbMainProcessData_t *data);

extern void nbDebugCamera(nbMainProcessData_t *data);

extern void DebugDisp2(nbMainProcessData_t *data);

void SetDummy(nbMainProcessData_t *data)
{
}

nbMainProcessData_t *nbGetMainProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessPriority(0x3FA));
}

datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, nbParty_t *party)
{
    u16 flag;

    flag = party->flag;
    if ((flag & 1) == 0) {
        return 0;
    }
    if (flag & 2) {
        return &data->enemyunit[party->statindex];
    }
    if (flag & 4) {
        return 0;
    }
    return (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + party->statindex;
}

nbParty_t *nbGetPartyFromFormindex(s32 formindex)
{
    nbMainProcessData_t *data;
    nbFormation_t *form;

    if (formindex >= 16) {
        return 0;
    }
    data = nbGetMainProcessData();
    form = &data->form[formindex];
    if ((form->stat & 1) == 0) {
        return 0;
    }
    if ((form->stat & 0x40) == 0) {
        return 0;
    }
    return &data->party[form->partyindex];
}

datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex)
{
    nbMainProcessData_t *data;
    nbParty_t *party;

    if (formindex >= 16) {
        s32 index = formindex - 16;
        return NB_PLAYERUNIT + index;
    }
    data = nbGetMainProcessData();
    party = nbGetPartyFromFormindex(formindex);
    if (party == 0) {
        return 0;
    }
    if ((party->flag & 1) == 0) {
        return 0;
    }
    return nbGetUnitWorkPointer(data, party);
}

u32 nbGetSelectBit(nbMainProcessData_t *data)
{
    nbFormation_t *form;
    u32 bits;
    s32 i;

    form = data->form;
    bits = 0;
    for (i = 0; i < 16; i++) {
        if (form[i].stat & 8) {
            bits |= 1 << i;
        }
    }
    return bits;
}

void nbSetFormationSideFlag(nbFormation_t *form, s32 index)
{
    form->formindex = index;
    if (index < 4) {
        form->stat |= 0x100;
    } else if (index < 14) {
        form->stat |= 0x200;
    } else {
        form->stat |= 0x400;
    }
}

void nbDebugSelect(nbMainProcessData_t *data)
{
    if (dds3PadButtonState[0x28] < 0) {
        data->stat ^= 0x100000;
    }
    if (dds3PadButtonState[0x33] < 0) {
        data->debug = (data->debug + 1) % 2;
    }
    if (data->stat & 8) {
        if (evtBitCheck(0xC11)) {
            if (dds3GetRandIntA(0, 5) == 0) {
                nbAddDeadPcmPacket(0, 0, &data->form[2]);
                nbAddPadShockPacket(0, 0, 1, 0x96, 0x14);
            }
            if (dds3GetRandIntA(0, 5) == 0) {
                nbAddDeadPcmPacket(0, 0, &data->form[2]);
                nbAddPadShockPacket(0, 0, 1, 0x96, 0x14);
            }
        }
    }
}

extern dds3Basic_t *dds3GetActiveScene(void);
extern dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *scene);
extern void dds3SetActiveCameraSceneBasic(dds3Basic_t *scene, dds3Basic_t *camera);
extern f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *camera);
extern void dds3SetScreenFovy(f32 fovy);
extern s32 D_00550F80;
extern char *D_00550F78;
extern char *D_00550F7C;
extern char D_00550F88[];
extern sdfDrawTag_t *dds3FontReqAllocDrawTag(void);
extern void *sdfDevPrintf(u32 x, u32 y, u32 z, s32 attr, const char *format, ...);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern sdfGrOt_t D_002F6978;

void nbDebugCamera(nbMainProcessData_t *data)
{
    dds3Basic_t *scene;
    dds3Basic_t *camera;
    char *name;
    sdfDrawTag_t *tag;

    scene = dds3GetActiveScene();
    if (scene == NULL || D_00550F80 == 0) {
        return;
    }
    camera = dds3GetActiveCameraSceneBasic(scene);
    if (camera == NULL) {
        return;
    }
    if (dds3PadButtonState[0x29] != 0) {
        if (dds3PadButtonState[0x24] < 0) {
            if (data->stat & 0x20) {
                data->stat &= ~0x20;
                camera = ((dds3SceneBasic_t *)scene->workdata)->basictable[4].firstbasic;
            } else {
                if (camera->next != NULL) {
                    camera = camera->next;
                } else {
                    if (data->gamecamera != NULL) {
                        camera = data->gamecamera;
                        data->stat |= 0x20;
                    } else {
                        camera = ((dds3SceneBasic_t *)scene->workdata)->basictable[4].firstbasic;
                    }
                }
            }
            dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
            dds3SetScreenFovy(dds3CameraObjectBasicGetFovy(camera));
        } else if (dds3PadButtonState[0x25] < 0) {
            if (data->stat & 0x20) {
                data->stat &= ~0x20;
                camera = ((dds3SceneBasic_t *)scene->workdata)->basictable[4].lastbasic;
            } else {
                if (camera->prev != NULL) {
                    camera = camera->prev;
                } else {
                    if (data->gamecamera != NULL) {
                        camera = data->gamecamera;
                        data->stat |= 0x20;
                    } else {
                        camera = ((dds3SceneBasic_t *)scene->workdata)->basictable[4].lastbasic;
                    }
                }
            }
            dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
            dds3SetScreenFovy(dds3CameraObjectBasicGetFovy(camera));
        }
    }
    if (camera->name != NULL) {
        name = camera->name;
    } else {
        name = (data->stat & 0x20) ? D_00550F7C : D_00550F78;
    }
    if (data->debug == 1) {
        tag = dds3FontReqAllocDrawTag();
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x8680, 0x79C0, 0xFF0000, 5, D_00550F88, name));
        D_002F6978.regFunc(&D_002F6978, tag);
    }
}

extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern sdfGrOt_t D_002F67F8;
extern char *nbDebugPhaseNameTable[];
extern char *D_003FA748[];
extern char D_00551060[]; /* "Turn:%d", shared small-data strings. */
extern char D_00551068[]; /* "ENEMY" */
extern char D_00551070[]; /* "PLAYER" */
extern char D_00551078[]; /* "." */
extern char D_00551080[]; /* "EXP :%d" */
extern char D_00551088[]; /* "MAKA:%d" */
extern char D_00551090[]; /* "SUMM:%d" */

void nbDebugMainProcess(nbMainProcessData_t *data)
{
    sdfDrawTag_t *tag;
    s32 y;
    s32 i;

    if (data->debug == 1) {
        tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(sdfDrawTag_t));
        sdfDrawTagInit(tag);
        y = 0x7980;
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7880, y, 0xFFFF80, 0, "STAT:%08X", data->stat));
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, "Phase:%d", data->phase));
        y += 0x60;
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551060, data->turn));
        if (data->stat & 8) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7880, y, 0xFFFF80, 6, D_00551068, data->turn));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7880, y, 0xFFFF80, 5, D_00551070, data->turn));
        }
        y += 0x60;
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, "Press:%2.3f", data->press3));
        y += 0x60;
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, "NewPress:%d/%d", data->press4_p, data->press4_ten));
        y += 0x60;
        if (data->maeProcess != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Mae Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->AtoProcessID != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Ato Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->ResultProcessID != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Result Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->StageProcessID != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Stage Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->UnitProcessID != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Unit Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->CommSelProcessID != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "CommSel Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->tarSelProcess != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "TarSel Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->EffectProcessID != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Effect Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->KoukaProcessID != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Kouka Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->NegoProcessID != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Nego Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->camera != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Camera Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->DebugProcessID != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Debug Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->AiScriptProcessID != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "AiScr Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y += 0x60;
        if (data->panelProcess != NULL) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 4, "Panel Process"));
        } else {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7100, y, 0xFFFF80, 0, D_00551078));
        }
        y = 0x7B00;
        for (i = 0; i < 4; i++) {
            if (data->ActionProcessID[i] != NULL) {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7D00, y, 0xFFFF80, 4, "Act Proc_%d [%s]", i,
                    nbDebugPhaseNameTable[nbGetActionSeq(data->ActionProcessID[i])]));
            } else {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7D00, y, 0xFFFF80, 0, D_00551078));
            }
            y += 0x60;
        }
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7D00, 0x7D40, 0xFFFF80, 0, D_00551080, data->exp));
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7D00, 0x7DA0, 0xFFFF80, 0, D_00551088, data->maka));
        y = 0x7E00;
        for (i = 0; i < 4; i++) {
            sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7D00, y, 0xFFFF80, 0, "ITEM:0x%02X", data->item[i]));
            y += 0x60;
        }
        sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7D00, y + 0x60, 0xFFFF80, 0, D_00551090, data->nakamayobi));
        y = 0x7D40;
        for (i = 0; i < 8; i++) {
            if (i < data->actionstackp) {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x8480, y, 0xFFFF80, 0, "%d:%s %d->%d %d", i,
                    D_003FA748[data->actionstack[i].type], data->actionstack[i].from,
                    data->actionstack[i].to, data->actionstack[i].data));
            } else {
                sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x8480, y, 0xFFFF80, 0, "%d:-----", i));
            }
            y += 0x60;
        }
        D_002F67F8.regFunc(&D_002F67F8, tag);
    }
}


INCLUDE_ASM(const s32, "newbattle/nbMainProcess", DebugDisp2);

s32 CheckActionProcessEnd(nbMainProcessData_t *data)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (data->ActionProcessID[i] != 0) {
            return 0;
        }
    }
    return 1;
}

void CheckSenseiOrBackAttack(nbMainProcessData_t *data)
{
    s32 stat;

    if (nbCheckSensei(data) && evtBitCheck(0xC0A) == 0) {
        data->stat |= 0x80;
        if (nbCheckBackAttack(data)) {
            data->stat |= 0x10;
            printf("backattack!!\n");
        }
        data->stat |= 8;
    }
    stat = data->stat;
    if (stat & 0x8000) {
        data->stat = ((stat | 0x80) & ~0x10) | 8;
        stat = data->stat;
    }
    if ((stat & 0x80000000) != 0) {
        data->stat = stat | 0x98;
    }
}

s32 CheckActionRun(nbMainProcessData_t *data)
{
    nbFormation_t *form;
    s32 i;

    form = data->form;
    for (i = 0; i < 16; i++) {
        if ((form->stat & 1) && (form->flag & 7)) {
            return 0;
        }
        form++;
    }
    return 1;
}

void SetBattleFlag(nbMainProcessData_t *data)
{
    s32 flag;

    flag = nbEncountFormTable[data->encno].flag;
    if (flag & 1) {
        data->stat |= 1;
    }
}

void nbPushAction(s32 type, s32 from, s32 to, s32 d)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    data->actionstack[data->actionstackp].type = type;
    data->actionstack[data->actionstackp].from = from;
    data->actionstack[data->actionstackp].to = to;
    data->actionstack[data->actionstackp].data = d;
    data->actionstackp++;
}

s32 CheckErrorActionStack(nbMainProcessData_t *data, s32 index)
{
    nbActionStack_t *act;
    nbParty_t *party;
    datUnitWork_t *unit;
    s32 type;

    act = (nbActionStack_t *)((u8 *)(index * sizeof(nbActionStack_t)) + (u32)data->actionstack);
    party = (nbParty_t *)((u8 *)(act->to * sizeof(nbParty_t)) + (u32)data->party);
    if ((party->flag & 1) == 0) {
        return 1;
    }
    if (party->flag & 0x20) {
        return 1;
    }
    if (data->endtype != 0 && act->type != 5) {
        return 1;
    }
    type = data->actionstack[index].type;
    if (type == 4) {
        unit = nbGetUnitWorkPointer(data, &data->party[data->actionstack[index].from]);
        switch (unit->badstatus & 0xFFF) {
        case 1:
        case 2:
        case 4:
        case 0x10:
        case 0x80:
        case 0x100:
        case 0x400:
            return 1;
        }
    }
    return 0;
}

s32 nbPopAction(nbActionStack_t *act)
{
    nbMainProcessData_t *data;
    s32 p;

    data = nbGetMainProcessData();
    while (1) {
        if (data->actionstackp <= 0) {
            goto fail;
        }
        p = data->actionstackp - 1;
        data->actionstackp = p;
        if (CheckErrorActionStack(data, p) == 0) {
            act->type = data->actionstack[data->actionstackp].type;
            act->from = data->actionstack[data->actionstackp].from;
            act->to = data->actionstack[data->actionstackp].to;
            act->data = data->actionstack[data->actionstackp].data;
            return 1;
        }
    }
fail:
    return 0;
}

s32 nbCheckActionStack(void)
{
    nbMainProcessData_t *data;
    s32 i;

    data = nbGetMainProcessData();
    if (data->actionstackp <= 0) {
        return 0;
    }
    for (i = 0; i < data->actionstackp; i++) {
        if (CheckErrorActionStack(data, i) == 0) {
            printf(D_00543448);
            return 1;
        }
    }
    return 0;
}

void nbClearActionStack(void)
{
    nbGetMainProcessData()->actionstackp = 0;
}

void nbSetMaePhase(nbMainProcessData_t *data)
{
    SetBattleFlag(data);
    CheckSenseiOrBackAttack(data);
    nbSetPlayerStartData(data);
    nbInitUnitProcess(data);
    nbInitKoukaProcess(data);
    nbInitTarSelProcess(data);
    nbInitMaeProcess(data);
    nbInitEffectProcess(data);
    nbInitCameraProcess(data);
    nbInitPanelProcess(data);
    nbInitHelpProcess(data);
    nbInitAutoCommProcess(data);
    nbInitMakaProcess(data->MainProcessID);
    nbInitStageProcess(data);
    nbPanelPressGaugeEncountHide();
}

void nbStartBattlePhase(nbMainProcessData_t *data)
{
    nbClearActionStack();
    nbSetupBattleUnits(data, data->encno);
    nbCameraSetState(1);
    nbGroupFormDispOff(0);
    nbInitEncProcess(data);
    nbPanelPressGaugeStart();
}

void nbSetEventPhase(nbMainProcessData_t *data)
{
    if (data->eventno > 0) {
        nbInitEventProcess(data);
        nbDelUnEventKoukaPacket();
    }
}

void nbSetAtoPhase(nbMainProcessData_t *data)
{
    datUnitWork_t *unit;
    s32 bad;
    s32 i;

    for (i = 0; i < 16; i++) {
        unit = &NB_PLAYERUNIT[i];
        bad = unit->badstatus;
        if (nbCheckBadKaifuku(unit, 3)) {
            if ((bad & 0xFFF) == 0x200) {
                unit->badstatus &= 0xFDFF;
                unit->maxhp = datGetMaxHp(unit);
                unit->maxmp = datGetMaxMp(unit);
            }
            unit->badstatus &= 0xF000;
        }
    }
    for (i = 0; i < 4; i++) {
        unit = nbGetUnitWorkFromFormindex(i);
        if (unit == 0) {
            continue;
        }
        if ((unit->badstatus & 0xFFF) == 0x800) {
            continue;
        }
        if (unit->hp == 0) {
            continue;
        }
        datSetAutoKaifukuSkill(unit, 0x15A);
        datSetAutoKaifukuSkill(unit, 0x15B);
        datSetAutoKaifukuSkill(unit, 0x15C);
    }
    nbInitAtoProcess(data);
}

void nbSetPressMaePhase(nbMainProcessData_t *data)
{
    datUnitWork_t *unit;
    s32 start;
    s32 cnt;
    s32 i;

    data->press3 = 1.0f;
    if (data->stat & 8) {
        start = 4;
        cnt = 10;
    } else {
        start = 0;
        cnt = 4;
    }
    data->press4_p = 0;
    for (i = start; i < start + cnt; i++) {
        if (data->party[i].flag & 1) {
            if ((data->party[i].flag & 0x20) == 0) {
                unit = nbGetUnitWorkPointer(data, &data->party[i]);
                if (unit != 0) {
                    if ((unit->flag & 4) == 0 && (datDevilTable[unit->id].flag & 0x10)) {
                        data->press4_p = data->press4_p + 2;
                    } else {
                        data->press4_p = data->press4_p + 1;
                    }
                }
            }
        }
    }
    if (evtBitCheck(0x8C0) && (data->stat & 8) == 0) {
        data->press4_p = data->press4_p + 1;
    }
    data->press4_ten = data->press4_p;
    if (data->stat & 8) {
        start = 4;
        cnt = 10;
    } else {
        start = 0;
        cnt = 4;
    }
    for (i = start; i < start + cnt; i++) {
        nbSetHojoCounter(i, 0xE, 0);
        nbSetHojoCounter(i, 0xD, 0);
    }
    data->endtype = 0;
    nbSetOrder();
}

void nbSetPressNiidaPhase(nbMainProcessData_t *data)
{
    nbCameraPressProcessInit();
}

void nbSetActionMaePhase(nbMainProcessData_t *data)
{
    if (nbCheckActionStack() == 0) {
        nbPushAction(0, data->order[data->orderindex], 0, 0);
    }
}

void nbSetActionPhase(nbMainProcessData_t *data)
{
    nbActionStack_t act;

    nbPopAction(&act);
    nbUpdateFormation(data);
    nbInitActionProcess(data, act.type, act.from, act.to, act.data);
    nbCameraCRSetPlayerFormRequest();
}

void nbSetChkEndPhase(nbMainProcessData_t *data)
{
    nbParty_t *party;
    nbParty_t *pparty;
    s32 i;

    if (data->endtype == 0) {
        if (data->enemypcnt == 0) {
            data->endtype = 1;
        }
        party = data->party;
        for (i = 4; i < 14; i++) {
            if (party[i].flag & 1) {
                if ((party[i].flag & 0x20) == 0) {
                    break;
                }
            }
        }
        if (i == 14) {
            data->endtype = 1;
        }
        if (nbCheckBadStatusGameOver(data)) {
            printf(D_00543458);
            data->endtype = 6;
        }
        if (NB_PLAYERUNIT->hp == 0) {
            data->endtype = 2;
        }
        pparty = data->party;
        if ((pparty->flag & 1) == 0) {
            data->endtype = 2;
        } else if (pparty->flag & 0x20) {
            data->endtype = 2;
        }
    }
    if (nbCheckActionStack() == 0) {
        if (data->endtype == 1) {
            if (nbCheckRenzokuEncount(data)) {
                data->endtype = 5;
            }
        }
    }
}

void nbSetChkTurnPhase(nbMainProcessData_t *data)
{
    datUnitWork_t *unit;
    s32 i;

    if (nbCheckActionStack()) {
        nbSetNextPhase(ACTIONMAE);
        return;
    }
    nbClearActionStack();
    if (data->eventno > 0) {
        nbSetNextPhase(EVENT);
        return;
    }
    if (data->press4_p > 0 || data->press4_ten > 0) {
        if ((data->stat & 0x40000) == 0) {
            nbSetNextOrder();
        }
        nbSetNextPhase(ACTIONMAE);
        return;
    }
    for (i = 0; i < 16; i++) {
        unit = nbGetUnitWorkFromFormindex(i);
        if (nbCheckBadKaifuku(unit, 0) == 0) {
            continue;
        }
        switch (unit->badstatus & 0xFFF) {
        case 4:
        case 8:
        case 0x10:
        case 0x80:
            if (data->stat & 8) {
                if ((unit->flag & 0x20) == 0) {
                    continue;
                }
            } else {
                if (unit->flag & 0x20) {
                    continue;
                }
            }
            break;
        }
        if (data->party[data->form[i].partyindex].flag & 0x20) {
            continue;
        }
        nbMakeBadKaifukuPacket(dds3GetRandIntA(0, 7), 0, &data->form[i]);
    }
    for (i = 0; i < 16; i++) {
        nbSetHojoCounter(i, 0x12, 0);
    }
    data->stat &= ~0x10;
    data->stat &= ~0x80;
    if ((data->stat & 8) == 0) {
        data->stat |= 8;
        nbSetNextPhase(EVENT);
    } else {
        data->stat &= ~8;
        data->turn = data->turn + 1;
        if (data->turn >= 100) {
            data->turn = 99;
        }
        nbSetNextPhase(EVENT);
        nbAddKagututiMoon(data);
    }
}

void nbSetResultPhase(nbMainProcessData_t *data)
{
    s32 i;

    evtDisableMoonDraw();
    for (i = 0; i < 16; i++) {
        if (data->form[i].stat & 1) {
            if (data->form[i].badeff != 0) {
                effBTLSystemDelete2(data->form[i].badeff);
                data->form[i].badeff = 0;
            }
        }
    }
    if (data->endtype == 1 || data->endtype == 9) {
        nbGetEncountTableItem(data);
    }
    switch (data->endtype) {
    case 4:
    case 1:
    case 9:
        nbInitResultProcess(data);
        break;
    default:
        nbBgmFadeOut();
        i = 0;
        break;
    }
}

void nbSetEndPhase(nbMainProcessData_t *data)
{
    s32 seq;
    s32 maxhp;
    s32 maxmp;

    seq = 0x31F;
    DDS3_GBWK->encountCount = 0;
    DDS3_GBWK->encountkyori = 0.0f;
    if (NB_PLAYERUNIT->hp == 0 || NB_PLAYERUNIT->badstatus == 0x800) {
        maxhp = NB_PLAYERUNIT->maxhp;
        maxmp = NB_PLAYERUNIT->maxmp;
        NB_PLAYERUNIT->flag = 7;
        NB_PLAYERUNIT->mp = maxmp;
        NB_PLAYERUNIT->hp = maxhp;
        NB_PLAYERUNIT->badstatus = 0;
    }
    if (data->endtype == 2 || data->endtype == 6) {
        if (evtBitCheck(0xC0C) == 0) {
            printf(D_00543468);
            dds3AdminiCallSequence(7, &seq, 4, 0);
        }
    } else if (rstChkResultEntry() != 0) {
        printf(D_00543478);
        dds3AdminiCallSequence(0xE, 0, 0, 1);
    }
}

void nbSetDebugPhase(nbMainProcessData_t *data)
{
    if (data->CommSelProcessID != 0) {
        dds3KillProcess(data->CommSelProcessID, 1);
    }
    if (data->ActionProcessID[data->NowActionProcess] != 0) {
        dds3KillProcess(data->ActionProcessID[data->NowActionProcess], 1);
    }
    data->NowActionProcess = 0;
    nbInitDebugProcess(data);
}

void nbSetNextPhase(s32 phase)
{
    nbGetMainProcessData()->nextphase = phase;
}

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543448);

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543458);

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543468);

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543478);

void nbSetPhase(s32 phase)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    data->nextphase = -1;
    data->phase = phase;
    data->phasetime = 0;
    switch (data->phase) {
    case NB_PHASE_NON:
    case PRESSMAE:
    case NPC:
    case RENZOKU:
        break;
    case MAE:
        nbSetMaePhase(data);
        break;
    case NB_PHASE_ENCOUNT:
        nbStartBattlePhase(data);
        break;
    case EVENT:
        nbSetEventPhase(data);
        break;
    case PRESSNIIDA:
        nbSetPressMaePhase(data);
        nbSetPressNiidaPhase(data);
        break;
    case ACTIONMAE:
        nbSetActionMaePhase(data);
        break;
    case ACTION:
        nbSetActionPhase(data);
        break;
    case CHKEND:
        nbSetChkEndPhase(data);
        break;
    case CHKTURN:
        nbSetChkTurnPhase(data);
        break;
    case ATO:
        nbSetAtoPhase(data);
        break;
    case RESULT:
        nbSetResultPhase(data);
        break;
    case NB_PHASE_END:
        nbSetEndPhase(data);
        break;
    case DEBUG:
        nbSetDebugPhase(data);
        phase = 0;
        break;
    }
}

/* NON_MATCHING: 150 of 263 words differ (first at +0xBC). */
#ifdef NON_MATCHING
s32 nbMainProcess1(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;
    s32 next;
    s32 seq;
    s32 limit;

    next = 0;
    data = dds3GetProcessData(id);
    data->flag = 0;
    nbCheckPackLoad(data);
    switch (data->phase) {
    case NB_PHASE_NON:
        nbSetNextPhase(MAE);
        next = 1;
        break;
    case MAE:
        if (nbGetStageData() == 1) {
            nbSetNextPhase(NB_PHASE_ENCOUNT);
            next = 1;
        }
        break;
    case NB_PHASE_ENCOUNT:
        if (data->EncProcessID == 0) {
            nbCameraPressProcessThrough();
            nbPanelPressTurnOmit();
            nbSetNextPhase(EVENT);
            next = 1;
        }
        break;
    case EVENT:
        if (*(s32 *)data->EventProcessID == 0) {
            if (data->endtype == 1 || data->endtype == 8) {
                nbSetNextPhase(ATO);
            } else {
                nbSetNextPhase(PRESSNIIDA);
            }
            next = 1;
        }
        break;
    case PRESSNIIDA:
        if (data->phasetime == 0) {
            nbPanelPressTurnStart();
        }
        if (nbCameraPressProcessGetFrame() >= 10) {
            nbSetNextPhase(PRESSMAE);
            next = 1;
        }
        break;
    case PRESSMAE:
        if (nbCameraPressProcessCheckRun() == 0) {
            nbSetNextPhase(ACTIONMAE);
            next = 1;
        }
        break;
    case ACTIONMAE:
        if (CheckActionRun(data)) {
            nbSetNextPhase(ACTION);
            next = 1;
        }
        break;
    case ACTION:
        if (data->endtype == 7) {
            if (nbCountKoukaByType(0x2E) == 0) {
                if (NB_PLAYERUNIT->hp != 0) {
                    nbSetNextPhase(EVENT);
                    next = 1;
                } else {
                    data->endtype = 0;
                }
            }
        } else if (data->ActionProcessID[data->NowActionProcess] == 0) {
            nbSetNextPhase(NPC);
            next = 1;
        } else {
            seq = nbGetActionSeq(data->ActionProcessID[data->NowActionProcess]);
            if (seq >= 0x14) {
                nbSetNextPhase(NPC);
                next = 1;
            } else if (seq == 1) {
                if ((data->stat & 0x40) == 0) {
                    if (dds3PadButtonState[0x2A] < 0) {
                        if (data->stat & 0x1000000) {
                            if (dds3DebugSign != 0) {
                                nbSetNextPhase(DEBUG);
                                next = 1;
                            }
                        }
                    }
                }
            }
        }
        break;
    case NPC:
        nbSetNextPhase(CHKEND);
        next = 1;
        break;
    case CHKEND:
        if (data->stat & 0x40) {
            if (nbCheckActionStack() != 0) {
                limit = 8;
            } else {
                limit = 0;
            }
        } else {
            limit = 8;
        }
        if (data->phasetime >= limit) {
            if (nbCheckActionStack()) {
                nbSetNextPhase(CHKTURN);
                next = 1;
            } else {
                switch (data->endtype) {
                case NB_PHASE_NON:
                    nbSetNextPhase(CHKTURN);
                    next = 1;
                    break;
                case PRESSNIIDA:
                    nbSetNextPhase(RENZOKU);
                    next = 1;
                    break;
                case ACTION:
                    next = 1;
                    break;
                default:
                    nbSetNextPhase(ATO);
                    next = 1;
                    break;
                }
            }
        }
        break;
    case CHKTURN:
        next = 1;
        break;
    case RENZOKU:
        if (CheckActionProcessEnd(data)) {
            nbDispMsgHelp(data->mesmngid, nbGetJyokyoMes(data, 0x48), 1);
            data->stat &= ~0x80;
            data->stat &= ~8;
            data->stat &= ~0x10;
            next = 1;
            nbCountActiveUnits(data);
            nbSetNextPhase(NB_PHASE_ENCOUNT);
        }
        break;
    case ATO:
        if (CheckActionProcessEnd(data) && data->AtoProcessID == 0 && p2sdGetPcmState() == 0) {
            nbSetNextPhase(RESULT);
            next = 1;
        }
        break;
    case RESULT:
        if (data->ResultProcessID == 0) {
            nbSetNextPhase(NB_PHASE_END);
            next = 1;
        }
        break;
    case NB_PHASE_END:
        return -1;
    case DEBUG:
        if (data->DebugProcessID == 0) {
            nbSetNextPhase(ACTIONMAE);
            next = 1;
        }
        break;
    }
    if (data->phasetime < 0xFFFF) {
        data->phasetime = data->phasetime + 1;
    }
    data->battletime = data->battletime + 1;
    if (next) {
        nbSetPhase(data->nextphase);
    }
    if ((data->stat & 0x200000) == 0 && dds3DebugSign != 0) {
        nbDebugSelect(data);
        nbDebugMainProcess(data);
        if (data->debug == 1) {
            DebugDisp2(data);
        }
        nbDebugCamera(data);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbMainProcess", nbMainProcess1);
#endif

void SetJyocyuData(nbMainProcessData_t *data)
{
    sdfMemHandle_t *handle;

    handle = sdfDevLoadFileEx(D_0054F0F4, 0, 0);
    data->HatudoEff = effBTLCreate(sdfMemLock(handle));
    sdfMemFreeDirect(handle);
}

void FreeJyocyuData(nbMainProcessData_t *data)
{
    effBTLDelete(data->HatudoEff);
    data->HatudoEff = 0;
}

void ClearLocalBit(void)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        evtBitOff(0xBF0 + i);
    }
}

void nbMainShutdown(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    data = dds3GetProcessData(id);
    fileSyncAll();
    if (data != 0) {
        sdfMemFreeDirect(data->form_hndl);
        sdfMemFreeDirect(data->enemyunit_hndl);
        sdfMemFreeDirect(data->party_hndl);
        sdfChipFreeDirect(data->actionstack);
        nbFreeAllFormResources(data);
        FreeJyocyuData(data);
        itfMesMngRelease(data->mesmngid);
        nbFreeEventPack(data);
        sdfChipFreeDirect(data);
    }
}

void SetDebugFlag(nbMainProcessData_t *data)
{
    if (evtBitCheck(0xC04)) {
        data->stat |= 0x800000;
    } else {
        data->stat &= ~0x800000;
    }
    if (dds3DebugSign == 0) {
        return;
    }
    data->stat |= 0x1000000;
    if (evtBitCheck(0xC00)) {
        data->stat |= 0x20000;
    } else {
        data->stat &= ~0x20000;
    }
    if (evtBitCheck(0xC01)) {
        data->stat |= 0x10000;
    } else {
        data->stat &= ~0x10000;
    }
    if (evtBitCheck(0xC02)) {
        data->stat |= 0x40000;
    } else {
        data->stat &= ~0x40000;
    }
    if (evtBitCheck(0xC03)) {
        data->stat |= 0x400000;
    } else {
        data->stat &= ~0x400000;
    }
    if (evtBitCheck(0xC05)) {
        data->stat |= 0x2000000;
    } else {
        data->stat &= ~0x2000000;
    }
    if (evtBitCheck(0xC06)) {
        data->stat |= 0x4000000;
    } else {
        data->stat &= ~0x4000000;
    }
    if (evtBitCheck(0xC07)) {
        data->stat |= 0x8000000;
    } else {
        data->stat &= ~0x8000000;
    }
    if (evtBitCheck(0xC0D)) {
        data->stat |= 0x40000000;
    } else {
        data->stat &= ~0x40000000;
    }
    if (evtBitCheck(0xC10)) {
        data->stat |= 0x80000000;
    } else {
        data->stat &= ~0x80000000;
    }
    if (evtBitCheck(0xC12)) {
        data->stat |= 0x8000;
    } else {
        data->stat &= ~0x8000;
    }
}

void *nbInitMainProcessData(void)
{
    nbMainProcessData_t *data;
    nbParty_t *party;
    s32 i;

    data = sdfChipAllocDirect(0x150);
    memset(data, 0, 0x150);
    data->form_hndl = sdfMemAllocDirect(0x1E00);
    data->form = sdfMemLock(data->form_hndl);
    for (i = 0; i < 32; i++) {
        nbFormatFormation(data, i);
    }
    data->enemyunit_hndl = sdfMemAllocDirect(0x938);
    data->enemyunit = sdfMemLock(data->enemyunit_hndl);
    memset(data->enemyunit, 0, 0x938);
    data->party_hndl = sdfMemAllocDirect(0x700);
    data->party = sdfMemLock(data->party_hndl);
    party = data->party;
    for (i = 0; i < 14; i++) {
        party->partyindex = i;
        party->flag = 0;
        party++;
    }
    data->actionstack = sdfChipAllocDirect(0x80);
    memset(data->actionstack, 0, 0x80);
    data->actionstackp = 0;
    nbAllocMainProcessData(data);
    data->playerpcnt = 0;
    data->enemypcnt = 0;
    data->activeunit = -1;
    data->orderindex = -1;
    data->previndex = -1;
    data->phase = NB_PHASE_NON;
    data->press3 = 1.0f;
    data->unk140 = 0;
    data->battletime = 0;
    data->d31_rebehansya = 0;
    data->stat = 0;
    SetDebugFlag(data);
    data->mesmngid = itfMesMngInitialize(itfMesEmbedded_3F8228);
    return data;
}

void SetFlag(nbMainProcessData_t *data)
{
    s32 encno;
    s32 flag;

    printf("encpackno = %d\n", data->encpackno);
    printf("packno = %d\n", data->packno);
    printf("packindex = %d\n", data->packindex);
    printf(D_00543540, data->encno);
    encno = data->encno;
    if (encno == 0xC7) {
        data->stat |= 0x400;
    }
    flag = nbEncountFormTable[encno].flag;
    if (flag & 8) {
        data->stat |= 0x800;
    }
}

void nbInitMainProcess(s32 encpackno, s32 packno, s32 packindex, s32 encno, s32 stagemajor,
                       s32 stageminor)
{
    nbMainProcessData_t *data;

    data = nbInitMainProcessData();
    data->encpackno = encpackno;
    data->packno = packno;
    data->packindex = packindex;
    data->encno = encno;
    data->formationtype = nbEncountFormTable[encno].formtype;
    printf("encpackno = %d\n", data->encpackno);
    printf("packno = %d\n", data->packno);
    printf("packindex = %d\n", data->packindex);
    printf(D_00543540, data->encno);
    printf(D_00543550, data->formationtype);
    data->StageMajor = stagemajor;
    data->StageMinor = stageminor;
    if (data->encno == 0x1D9) {
        evtSetAgeOfMoon(0);
    }
    SetDummy(data);
    SetJyocyuData(data);
    SetFlag(data);
    ClearLocalBit();
    data->MainProcessID = dds3StartProcess(D_005510B8, 0x3FA, 1, 1, nbMainProcess1, nbMainShutdown, data);
    evtEnableMoonDraw();
    if (data->encno == 0x1C3) {
        nbLoadDeadMoonTex();
        evtSetAgeOfMoon(1);
    }
}

void nbKillMainProcess(void)
{
    if (dds3SearchProcessPriority(0x3FA) != 0) {
        dds3KillProcess(nbGetMainProcessData()->MainProcessID, 1);
    }
    evtDisableMoonDraw();
    nbFreeDeadMoonTex();
}

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543540);

INCLUDE_RODATA(const s32, "newbattle/nbMainProcess", D_00543550);

