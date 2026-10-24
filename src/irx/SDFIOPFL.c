#include "iop.h"

/* SDFIOPFL.IRX: exports a four call file library to other IOP modules and
 * serves each call by an RPC to the EE. See docs/sdf-irx-modules.md. */

/* The strings stay in asm/irx/SDFIOPFL/rodata.s so .rodata keeps its layout. */
extern const char fmt[];                /* "sdfIopFileReadOpen : fail!\n" */
extern const char aSdfiopfilegetf[];    /* "sdfIopFileGetFileSize : fail!\n" */
extern const char aSdfiopfileread_0[];  /* "sdfIopFileRead : fail!\n" */
extern const char aSdfiopfileclos[];    /* "sdfIopFileClose : fail!\n" */
extern const char aSdfiopflIrxReg[];    /* "sdfiopfl.irx : RegisterLibraryEntries fail\n" */
extern const char aSdfiopflIrxSce[];    /* "sdfiopfl.irx : sceSifBindRpc error\n" */

extern void *sdfIopFileLibEntries;
extern SifRpcClientData_t sdfIopFileRpcClient;
extern u8 sdfIopFileSendBuf[0x100];
extern s32 sdfIopFileRecvBuf[0x40];
extern s32 sdfIopFileSema;

void sdfIopFilePrintf(const char *fmt, ...);
s32 sdfIopFileReadOpen(const char *path);
s32 sdfIopFileGetFileSize(s32 fd);
s32 sdfIopFileRead(s32 fd, void *buf, s32 len);
void sdfIopFileClose(s32 fd);

void sdfIopFilePrintf(const char *format, ...)
{
    char buf[0x400];
    va_list ap;

    va_start(ap, format);
    sysclib_vsprintf(buf, format, ap);
    stdio_printf(buf);
}

/* The path goes out zero padded to a word boundary; the reply is the EE side
 * file handle. A failed call leaves the semaphore held. */
s32 sdfIopFileReadOpen(const char *path)
{
    s32 len;
    s32 size;
    s32 fd;

    thsemap_WaitSema(sdfIopFileSema);
    len = sysclib_strlen(path);
    sysclib_memcpy(sdfIopFileSendBuf, path, len);
    size = (len + 4) & ~3;
    sysclib_memset(&sdfIopFileSendBuf[len], 0, size - len);
    if (sifcmd_SifCallRpc(&sdfIopFileRpcClient, 0x20, 0, sdfIopFileSendBuf, size,
                          sdfIopFileRecvBuf, 4, NULL, NULL) != 0) {
        sdfIopFilePrintf(fmt);
        return 0;
    }
    fd = sdfIopFileRecvBuf[0];
    thsemap_SignalSema(sdfIopFileSema);
    return fd;
}

s32 sdfIopFileGetFileSize(s32 fd)
{
    s32 *req;
    s32 size;

    thsemap_WaitSema(sdfIopFileSema);
    req = (s32 *)sdfIopFileSendBuf;
    req[0] = fd;
    if (sifcmd_SifCallRpc(&sdfIopFileRpcClient, 0x21, 0, req, 0xc,
                          sdfIopFileRecvBuf, 4, NULL, NULL) != 0) {
        sdfIopFilePrintf(aSdfiopfilegetf);
        return 0;
    }
    size = sdfIopFileRecvBuf[0];
    thsemap_SignalSema(sdfIopFileSema);
    return size;
}

s32 sdfIopFileRead(s32 fd, void *buf, s32 len)
{
    s32 *req;
    s32 read;

    thsemap_WaitSema(sdfIopFileSema);
    req = (s32 *)sdfIopFileSendBuf;
    req[0] = fd;
    req[1] = (s32)buf;
    req[2] = len;
    if (sifcmd_SifCallRpc(&sdfIopFileRpcClient, 0x22, 0, req, 0xc,
                          sdfIopFileRecvBuf, 4, NULL, NULL) != 0) {
        sdfIopFilePrintf(aSdfiopfileread_0);
        return 0;
    }
    read = sdfIopFileRecvBuf[0];
    thsemap_SignalSema(sdfIopFileSema);
    return read;
}

void sdfIopFileClose(s32 fd)
{
    s32 *req;

    thsemap_WaitSema(sdfIopFileSema);
    req = (s32 *)sdfIopFileSendBuf;
    req[0] = fd;
    if (sifcmd_SifCallRpc(&sdfIopFileRpcClient, 0x23, 0, req, 0xc,
                          NULL, 0, NULL, NULL) != 0) {
        sdfIopFilePrintf(aSdfiopfileclos);
    }
    thsemap_SignalSema(sdfIopFileSema);
}

s32 start(s32 argc, char **argv)
{
    iop_sema_t sema;
    s32 i;

    if (sifman_SifSetIOPrcvaddr() == 0) {
        sifman_SifSIF2Init();
    }
    sema.attr = 0;
    sema.initial = 1;
    sema.max = 0x80;
    sema.option = 0;
    sdfIopFileSema = thsemap_CreateSema(&sema);
    if (loadcore_RegisterLibraryEntries(&sdfIopFileLibEntries) != 0) {
        sdfIopFilePrintf(aSdfiopflIrxReg);
        return 1;
    }
    while (1) {
        if (sifcmd_SifBindRpc(&sdfIopFileRpcClient, 0x66504F49, 0) != 0) {
            sdfIopFilePrintf(aSdfiopflIrxSce);
            return 1;
        }
        if (sdfIopFileRpcClient.server != NULL) {
            break;
        }
        i = 0x10000;
        do {
        } while (--i != 0);
    }
    return 0;
}

INCLUDE_ASM("SDFIOPFL", D_00000414);
INCLUDE_ASM("SDFIOPFL", sdfIopFileLibStub);
INCLUDE_ASM("SDFIOPFL", D_00000460);
INCLUDE_ASM("SDFIOPFL", loadcore_RegisterLibraryEntries);
INCLUDE_ASM("SDFIOPFL", D_0000047C);
INCLUDE_ASM("SDFIOPFL", sifcmd_SifBindRpc);
INCLUDE_ASM("SDFIOPFL", sifcmd_SifCallRpc);
INCLUDE_ASM("SDFIOPFL", D_000004A8);
INCLUDE_ASM("SDFIOPFL", sifman_SifSIF2Init);
INCLUDE_ASM("SDFIOPFL", sifman_SifSetIOPrcvaddr);
INCLUDE_ASM("SDFIOPFL", D_000004D4);
INCLUDE_ASM("SDFIOPFL", stdio_printf);
INCLUDE_ASM("SDFIOPFL", D_000004F8);
INCLUDE_ASM("SDFIOPFL", sysclib_memcpy);
INCLUDE_ASM("SDFIOPFL", sysclib_memset);
INCLUDE_ASM("SDFIOPFL", sysclib_strlen);
INCLUDE_ASM("SDFIOPFL", sysclib_vsprintf);
INCLUDE_ASM("SDFIOPFL", D_00000534);
INCLUDE_ASM("SDFIOPFL", thsemap_CreateSema);
INCLUDE_ASM("SDFIOPFL", thsemap_SignalSema);
INCLUDE_ASM("SDFIOPFL", thsemap_WaitSema);
INCLUDE_ASM("SDFIOPFL", D_00000568);
