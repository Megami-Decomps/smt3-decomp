#ifndef SDK_LIBKERNL_H
#define SDK_LIBKERNL_H

#include "common.h"

struct sifRpcServerData_t;
struct sifRpcDataQueue_t;
typedef void *(*sceSifRpcFunc)(s32 function, void *request, s32 size);
s32 AddDmacHandler2(s32 channel, s32 (*handler)(s32, void *, void *),
                    s32 next, void *argument);
s32 _StartThread(s32 threadId, void *argument);
s32 sceDevctl(const char *device, s32 command, const void *arguments,
              u32 argumentSize, void *result, u32 resultSize);
void sceSifRegisterRpc(struct sifRpcServerData_t *server, u32 serviceId,
                        sceSifRpcFunc handler, void *buffer, sceSifRpcFunc callback,
                        void *callbackBuffer, struct sifRpcDataQueue_t *queue);
s32 sceWrite(s32 descriptor, const void *source, s32 size);

s32 sceOpen(const char *name, s32 flags, ...);

struct sifDmaTransfer_t;
s32 sceSifSetDma(struct sifDmaTransfer_t *transfers, s32 count);

#endif /* SDK_LIBKERNL_H */
