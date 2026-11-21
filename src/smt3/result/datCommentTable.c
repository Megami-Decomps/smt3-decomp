#include "datCommentTable.h"

/* datFindCommentEntry scans exactly one initialized record. */
datCommentEntry_t datCommentTable[1] __attribute__((section(".data"))) = {
    { 0, 0, 0 },
};
