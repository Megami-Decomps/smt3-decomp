#include "common.h"

typedef s32 (*evtViewerAction_t)(s32 x, s32 y);

s32 evtMainMenu(s32 x, s32 y);
s32 evtSelectMenu(s32 x, s32 y);
s32 evtFileNameMenu(s32 x, s32 y);
s32 evtOjbectListMenu(s32 x, s32 y);
s32 evtFrameListMenu(s32 x, s32 y);
s32 evtFrameChangeMenu(s32 x, s32 y);
s32 evtIntChangeMenu(s32 x, s32 y);
s32 evtFloatChangeMenu(s32 x, s32 y);
s32 evtFileName2Menu(s32 x, s32 y);
s32 evtBasicListMenu(s32 x, s32 y);
s32 evtPollEffectFrameControl(s32 x, s32 y);
s32 evtEventPosMenu(s32 x, s32 y);

/* Indexed by the same menu modes as evtViewerMenuHandlers. */
evtViewerAction_t evtViewerActionTable[13] = {
    NULL,
    evtMainMenu,
    evtSelectMenu,
    evtFileNameMenu,
    evtOjbectListMenu,
    evtFrameListMenu,
    evtFrameChangeMenu,
    evtIntChangeMenu,
    evtFloatChangeMenu,
    evtFileName2Menu,
    evtBasicListMenu,
    evtPollEffectFrameControl,
    evtEventPosMenu,
};
