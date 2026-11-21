const char D_005454F0[24] = "\x81\xf3\x82\x99\x81\xc8\x81\xf8\x82\x84\x81\xd7\x81\xca\x81\xd9";

const char D_00545508[16] = "\x81\xE1\x81\xD4\x81\xCA\x81\xD7\x84\xA0\x84\xA1";

const char D_00545518[16] = "\x80\xAD\x80\xB0\x84\xA0\x84\xA1";

const char D_00545528[16] = "\x80\xA8\x80\xB0\x84\xA0\x84\xA1";

const char D_00545538[16] = "\x81\xBE\x81\xF4\x81\xFA\x84\xA0\x84\xA1";

const char D_00545548[16] = "\x81\xB3\x81\xB5\x81\xD7\x81\xF1";

const char D_00545558[16] = "\x81\xDE\x81\xC5\x87\xAC\x85\xC7";

const char D_00545568[16] = "\x81\xDE\x81\xC5\x87\xAE\x85\xCC";

const char D_00545578[16] = "\x81\xDE\x81\xC5\x84\xE6\x84\xE7";

const char D_00545588[16] = "\x81\xB3\x81\xD0\x81\xD4\x81\xC0";

const char D_00545598[16] = "\x81\xE1\x81\xD4\x81\xCA\x81\xD7";

const char D_005455A8[16] = "\x81\xD1\x81\xF2\x82\x99\x81\xC9";

const char nbBadStatusParalyze[16] = "PARALYZE";

extern char nbDebugCountDmg[], nbDebugCountAi0[], nbDebugCountAi1[], nbDebugCountAi2[],
    nbDebugCountBatk[], nbDebugCountMatk[], nbDebugCountEsc[], nbDebugCountDef[], nbDebugCountHit[],
    nbDebugCountHyooff[], nbDebugCountKaeoff[], nbDebugCountJyuoff[], nbDebugCountHamoff[],
    nbDebugCountMref[], nbDebugCountBref[], nbDebugCountAtkx2[], nbDebugCountTarget[],
    nbDebugCountKCnt[], nbDebugCountPCnt[], nbDebugCountAisyo[], nbDebugAisyoButu[],
    nbDebugAisyoKaen[], nbDebugAisyoHyou[], nbDebugAisyoDeng[], nbDebugAisyoShog[],
    nbDebugAisyoBann[], nbDebugAisyoHama[], nbDebugAisyoJyus[], nbDebugAisyoMary[],
    nbDebugAisyoSnke[], nbDebugAisyoSsin[], nbDebugAisyoJiba[], nbDebugAisyoNego[],
    nbDebugAisyoRec[], nbDebugAisyoHojo[], nbDebugAisyoSpec[];

typedef struct {
    const char *count[20];
    const char *aisyo[16];
} nbDebugUnitLabels_t;

const nbDebugUnitLabels_t nbDebugUnitLabelTable = {
    {nbDebugCountDmg, nbDebugCountAi0, nbDebugCountAi1, nbDebugCountAi2, nbDebugCountBatk, nbDebugCountMatk, nbDebugCountEsc, nbDebugCountDef, nbDebugCountHit, nbDebugCountHyooff, nbDebugCountKaeoff, nbDebugCountJyuoff, nbDebugCountHamoff, nbDebugCountMref, nbDebugCountBref, nbDebugCountAtkx2, nbDebugCountTarget, nbDebugCountKCnt, nbDebugCountPCnt, nbDebugCountAisyo},
    {nbDebugAisyoButu, nbDebugAisyoKaen, nbDebugAisyoHyou, nbDebugAisyoDeng, nbDebugAisyoShog, nbDebugAisyoBann, nbDebugAisyoHama, nbDebugAisyoJyus, nbDebugAisyoMary, nbDebugAisyoSnke, nbDebugAisyoSsin, nbDebugAisyoJiba, nbDebugAisyoNego, nbDebugAisyoRec, nbDebugAisyoHojo, nbDebugAisyoSpec},
};

const char nbDebugIdFmt[16] = "ID  : 0x%03X";

const char nbDebugLevelFmt[16] = "LV  : %d";

const char nbDebugHpFmt[16] = "HP  : %d/%d";

const char nbDebugMpFmt[16] = "MP  : %d/%d";

extern char nbDebugMotionNameNormal[], nbDebugMotionNamePause[], nbDebugMotionNameOut[], nbDebugMotionNameStop[];

const char *const nbDebugMotionNames[6] = {nbDebugMotionNameNormal, nbDebugMotionNamePause, nbDebugMotionNameOut, nbDebugMotionNameStop, 0, 0};
