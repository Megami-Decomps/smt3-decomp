#include "common.h"

extern char evtViewerStrYes[4];
extern char evtViewerStrNo[3];

char *evtViewerDeleteConfirmNames[2] __attribute__((section(".data"), aligned(8))) = {
    evtViewerStrYes,
    evtViewerStrNo,
};
