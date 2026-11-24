#include "sdk_libmrpc_layout.h"

/* The IOP file service retains its bound M-RPC client between calls. */
sceSifMClientData sdfIopRpcClient __attribute__((section(".bss"), aligned(16)));
