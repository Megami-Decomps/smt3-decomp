#ifndef PIPE_ENEMY_DATA_H
#define PIPE_ENEMY_DATA_H

#include "common.h"

typedef struct tagPipeEnemyInfo {
    u8 chID;
    f32 w;
    u8 resv;
    u16 resv2;
} tagPipeEnemyInfo;

extern tagPipeEnemyInfo pipeEnemyInfoTable[11];

#endif
