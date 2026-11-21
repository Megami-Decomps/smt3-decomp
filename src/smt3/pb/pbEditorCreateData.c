#include "pbMenuDispatch.h"

extern pbProcHandle_t *func_001D4B28(pbProcHandle_t *parent);
extern pbProcHandle_t *pbStartGame(pbProcHandle_t *parent);
extern pbProcHandle_t *func_001DB020(pbProcHandle_t *parent);

/* The dispatcher halts when the current editor state has no constructor. */
pbProcHandle_t *(*pbEditorCreateTable[4])(pbProcHandle_t *parent) = {
    func_001D4B28,
    pbStartGame,
    func_001DB020,
    NULL,
};
