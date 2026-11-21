#include "common.h"

extern const char shpTmxPath01[32];
extern const char shpTmxPath02[32];
extern char shpCmdBuy[];
extern char shpCmdSell[];
extern char shpCmdTalk[];
extern char shpCmdLeave[];

const char *shpTmxPathTable[2] __attribute__((section(".data"))) = { shpTmxPath01, shpTmxPath02 };

char *shpCommandTable[4] = { shpCmdBuy, shpCmdSell, shpCmdTalk, shpCmdLeave };
