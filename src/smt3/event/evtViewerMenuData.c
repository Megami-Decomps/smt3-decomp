#include "common.h"

extern const char evtFadeNameWhiteIn[9];
extern const char evtFadeNameWhiteOut[10];
extern const char evtFadeNameBlackIn[9];
extern const char evtFadeNameBlackOut[10];
extern char D_0054FA30[8];
extern char D_0054FA28[8];
extern char D_0054FA20[8];
extern char D_0054FA18[8];
extern char D_0054FA10[8];
extern char D_0054FA08[8];
extern char D_0054FA00[8];
extern char D_0054FA38[8];
extern const char evtDispNameOff[9];
extern const char evtDispNameShadowOff[11];
extern const char evtDispNameMShadowOn[11];
extern const char evtDispNameVShadowOn[11];

/* The viewer stores these in writable pointer tables but only reads the
 * labels. Preserve const qualification on the original backing strings. */

s8 evtFileNameChars[4][12] __attribute__((section(".data"), aligned(8))) = {
    "1234567890A",
    "BCDEFGHIJKL",
    "MNOPQRSTUVW",
    "XYZ_[B][OK]",
};

char *evtItemFadeNames[12] __attribute__((section(".data"), aligned(8))) = {
    (char *)evtFadeNameWhiteIn,
    (char *)evtFadeNameWhiteOut,
    (char *)evtFadeNameBlackIn,
    (char *)evtFadeNameBlackOut,
    D_0054FA30,
    D_0054FA28,
    D_0054FA20,
    D_0054FA18,
    D_0054FA10,
    D_0054FA08,
    D_0054FA00,
    NULL,
};

char *evtDispModeNames[6] __attribute__((section(".data"), aligned(8))) = {
    D_0054FA38,
    (char *)evtDispNameOff,
    (char *)evtDispNameShadowOff,
    (char *)evtDispNameMShadowOn,
    (char *)evtDispNameVShadowOn,
    NULL,
};
