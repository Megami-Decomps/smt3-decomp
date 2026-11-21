#include "facility.h"

extern void DestPhaseRegistTop(fclEncyc_instance_tag *);
extern void DestPhaseRegistConfirm(fclEncyc_instance_tag *);
extern void DestPhaseReadTop(fclEncyc_instance_tag *);
extern void DestPhaseReadStatus(fclEncyc_instance_tag *);

/* Phase 8 tears down the process rather than dispatching a callback. */
void (*DestPhaseTable[8])(fclEncyc_instance_tag *) = {
    DestPhaseRegistTop,
    DestPhaseRegistConfirm,
    DestPhaseRegistTop,
    DestPhaseReadTop,
    DestPhaseReadStatus,
    DestPhaseReadStatus,
    DestPhaseReadStatus,
    DestPhaseReadStatus,
};
