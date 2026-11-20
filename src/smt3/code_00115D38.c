#include "common.h"
#include "basic.h"
#include "model.h"
#include "motion.h"

extern void sdfChipFreeDirect(void *aPtr);
extern s32 dds3AllocEventBasicWorkData(dds3Basic_t *basic);

void dds3PathBasicFreeControl(dds3Basic_t *pControl) {
    sdfChipFreeDirect(pControl->workdata);
}

dds3BasicFunction_t dds3PathBasicTable = {
    dds3AllocEventBasicWorkData,
    dds3PathBasicFreeControl,
    NULL,
    NULL,
    NULL,
    NULL,
};
