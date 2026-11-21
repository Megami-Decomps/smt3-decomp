#include "common.h"

typedef s32 (*evtViewerMenuHandler_t)(s32 action, s32 previousMenu);

s32 EnterMain(s32 action, s32 previousMenu);
s32 EnterSelect(s32 action, s32 previousMenu);
s32 EnterObjList(s32 action, s32 previousMenu);
s32 EnterFrameList(s32 action, s32 previousMenu);
s32 EnterFrameChange(s32 action, s32 previousMenu);
s32 EnterIntFloatChange(s32 action, s32 previousMenu);
s32 EnterFilename2(s32 action, s32 previousMenu);
s32 EnterBasicList(s32 action, s32 previousMenu);
s32 EnterData(s32 action, s32 previousMenu);
s32 EnterEventPos(s32 action, s32 previousMenu);

/* Indexed by EVT_VIEWER_MENU, whose last mode is EVENTPOS (12). */
evtViewerMenuHandler_t evtViewerMenuHandlers[13] = {
    NULL,
    EnterMain,
    EnterSelect,
    NULL,
    EnterObjList,
    EnterFrameList,
    EnterFrameChange,
    EnterIntFloatChange,
    EnterIntFloatChange,
    EnterFilename2,
    EnterBasicList,
    EnterData,
    EnterEventPos,
};
