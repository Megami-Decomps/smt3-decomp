#include "common.h"

extern const char D_00542160[16];
extern const char cmpTextMuscleDrink[16];
extern const char cmpTextMedicine[16];
extern const char cmpTextLifeStone[16];
extern char D_005507D8[8];
extern const char cmpTextBeadChain[16];
extern const char cmpTextChakraDrop[16];
extern const char cmpTextChakraPot[16];
extern const char cmpTextGreatChakra[16];
extern const char cmpTextSomaDroplet[16];
extern char D_005507D0[8];
extern const char cmpTextBeadOfLife[16];
extern const char cmpTextChakraElixir[16];
extern const char cmpTextRevivalBead[16];
extern const char cmpTextBalmOfRising[16];
extern const char cmpTextSacredWater[16];
extern const char cmpTextDisPoison[16];
extern const char cmpTextDisStun[16];
extern const char cmpTextDisCharm[16];
extern const char cmpTextDisMute[16];
extern const char cmpTextDisStone[16];
extern const char cmpTextMaragiRock[16];
extern const char cmpTextMabufuRock[16];
extern const char cmpTextMazioRock[16];
extern const char cmpTextMazanRock[16];
extern const char cmpTextMahamaRock[16];
extern const char cmpTextMamudoRock[16];
extern const char cmpTextMegidoRock[16];
extern const char cmpTextMegidolaRock[16];
extern const char cmpTextPoisonArrow[16];
extern const char cmpTextWagtailPlume[16];
extern const char cmpTextMakajamRock[16];
extern const char cmpTextPotOfDeath[16];
extern char D_0054FF38[8];
extern const char cmpTextMagicMirror[16];
extern const char cmpTextAttackMirror[16];
extern const char cmpTextDekajaRock[16];
extern const char cmpTextTetrajaRock[16];
extern const char cmpTextStIncense[16];
extern const char cmpTextMaIncense[16];
extern const char cmpTextViIncense[16];
extern const char cmpTextAgIncense[16];
extern const char cmpTextLuIncense[16];
extern const char cmpTextBlessedFan[16];
extern const char cmpTextSoulReturn[16];
extern const char cmpTextSpyglass[16];
extern const char cmpTextRepulseBell[16];
extern const char cmpTextAttractPipe[16];
extern const char cmpTextFloatBall[16];
extern const char cmpTextLightBall[16];
extern const char cmpTextSmokeBall[16];
extern const char cmpTextDiviningWater[16];
extern const char cmpTextLuckyTicket[16];
extern const char cmpTextMarogareh[16];
extern const char cmpTextWadatsumi[16];
extern char D_005507C8[8];
extern const char cmpTextIyomante[16];
extern const char cmpTextShiranui[16];
extern char D_005507C0[8];
extern char D_005507B8[8];
extern const char cmpTextNarukami[16];
extern const char cmpTextAnathema[16];
extern char D_005507B0[8];
extern char D_005507A8[8];
extern const char cmpTextMurakumo[16];
extern char D_005507A0[8];
extern char D_00550798[8];
extern char D_00550790[8];
extern char D_00550788[8];
extern const char cmpTextKamurogi[16];
extern char D_00550780[8];
extern char D_00550778[8];
extern char D_00550770[8];
extern char D_00550768[8];
extern char D_00550760[8];
extern char D_00550758[8];
extern char D_00550750[8];
extern const char cmpTextMasakados[16];
extern char D_00550748[8];
extern char D_00550740[8];
extern const char cmpTextSapphire[16];
extern char D_00550738[8];
extern char D_00550730[8];
extern char D_00550728[8];
extern char D_00550720[8];
extern const char cmpTextAmethyst[16];
extern char D_00550718[8];
extern const char cmpTextTurquoise[16];
extern char D_00550710[8];
extern char D_00550708[8];
extern char D_00550700[8];
extern const char cmpTextAquamarine[16];
extern char D_005506F8[8];
extern const char cmpTextAnnexPass[16];
extern const char cmpTextWhiteKila[16];
extern const char cmpTextYellowKila[16];
extern const char cmpTextGreenKila[16];
extern const char cmpTextRedKila[16];
extern const char cmpText1000YenBill[16];
extern char D_005506F0[8];
extern const char cmpTextUmugiStone[16];
extern char D_005506E8[8];
extern const char cmpTextHimorogi[16];
extern const char cmpTextKimonStone[16];
extern const char cmpTextLordSSword[16];
extern const char cmpTextParkKey[16];
extern const char cmpTextHeavenstone[16];
extern const char cmpTextEarthstone[16];
extern const char cmpTextNetherstone[16];
extern char D_005506E0[8];
extern char D_005506D8[8];
extern char D_005506D0[8];
extern const char cmpTextCompassion[16];
extern const char cmpTextGodliness[16];
extern char D_005506C8[8];
extern const char cmpTextEternity[16];
extern char D_005506C0[8];
extern const char cmpTextFoundation[16];
extern const char cmpTextSovereignty[16];
extern const char cmpTextKnowledge[16];
extern const char cmpTextMoonKey[16];
extern const char cmpTextDeathstone[16];
extern const char cmpTextBlackVisor[16];
extern const char cmpTextAfterlifeBell[16];
extern const char cmpTextStarKey[16];

/* The established table ABI uses char pointers. These borrows keep the original
 * string storage and are only read by the message/front-end consumers. */
#define READONLY_TEXT(name) ((char *)(name))

/* Item names in native item-ID order. */
char *pStr[160] = {
    READONLY_TEXT(D_00542160), READONLY_TEXT(cmpTextMuscleDrink), READONLY_TEXT(cmpTextMedicine),
    READONLY_TEXT(cmpTextLifeStone), D_005507D8, READONLY_TEXT(cmpTextBeadChain),
    READONLY_TEXT(cmpTextChakraDrop), READONLY_TEXT(cmpTextChakraPot), READONLY_TEXT(cmpTextGreatChakra),
    READONLY_TEXT(cmpTextSomaDroplet), D_005507D0, READONLY_TEXT(cmpTextBeadOfLife),
    READONLY_TEXT(cmpTextChakraElixir), READONLY_TEXT(cmpTextRevivalBead), READONLY_TEXT(cmpTextBalmOfRising),
    READONLY_TEXT(cmpTextSacredWater), READONLY_TEXT(cmpTextDisPoison), READONLY_TEXT(cmpTextDisStun),
    READONLY_TEXT(cmpTextDisCharm), READONLY_TEXT(cmpTextDisMute), READONLY_TEXT(cmpTextDisStone),
    READONLY_TEXT(cmpTextMaragiRock), READONLY_TEXT(cmpTextMabufuRock), READONLY_TEXT(cmpTextMazioRock),
    READONLY_TEXT(cmpTextMazanRock), READONLY_TEXT(cmpTextMahamaRock), READONLY_TEXT(cmpTextMamudoRock),
    READONLY_TEXT(cmpTextMegidoRock), READONLY_TEXT(cmpTextMegidolaRock), READONLY_TEXT(cmpTextPoisonArrow),
    READONLY_TEXT(cmpTextWagtailPlume), READONLY_TEXT(cmpTextMakajamRock), READONLY_TEXT(cmpTextPotOfDeath),
    D_0054FF38, READONLY_TEXT(cmpTextMagicMirror), READONLY_TEXT(cmpTextAttackMirror),
    READONLY_TEXT(cmpTextDekajaRock), READONLY_TEXT(cmpTextTetrajaRock), READONLY_TEXT(cmpTextStIncense),
    D_0054FF38, READONLY_TEXT(cmpTextMaIncense), READONLY_TEXT(cmpTextViIncense),
    READONLY_TEXT(cmpTextAgIncense), READONLY_TEXT(cmpTextLuIncense), READONLY_TEXT(cmpTextBlessedFan),
    READONLY_TEXT(cmpTextSoulReturn), READONLY_TEXT(cmpTextSpyglass), D_0054FF38,
    D_0054FF38, D_0054FF38, D_0054FF38,
    D_0054FF38, READONLY_TEXT(cmpTextRepulseBell), READONLY_TEXT(cmpTextAttractPipe),
    READONLY_TEXT(cmpTextFloatBall), READONLY_TEXT(cmpTextLightBall), READONLY_TEXT(cmpTextSmokeBall),
    D_0054FF38, READONLY_TEXT(cmpTextDiviningWater), D_0054FF38,
    D_0054FF38, READONLY_TEXT(cmpTextLuckyTicket), D_0054FF38,
    D_0054FF38, READONLY_TEXT(cmpTextMarogareh), READONLY_TEXT(cmpTextWadatsumi),
    D_005507C8, READONLY_TEXT(cmpTextIyomante), READONLY_TEXT(cmpTextShiranui),
    D_005507C0, D_005507B8, READONLY_TEXT(cmpTextNarukami),
    READONLY_TEXT(cmpTextAnathema), D_005507B0, D_005507A8,
    READONLY_TEXT(cmpTextMurakumo), D_005507A0, D_00550798,
    D_00550790, D_00550788, READONLY_TEXT(cmpTextKamurogi),
    D_00550780, D_00550778, D_00550770,
    D_00550768, D_00550760, D_00550758,
    D_00550750, READONLY_TEXT(cmpTextMasakados), D_0054FF38,
    D_0054FF38, D_0054FF38, D_0054FF38,
    D_0054FF38, D_0054FF38, D_0054FF38,
    D_00550748, D_00550740, READONLY_TEXT(cmpTextSapphire),
    D_00550738, D_00550730, D_00550728,
    D_00550720, READONLY_TEXT(cmpTextAmethyst), D_00550718,
    READONLY_TEXT(cmpTextTurquoise), D_00550710, D_0054FF38,
    D_00550708, D_0054FF38, D_00550700,
    READONLY_TEXT(cmpTextAquamarine), D_005506F8, READONLY_TEXT(cmpTextAnnexPass),
    READONLY_TEXT(cmpTextWhiteKila), READONLY_TEXT(cmpTextYellowKila), READONLY_TEXT(cmpTextGreenKila),
    READONLY_TEXT(cmpTextRedKila), READONLY_TEXT(cmpText1000YenBill), D_005506F0,
    READONLY_TEXT(cmpTextUmugiStone), D_005506E8, READONLY_TEXT(cmpTextHimorogi),
    READONLY_TEXT(cmpTextKimonStone), READONLY_TEXT(cmpTextLordSSword), READONLY_TEXT(cmpTextParkKey),
    READONLY_TEXT(cmpTextHeavenstone), READONLY_TEXT(cmpTextEarthstone), READONLY_TEXT(cmpTextNetherstone),
    D_005506E0, D_005506D8, D_005506D0,
    READONLY_TEXT(cmpTextCompassion), READONLY_TEXT(cmpTextGodliness), D_005506C8,
    READONLY_TEXT(cmpTextEternity), D_005506C0, READONLY_TEXT(cmpTextFoundation),
    READONLY_TEXT(cmpTextSovereignty), READONLY_TEXT(cmpTextKnowledge), READONLY_TEXT(cmpTextMoonKey),
    READONLY_TEXT(cmpTextDeathstone), READONLY_TEXT(cmpTextBlackVisor), READONLY_TEXT(cmpTextAfterlifeBell),
    READONLY_TEXT(cmpTextStarKey), D_0054FF38, D_0054FF38,
    D_0054FF38, D_0054FF38, D_0054FF38,
    D_0054FF38, D_0054FF38, D_0054FF38,
    D_0054FF38, D_0054FF38, D_0054FF38,
    D_0054FF38, D_0054FF38, D_0054FF38,
    D_0054FF38,
};
