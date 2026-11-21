#include "basic.h"

extern s32 dds3CameraObjectBasicConstructor(dds3Basic_t *basic);
extern void dds3CameraObjectBasicDestructor(dds3Basic_t *basic);
extern s32 dds3CameraObjectBasicMainProcedure(dds3Basic_t *basic);
extern s32 dds3CameraObjectBasicDrawProcedure(dds3Basic_t *basic);

dds3BasicFunction_t dds3CameraObjectBasicTable = {
    dds3CameraObjectBasicConstructor,
    dds3CameraObjectBasicDestructor,
    dds3CameraObjectBasicMainProcedure,
    dds3CameraObjectBasicDrawProcedure,
    NULL,
    NULL,
};
