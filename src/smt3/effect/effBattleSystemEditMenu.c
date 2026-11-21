#include "effDebug.h"

extern const char D_00533C10[16];
extern effDebugMenuItem_t D_0031CDF8[14];
extern void *nb_175338(void *value);
extern void nb_175440(void *unused);

/* Read status bits on entry and apply the editor's values on update. */
effDebugMenu_t D_0031CF48 = {
    .title = D_00533C10,
    .x = 16,
    .y = 16,
    .items = D_0031CDF8,
    .itemCount = 14,
    .mode = 1,
    .create = nb_175338,
    .update = nb_175440,
};
