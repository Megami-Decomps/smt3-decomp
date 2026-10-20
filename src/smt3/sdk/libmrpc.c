#include "common.h"

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMInitRpc);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMExitRpc);

INCLUDE_ASM(const s32, "sdk/libmrpc", _sceRpcGetPacket);

INCLUDE_ASM(const s32, "sdk/libmrpc", _sceRpcFreePacket);

INCLUDE_ASM(const s32, "sdk/libmrpc", _request_end);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMBindRpc);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMBindRpcParam);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMUnBindRpc);

INCLUDE_RODATA(const s32, "sdk/libmrpc", D_0054ADD8);

INCLUDE_ASM(const s32, "sdk/libmrpc", sceSifMCallRpc);
