#include "common.h"

/* Module loader argument blocks are NUL-separated and have fixed native extents. */
char sdfHddIrxArgs[16] = "-o\0" "18\0" "-n\0" "32";
char sdfHddPfsIrxArgs[32] = "-m\0" "18\0" "-o\0" "10\0" "-n\0" "64";
