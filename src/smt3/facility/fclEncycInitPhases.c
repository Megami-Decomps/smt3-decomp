#include "facility.h"

extern void InitPhaseRegistTop(fclEncyc_instance_tag *);
extern void InitPhaseRegistConfirm(fclEncyc_instance_tag *);
extern void InitPhaseReadTop(fclEncyc_instance_tag *);
extern void InitPhaseReadStatus(fclEncyc_instance_tag *);

/* Phase 8 tears down the process rather than dispatching a callback. */
void (*fclEncycInitPhaseTable[8])(fclEncyc_instance_tag *) = {
    InitPhaseRegistTop,
    InitPhaseRegistConfirm,
    NULL,
    InitPhaseReadTop,
    InitPhaseReadStatus,
    NULL,
    NULL,
    NULL,
};
