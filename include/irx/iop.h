#ifndef IRX_IOP_H
#define IRX_IOP_H

/* The IOP library calls the three modules import. Every module links its own
 * stubs, so a name declared here only resolves in the modules that import it.
 * The stub chunks live in asm/irx/<module>/text/. */

#include "irx.h"

typedef struct {
    u32 attr;
    u32 option;
    void *thread;
    s32 stacksize;
    s32 priority;
    s32 unused;
} iop_thread_t;

typedef struct {
    u32 attr;
    u32 option;
    s32 initial;
    s32 max;
} iop_sema_t;

/* sceSifClientData: the EE side fills in server once the bind completes. */
typedef struct {
    void *pkt_addr;
    u32 rpc_id;
    s32 sema_id;
    u32 mode;
    u32 command;
    void *buff;
    void *cbuff;
    void (*end_function)(void *);
    void *end_param;
    void *server;
} SifRpcClientData_t;


/* ioman */
s32 ioman_open(const char *path, s32 flags, s32 mode);
s32 ioman_close(s32 fd);
s32 ioman_read(s32 fd, void *buf, s32 len);
s32 ioman_lseek(s32 fd, s32 offset, s32 whence);

/* sysmem */
void *sysmem_AllocSysMemory(s32 place, s32 size, void *addr);
s32 sysmem_FreeSysMemory(void *addr);

/* modload */
s32 modload_LoadModuleBuffer(void *buf);
s32 modload_StartModule(s32 mid, const char *path, s32 arglen, const char *args, s32 *result);

/* stdio, sysclib */
s32 stdio_printf(const char *fmt, ...);
s32 sysclib_vsprintf(char *buf, const char *fmt, va_list ap);
void *sysclib_memcpy(void *dst, const void *src, s32 len);
void *sysclib_memset(void *dst, s32 c, s32 len);
s32 sysclib_strlen(const char *s);

/* thbase */
s32 thbase_CreateThread(iop_thread_t *param);
s32 thbase_StartThread(s32 tid, void *arg);
s32 thbase_ExitThread(void);
s32 thbase_SleepThread(void);
s32 thbase_iWakeupThread(s32 tid);
s32 thbase_iCancelWakeupThread(s32 tid);
s32 thbase_DelayThread(u32 usec);
s32 thbase_USec2SysClock(u32 usec, u32 *sysclock);
s32 thbase_e43(void);

/* thsemap */
s32 thsemap_CreateSema(iop_sema_t *param);
s32 thsemap_SignalSema(s32 sid);
s32 thsemap_iSignalSema(s32 sid);
s32 thsemap_WaitSema(s32 sid);

/* sifcmd, sifman */
s32 sifcmd_SifInitRpc(s32 mode);
s32 sifcmd_SifBindRpc(SifRpcClientData_t *client, u32 sid, u32 mode);
s32 sifcmd_SifCallRpc(SifRpcClientData_t *client, u32 fno, u32 mode,
                      void *send, s32 ssize, void *recv, s32 rsize,
                      void (*end_function)(void *), void *end_param);
s32 sifman_SifSIF2Init(void);
s32 sifman_SifSetIOPrcvaddr(void);

/* loadcore */
s32 loadcore_RegisterLibraryEntries(void *lib);
void loadcore_FlushDcache(void);

/* intrman */
s32 intrman_CpuSuspendIntr(s32 *state);
s32 intrman_CpuResumeIntr(s32 state);

#endif /* IRX_IOP_H */
