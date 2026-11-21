/* The eight names selected by scePadStateIntToStr. Undefined state values
 * within the native table use libpad's existing pooled empty string. */
extern const char padStateNameDisconnect[16];
extern const char D_0054AA80[];
extern const char padStateNameFindCtp1[16];
extern const char padStateNameExecCmd[8];
extern const char padStateNameStable[8];
extern const char padStateNameError[8];

const char *padStateNames[8] = {
    padStateNameDisconnect,
    D_0054AA80,
    padStateNameFindCtp1,
    D_0054AA80,
    D_0054AA80,
    padStateNameExecCmd,
    padStateNameStable,
    padStateNameError,
};
