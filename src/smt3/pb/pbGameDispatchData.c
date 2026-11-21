#include "common.h"

extern s32 gameKey0(void);
extern s32 gameKey1(void);
extern s32 gameKey2(void);
extern void pbSetGameCamera1(void);

/* Native control and camera callbacks indexed by the selected character. */
s32 (*keyFunc[4])(void) = { gameKey0, gameKey1, gameKey2, 0 };
void (*pbCharacterTable[4])(void) = { 0, pbSetGameCamera1, 0, 0 };
