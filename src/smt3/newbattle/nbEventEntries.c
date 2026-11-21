#include "nbEventEntries.h"

extern s32 nbE901Process_1(struct dds3ProcessID_t *id);
extern s32 nbE902Process_1(struct dds3ProcessID_t *id);
extern s32 nbE903Process_1(struct dds3ProcessID_t *id);
extern s32 nbE904Process_1(struct dds3ProcessID_t *id);
extern s32 nbE905Process_1(struct dds3ProcessID_t *id);
extern s32 nbE906Process_1(struct dds3ProcessID_t *id);
extern s32 nbE907Process_1(struct dds3ProcessID_t *id);
extern s32 nbE908Process_1(struct dds3ProcessID_t *id);
extern s32 nbE909Process_1(struct dds3ProcessID_t *id);
extern s32 nbE910Process_1(struct dds3ProcessID_t *id);
extern s32 nbE911Process_1(struct dds3ProcessID_t *id);
extern s32 nb_295D98(struct dds3ProcessID_t *id);
extern s32 nbE913Process_1(struct dds3ProcessID_t *id);
extern s32 nbE914Process_1(struct dds3ProcessID_t *id);
extern s32 nbE915Process_1(struct dds3ProcessID_t *id);

/* Indexed by the battle event number within the 900-series. */
nbEventEntry_t nbEventEntryTable[16] = {
    /* 900 */ { 0, NULL, 0, 0, 0, 0, 0, 0, 0 },
    /* 901 */ { 901, nbE901Process_1, 20, 0x01E70000, 0, 0, 0, 0, 0 },
    /* 902 */ { 0, nbE902Process_1, 199, 0x01E80000, 0, 0, 0, 0, 0 },
    /* 903 */ { 903, nbE903Process_1, 284, 0x01E90000, 0, 0, 0, 0, 0 },
    /* 904 */ { 904, nbE904Process_1, 333, 0x01EA0000, 0, 20, 20, 20, 20 },
    /* 905 */ { 905, nbE905Process_1, 334, 0x01EB0000, 0, 0, 0, 0, 0 },
    /* 906 */ { 906, nbE906Process_1, 334, 0x01EB0000, 1, 20, 20, 20, 20 },
    /* 907 */ { 907, nbE907Process_1, 472, 0x01ED0000, 0, 0, 0, 0, 0 },
    /* 908 */ { 908, nbE908Process_1, 472, 0x01ED0000, 1, 15, 20, 20, 20 },
    /* 909 */ { 909, nbE909Process_1, 473, 0x01EF0000, 0, 0, 0, 0, 0 },
    /* 910 */ { 0, nbE910Process_1, 473, 0x01EF0000, 1, 20, 20, 20, 0 },
    /* 911 */ { 911, nbE911Process_1, 333, 0x01EA0000, 1, 0, 0, 0, 0 },
    /* 912 */ { 912, nb_295D98, 284, 0x01E90000, 1, 0, 0, 0, 0 },
    /* 913 */ { 913, nbE913Process_1, 14, 0x01F30000, 0, 10, 20, 10, 20 },
    /* 914 */ { 914, nbE914Process_1, 449, 0x01F40000, 0, 10, 20, 10, 20 },
    /* 915 */ { 915, nbE915Process_1, 450, 0x01F50000, 0, 10, 20, 10, 20 },
};
