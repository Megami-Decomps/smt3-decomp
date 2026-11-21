#include "facility.h"

extern void CalcPhaseRegistTop(fclEncyc_instance_tag *);
extern void CalcPhaseRegistConfirm(fclEncyc_instance_tag *);
extern void CalcPhaseRegistAll(fclEncyc_instance_tag *);
extern void CalcPhaseReadTop(fclEncyc_instance_tag *);
extern void CalcPhaseReadStatus(fclEncyc_instance_tag *);
extern void CalcPhaseReadSummon(fclEncyc_instance_tag *);
extern void CalcPhaseReadExplain(fclEncyc_instance_tag *);
extern void CalcPhaseReadView(fclEncyc_instance_tag *);

/* Phase 8 tears down the process rather than dispatching a callback. */
void (*CalcPhaseTable[8])(fclEncyc_instance_tag *) = {
    CalcPhaseRegistTop,
    CalcPhaseRegistConfirm,
    CalcPhaseRegistAll,
    CalcPhaseReadTop,
    CalcPhaseReadStatus,
    CalcPhaseReadSummon,
    CalcPhaseReadExplain,
    CalcPhaseReadView,
};
