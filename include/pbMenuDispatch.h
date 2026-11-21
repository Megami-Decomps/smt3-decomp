#ifndef PB_MENU_DISPATCH_H
#define PB_MENU_DISPATCH_H

#include "common.h"

typedef struct pbProcHandle_t pbProcHandle_t;

/* Both dispatches receive the active panel; the check result is compared to 1. */
typedef s32 (*pbMenuCheck_t)(pbProcHandle_t *panel);
typedef void (*pbMenuAction_t)(pbProcHandle_t *panel);

extern pbMenuCheck_t pbMenuCheckTable[2][8];
extern pbMenuAction_t pbMenuActionTable[2][8];

s32 func_001D4BC0(pbProcHandle_t *panel);
s32 func_001D4BC8(pbProcHandle_t *panel);
s32 func_001D4BE8(pbProcHandle_t *panel);

void func_001D4C08(pbProcHandle_t *panel);
void pbMakeMapPath(pbProcHandle_t *panel);
void func_001D4E10(pbProcHandle_t *panel);
void pbMakeMapPathHdd(pbProcHandle_t *panel);
void func_001D4E08(pbProcHandle_t *panel);
void pbSetDebugOwner(pbProcHandle_t *panel);
void exitPB(pbProcHandle_t *panel);
void func_001D4F08(pbProcHandle_t *panel);
void func_001D4EB0(pbProcHandle_t *panel);
void func_001D4E70(pbProcHandle_t *panel);

#endif
