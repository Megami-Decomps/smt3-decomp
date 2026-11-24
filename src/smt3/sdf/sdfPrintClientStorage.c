#include "sdk_libmrpc_layout.h"

/* The print service keeps its bound RPC client across requests. */
sceSifMClientData sdfPrintRpcClient __attribute__((section(".bss"), aligned(16)));
