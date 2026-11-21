#include "mviManager.h"

extern const char stfMovieFile_dds[];
extern const char stfMovieFile_e500_002[];
extern const char stfMovieFile_e616[];
extern const char stfMovieFile_e804[];
extern const char stfMovieFile_e804_f[];
extern const char stfMovieFile_e804_i[];
extern const char stfMovieFile_e658[];
extern const char stfMovieFile_gOver[];
extern const char stfMovieFile_e673_020[];
extern const char stfMovieFile_e674[];
extern const char stfMovieTitle1[];
extern const char stfMovieE617[];
extern const char stfMovieE676[];
extern const char stfMovieMenu[];
extern const char stfMovieTitle2[];
extern const char stfMovieE688[];
extern const char stfMovieE684[];
extern const char stfMovieE686[];
extern const char stfMovieTitle3[];
extern const char stfMovieE727[];
extern const char stfMovieE731[];
extern const char stfMoviePeephole[];
extern const char stfMovieE616M[];
extern const char stfMovieTitle1M[];
extern const char stfMovieTitle2M[];

SIZE_CHECK(dds3MovieEntrySize, dds3MovieEntry_t, 24);
SIZE_CHECK(sdfMovieInitParmSize, sdfMovieInitParm_t, 20);

/* Every indexed movie uses the same native playback parameters. */
#define MOVIE_PLAYBACK_INIT { 28672, 30976, 0x00FF0080, 8192, 3584, 0, 0, 0, 30 }

dds3MovieEntry_t movieEntryTable[25] = {
    { stfMovieFile_dds, MOVIE_PLAYBACK_INIT },
    { stfMovieFile_e500_002, MOVIE_PLAYBACK_INIT },
    { stfMovieFile_e616, MOVIE_PLAYBACK_INIT },
    { stfMovieFile_e804, MOVIE_PLAYBACK_INIT },
    { stfMovieFile_e804_f, MOVIE_PLAYBACK_INIT },
    { stfMovieFile_e804_i, MOVIE_PLAYBACK_INIT },
    { stfMovieFile_e658, MOVIE_PLAYBACK_INIT },
    { stfMovieFile_gOver, MOVIE_PLAYBACK_INIT },
    { stfMovieFile_e673_020, MOVIE_PLAYBACK_INIT },
    { stfMovieFile_e674, MOVIE_PLAYBACK_INIT },
    { stfMovieTitle1, MOVIE_PLAYBACK_INIT },
    { stfMovieE617, MOVIE_PLAYBACK_INIT },
    { stfMovieE676, MOVIE_PLAYBACK_INIT },
    { stfMovieMenu, MOVIE_PLAYBACK_INIT },
    { stfMovieTitle2, MOVIE_PLAYBACK_INIT },
    { stfMovieE688, MOVIE_PLAYBACK_INIT },
    { stfMovieE684, MOVIE_PLAYBACK_INIT },
    { stfMovieE686, MOVIE_PLAYBACK_INIT },
    { stfMovieTitle3, MOVIE_PLAYBACK_INIT },
    { stfMovieE727, MOVIE_PLAYBACK_INIT },
    { stfMovieE731, MOVIE_PLAYBACK_INIT },
    { stfMoviePeephole, MOVIE_PLAYBACK_INIT },
    { stfMovieE616M, MOVIE_PLAYBACK_INIT },
    { stfMovieTitle1M, MOVIE_PLAYBACK_INIT },
    { stfMovieTitle2M, MOVIE_PLAYBACK_INIT },
};
