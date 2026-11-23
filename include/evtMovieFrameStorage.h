#ifndef EVT_MOVIE_FRAME_STORAGE_H
#define EVT_MOVIE_FRAME_STORAGE_H

#include "effect.h"
#include "event.h"

extern f32 evtSceneLightParam[12];
extern pmdSceneFogTable_t evtSceneFogParam;
extern dds3Effect_Blur_t evtBlur2Param;
extern dds3Effect_PartialMultiBlurParam_t evtMultBlurParam;
extern dds3Effect_DistortionBlurParam_t evtDistBlurParam;
extern dds3Effect_RippleBlurParam_t evtRipBlurParam;
extern dds3Effect_PartialMultiFilterParam_t evtMultFilterParam;
extern dds3Effect_Filter_t evtFilterParam;

#endif
