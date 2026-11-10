#include "common.h"
#include "nb.h"
#include "effBattle.h"

extern void nbInitPlayer(void);

extern void nbInitMainProcess(s32 encpackno, s32 packno, s32 packindex, s32 encno,
                              s32 stagemajor, s32 stageminor);

#include "encount.h"

extern nbEncountEntry_t nbEncountTable[];

extern nbEncountForm_t nbEncountFormTable[];

extern void nbKillMainProcess(void);

void nb_246898(void)
{
    nbInitPlayer();
}

void nb_2468B0(void)
{
    nbInitMainProcess(0, 0, 0, 1, 0xC9, 1);
}

void nbExit(void)
{
    nbKillMainProcess();
}

/* The native address calculation keeps row + 0x10 separate from the table
 * base until the formation arm. The byte views below restore the actual row
 * type for its two u16 stage fields; they do not describe a new substructure.
 * These local constraints preserve the real offset and pointer lifetimes in
 * ee-gcc, including the pointer copy between the two loads. They emit no
 * instructions and are a target-compiler aid, not original source evidence. */
void nbCallNewBattle(s32 encpackno, s32 code)
{
    FAKEMATCH_ASM_REG(s32, encno, "$8") = (u16)code;
    /* Compute the native wrapping address as an integer. A nonpositive pack
     * number must not form an out-of-bounds C pointer on the unused arm. */
    u32 packAddress = EE_ADDR(nbEncountTable) + (u32)encpackno * sizeof(nbEncountEntry_t);
    u32 formOffset = encno * sizeof(nbEncountForm_t) + 0x10;
    s32 stagemajor;
    s32 stageminor;
    if (encpackno > 0) {
        nbEncountEntry_t *pack = EE_ADDR_AS(nbEncountEntry_t *, packAddress);
        stagemajor = pack->stagemajor;
        stageminor = pack->stageminor;
    } else {
        FAKEMATCH_ASM_REG(u32, offset, "$11") = formOffset;
        FAKEMATCH_ASM_REG(u8 *, base, "$2") = (u8 *)nbEncountFormTable;
        FAKEMATCH_ASM_REG(u8 *, form, "$3");
        u8 *minor;
        FAKEMATCH_ASM_LIVE_PAIR(offset, base);
        form = base + offset;
        FAKEMATCH_ASM_LIVE(form);
        stagemajor = ((nbEncountForm_t *)(form - 0x10))->stagemajor;
        minor = form;
        FAKEMATCH_ASM_LIVE(minor);
        stageminor = ((nbEncountForm_t *)(minor - 0x10))->stageminor;
        if (stagemajor == 0 && stageminor == 0) {
            stagemajor = 0xC9;
            stageminor = 1;
        }
    }
    nbInitMainProcess(encpackno, (u32)code >> 24, (code >> 16) & 0xFF, encno, stagemajor, stageminor);
}
