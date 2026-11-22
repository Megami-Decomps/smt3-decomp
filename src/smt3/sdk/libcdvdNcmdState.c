#include "sdk_cdvd_ncmd_state.h"

SIZE_CHECK(cdvdNcmdStateNativeSize, cdvdNcmdState_t, 0xC);
SIZE_CHECK(cdvdScmdStateNativeSize, cdvdScmdState_t, 0x18);
SIZE_CHECK(cdvdCallbackReasonsNativeSize, cdvdCallbackReasons_t, 0x8);
SIZE_CHECK(cdvdStartupRecordsNativeSize, cdvdStartupRecords_t, 0x2C);

/* The initialization words start at -1. The callback reasons and the
 * S-command invocation count start at zero. */
cdvdStartupRecords_t cdvdNcmdState = {
    { 0, 0, -1 },
    { -1, -1, -1, -1, -1, 0 },
    { 0, 0 },
};
