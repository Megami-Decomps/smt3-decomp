#include "iop.h"

/* MODMSIN.IRX, Sony's MIDI message input module: the queue
 * sceMSIn_PutMsg writes into, which the synthesiser drains.
 * See docs/irx-modules.md.
 *
 * Every chunk is still assembly: the module has no C yet, and the names are
 * the addresses until the IDB has them. */

INCLUDE_ASM("MODMSIN", modmsin_e10);
INCLUDE_ASM("MODMSIN", D_00000044);
INCLUDE_ASM("MODMSIN", modmsin_sceMSIn_Init);
INCLUDE_ASM("MODMSIN", modmsin_e5);
INCLUDE_ASM("MODMSIN", modmsin_e6);
INCLUDE_ASM("MODMSIN", msinBufferAppend);
INCLUDE_ASM("MODMSIN", modmsin_e7);
INCLUDE_ASM("MODMSIN", modmsin_e8);
INCLUDE_ASM("MODMSIN", modmsin_e9);
INCLUDE_ASM("MODMSIN", D_00000300);
INCLUDE_ASM("MODMSIN", modmsin_e3);
INCLUDE_ASM("MODMSIN", D_00000348);
INCLUDE_ASM("MODMSIN", intrman_CpuSuspendIntr);
INCLUDE_ASM("MODMSIN", intrman_CpuResumeIntr);
INCLUDE_ASM("MODMSIN", D_00000374);
INCLUDE_ASM("MODMSIN", loadcore_ReleaseLibraryEntries);
INCLUDE_ASM("MODMSIN", D_00000398);
