#include "pbBoardData.h"

/* Puzzle Boy's saved 20-by-20 board starts empty in file-backed storage. */
pbMapBlock_t pbMapSnapshot __attribute__((section(".data"))) = { { { 0 } } };
