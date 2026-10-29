#include "common.h"
#include "nb.h"
#include "effBattle.h"

extern void nbInitPlayer(void);

extern void nbInitMainProcess(s32 encpackno, s32 packno, s32 packindex, s32 encno,
                              s32 stagemajor, s32 stageminor);

#include "encount.h"

extern nbEncountEntry_t nbEncountTable[];

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

/* NON_MATCHING: 29 of 40 words differ, at the exact 160-byte size. */
#ifdef NON_MATCHING
void nbCallNewBattle(s32 encpackno, s32 code)
{
    s32 encno = (u16)code;
    nbEncountForm_t *form = &nbEncountFormTable[encno];
    s32 stagemajor;
    s32 stageminor;

    if (encpackno > 0) {
        stagemajor = nbEncountTable[encpackno].stagemajor;
        stageminor = nbEncountTable[encpackno].stageminor;
    } else {
        stagemajor = form->stagemajor;
        stageminor = form->stageminor;
        if (stagemajor == 0 && stageminor == 0) {
            stagemajor = 0xC9;
            stageminor = 1;
        }
    }
    nbInitMainProcess(encpackno, (u32)code >> 24, (code >> 16) & 0xFF, encno, stagemajor,
                      stageminor);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbInit", nbCallNewBattle);
#endif
