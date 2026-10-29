#ifndef SMT3_ENCOUNT_H
#define SMT3_ENCOUNT_H

#include "common.h"

/* The encounter tables: each field's packs by area (fldReadyScene), the
 * packs the encounter roll draws from (nbEncountTable, nbEncountCalc), and
 * the formations their rows name (nbEncountFormTable). */

/* The fields fldEncounterFloorTable holds, from field 0: fldReadyScene reads
 * it for a field below this alone. */
#define FLD_ENCOUNTER_FIELDS 46

/* A field's encounter packs by area: the normal ones, or the alternate ones
 * once its event flag is set (fldReadyScene, fldGlobalBase.defaultEncounterId). */
typedef struct fldEncounterFloorTable_t {
    s32 eventFlag;
    s16 normal[32];
    s16 alternate[32];
} fldEncounterFloorTable_t;

/* A pack table's row: a formation and its weight out of 1000. */
typedef struct datEncountRitu_t {
    u16 encountid;
    u16 ritu;
    u8 sensei;
    u8 renzokuritu;
    u16 renzokuindex;
} datEncountRitu_t;

typedef struct datEncountTbl_t {
    s32 normalritu;
    datEncountRitu_t ritu[12];
} datEncountTbl_t;

/* An encounter pack: its battle stage, its music, what chooses one of its
 * four tables (nbGetEncountJoken: the moon's age, two event bits) and the
 * tables. */
typedef struct nbEncountEntry_t {
    u16 stagemajor;
    u16 stageminor;
    u16 btlsound;
    u16 moonchk;
    u16 table2;
    u16 eventbit1;
    u16 eventbit2;
    u8 table1[4];
    u8 reserved12[2];
    datEncountTbl_t enctable[4];
} nbEncountEntry_t;

/* A formation: its demons (datDevilTable's ids, 0 for none) and its own
 * battle stage (0, 0: the pack's). */
typedef struct nbEncountForm_t {
    s8 backAttack;
    s8 esc;
    u8 item;
    u8 itemcnt;
    u8 formationtype;
    u8 unk_05;
    u16 id[11];
    u16 stagemajor;
    u16 stageminor;
    u16 flag;
    u8 unk_22;
    u8 nakamayobi;
    u16 mae;
} nbEncountForm_t;

#endif
