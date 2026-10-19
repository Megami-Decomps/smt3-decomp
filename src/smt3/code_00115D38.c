#include "common.h"
#include "basic.h"
#include "model.h"
#include "motion.h"

extern void sdfChipFreeDirect(void *block);

void dds3PathBasicFreeControl(dds3Basic_t *basic) {
    sdfChipFreeDirect(basic->workdata);
}
