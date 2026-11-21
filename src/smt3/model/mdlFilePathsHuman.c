#include "mdlBuiltinFilePaths.h"

extern const char pbFileHumanPcTB[12];
extern const char pbFileHumanPcMB[12];
extern const char pbFileHumanPcAB[12];
extern const char pbFileHumanPcxxxTB[15];
extern const char pbFileHumanPcxxxMB[15];
extern const char pbFileHumanPcxxxAB[15];

dds3ModelFileTable_t mdlBuiltinHumanPaths[2] = {
    { MDL_BUILTIN_PATH(pbFileHumanPcTB), MDL_BUILTIN_PATH(pbFileHumanPcMB),
      MDL_BUILTIN_PATH(pbFileHumanPcAB) },
    { MDL_BUILTIN_PATH(pbFileHumanPcxxxTB), MDL_BUILTIN_PATH(pbFileHumanPcxxxMB),
      MDL_BUILTIN_PATH(pbFileHumanPcxxxAB) },
};
