#include "common.h"

extern char nmeKyaraTmxPath01[];
extern char nmeKyaraTmxPath02[];
extern char nmeKyaraTmxPath03[];
extern char nmeKyaraTmxPath04[];
extern char nmeKyaraTmxPath05[];

char *nmeCharTexFile[4] = {
    nmeKyaraTmxPath05, nmeKyaraTmxPath02, nmeKyaraTmxPath03, nmeKyaraTmxPath04,
};

char *nmeAltCharTexFile __attribute__((section(".data"))) = nmeKyaraTmxPath01;
