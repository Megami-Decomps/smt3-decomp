#include "common.h"

#include "sdk_libmrpc_layout.h"

#include "sdk_libc.h"

extern s32 sceSifLoadModule(const char *a, s32 b, const char *c);
extern void sceSifMInitRpc(s32 mode);
extern s32 sdfGetRunTimeCountBase(void);
extern s32 sdfGetRunTimeCount(s32 aBaseTime);

extern sceSifMClientData sdfIopRpcClient;

void SdfIopInit(const char *driverPath, const char *modulePath)
{
    s32 startTicks;

    if (modulePath != NULL) {
        while (sceSifLoadModule(modulePath, 0, NULL) < 0) {
            printf("%s can't load.\n", modulePath);
        }
    }
    while (sceSifLoadModule(driverPath, 0, NULL) < 0) {
        printf("%s can't load.\n", driverPath);
    }
    if (modulePath != NULL) {
        sceSifMInitRpc(0);
    }
    while (1) {
        sceSifMBindRpc(&sdfIopRpcClient, 0x6F496453, 0);
        if (sdfIopRpcClient.server != NULL) {
            break;
        }
        startTicks = sdfGetRunTimeCountBase();
        while (sdfGetRunTimeCount(startTicks) < 0x1ED2) {
        }
    }
}

