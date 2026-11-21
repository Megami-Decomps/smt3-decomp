#include "sdk_cdvd_ncmd_state.h"

SIZE_CHECK(cdvdNcmdStateNativeSize, cdvdNcmdState_t, 0xC);

/* N-command state starts idle; the negative init value requests an RPC bind. */
cdvdNcmdState_t cdvdNcmdState = { 0, 0, -1 };
