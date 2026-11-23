#include "evtMovieFrameStorage.h"

/* Interpolated scene state written by the movie-frame callbacks. Each
 * parameter starts on its native quadword boundary; the trailing filter ends
 * immediately before the unrelated event-pack script path. */
f32 evtSceneLightParam[12] __attribute__((section(".bss"), aligned(16)));
pmdSceneFogTable_t evtSceneFogParam __attribute__((section(".bss"), aligned(16)));
dds3Effect_Blur_t evtBlur2Param __attribute__((section(".bss"), aligned(16)));
dds3Effect_PartialMultiBlurParam_t evtMultBlurParam __attribute__((section(".bss"), aligned(16)));
dds3Effect_DistortionBlurParam_t evtDistBlurParam __attribute__((section(".bss"), aligned(16)));
dds3Effect_RippleBlurParam_t evtRipBlurParam __attribute__((section(".bss"), aligned(16)));
dds3Effect_PartialMultiFilterParam_t evtMultFilterParam __attribute__((section(".bss"), aligned(16)));
dds3Effect_Filter_t evtFilterParam __attribute__((section(".bss"), aligned(16)));

SIZE_CHECK(evtMovieLightBytes, evtSceneLightParam, 0x30);
SIZE_CHECK(evtMovieFogBytes, pmdSceneFogTable_t, 0x20);
SIZE_CHECK(evtMovieBlurBytes, dds3Effect_Blur_t, 0x28);
SIZE_CHECK(evtMovieMultiBlurBytes, dds3Effect_PartialMultiBlurParam_t, 0x2C);
SIZE_CHECK(evtMovieDistortionBytes, dds3Effect_DistortionBlurParam_t, 0x2C);
SIZE_CHECK(evtMovieRippleBytes, dds3Effect_RippleBlurParam_t, 0x2C);
SIZE_CHECK(evtMovieMultiFilterBytes, dds3Effect_PartialMultiFilterParam_t, 0x24);
SIZE_CHECK(evtMovieFilterBytes, dds3Effect_Filter_t, 0x18);
