#include "effBattlePositionDispatch.h"

/* scaleKind selects the provider; each leaves the result in vf10. */
effBattlePositionProvider_t GetPositionPtrTable[6] = {
    effBTLPartGetModelPositionPtr,
    effBTLPartGetInvokePartyPositionPtr,
    effBTLPartGetTargetPartyPositionPtr,
    effBTLPartGetWorldPositionPtr,
    effBTLPartGetModelMarkPositionPtr,
    effBTLPartGetModelMarkPositionPtr,
};
