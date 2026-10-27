#include "common.h"
#include "nb.h"
#include "effBattle.h"

extern void nbInitPlayer(void);

extern void nbInitMainProcess(s32 encpackno, s32 packno, s32 packindex, s32 encno,
                              s32 stagemajor, s32 stageminor);

/* 420-byte entries; only the two fields this unit reads are named. */
typedef struct
{
    u16 stagemajor;
    u16 stageminor;
    u8 unk_04[0x1A0];
} nbEncountEntry_t;

extern nbEncountEntry_t nbEncountTable[];

/* 38-byte entries; only the two fields this unit reads are named. */
typedef struct
{
    u8 unk_00[0x1C];
    u16 stagemajor;
    u16 stageminor;
    u8 unk_20[0x6];
} nbEncountForm_t;

extern nbEncountForm_t nbEncountFormTable[];

extern void nbKillMainProcess(void);

void nb_246898(void)
{
    nbInitPlayer();
}

void nb_2468B0(void)
{
    nbInitMainProcess(0, 0, 0, 1, 0xC9, 1);
}

void nbExit(void)
{
    nbKillMainProcess();
}

void nbCallNewBattle(s32 encpackno, s32 code)
{
    s32 encno = (u16)code;
    s32 stagemajor;
    s32 stageminor;

    if (encpackno > 0) {
        stagemajor = nbEncountTable[encpackno].stagemajor;
        stageminor = nbEncountTable[encpackno].stageminor;
    } else {
        stagemajor = nbEncountFormTable[encno].stagemajor;
        stageminor = nbEncountFormTable[encno].stageminor;
        if (stagemajor == 0 && stageminor == 0) {
            stagemajor = 0xC9;
            stageminor = 1;
        }
    }
    nbInitMainProcess(encpackno, (u32)code >> 24, (code >> 16) & 0xFF, encno, stagemajor,
                      stageminor);
}
