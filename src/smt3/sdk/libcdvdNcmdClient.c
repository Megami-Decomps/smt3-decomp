#include "sdk_cdvd_rpc_client.h"

SIZE_CHECK(cdvdNcmdClientNativeSize, cdvdRpcClient_t, 0x28);

/* The N-command RPC client begins disconnected. */
cdvdRpcClient_t cdvdNcmdClient __attribute__((section(".data"))) = { { 0 }, NULL };
