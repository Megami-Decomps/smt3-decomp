#ifndef EFF_BATTLE_POSITION_DISPATCH_H
#define EFF_BATTLE_POSITION_DISPATCH_H

#include "effBattle.h"
#include "newbattle.h"

typedef void (*effBattlePositionProvider_t)(nbFormation_t *, effBTLAimParams_t *);

extern effBattlePositionProvider_t GetPositionPtrTable[];

void effBTLPartGetModelPositionPtr(nbFormation_t *formation, effBTLAimParams_t *request);
void effBTLPartGetInvokePartyPositionPtr(nbFormation_t *formation, effBTLAimParams_t *request);
void effBTLPartGetTargetPartyPositionPtr(nbFormation_t *formation, effBTLAimParams_t *request);
void effBTLPartGetWorldPositionPtr(nbFormation_t *formation, effBTLAimParams_t *request);
void effBTLPartGetModelMarkPositionPtr(nbFormation_t *formation, effBTLAimParams_t *request);

#endif
