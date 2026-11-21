extern char rcvHealText[8];
extern const char rcvHealHpMpText[16];
extern const char rcvCureStatusText[16];
extern char rcvReviveText[8];
extern const char rcvRemoveCurseText[16];

/* The other four native label symbols are individual slots in this table. */
const char *rcvHealLabel[5] = {
    rcvHealText,
    rcvHealHpMpText,
    rcvCureStatusText,
    rcvReviveText,
    rcvRemoveCurseText,
};
