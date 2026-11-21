#include "common.h"

extern char cmpTextItem[8], cmpTextSkill[8], cmpTextParty[8];
extern char cmpTextStatus[8], cmpTextConfig[8], cmpTextLoad[8];
extern char cmpTextUse[8], cmpTextDiscard[8], cmpTextGems[8];
extern char cmpTextSummon[8], cmpTextEquip[8], cmpTextUnequip[8];
extern const char D_0053F438[16], D_0053F448[16], D_0053F458[16], D_0053F468[16];

/* The main cursor draws seven rows; the three pages draw four, three and two. */
const char *cmpCursorCells[8] = {
    cmpTextItem, cmpTextSkill, cmpTextParty, D_0053F438,
    cmpTextStatus, cmpTextConfig, cmpTextLoad, NULL,
};
const char *cmpPage0Cells[4] = {
    cmpTextUse, cmpTextDiscard, cmpTextGems, D_0053F448,
};
const char *cmpPage1Cells[4] = {
    cmpTextSummon, D_0053F468, D_0053F458, NULL,
};
/* Keep the two label pointers in the native main-data cursor table. */
const char *cmpPage2Cells[2] __attribute__((section(".data"))) = { cmpTextEquip, cmpTextUnequip };
