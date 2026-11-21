#include "facility.h"

extern void DrawPhaseRegistTop(fclEncyc_instance_tag *);
extern void DrawPhaseRegistConfirm(fclEncyc_instance_tag *);
extern void DrawPhaseReadTop(fclEncyc_instance_tag *);
extern void DrawPhaseReadStatus(fclEncyc_instance_tag *);
extern void DrawPhaseReadSummon(fclEncyc_instance_tag *);
extern void DrawPhaseReadExplain(fclEncyc_instance_tag *);
extern void DrawPhaseReadView(fclEncyc_instance_tag *);

/* Phase 8 tears down the process rather than dispatching a callback. */
void (*DrawPhaseTable[8])(fclEncyc_instance_tag *) = {
    DrawPhaseRegistTop,
    DrawPhaseRegistConfirm,
    DrawPhaseRegistTop,
    DrawPhaseReadTop,
    DrawPhaseReadStatus,
    DrawPhaseReadSummon,
    DrawPhaseReadExplain,
    DrawPhaseReadView,
};
