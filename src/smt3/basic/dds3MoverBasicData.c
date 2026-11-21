#include "common.h"
#include "basic.h"

extern s32 MoverBasicConstructor(dds3Basic_t *pBasic);
extern void MoverBasicDestructor(dds3Basic_t *pBasic);
extern s32 MoverBasicMainprocuder(dds3Basic_t *pBasic);

dds3BasicFunction_t MoverBasicTable = {
    MoverBasicConstructor,
    MoverBasicDestructor,
    MoverBasicMainprocuder,
    NULL,
    NULL,
    NULL,
};

char dds3MoverNameTable[10][16] = {
    "MOVER00", "MOVER01", "MOVER02", "MOVER03", "MOVER04",
    "MOVER05", "MOVER06", "MOVER07", "MOVER08", "MOVER09",
};
