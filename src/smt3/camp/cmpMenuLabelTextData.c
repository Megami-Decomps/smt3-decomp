#include "common.h"

extern const char cmpTextMAINMENU[16];
extern const char cmpTextWhatWillYouUse[24];
extern char D_0054FE60[8];
extern char D_0054FE58[8];
extern char cmpTextITEM[8];
extern const char cmpTextWhoWillUseASkill[24];
extern const char cmpTextWhoWillUseIt[24];
extern const char cmpTextUseWhatItem[16];
extern char cmpTextGEMS[8];
extern const char cmpTextSPITEMS[16];
extern const char cmpTextWhichWillYouIngest[24];
extern const char cmpTextWhichWillYouUnequip[24];
extern const char cmpTextFromWhomWillYouExtract[32];
extern const char cmpTextWhoWillYouSummon[24];
extern const char cmpTextWhoWillYouReturn[24];
extern const char cmpTextWhoWillYouPartWith[24];
extern const char cmpTextWhatWillYouDiscard[24];
extern const char cmpTextSummoned[16];
extern const char cmpTextNoDemonsToExtractFrom[32];
extern char D_0054FE68[8];

/* Original menu label order, including the unused empty entries. */
const char *cmpMenuTextTable[22] = {
    cmpTextMAINMENU,
    cmpTextWhatWillYouUse,
    D_0054FE60,
    D_0054FE58,
    cmpTextITEM,
    cmpTextWhoWillUseASkill,
    cmpTextWhoWillUseIt,
    cmpTextUseWhatItem,
    cmpTextGEMS,
    cmpTextSPITEMS,
    cmpTextWhichWillYouIngest,
    cmpTextWhichWillYouUnequip,
    cmpTextFromWhomWillYouExtract,
    cmpTextWhoWillYouSummon,
    cmpTextWhoWillYouReturn,
    cmpTextWhoWillYouPartWith,
    cmpTextWhatWillYouDiscard,
    NULL,
    cmpTextSummoned,
    cmpTextNoDemonsToExtractFrom,
    D_0054FE68,
    NULL,
};
