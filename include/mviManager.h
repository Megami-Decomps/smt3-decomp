#ifndef MVI_MANAGER_H
#define MVI_MANAGER_H

#include "sdf_movie_types.h"

extern sdfMovieCb_t movieCb;

typedef struct
{
    /* 0x00 */ const char *path;
    /* 0x04 */ sdfMovieInitParm_t descriptor;
} dds3MovieEntry_t;

extern dds3MovieEntry_t movieEntryTable[25];

#endif
