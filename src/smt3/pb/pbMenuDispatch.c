#include "pbMenuDispatch.h"

/* Each row follows pbMenuState order for the corresponding panel value. */
pbMenuCheck_t pbMenuCheckTable[2][8] = {
    { func_001D4BC8, func_001D4BC0, func_001D4BC0, func_001D4BC0,
      func_001D4BC8, func_001D4BC8, func_001D4BC0, func_001D4BC0 },
    { func_001D4BE8, func_001D4BC0, func_001D4BC0, func_001D4BC0,
      NULL, NULL, NULL, NULL },
};

pbMenuAction_t pbMenuActionTable[2][8] = {
    { func_001D4C08, pbMakeMapPath, func_001D4E10, pbMakeMapPathHdd,
      func_001D4E08, NULL, pbSetDebugOwner, exitPB },
    { func_001D4F08, func_001D4EB0, pbSetDebugOwner, func_001D4E70,
      NULL, NULL, NULL, NULL },
};
