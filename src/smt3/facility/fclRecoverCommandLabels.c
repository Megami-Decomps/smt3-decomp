#include "common.h"

extern char rcvCmdHeal[8];
extern const char rcvCmdHealHpMp[11];
extern const char rcvCmdCureStatus[12];
extern char rcvCmdRevive[8];
extern const char rcvCmdRemoveCurse[13];
extern char rcvCmdTalk[8];
extern char rcvCmdLeave[8];

/* The recovery cursor is initialized with seven command names. */
char *rcv_data[7] = {
    rcvCmdHeal, (char *)rcvCmdHealHpMp, (char *)rcvCmdCureStatus,
    rcvCmdRevive, (char *)rcvCmdRemoveCurse, rcvCmdTalk, rcvCmdLeave,
};
