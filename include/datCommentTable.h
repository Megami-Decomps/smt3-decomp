#ifndef DAT_COMMENT_TABLE_H
#define DAT_COMMENT_TABLE_H

#include "sdf_types.h"

typedef struct
{
    /* 0x00 */ u16 id;
    /* 0x02 */ s8 comment;
    /* 0x03 */ u8 unk_03;
} datCommentEntry_t;

extern datCommentEntry_t datCommentTable[];

#endif
