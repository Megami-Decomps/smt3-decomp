#include "basic.h"

extern dds3BasicFunction_t worldBasicTable;
extern dds3BasicFunction_t sceneBasicTable;
extern dds3BasicFunction_t AreaBasicTable;
extern dds3BasicFunction_t MoverBasicTable;
extern dds3BasicFunction_t dds3CameraObjectBasicTable;
extern dds3BasicFunction_t dds3UnitObjectBasicTable;
extern dds3BasicFunction_t dds3MatterObjectBasicTable;
extern dds3BasicFunction_t dds3EffectObjectBasicTable;
extern dds3BasicFunction_t EventObjectBasicTable;
extern dds3BasicFunction_t lightBasicTable;
extern dds3BasicFunction_t dds3PathBasicTable;
extern dds3BasicFunction_t dds3ModelBasicTable;
extern dds3BasicFunction_t D_002FA7E8;

/* The allocator's type check admits every entry, including the last null slot. */
dds3BasicFunction_t *dds3BasicFuncTablePtr[BASIC_TYPE_MAX] = {
    &worldBasicTable, &sceneBasicTable, &AreaBasicTable, &MoverBasicTable,
    &dds3CameraObjectBasicTable, &dds3UnitObjectBasicTable,
    &dds3MatterObjectBasicTable, &dds3EffectObjectBasicTable,
    &EventObjectBasicTable, &lightBasicTable, &dds3PathBasicTable, &dds3ModelBasicTable,
    NULL, NULL, NULL, NULL, &D_002FA7E8, NULL,
};
