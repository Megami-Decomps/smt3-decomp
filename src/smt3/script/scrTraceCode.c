#include "common.h"
#include "sdk_libgcc.h"
#include "newdata.h"
#include "scr.h"

enum code_operation
{
    CODE_OPERATION_ADD = 0,
    CODE_OPERATION_SUB = 1,
    CODE_OPERATION_MUL = 2,
    CODE_OPERATION_DIV = 3,
    CODE_OPERATION_OR = 4,
    CODE_OPERATION_AND = 5,
    CODE_OPERATION_EQ = 6,
    CODE_OPERATION_NEQ = 7,
    CODE_OPERATION_S = 8,
    CODE_OPERATION_L = 9,
    CODE_OPERATION_SE = 10,
    CODE_OPERATION_LE = 11,
    CODE_OPERATION_MAX = 12
};

extern scrProcessWork_s *nowpwork;
extern const scrCommandTable_t scrCommandTable[];
extern s32 (*codefunc[])(scrProcessWork_s *pw);

extern s32 PopInt(scrProcessWork_s *pwork);
extern f32 PopFloat(scrProcessWork_s *pwork);
extern void CodeFunc_2Operation(scrProcessWork_s *pwork, s32 type);

void PushInt(scrProcessWork_s *pwork, s32 data)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_INT;
    pwork->StackData[pwork->SP].i = data;
    pwork->SP++;
}

void PushFloat(scrProcessWork_s *pw, f32 value)
{
    pw->StackType[pw->SP] = SCR_STACK_TYPE_FLOAT;
    pw->StackData[pw->SP].f = value;
    pw->SP++;
}

void PushStr(scrProcessWork_s *pwork, u8 *data)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_STRING;
    pwork->StackData[pwork->SP].i = EE_ADDR_S(data);
    pwork->SP++;
}

void PushRet(scrProcessWork_s *pwork, s32 cp)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_RET;
    pwork->StackData[pwork->SP].i = cp;
    pwork->SP++;
}

extern s32 itfMesMngMessage();
extern s32 itfMesMngOpenWindow();
extern s32 itfMesMngCloseWindow();
extern s32 itfMesMngSelect();
extern s32 itfMesMngOpenSelectWindow();
extern s32 itfMesMngCloseSelectWindow();
extern s32 itfMesMngMaskSelect();
extern s32 scrCommand_BIT_CHK();
extern s32 scrCommand_BIT_ON();
extern s32 scrCommand_BIT_OFF();
extern s32 scrCommand_RND();
extern s32 dds3_2263B8();
extern s32 snd_226390();
extern s32 scrCommand_SYNC();
extern s32 scrCommand_WAIT();
extern s32 scrCommand_FADE_IN();
extern s32 scrCommand_FADE_OUT();
extern s32 scrCommand_SEL_JMP();
extern s32 evtCommand_CAM_SEL();
extern s32 evtCommand_CAM_PATH_MOVE();
extern s32 evtCommand_SET_SKY();
extern s32 evtCommand_MODEL_DSP();
extern s32 evt_1E54B8();
extern s32 evtCommand_MODEL_MOVE();
extern s32 evt_1E50C0();
extern s32 evtCommand_CAM_SEL_FOVY();
extern s32 scrCommand_PAD_ACT();
extern s32 evtCommand_MODEL_SCL_XYZ();
extern s32 prm_MAKA_CHECK();
extern s32 prm_STOCK_FULL_CHECK();
extern s32 prm_PLAYER_ADD_PARAM();
extern s32 prm_DH_EQUIP();
extern s32 evtCommand_PMV_LOAD();
extern s32 evtCommand_PMV_RUN2();
extern s32 evtCommand_FIELD_CLS_STOP();
extern s32 scrCommand_PUT();
extern s32 evtCommand_GET_MOON();
extern s32 evtCommand_UNIT_CLS();
extern s32 evtCommand_GET_MOON16();
extern s32 evtCommand_RESTORE_BATTLE();
extern s32 evtCommand_UNIT_CLS_STOP();
extern s32 evtCommand_TEX_LOAD();
extern s32 evtCommand_TEX_ON();
extern s32 evtCommand_TEX_OFF();
extern s32 evtCommand_TEX_CLS();
extern s32 evtCommand_EXIT_EVENT();
extern s32 nbCommand_AI_TBL();
extern s32 nbCommand_AI_ACT_ATTACK();
extern s32 nbCommand_AI_ACT_ESCAPE();
extern s32 nbCommand_AI_ACT_WAIT();
extern s32 nbCommand_AI_ACT_SKILL();
extern s32 nbCommand_AI_TAR_AI();
extern s32 nbCommand_AI_TAR_LIFEMIN();
extern s32 nbCommand_AI_TAR_BAD();
extern s32 nbCommand_AI_TAR_ID();
extern s32 nbCommand_BTL_EVENT();
extern s32 nbCommand_AI_TAR_LIGHT();
extern s32 nbCommand_AI_TAR_DARK();
extern s32 nbCommand_AI_TAR_PLAYER();
extern s32 nbCommand_AI_TAR_RND();
extern s32 nbCommand_AI();
extern s32 nbCommand_BTL_PNL_OFF();
extern s32 nbCommand_BTL_PNL_ON();
extern s32 itfMesMng_SEL_SELNO();
extern s32 scrCommand_BLUR2_ON();
extern s32 scrCommand_BLUR2_OFF();
extern s32 scrCommand_BLUR_ALLOFF();
extern s32 evtCommand_MODEL_EXWAIT();
extern s32 dds3_2263E8();
extern s32 evtCommand_RTRN_TITLE();
extern s32 scrCommand_SOFTEN_BIAS();
extern s32 evtCommand_SET_SKY_A();
extern s32 evtCommand_MODEL_WAIT();
extern s32 evtCommand_MODEL_POS();
extern s32 evtCommand_MODEL_WAIT_MOT();
extern s32 evtCommand_MODEL_MOVE_MOT();
extern s32 evtCommand_FIELD_DSP();
extern s32 evtCommand_FIELD_CLS();
extern s32 itfCamp();
extern s32 dds3_22B880();
extern s32 dds3_22B8A8();
extern s32 movie_22B8C8();
extern s32 scrCommand_PAD_TRIG();
extern s32 scrCommand_PAD_PRESS();
extern s32 evtCommand_CAM_PATH_WAIT();
extern s32 evtCommand_MODEL_ROT();
extern s32 evtCommand_CALL_PUZZLEBOY();
extern s32 evtCommand_CALL_COMBINE();
extern s32 evtCommand_CALL_SAVE();
extern s32 evtCommand_CALL_RECOVER();
extern s32 evtCommand_CALL_SHOP();
extern s32 evtCommand_CALL_RAG();
extern s32 evtCommand_CALL_FIELD();
extern s32 fldCommand_ACT_WIN_DSP();
extern s32 fldCommand_DIR_HIT_CNTR();
extern s32 fldCommand_PAD_DISABLE();
extern s32 fldCommand_PAD_ENABLE();
extern s32 msel_2286A0();
extern s32 evtCommand_MOON_START();
extern s32 evtCommand_MOON_STOP();
extern s32 evtCommand_CHG_MOON();
extern s32 evtCommand_CALL_EVENT();
extern s32 evtCommand_CALL_BATTLE();
extern s32 evtCommand_MODEL_PLAYER();
extern s32 evtCommand_MODEL_ON();
extern s32 evtCommand_MODEL_OFF();
extern s32 evtCommand_OBJ_PATH_MOVE();
extern s32 evtCommand_OBJ_PATH_WAIT();
extern s32 scrCommand_SET_FOVY();
extern s32 fldCommand_ENCOUNT_ON();
extern s32 fldCommand_ENCOUNT_OFF();
extern s32 prm_ITEM_PLUS();
extern s32 itfMesMngSetVarScr();
extern s32 scrCommand_PUTS();
extern s32 evt_1E5158();
extern s32 evtCommand_MODEL_WAIT_HOKAN();
extern s32 evtCommand_CAM_CREATE();
extern s32 evtCommand_CAM_CLS();
extern s32 evtCommand_CAM_POS_XYZ();
extern s32 evtCommand_CAM_ROT_XYZ();
extern s32 evtCommand_MODEL_POS_XYZ();
extern s32 evtCommand_MODEL_ROT_XYZ();
extern s32 nbCommand_AI_CHK_MYHP();
extern s32 nbCommand_AI_CHK_FRHP();
extern s32 nbCommand_AI_CHK_DMGCNT();
extern s32 nbCommand_AI_CHK_FRCNT();
extern s32 nbCommand_AI_CHK_ENCNT();
extern s32 nbCommand_AI_CHK_MYBAD();
extern s32 nbCommand_AI_CHK_FRBAD();
extern s32 nbCommand_AI_CHK_ENBAD();
extern s32 nbCommand_AI_CHK_ENALLBAD();
extern s32 nbCommand_AI_CHK_ENID();
extern s32 nbCommand_AI_CHK_FRID();
extern s32 nbCommand_AI_CHK_ENHOJO();
extern s32 nbCommand_AI_CHK_FRHOJO();
extern s32 nbCommand_AI_CHK_BA();
extern s32 nbCommand_AI_CHK_ENLIGHT();
extern s32 nbCommand_AI_CHK_ENDARK();
extern s32 evtCommand_MODEL_MOVE2();
extern s32 nbCommand_AI_CHK_POW();
extern s32 evtCommand_LOAD_SOUND_E();
extern s32 evtCommand_BGM_PLAY_E();
extern s32 evtCommand_SE_PLAY_E();
extern s32 evtCommand_FADEOUT_E();
extern s32 evtCommand_BGM_VOL_DOWN();
extern s32 evtCommand_BGM_VOL_UP();
extern s32 itfMesMngIgnoreConfig();
extern s32 evtCommand_SEARCH_ID();
extern s32 evt_1E51F8();
extern s32 evtCommand_MODEL_WAIT_EXMOT();
extern s32 evtCommand_CALL_NEXT();
extern s32 evtCommand_CALL_FIELD2();
extern s32 scrCommand_FILTER_ALLOFF();
extern s32 evtCommand_EFFECT_LOAD();
extern s32 evtCommand_EFFECT_CLS();
extern s32 evtCommand_EFFECT_POS_XYZ();
extern s32 evtCommand_EFFECT_POS();
extern s32 evtCommand_EFFECT_ON();
extern s32 evtCommand_EFFECT_OFF();
extern s32 evtCommand_EFFECT_LINK();
extern s32 evtCommand_EFFECT_LINK2();
extern s32 evtCommand_EFFECT_UNLINK();
extern s32 fldCommand_SET_FIX_CAMERA();
extern s32 fldCommand_SET_CHASE_CAMERA();
extern s32 scrCommand_SCR_RUN();
extern s32 scrCommand_SCR_KILL();
extern s32 scrCommand_SCR_WAIT();
extern s32 scrCommand_SCR_STAT();
extern s32 scrCommand_SCR_TIME();
extern s32 evtCommand_PMV_RUN();
extern s32 scrCommand_QUAKE_ON();
extern s32 scrCommand_QUAKE_OFF();
extern s32 evtCommand_MODEL_LIGHT_OFF();
extern s32 evtCommand_MODEL_LIGHT_ON();
extern s32 evtCommand_MODEL_LIGHT_RGB();
extern s32 evtCommand_MODEL_LIGHT_DIR();
extern s32 evtCommand_MODEL_LIGHT_WAIT();
extern s32 evtCommand_MODEL_RGB();
extern s32 evtCommand_MODEL_A();
extern s32 scrCommand_SCENE_LIGHT_RGB();
extern s32 scrCommand_SCENE_LIGHT_DIR();
extern s32 scrCommand_SCENE_AMBIENT();
extern s32 evtCommand_PATH_STOP();
extern s32 evtCommand_PATH_START();
extern s32 evtCommand_PATH_RATE();
extern s32 evtCommand_PATH_DIR();
extern s32 scrCommand_SCENE_FOG_RGB();
extern s32 scrCommand_SCENE_FOG_PARA();
extern s32 scrCommand_BLUR();
extern s32 fldCommand_SET_CASH_F();
extern s32 fldCommand_FREE_CASH_F();
extern s32 func_00158548();
extern s32 fldCommand_CAM_DIR();
extern s32 fldCommand_CAM_SEL_PHOKAN();
extern s32 fldCommand_SET_ALLPANEL();
extern s32 evtCommand_MODEL_LIGHT_LINK();
extern s32 evtCommand_MODEL_LIGHT_UNLINK();
extern s32 evtCommand_LIGHT_PATH_MOVE();
extern s32 evtCommand_LIGHT_PATH_WAIT();
extern s32 evtCommand_LOAD_BE();
extern s32 evtCommand_MODEL_BE();
extern s32 evtCommand_FIELD_BE();
extern s32 evtCommand_EFFECT_BE();
extern s32 evtCommand_TEX_BE();
extern s32 evtCommand_EFFBED_LOAD();
extern s32 evtCommand_EFFBED_START();
extern s32 evtCommand_EFFECT_ROT_XYZ();
extern s32 evtCommand_EFFBED_BE();
extern s32 evtCommand_EFFECT_PATH_MOVE();
extern s32 evtCommand_EFFECT_PATH_WAIT();
extern s32 evtCommand_EFFMG1_LOAD();
extern s32 evtCommand_EFFMG1_BE();
extern s32 evtCommand_EFFMG2_LOAD();
extern s32 evtCommand_EFFMG2_BE();
extern s32 evtCommand_EFFMG1_POS();
extern s32 evtCommand_EFFMG2_POS();
extern s32 evtCommandStartBgmWithFade();
extern s32 evtCommand_CAM_ROT_Q();
extern s32 evtCommand_FIELD_ANIME_START();
extern s32 evtCommand_FIELD_ANIME_STOP();
extern s32 evtCommand_MODEL_SHADOW_ON2();
extern s32 nbCommand_AI_CHK_PLV();
extern s32 nbCommand_AI_CHK_PPARTYLV();
extern s32 nbCommand_AI_GET_TURN();
extern s32 prm_DV_PLUS();
extern s32 nbCommand_AI_ACT_SUMMON();
extern s32 evtCommand_EFFMG2_PRESET();
extern s32 nbCommand_BTL_TAIKYO();
extern s32 nbCommand_BTL_ALLTAIKYO();
extern s32 nbCommand_BTL_PRESS();
extern s32 nbCommand_BTL_NAKAMA();
extern s32 evtCommand_PATH_LOOP_ON();
extern s32 evtCommand_PATH_LOOP_OFF();
extern s32 scrCommand_MUL_BLUR_PARA();
extern s32 scrCommand_MUL_BLUR_RGBA();
extern s32 scrCommand_MUL_BLUR_AREA();
extern s32 scrCommand_MUL_BLUR_ON();
extern s32 scrCommand_MUL_BLUR_OFF();
extern s32 scrCommand_DIST_BLUR_PARA();
extern s32 scrCommand_DIST_BLUR_RGBA();
extern s32 scrCommand_DIST_BLUR_AREA();
extern s32 scrCommand_DIST_BLUR_ON();
extern s32 scrCommand_DIST_BLUR_OFF();
extern s32 nbCommand_BTL_CAM_SET();
extern s32 nbCommand_BTL_CAM_ORG();
extern s32 nbCommand_AI_SET_CRITICAL();
extern s32 evtCommand_READ_BE();
extern s32 evtCommand_CHK_BE();
extern s32 evtCommand_DEL_BE();
extern s32 nbCommand_BTL_CAM_1SHOT();
extern s32 nbCommand_BTL_CAM_2SHOT();
extern s32 nbCommand_BTL_CAM_BOUGAI();
extern s32 fldCommand_FLD_CHK_BTL();
extern s32 fldCommand_FLD_CHK_ANA();
extern s32 fldCommand_FLD_CHK_SISETU();
extern s32 evtCommand_CALL_FIELD3();
extern s32 fldCommand_FLD_KANKYO();
extern s32 fldCommand_FLD_FILTER();
extern s32 fldCommand_FLD_MODEL_ON();
extern s32 fldCommand_FLD_MODEL_OFF();
extern s32 fldCommand_FLD_ANIME_ON();
extern s32 fldCommand_FLD_ANIME_OFF();
extern s32 fldCommand_FLD_ATARI_ON();
extern s32 fldCommand_FLD_ATARI_OFF();
extern s32 fldCommand_FLD_MODEL_ON2();
extern s32 fldCommand_FLD_MODEL_OFF2();
extern s32 fldCommand_FLD_ANIME_ON2();
extern s32 fldCommand_FLD_ANIME_OFF2();
extern s32 fldCommand_FLD_ANIME_ID2();
extern s32 fldCommand_FLD_NPC_ON();
extern s32 fldCommand_FLD_NPC_OFF();
extern s32 fldCommand_FLD_GIMIC_HOJI();
extern s32 fldCommand_FLD_MAP_ON();
extern s32 fldCommand_FLD_MAP_OFF();
extern s32 fldCommand_SET_CAMTBL();
extern s32 fldCommand_FLD_GET_TAKARA_TBL();
extern s32 fldCommand_FLD_SET_TAKARA_OPEN();
extern s32 fldCommand_FLD_CHK_TAKARA_OPEN();
extern s32 itfMesMngSetMesPositionScript();
extern s32 itfMesMngSetMesPosDefault();
extern s32 itfMesMngValidateNext();
extern s32 trm_210A00();
extern s32 trm_210AF8();
extern s32 func_00210B38();
extern s32 fclSetFlagScript();
extern s32 fclGetWorkValueScript();
extern s32 func_00210B68();
extern s32 func_00210B90();
extern s32 nme_298698();
extern s32 prm_DH_PLUS();
extern s32 evtCommand_MODEL_SHADOW_ON();
extern s32 evtCommand_MODEL_SHADOW_OFF();
extern s32 evtCommand_BOKASI_ON();
extern s32 evtCommand_BOKASI_OFF();
extern s32 scrCommand_MUL_BLUR_SWAP();
extern s32 scrCommand_DIST_BLUR_SWAP();
extern s32 scrCommand_BLUR2_PARA();
extern s32 scrCommand_BLUR2_RGBA();
extern s32 scrCommand_BLUR2_AREA();
extern s32 scrCommand_BLUR2_SWAP();
extern s32 scrCommand_FILTER2_PARA();
extern s32 scrCommand_FILTER2_RGBA();
extern s32 scrCommand_FILTER2_SWAP();
extern s32 scrCommand_FILTER2_ON();
extern s32 scrCommand_FILTER2_OFF();
extern s32 scrCommand_MFILTER_PARA();
extern s32 scrCommand_MFILTER_RGBA();
extern s32 scrCommand_MFILTER_AREA();
extern s32 scrCommand_MFILTER_SWAP();
extern s32 scrCommand_MFILTER_ON();
extern s32 scrCommand_MFILTER_OFF();
extern s32 evtCommand_BGM_TRANS_E();
extern s32 prm_MAKA_PLUS();
extern s32 prm_PARTY_DAMAGE();
extern s32 itfMesMngWindowType();
extern s32 itfMesMngSelectDefKey();
extern s32 evt_1E91A8();
extern s32 evt_1E91E8();
extern s32 evt_1E9220();
extern s32 evt_1E9248();
extern s32 evt_1E9270();
extern s32 scrCommand_RIP_BLUR_ON();
extern s32 scrCommand_RIP_BLUR_OFF();
extern s32 scrCommand_RIP_BLUR_PARA();
extern s32 scrCommand_RIP_BLUR_RGBA();
extern s32 scrCommand_RIP_BLUR_AREA();
extern s32 scrCommand_RIP_BLUR_SWAP();
extern s32 evtOpcodeRefreshTaskData();
extern s32 evt_1E92D8();
extern s32 evtCommand_EFFECT_PRESET();
extern s32 nbCommand_AI_CHK_MYMP();
extern s32 nbCommand_AI_GET_PRESSKOUDO();
extern s32 nbCommand_AI_GET_ALLKOUDO();
extern s32 nbCommand_AI_GET_MYLV();
extern s32 prm_UDV_LEAVE();
extern s32 prm_UDV_CHECK();
extern s32 prm_UDV_DELETE_OK();
extern s32 prm_UDV_DELETE_NG();
extern s32 nbCommand_BTL_FORM_ALL_ON();
extern s32 nbCommand_BTL_FORM_PLY_ON();
extern s32 nbCommand_BTL_FORM_ENE_ON();
extern s32 nbCommand_AI_GET_MYAISYO();
extern s32 evtCommand_CALL_STAFF();
extern s32 prm_STOCK_PLUS();
extern s32 nbCommand_AI_TAR_BAARLV();
extern s32 prm_UDV_GETID();
extern s32 nbCommand_AI_TAR_LVMIN();
extern s32 prm_PLAYER_GET_PARAM();
extern s32 nbCommand_AI_GET_FUREA();
extern s32 evtCommand_CALL_ENDSAVE();
extern s32 evtCommand_KUTI();
extern s32 itfMesMngWindowTypeAlpha();
extern s32 fldCommand_FLD_ANAHORI_INIT();
extern s32 fldCommand_FLD_ANAHORI_GET();
extern s32 fldCommand_FLD_ANAHORI_NEXT();
extern s32 fldCommand_FLD_ANAHORI_DEL();
extern s32 evtCommand_KUTISET();
extern s32 evtCommand_CALL_BATTLE2();
extern s32 evtCommand_EFFBED_BE_EVO();
extern s32 evtCommand_EFFBED_START_EVO();
extern s32 nbCommand_AI_CHK_ESCAPE();
extern s32 evtCommandSetDrawFlagWhileWaiting();
extern s32 movie_22B8F0();
extern s32 prm_BOSS_PRESS_REC();
extern s32 prm_NAKAMA_GET_PARAM();
extern s32 prm_PLAYER_GET_LND();
extern s32 prm_NAKAMA_DEAD();
extern s32 prm_NAKAMA_CHK_PIKUKO();
extern s32 prm_PIKUKO_GETID();
extern s32 prm_PARTY_KAIFUKU();
extern s32 prm_DV_COPY_PARAM();
extern s32 nbCommand_BTL_AI_ROT_CLR();
extern s32 nbCommand_BTL_AI_ROT_NEXT();
extern s32 nbCommand_BTL_AI_ROT_GET();
extern s32 nbCommand_AI_CHK_ENHOJOMAX();
extern s32 nbCommand_AI_CHK_FRHOJOMAX();
extern s32 nbCommand_BTL_AI_CHANGE_OFF();
extern s32 nbCommand_BTL_AI_CHANGE_ON();
extern s32 nbCommand_AI_CHK_ENHAMAMHK();
extern s32 nbCommand_AI_TAR_JOKEN();
extern s32 nbCommand_AI_CHK_SKILLNOMP();
extern s32 prm_PLAYER_GET_LEVEL();
extern s32 prm_MAKA_PLUS_PER();
extern s32 prm_DV_CHECK();
extern s32 evtCommand_PMV_LOAD_END_WAIT();
extern s32 prm_PARTY_KAIFUKU2();
extern s32 evtCommand_TEX_SWAP();
extern s32 evtCommand_TEX_PRI();
extern s32 evtCommand_TEX_SIZE();
extern s32 evtCommand_TEX_POS();
extern s32 evtCommand_TEX_ALP();
extern s32 evtCommand_TEX_ZOOM();
extern s32 evtCommand_TEX_OT();
extern s32 evtCommand_TEX_STAT();
extern s32 fldCommand_FLD_TITLE_START2();
extern s32 fldCommand_FLD_GET_KIRETA();
extern s32 fldCommand_FLD_GET_ITEMBOX_RND();
extern s32 fldCommand_FLD_MSE_PLAY2();
extern s32 fldCommand_FLD_MSE_STOP2();
extern s32 fldCommand_SET_NPC_CAMERA();
extern s32 fldCommand_FLD_MODEL_DANTE();
extern s32 fldCommand_FLD_DANTE_AI_STOP();
extern s32 fldCommand_FLD_DANTE_AI_START();
extern s32 fldCommand_FLD_DANTE_STANCE();
extern s32 fldCommand_FLD_BGM_RELOAD();
extern s32 fldCommand_FLD_BGM_RELOAD_WAIT();
extern s32 fldCommand_FLD_WARP_ELV_GETTBL();
extern s32 fldCommand_FLD_WARP_ELV_EXIT();
extern s32 fldCommand_FLD_WARP_CALL_FIELD();
extern s32 fldCommand_FLD_WARP_CASH_F();
extern s32 fldCommand_FLD_WARP_RUN_START();
extern s32 fldCommand_FLD_INF_NEXT();
extern s32 fldCommand_FLD_INF_GET();
extern s32 fldCommand_FLD_INF_GETPANEL();
extern s32 fldCommand_FLD_WARP_HASIGO_GETTBL();
extern s32 fldCommand_FLD_ELV_FLOORSTART();
extern s32 fldCommand_FLD_ELV_FLOOREND();
extern s32 fldCommand_FLD_ELV_ENDCHK();
extern s32 fldCommand_FLD_WARP_GET_DOOR();
extern s32 fldCommand_FLD_EXIT_CAMERA();
extern s32 fldCommand_FLD_INF_GOWAP();
extern s32 fldCommand_FLD_CALL_WAPEX();
extern s32 fldCommand_FLD_BGM_PLAY();
extern s32 fldCommand_FLD_BGM_CONTINUE();
extern s32 fldCommand_FLD_BGM_PAUSE();
extern s32 fldCommand_FLD_BGM_STOP();
extern s32 fldCommand_FLD_SE_PLAY();
extern s32 fldCommand_FLD_SE_STOP();
extern s32 fldCommand_FLD_BGM_FADE();
extern s32 fldCommand_FLD_MSE_PLAY();
extern s32 fldCommand_FLD_NPC_SINEN();
extern s32 fldCommand_FLD_WARP_GET_WARPEFF();
extern s32 fldCommand_FLD_TITLE_START();
extern s32 fldCommand_ACT_WIN_DSP2();
extern s32 fldCommand_FLD_SET_ENCOUNT();
extern s32 fldCommand_ACT_WIN_DSP3();
extern s32 fldCommand_FLD_MODEL_LIGHT();
extern s32 fldCommand_FLD_SAGASI_AKUMA();

const scrCommandTable_t scrCommandTable[544] = {
    {itfMesMngMessage, 1},
    {itfMesMngOpenWindow, 0},
    {itfMesMngCloseWindow, 0},
    {itfMesMngSelect, 1},
    {itfMesMngOpenSelectWindow, 0},
    {itfMesMngCloseSelectWindow, 0},
    {itfMesMngMaskSelect, 1},
    {scrCommand_BIT_CHK, 1},
    {scrCommand_BIT_ON, 1},
    {scrCommand_BIT_OFF, 1},
    {scrCommand_RND, 1},
    {dds3_2263B8, 1},
    {snd_226390, 1},
    {scrCommand_SYNC, 0},
    {scrCommand_WAIT, 1},
    {scrCommand_FADE_IN, 2},
    {scrCommand_FADE_OUT, 2},
    {scrCommand_SEL_JMP, 8},
    {evtCommand_CAM_SEL, 1},
    {evtCommand_CAM_PATH_MOVE, 2},
    {evtCommand_SET_SKY, 3},
    {evtCommand_MODEL_DSP, 2},
    {evt_1E54B8, 1},
    {evtCommand_MODEL_MOVE, 5},
    {evt_1E50C0, 4},
    {evtCommand_CAM_SEL_FOVY, 1},
    {scrCommand_PAD_ACT, 3},
    {evtCommand_MODEL_SCL_XYZ, 4},
    {prm_MAKA_CHECK, 1},
    {prm_STOCK_FULL_CHECK, 0},
    {prm_PLAYER_ADD_PARAM, 2},
    {0, 0},
    {prm_DH_EQUIP, 1},
    {evtCommand_PMV_LOAD, 2},
    {evtCommand_PMV_RUN2, 1},
    {evtCommand_FIELD_CLS_STOP, 0},
    {scrCommand_PUT, 1},
    {evtCommand_GET_MOON, 0},
    {evtCommand_UNIT_CLS, 0},
    {evtCommand_GET_MOON16, 0},
    {evtCommand_RESTORE_BATTLE, 2},
    {evtCommand_UNIT_CLS_STOP, 0},
    {evtCommand_TEX_LOAD, 1},
    {evtCommand_TEX_ON, 1},
    {evtCommand_TEX_OFF, 1},
    {evtCommand_TEX_CLS, 1},
    {evtCommand_EXIT_EVENT, 0},
    {nbCommand_AI_TBL, 1},
    {nbCommand_AI_ACT_ATTACK, 0},
    {nbCommand_AI_ACT_ESCAPE, 0},
    {nbCommand_AI_ACT_WAIT, 0},
    {nbCommand_AI_ACT_SKILL, 1},
    {nbCommand_AI_TAR_AI, 0},
    {nbCommand_AI_TAR_LIFEMIN, 0},
    {nbCommand_AI_TAR_BAD, 1},
    {nbCommand_AI_TAR_ID, 1},
    {nbCommand_BTL_EVENT, 1},
    {nbCommand_AI_TAR_LIGHT, 0},
    {nbCommand_AI_TAR_DARK, 0},
    {nbCommand_AI_TAR_PLAYER, 0},
    {nbCommand_AI_TAR_RND, 0},
    {nbCommand_AI, 0},
    {nbCommand_BTL_PNL_OFF, 0},
    {nbCommand_BTL_PNL_ON, 0},
    {itfMesMng_SEL_SELNO, 1},
    {scrCommand_BLUR2_ON, 1},
    {scrCommand_BLUR2_OFF, 1},
    {scrCommand_BLUR_ALLOFF, 0},
    {evtCommand_MODEL_EXWAIT, 2},
    {dds3_2263E8, 0},
    {evtCommand_RTRN_TITLE, 0},
    {scrCommand_SOFTEN_BIAS, 3},
    {evtCommand_SET_SKY_A, 2},
    {evtCommand_MODEL_WAIT, 1},
    {evtCommand_MODEL_POS, 2},
    {evtCommand_MODEL_WAIT_MOT, 2},
    {evtCommand_MODEL_MOVE_MOT, 2},
    {evtCommand_FIELD_DSP, 2},
    {evtCommand_FIELD_CLS, 0},
    {itfCamp, 0},
    {dds3_22B880, 1},
    {dds3_22B8A8, 0},
    {movie_22B8C8, 0},
    {scrCommand_PAD_TRIG, 1},
    {scrCommand_PAD_PRESS, 1},
    {evtCommand_CAM_PATH_WAIT, 1},
    {evtCommand_MODEL_ROT, 3},
    {evtCommand_CALL_PUZZLEBOY, 1},
    {evtCommand_CALL_COMBINE, 1},
    {evtCommand_CALL_SAVE, 2},
    {evtCommand_CALL_RECOVER, 1},
    {evtCommand_CALL_SHOP, 1},
    {evtCommand_CALL_RAG, 1},
    {evtCommand_CALL_FIELD, 3},
    {fldCommand_ACT_WIN_DSP, 1},
    {fldCommand_DIR_HIT_CNTR, 1},
    {fldCommand_PAD_DISABLE, 0},
    {fldCommand_PAD_ENABLE, 0},
    {msel_2286A0, 0},
    {evtCommand_MOON_START, 0},
    {evtCommand_MOON_STOP, 0},
    {evtCommand_CHG_MOON, 1},
    {evtCommand_CALL_EVENT, 1},
    {evtCommand_CALL_BATTLE, 1},
    {evtCommand_MODEL_PLAYER, 0},
    {evtCommand_MODEL_ON, 1},
    {evtCommand_MODEL_OFF, 1},
    {evtCommand_OBJ_PATH_MOVE, 3},
    {evtCommand_OBJ_PATH_WAIT, 1},
    {scrCommand_SET_FOVY, 1},
    {fldCommand_ENCOUNT_ON, 0},
    {fldCommand_ENCOUNT_OFF, 0},
    {prm_ITEM_PLUS, 2},
    {itfMesMngSetVarScr, 3},
    {scrCommand_PUTS, 1},
    {evt_1E5158, 5},
    {evtCommand_MODEL_WAIT_HOKAN, 3},
    {evtCommand_CAM_CREATE, 0},
    {evtCommand_CAM_CLS, 1},
    {evtCommand_CAM_POS_XYZ, 4},
    {evtCommand_CAM_ROT_XYZ, 4},
    {evtCommand_MODEL_POS_XYZ, 4},
    {evtCommand_MODEL_ROT_XYZ, 4},
    {nbCommand_AI_CHK_MYHP, 1},
    {nbCommand_AI_CHK_FRHP, 1},
    {nbCommand_AI_CHK_DMGCNT, 1},
    {nbCommand_AI_CHK_FRCNT, 1},
    {nbCommand_AI_CHK_ENCNT, 1},
    {nbCommand_AI_CHK_MYBAD, 1},
    {nbCommand_AI_CHK_FRBAD, 1},
    {nbCommand_AI_CHK_ENBAD, 1},
    {nbCommand_AI_CHK_ENALLBAD, 1},
    {nbCommand_AI_CHK_ENID, 1},
    {nbCommand_AI_CHK_FRID, 1},
    {nbCommand_AI_CHK_ENHOJO, 1},
    {nbCommand_AI_CHK_FRHOJO, 1},
    {nbCommand_AI_CHK_BA, 0},
    {nbCommand_AI_CHK_ENLIGHT, 0},
    {nbCommand_AI_CHK_ENDARK, 0},
    {evtCommand_MODEL_MOVE2, 7},
    {nbCommand_AI_CHK_POW, 0},
    {evtCommand_LOAD_SOUND_E, 1},
    {evtCommand_BGM_PLAY_E, 2},
    {evtCommand_SE_PLAY_E, 2},
    {evtCommand_FADEOUT_E, 2},
    {evtCommand_BGM_VOL_DOWN, 0},
    {evtCommand_BGM_VOL_UP, 0},
    {itfMesMngIgnoreConfig, 1},
    {evtCommand_SEARCH_ID, 1},
    {evt_1E51F8, 6},
    {evtCommand_MODEL_WAIT_EXMOT, 3},
    {evtCommand_CALL_NEXT, 3},
    {evtCommand_CALL_FIELD2, 2},
    {scrCommand_FILTER_ALLOFF, 0},
    {evtCommand_EFFECT_LOAD, 1},
    {evtCommand_EFFECT_CLS, 1},
    {evtCommand_EFFECT_POS_XYZ, 4},
    {evtCommand_EFFECT_POS, 2},
    {evtCommand_EFFECT_ON, 1},
    {evtCommand_EFFECT_OFF, 1},
    {evtCommand_EFFECT_LINK, 2},
    {evtCommand_EFFECT_LINK2, 3},
    {evtCommand_EFFECT_UNLINK, 1},
    {fldCommand_SET_FIX_CAMERA, 1},
    {fldCommand_SET_CHASE_CAMERA, 0},
    {scrCommand_SCR_RUN, 2},
    {scrCommand_SCR_KILL, 1},
    {scrCommand_SCR_WAIT, 1},
    {scrCommand_SCR_STAT, 1},
    {scrCommand_SCR_TIME, 1},
    {evtCommand_PMV_RUN, 2},
    {scrCommand_QUAKE_ON, 2},
    {scrCommand_QUAKE_OFF, 0},
    {evtCommand_MODEL_LIGHT_OFF, 2},
    {evtCommand_MODEL_LIGHT_ON, 1},
    {evtCommand_MODEL_LIGHT_RGB, 8},
    {evtCommand_MODEL_LIGHT_DIR, 5},
    {evtCommand_MODEL_LIGHT_WAIT, 1},
    {evtCommand_MODEL_RGB, 5},
    {evtCommand_MODEL_A, 3},
    {scrCommand_SCENE_LIGHT_RGB, 4},
    {scrCommand_SCENE_LIGHT_DIR, 4},
    {scrCommand_SCENE_AMBIENT, 4},
    {evtCommand_PATH_STOP, 1},
    {evtCommand_PATH_START, 1},
    {evtCommand_PATH_RATE, 2},
    {evtCommand_PATH_DIR, 2},
    {scrCommand_SCENE_FOG_RGB, 4},
    {scrCommand_SCENE_FOG_PARA, 5},
    {scrCommand_BLUR, 3},
    {fldCommand_SET_CASH_F, 2},
    {fldCommand_FREE_CASH_F, 0},
    {func_00158548, 0},
    {fldCommand_CAM_DIR, 1},
    {fldCommand_CAM_SEL_PHOKAN, 2},
    {fldCommand_SET_ALLPANEL, 1},
    {evtCommand_MODEL_LIGHT_LINK, 3},
    {evtCommand_MODEL_LIGHT_UNLINK, 2},
    {evtCommand_LIGHT_PATH_MOVE, 2},
    {evtCommand_LIGHT_PATH_WAIT, 1},
    {evtCommand_LOAD_BE, 1},
    {evtCommand_MODEL_BE, 2},
    {evtCommand_FIELD_BE, 2},
    {evtCommand_EFFECT_BE, 2},
    {evtCommand_TEX_BE, 2},
    {evtCommand_EFFBED_LOAD, 1},
    {evtCommand_EFFBED_START, 2},
    {evtCommand_EFFECT_ROT_XYZ, 4},
    {evtCommand_EFFBED_BE, 2},
    {evtCommand_EFFECT_PATH_MOVE, 2},
    {evtCommand_EFFECT_PATH_WAIT, 1},
    {evtCommand_EFFMG1_LOAD, 1},
    {evtCommand_EFFMG1_BE, 2},
    {evtCommand_EFFMG2_LOAD, 1},
    {evtCommand_EFFMG2_BE, 2},
    {evtCommand_EFFMG1_POS, 3},
    {evtCommand_EFFMG2_POS, 5},
    {evtCommandStartBgmWithFade, 2},
    {evtCommand_CAM_ROT_Q, 5},
    {evtCommand_FIELD_ANIME_START, 1},
    {evtCommand_FIELD_ANIME_STOP, 1},
    {evtCommand_MODEL_SHADOW_ON2, 1},
    {nbCommand_AI_CHK_PLV, 1},
    {nbCommand_AI_CHK_PPARTYLV, 1},
    {nbCommand_AI_GET_TURN, 0},
    {prm_DV_PLUS, 3},
    {nbCommand_AI_ACT_SUMMON, 2},
    {evtCommand_EFFMG2_PRESET, 1},
    {nbCommand_BTL_TAIKYO, 0},
    {nbCommand_BTL_ALLTAIKYO, 0},
    {nbCommand_BTL_PRESS, 0},
    {nbCommand_BTL_NAKAMA, 0},
    {evtCommand_PATH_LOOP_ON, 1},
    {evtCommand_PATH_LOOP_OFF, 1},
    {scrCommand_MUL_BLUR_PARA, 3},
    {scrCommand_MUL_BLUR_RGBA, 4},
    {scrCommand_MUL_BLUR_AREA, 3},
    {scrCommand_MUL_BLUR_ON, 1},
    {scrCommand_MUL_BLUR_OFF, 1},
    {scrCommand_DIST_BLUR_PARA, 7},
    {scrCommand_DIST_BLUR_RGBA, 4},
    {scrCommand_DIST_BLUR_AREA, 3},
    {scrCommand_DIST_BLUR_ON, 1},
    {scrCommand_DIST_BLUR_OFF, 1},
    {nbCommand_BTL_CAM_SET, 7},
    {nbCommand_BTL_CAM_ORG, 0},
    {nbCommand_AI_SET_CRITICAL, 0},
    {evtCommand_READ_BE, 1},
    {evtCommand_CHK_BE, 1},
    {evtCommand_DEL_BE, 1},
    {nbCommand_BTL_CAM_1SHOT, 0},
    {nbCommand_BTL_CAM_2SHOT, 0},
    {nbCommand_BTL_CAM_BOUGAI, 0},
    {fldCommand_FLD_CHK_BTL, 0},
    {fldCommand_FLD_CHK_ANA, 0},
    {fldCommand_FLD_CHK_SISETU, 0},
    {evtCommand_CALL_FIELD3, 2},
    {fldCommand_FLD_KANKYO, 2},
    {fldCommand_FLD_FILTER, 4},
    {fldCommand_FLD_MODEL_ON, 4},
    {fldCommand_FLD_MODEL_OFF, 4},
    {fldCommand_FLD_ANIME_ON, 4},
    {fldCommand_FLD_ANIME_OFF, 4},
    {fldCommand_FLD_ATARI_ON, 3},
    {fldCommand_FLD_ATARI_OFF, 3},
    {fldCommand_FLD_MODEL_ON2, 3},
    {fldCommand_FLD_MODEL_OFF2, 3},
    {fldCommand_FLD_ANIME_ON2, 3},
    {fldCommand_FLD_ANIME_OFF2, 3},
    {fldCommand_FLD_ANIME_ID2, 4},
    {fldCommand_FLD_NPC_ON, 3},
    {fldCommand_FLD_NPC_OFF, 3},
    {fldCommand_FLD_GIMIC_HOJI, 4},
    {fldCommand_FLD_MAP_ON, 3},
    {fldCommand_FLD_MAP_OFF, 3},
    {fldCommand_SET_CAMTBL, 1},
    {fldCommand_FLD_GET_TAKARA_TBL, 1},
    {fldCommand_FLD_SET_TAKARA_OPEN, 0},
    {fldCommand_FLD_CHK_TAKARA_OPEN, 0},
    {itfMesMngSetMesPositionScript, 2},
    {itfMesMngSetMesPosDefault, 0},
    {itfMesMngValidateNext, 1},
    {trm_210A00, 2},
    {trm_210AF8, 1},
    {func_00210B38, 0},
    {fclSetFlagScript, 1},
    {fclGetWorkValueScript, 1},
    {func_00210B68, 0},
    {func_00210B90, 0},
    {nme_298698, 2},
    {prm_DH_PLUS, 1},
    {evtCommand_MODEL_SHADOW_ON, 1},
    {evtCommand_MODEL_SHADOW_OFF, 1},
    {evtCommand_BOKASI_ON, 0},
    {evtCommand_BOKASI_OFF, 0},
    {scrCommand_MUL_BLUR_SWAP, 1},
    {scrCommand_DIST_BLUR_SWAP, 1},
    {scrCommand_BLUR2_PARA, 3},
    {scrCommand_BLUR2_RGBA, 4},
    {scrCommand_BLUR2_AREA, 2},
    {scrCommand_BLUR2_SWAP, 1},
    {scrCommand_FILTER2_PARA, 1},
    {scrCommand_FILTER2_RGBA, 4},
    {scrCommand_FILTER2_SWAP, 1},
    {scrCommand_FILTER2_ON, 1},
    {scrCommand_FILTER2_OFF, 1},
    {scrCommand_MFILTER_PARA, 1},
    {scrCommand_MFILTER_RGBA, 4},
    {scrCommand_MFILTER_AREA, 3},
    {scrCommand_MFILTER_SWAP, 1},
    {scrCommand_MFILTER_ON, 1},
    {scrCommand_MFILTER_OFF, 1},
    {evtCommand_BGM_TRANS_E, 2},
    {prm_MAKA_PLUS, 1},
    {prm_PARTY_DAMAGE, 1},
    {itfMesMngWindowType, 1},
    {itfMesMngSelectDefKey, 2},
    {evt_1E91A8, 2},
    {evt_1E91E8, 1},
    {evt_1E9220, 0},
    {evt_1E9248, 0},
    {evt_1E9270, 0},
    {scrCommand_RIP_BLUR_ON, 1},
    {scrCommand_RIP_BLUR_OFF, 1},
    {scrCommand_RIP_BLUR_PARA, 7},
    {scrCommand_RIP_BLUR_RGBA, 4},
    {scrCommand_RIP_BLUR_AREA, 3},
    {scrCommand_RIP_BLUR_SWAP, 1},
    {evtOpcodeRefreshTaskData, 2},
    {evt_1E92D8, 0},
    {evtCommand_EFFECT_PRESET, 1},
    {nbCommand_AI_CHK_MYMP, 1},
    {nbCommand_AI_GET_PRESSKOUDO, 0},
    {nbCommand_AI_GET_ALLKOUDO, 0},
    {nbCommand_AI_GET_MYLV, 0},
    {prm_UDV_LEAVE, 1},
    {prm_UDV_CHECK, 1},
    {prm_UDV_DELETE_OK, 1},
    {prm_UDV_DELETE_NG, 1},
    {nbCommand_BTL_FORM_ALL_ON, 0},
    {nbCommand_BTL_FORM_PLY_ON, 0},
    {nbCommand_BTL_FORM_ENE_ON, 0},
    {nbCommand_AI_GET_MYAISYO, 0},
    {evtCommand_CALL_STAFF, 0},
    {prm_STOCK_PLUS, 1},
    {nbCommand_AI_TAR_BAARLV, 0},
    {prm_UDV_GETID, 1},
    {nbCommand_AI_TAR_LVMIN, 0},
    {prm_PLAYER_GET_PARAM, 1},
    {nbCommand_AI_GET_FUREA, 0},
    {evtCommand_CALL_ENDSAVE, 0},
    {evtCommand_KUTI, 3},
    {itfMesMngWindowTypeAlpha, 2},
    {fldCommand_FLD_ANAHORI_INIT, 0},
    {fldCommand_FLD_ANAHORI_GET, 0},
    {fldCommand_FLD_ANAHORI_NEXT, 0},
    {fldCommand_FLD_ANAHORI_DEL, 0},
    {evtCommand_KUTISET, 3},
    {evtCommand_CALL_BATTLE2, 1},
    {evtCommand_EFFBED_BE_EVO, 3},
    {evtCommand_EFFBED_START_EVO, 3},
    {nbCommand_AI_CHK_ESCAPE, 0},
    {evtCommandSetDrawFlagWhileWaiting, 1},
    {movie_22B8F0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {prm_BOSS_PRESS_REC, 2},
    {prm_NAKAMA_GET_PARAM, 2},
    {prm_PLAYER_GET_LND, 0},
    {prm_NAKAMA_DEAD, 1},
    {prm_NAKAMA_CHK_PIKUKO, 1},
    {prm_PIKUKO_GETID, 0},
    {prm_PARTY_KAIFUKU, 0},
    {prm_DV_COPY_PARAM, 2},
    {nbCommand_BTL_AI_ROT_CLR, 0},
    {nbCommand_BTL_AI_ROT_NEXT, 1},
    {nbCommand_BTL_AI_ROT_GET, 0},
    {nbCommand_AI_CHK_ENHOJOMAX, 1},
    {nbCommand_AI_CHK_FRHOJOMAX, 1},
    {nbCommand_BTL_AI_CHANGE_OFF, 0},
    {nbCommand_BTL_AI_CHANGE_ON, 0},
    {nbCommand_AI_CHK_ENHAMAMHK, 0},
    {nbCommand_AI_TAR_JOKEN, 0},
    {nbCommand_AI_CHK_SKILLNOMP, 1},
    {prm_PLAYER_GET_LEVEL, 0},
    {prm_MAKA_PLUS_PER, 1},
    {prm_DV_CHECK, 1},
    {evtCommand_PMV_LOAD_END_WAIT, 1},
    {prm_PARTY_KAIFUKU2, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {evtCommand_TEX_SWAP, 4},
    {evtCommand_TEX_PRI, 2},
    {evtCommand_TEX_SIZE, 5},
    {evtCommand_TEX_POS, 6},
    {evtCommand_TEX_ALP, 4},
    {evtCommand_TEX_ZOOM, 4},
    {evtCommand_TEX_OT, 2},
    {evtCommand_TEX_STAT, 2},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {fldCommand_FLD_TITLE_START2, 2},
    {fldCommand_FLD_GET_KIRETA, 1},
    {fldCommand_FLD_GET_ITEMBOX_RND, 0},
    {fldCommand_FLD_MSE_PLAY2, 2},
    {fldCommand_FLD_MSE_STOP2, 2},
    {fldCommand_SET_NPC_CAMERA, 1},
    {fldCommand_FLD_MODEL_DANTE, 0},
    {fldCommand_FLD_DANTE_AI_STOP, 0},
    {fldCommand_FLD_DANTE_AI_START, 0},
    {fldCommand_FLD_DANTE_STANCE, 1},
    {fldCommand_FLD_BGM_RELOAD, 0},
    {fldCommand_FLD_BGM_RELOAD_WAIT, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {fldCommand_FLD_WARP_ELV_GETTBL, 1},
    {fldCommand_FLD_WARP_ELV_EXIT, 1},
    {fldCommand_FLD_WARP_CALL_FIELD, 0},
    {fldCommand_FLD_WARP_CASH_F, 0},
    {fldCommand_FLD_WARP_RUN_START, 0},
    {fldCommand_FLD_INF_NEXT, 2},
    {fldCommand_FLD_INF_GET, 2},
    {fldCommand_FLD_INF_GETPANEL, 0},
    {fldCommand_FLD_WARP_HASIGO_GETTBL, 1},
    {fldCommand_FLD_ELV_FLOORSTART, 2},
    {fldCommand_FLD_ELV_FLOOREND, 0},
    {fldCommand_FLD_ELV_ENDCHK, 0},
    {fldCommand_FLD_WARP_GET_DOOR, 1},
    {fldCommand_FLD_EXIT_CAMERA, 0},
    {fldCommand_FLD_INF_GOWAP, 0},
    {fldCommand_FLD_CALL_WAPEX, 1},
    {fldCommand_FLD_BGM_PLAY, 1},
    {fldCommand_FLD_BGM_CONTINUE, 1},
    {fldCommand_FLD_BGM_PAUSE, 0},
    {fldCommand_FLD_BGM_STOP, 0},
    {fldCommand_FLD_SE_PLAY, 1},
    {fldCommand_FLD_SE_STOP, 1},
    {fldCommand_FLD_BGM_FADE, 0},
    {fldCommand_FLD_MSE_PLAY, 1},
    {fldCommand_FLD_NPC_SINEN, 1},
    {fldCommand_FLD_WARP_GET_WARPEFF, 0},
    {fldCommand_FLD_TITLE_START, 1},
    {fldCommand_ACT_WIN_DSP2, 1},
    {fldCommand_FLD_SET_ENCOUNT, 1},
    {fldCommand_ACT_WIN_DSP3, 1},
    {fldCommand_FLD_MODEL_LIGHT, 1},
    {fldCommand_FLD_SAGASI_AKUMA, 1},
};

s32 PopInt(scrProcessWork_s *pwork)
{
    s32 stackIndex = pwork->SP;

    pwork->SP = stackIndex - 1;
    switch ((s8)pwork->StackType[stackIndex - 1]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_RET:
        return pwork->StackData[pwork->SP].i;
    case SCR_STACK_TYPE_FLOAT:
        return pwork->StackData[pwork->SP].f;
    case SCR_STACK_TYPE_INTVAR:
        return DDS3_GBWK->integers[pwork->StackData[pwork->SP].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->floats[pwork->StackData[pwork->SP].i];
    }
    return 0;
}

f32 PopFloat(scrProcessWork_s *pwork)
{
    s32 stackIndex = pwork->SP;

    pwork->SP = stackIndex - 1;
    switch ((s8)pwork->StackType[stackIndex - 1]) {
    case SCR_STACK_TYPE_INT:
        return (f32)pwork->StackData[pwork->SP].i;
    case SCR_STACK_TYPE_RET:
        return 0.0f;
    case SCR_STACK_TYPE_FLOAT:
        return pwork->StackData[pwork->SP].f;
    case SCR_STACK_TYPE_INTVAR:
        return (f32)DDS3_GBWK->integers[pwork->StackData[pwork->SP].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->floats[pwork->StackData[pwork->SP].i];
    }
    return 0.0f;
}

s32 CodeFunc_PushI(scrProcessWork_s *pwork)
{
    pwork->CP++;
    PushInt(pwork, pwork->Code[pwork->CP].i);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushIS(scrProcessWork_s *pwork)
{
    PushInt(pwork, pwork->Code[pwork->CP].s.s1);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushF(scrProcessWork_s *pwork)
{
    pwork->CP++;
    PushFloat(pwork, pwork->Code[pwork->CP].f);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushIX(scrProcessWork_s *pwork)
{
    PushInt(pwork, DDS3_GBWK->integers[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushFX(scrProcessWork_s *pwork)
{
    PushFloat(pwork, DDS3_GBWK->floats[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushLIX(scrProcessWork_s *pwork)
{
    PushInt(pwork, pwork->LocalInt[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushLFX(scrProcessWork_s *pwork)
{
    PushFloat(pwork, pwork->LocalFloat[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushSTR(scrProcessWork_s *pwork)
{
    PushStr(pwork, pwork->StringData + pwork->Code[pwork->CP].s.s1);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushReg(scrProcessWork_s *pwork)
{
    pwork->StackType[pwork->SP] = pwork->StackType[27];
    pwork->StackData[pwork->SP].i = pwork->StackData[27].i;
    pwork->SP++;
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PopIX(scrProcessWork_s *pwork)
{
    DDS3_GBWK->integers[pwork->Code[pwork->CP].s.s1] = PopInt(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PopFX(scrProcessWork_s *pwork)
{
    f32 value = PopFloat(pwork);

    DDS3_GBWK->floats[pwork->Code[pwork->CP].s.s1] = value;
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PopLIX(scrProcessWork_s *pwork)
{
    pwork->LocalInt[pwork->Code[pwork->CP].s.s1] = PopInt(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PopLFX(scrProcessWork_s *pwork)
{
    pwork->LocalFloat[pwork->Code[pwork->CP].s.s1] = PopFloat(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Proc(scrProcessWork_s *pwork)
{
    pwork->CP++;
    return CODE_RET_NEXT;
}

/* A pending command yields without consuming its arguments. */
s32 CodeFunc_Comm(scrProcessWork_s *pwork)
{
    s32 commandCP = pwork->CP;
    s32 commandIndex = pwork->Code[commandCP].s.s1;

    nowpwork = pwork;
    if (scrCommandTable[commandIndex].comm() == 0) {
        return CODE_RET_SYNC;
    }
    pwork->SP -= scrCommandTable[commandIndex].paranum;
    if (commandCP == pwork->CP) {
        pwork->CP = commandCP + 1;
    }
    return CODE_RET_NEXT;
}

s32 CodeFunc_End(scrProcessWork_s *pwork)
{
    if (pwork->SP == 0) {
        return CODE_RET_END;
    }

    pwork->CP = PopInt(pwork) + 1;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Jump(scrProcessWork_s *pwork)
{
    pwork->CP = pwork->ProcTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Call(scrProcessWork_s *pwork)
{
    PushRet(pwork, pwork->CP);
    pwork->CP = pwork->ProcTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Run(scrProcessWork_s *pwork)
{
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Goto(scrProcessWork_s *pwork)
{
    pwork->CP = pwork->LabelTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

extern s32 CodeFunc_2Operation_i1;
extern s32 CodeFunc_2Operation_i2;
extern s32 CodeFunc_2Operation_ri;
extern f32 CodeFunc_2Operation_f1;
extern f32 CodeFunc_2Operation_f2;
extern f32 CodeFunc_2Operation_rf;
extern s32 CodeFunc_2Operation_t1;
extern s32 CodeFunc_2Operation_t2;

void CodeFunc_2Operation(scrProcessWork_s *pwork, s32 type)
{
    CodeFunc_2Operation_t1 = (s8)pwork->StackType[pwork->SP - 1];
    CodeFunc_2Operation_t2 = (s8)pwork->StackType[pwork->SP - 2];
    if ((CodeFunc_2Operation_t1 == 0 || CodeFunc_2Operation_t1 == 2) && (CodeFunc_2Operation_t2 == 0 || CodeFunc_2Operation_t2 == 2)) {
        CodeFunc_2Operation_i1 = PopInt(pwork);
        CodeFunc_2Operation_i2 = PopInt(pwork);
        switch (type) {
        case 0:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 + CodeFunc_2Operation_i2;
            break;
        case 1:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 - CodeFunc_2Operation_i2;
            break;
        case 2:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 * CodeFunc_2Operation_i2;
            break;
        case 3:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 / CodeFunc_2Operation_i2;
            break;
        case 4:
            if (CodeFunc_2Operation_i1 != 0 || CodeFunc_2Operation_i2 != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 5:
            if (CodeFunc_2Operation_i1 != 0 && CodeFunc_2Operation_i2 != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 6:
            if (CodeFunc_2Operation_i1 == CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 7:
            if (CodeFunc_2Operation_i1 != CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 8:
            if (CodeFunc_2Operation_i1 < CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 9:
            if (CodeFunc_2Operation_i1 > CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 10:
            if (CodeFunc_2Operation_i1 <= CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 11:
            if (CodeFunc_2Operation_i1 >= CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        }
        PushInt(pwork, CodeFunc_2Operation_ri);
    } else {
        CodeFunc_2Operation_f1 = PopFloat(pwork);
        CodeFunc_2Operation_f2 = PopFloat(pwork);
        switch (type) {
        case 0:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 + CodeFunc_2Operation_f2;
            break;
        case 1:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 - CodeFunc_2Operation_f2;
            break;
        case 2:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 * CodeFunc_2Operation_f2;
            break;
        case 3:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 / CodeFunc_2Operation_f2;
            break;
        case 4:
            if (dpcmp(fptodp(CodeFunc_2Operation_f1), 0.0) != 0 ||
                dpcmp(fptodp(CodeFunc_2Operation_f2), 0.0) != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 5:
            if (dpcmp(fptodp(CodeFunc_2Operation_f1), 0.0) != 0 &&
                dpcmp(fptodp(CodeFunc_2Operation_f2), 0.0) != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 6:
            if (CodeFunc_2Operation_f1 == CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 7:
            if (CodeFunc_2Operation_f1 != CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 8:
            if (CodeFunc_2Operation_f1 < CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 9:
            if (CodeFunc_2Operation_f1 > CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 10:
            if (CodeFunc_2Operation_f1 <= CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 11:
            if (CodeFunc_2Operation_f1 >= CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        }
        PushFloat(pwork, CodeFunc_2Operation_rf);
    }
}

s32 CodeFunc_Add(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_ADD);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Sub(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_SUB);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Mul(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_MUL);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Div(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_DIV);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Minus(scrProcessWork_s *pwork)
{
    switch ((s8)pwork->StackType[pwork->SP - 1]) {
    case SCR_STACK_TYPE_INT:
        pwork->StackData[pwork->SP - 1].i = -pwork->StackData[pwork->SP - 1].i;
        break;
    case SCR_STACK_TYPE_FLOAT:
        pwork->StackData[pwork->SP - 1].f = -pwork->StackData[pwork->SP - 1].f;
        break;
    case SCR_STACK_TYPE_INTVAR:
        DDS3_GBWK->integers[pwork->StackData[pwork->SP - 1].i] =
            -DDS3_GBWK->integers[pwork->StackData[pwork->SP - 1].i];
        break;
    case SCR_STACK_TYPE_FLOATVAR:
        DDS3_GBWK->floats[pwork->StackData[pwork->SP - 1].i] =
            -DDS3_GBWK->floats[pwork->StackData[pwork->SP - 1].i];
        break;
    case SCR_STACK_TYPE_RET:
        break;
    }
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Not(scrProcessWork_s *pwork)
{
    switch ((s8)pwork->StackType[pwork->SP - 1]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_INTVAR:
        PushInt(pwork, PopInt(pwork) == 0);
        break;
    case SCR_STACK_TYPE_FLOAT:
    case SCR_STACK_TYPE_FLOATVAR:
        PushInt(pwork, PopFloat(pwork) == 0.0f);
        break;
    case SCR_STACK_TYPE_RET:
        break;
    }

    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Or(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_OR);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_And(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_AND);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_EQ(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_EQ);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_NEQ(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_NEQ);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_S(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_S);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_L(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_L);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_SE(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_SE);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_LE(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_LE);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_IF(scrProcessWork_s *pwork)
{
    s32 cond;

    switch ((s8)pwork->StackType[pwork->SP - 1]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_INTVAR:
        cond = PopInt(pwork);
        break;
    case SCR_STACK_TYPE_FLOAT:
    case SCR_STACK_TYPE_FLOATVAR:
        cond = PopFloat(pwork) != 0.0f;
        break;
    case SCR_STACK_TYPE_RET:
    default:
        cond = 0;
        break;
    }

    if (cond) {
        pwork->CP++;
    } else {
        pwork->CP = pwork->LabelTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    }
    return CODE_RET_NEXT;
}

/* Continue until an instruction completes the procedure or yields a tick. */
s32 scrTraceCode(scrProcessWork_s *pwork)
{
    s32 result;

    while (1) {
        result = codefunc[pwork->Code[pwork->CP].s.s0](pwork);
        if (result == CODE_RET_END) {
            pwork->CommandTimer = 0;
            return SCR_RESULT_END;
        }
        if (result == CODE_RET_SYNC) {
            break;
        }
        if (result == CODE_RET_NEXT) {
            pwork->CommandTimer = 0;
        }
    }
    pwork->CommandTimer++;
    pwork->ScriptTimer++;
    return SCR_RESULT_CONTINUE;
}

s32 scrGetIntPara(s32 n)
{
    scrProcessWork_s *pw = nowpwork;
    s32 stackIndex = pw->SP - n - 1;

    switch ((s8)pw->StackType[stackIndex]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_RET:
        return pw->StackData[stackIndex].i;
    case SCR_STACK_TYPE_FLOAT:
        return pw->StackData[stackIndex].f;
    case SCR_STACK_TYPE_INTVAR:
        return DDS3_GBWK->integers[pw->StackData[stackIndex].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->floats[pw->StackData[stackIndex].i];
    }
    return 0;
}

f32 scrGetFloatPara(s32 n)
{
    scrProcessWork_s *pw = nowpwork;
    s32 stackIndex = pw->SP - n - 1;

    switch ((s8)pw->StackType[stackIndex]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_RET:
        return (f32)pw->StackData[stackIndex].i;
    case SCR_STACK_TYPE_FLOAT:
        return pw->StackData[stackIndex].f;
    case SCR_STACK_TYPE_INTVAR:
        return (f32)DDS3_GBWK->integers[pw->StackData[stackIndex].i];
    case SCR_STACK_TYPE_FLOATVAR:
        return DDS3_GBWK->floats[pw->StackData[stackIndex].i];
    }
    return 0.0f;
}

u8 *scrGetStrPara(s32 n)
{
    scrProcessWork_s *pw = nowpwork;
    s32 stackIndex = pw->SP - n - 1;
    s32 stackType = (s8)pw->StackType[stackIndex];

    if (stackType >= 0) {
        if (stackType >= SCR_STACK_TYPE_STRING) {
            if (stackType == SCR_STACK_TYPE_STRING) {
                return EE_ADDR_AS(u8 *, pw->StackData[stackIndex].i);
            }
        }
    }
    return NULL;
}

void scrSetIntReturnValue(s32 i)
{
    nowpwork->StackType[27] = SCR_STACK_TYPE_INT;
    nowpwork->StackData[27].i = i;
}

void scrSetFloatReturnValue(f32 f)
{
    nowpwork->StackType[27] = SCR_STACK_TYPE_FLOAT;
    nowpwork->StackData[27].f = f;
}

s32 scrGetProcedureCodeAddress(s32 procindex)
{
    return nowpwork->ProcTable[procindex].CodeAddress;
}

s32 scrGetLabelCodeAddress(s32 labelindex)
{
    return nowpwork->LabelTable[labelindex].CodeAddress;
}

s32 scrGetCodePointer(void)
{
    return nowpwork->CP;
}

void scrSetCodePointer(s32 cp)
{
    nowpwork->CP = cp;
}

s32 scrGetScriptTimer(void)
{
    return nowpwork->ScriptTimer;
}

s32 scrGetCommandTimer(void)
{
    return nowpwork->CommandTimer;
}

s32 scrGetMessageHandle(void)
{
    return nowpwork->MesHandle;
}

scrProcessWork_s *scrGetScriptHandle(void)
{
    return nowpwork;
}

void *scrGetData(void)
{
    return nowpwork->data;
}
