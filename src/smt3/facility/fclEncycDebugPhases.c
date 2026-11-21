#include "facility.h"

/* The native encyclopedia debug callbacks share this work layout. */
typedef struct {
    void *allocation;
    s16 state;
    s16 selection;
    s16 firstId;
    s16 row;
} fclEncycDebugWork_t;

typedef s32 (*fclEncycDebugStep_t)(fclEncyc_instance_tag *,
                                 fclEncycDebugWork_t *, sdfDrawTag_t *);

extern s32 DebugProcMenu_fclEncyc(fclEncyc_instance_tag *, fclEncycDebugWork_t *, sdfDrawTag_t *);
extern s32 DebugProcRegister(fclEncyc_instance_tag *, fclEncycDebugWork_t *, sdfDrawTag_t *);
extern s32 DebugProcViewkey(fclEncyc_instance_tag *, fclEncycDebugWork_t *, sdfDrawTag_t *);
extern s32 DebugProcDispBound(fclEncyc_instance_tag *, fclEncycDebugWork_t *, sdfDrawTag_t *);

/* Debug states 0 through 3 are selected by the debug menu. */
fclEncycDebugStep_t fclEncycDebugStepTable[4] = {
    DebugProcMenu_fclEncyc,
    DebugProcRegister,
    DebugProcViewkey,
    DebugProcDispBound,
};
