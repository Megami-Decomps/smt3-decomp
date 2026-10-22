#include "common.h"
#include "basic.h"
#include "model.h"
#include "motion.h"

extern void sdfChipFreeDirect(void *aPtr);

void dds3PathBasicFreeControl(dds3Basic_t *pControl) {
    sdfChipFreeDirect(pControl->workdata);
}
