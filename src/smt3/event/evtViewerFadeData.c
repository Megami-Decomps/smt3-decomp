#include "common.h"

extern char evtViewerStrNormal[7];
extern char evtViewerStrAdd[4];
extern char evtViewerStrSub[4];
extern const char evtFrameFadeWhiteIn[16];
extern const char evtFrameFadeWhiteOut[16];
extern const char evtFrameFadeBlackIn[16];
extern const char evtFrameFadeBlackOut[16];
extern char evtFrameFadeNamePlay[5];
extern char evtFrameFadeNameFadeIn[7];
extern char evtFrameFadeNameFadeOut[8];
extern char evtFrameFadeNameTrans[6];
extern char evtFrameFadeNameVolDown[8];
extern char evtFrameFadeNameVolUp[6];
extern char evtFrameFadeNameAllStop[8];

/* The viewer stores these in writable pointer tables but only reads the
 * labels. Preserve const qualification on the original backing strings. */

char *evtViewerTransModeNames[4] __attribute__((section(".data"), aligned(8))) = {
    evtViewerStrNormal,
    evtViewerStrAdd,
    evtViewerStrSub,
    NULL,
};

char *evtFrameFadeNames[11] __attribute__((section(".data"), aligned(8))) = {
    (char *)evtFrameFadeWhiteIn,
    (char *)evtFrameFadeWhiteOut,
    (char *)evtFrameFadeBlackIn,
    (char *)evtFrameFadeBlackOut,
    evtFrameFadeNamePlay,
    evtFrameFadeNameFadeIn,
    evtFrameFadeNameFadeOut,
    evtFrameFadeNameTrans,
    evtFrameFadeNameVolDown,
    evtFrameFadeNameVolUp,
    evtFrameFadeNameAllStop,
};
