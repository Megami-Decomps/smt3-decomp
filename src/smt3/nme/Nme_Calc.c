#include "common.h"

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeCalc);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawCharRow);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawSymbolRow);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawNameText);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawNameString);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawEditIcons);

void nmeDrawTextBox(s32 x, s32 y, s32 z)
{
    s32 colors[4];

    colors[0] = 0x45202680;
    colors[1] = 0;
    colors[2] = 0x45202680;
    colors[3] = 0;
    cmpDrawBoxGDefault(x, y, z, 0x4F, 0x16, colors, 0x47);
}

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawColoredText);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawButtonGuide);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawPanelSprites);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawNamePanel);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawCharCell);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawCursorAt);

void nmeDrawCursor(void)
{
    nmeDrawCursorAt(0x29, 0xDA, 0, 0, 0x47);
}

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawTabSprite);

INCLUDE_ASM(const s32, "nme/Nme_Calc", nmeDrawModeTabs);

INCLUDE_RODATA(const s32, "nme/Nme_Calc", D_005487F8);

INCLUDE_RODATA(const s32, "nme/Nme_Calc", D_00548808);

INCLUDE_RODATA(const s32, "nme/Nme_Calc", D_00548818);

INCLUDE_RODATA(const s32, "nme/Nme_Calc", D_00548828);

