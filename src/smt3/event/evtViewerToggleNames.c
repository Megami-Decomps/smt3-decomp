#include "common.h"

extern char evtViewerStrOff[4], evtViewerStrOn[3];

/* The frame editor exposes exactly two selections in this order. */
char *evtFrameListTable[2] __attribute__((section(".data"))) = {
    evtViewerStrOff, evtViewerStrOn,
};
