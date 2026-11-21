#include "common.h"

/* The private-question selector rolls across four weights per profile. */
u8 nbQuestionWeightTable[12][4] = {
    { 30, 23, 23, 24 },
    { 25, 25, 25, 25 },
    { 25, 25, 25, 25 },
    { 25, 25, 25, 25 },
    { 25, 25, 25, 25 },
    { 25, 25, 25, 25 },
    { 25, 25, 25, 25 },
    { 25, 25, 25, 25 },
    { 25, 25, 25, 25 },
    { 25, 25, 25, 25 },
    { 25, 25, 25, 25 },
    { 0, 0, 0, 0 },
};
