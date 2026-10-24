#include "iop.h"

/* MODMSIN.IRX, Sony's MIDI message input module: the queue
 * sceMSIn_PutMsg writes into, which the synthesiser drains.
 * See docs/irx-modules.md.
 *
 * Every chunk is still assembly: the module has no C yet, and the names are
 * the addresses until the IDB has them. */

INCLUDE_ASM("MODMSIN", func_00000000);
INCLUDE_ASM("MODMSIN", D_00000044);
INCLUDE_ASM("MODMSIN", func_00000050);
INCLUDE_ASM("MODMSIN", func_000000E0);
INCLUDE_ASM("MODMSIN", func_000000E8);
INCLUDE_ASM("MODMSIN", func_000000F0);
INCLUDE_ASM("MODMSIN", func_00000184);
INCLUDE_ASM("MODMSIN", func_00000238);
INCLUDE_ASM("MODMSIN", func_000002B4);
INCLUDE_ASM("MODMSIN", D_00000300);
INCLUDE_ASM("MODMSIN", func_00000340);
INCLUDE_ASM("MODMSIN", D_00000348);
INCLUDE_ASM("MODMSIN", func_00000364);
INCLUDE_ASM("MODMSIN", func_0000036C);
INCLUDE_ASM("MODMSIN", D_00000374);
INCLUDE_ASM("MODMSIN", func_00000390);
INCLUDE_ASM("MODMSIN", D_00000398);
