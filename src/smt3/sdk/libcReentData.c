#include "sdk_reent.h"

SIZE_CHECK(sdkFileSize, FILE, 0x58);
SIZE_CHECK(sdkReentSize, struct _reent, 0x2F0);

extern const char D_0054B658[8]; /* Initial C locale. */

struct _reent impure_data = {
    ._stdin = &impure_data.__sf[0],
    ._stdout = &impure_data.__sf[1],
    ._stderr = &impure_data.__sf[2],
    ._current_locale = D_0054B658,
    ._rand_next = 1,
};

struct _reent *_impure_ptr = &impure_data;
