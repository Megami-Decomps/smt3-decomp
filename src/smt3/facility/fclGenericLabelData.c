#include "common.h"
typedef struct {
    const char *common[5];
    const char *inventory[5];
} fclGenericLabelData_t;

SIZE_CHECK(fclGenericLabelDataSize, fclGenericLabelData_t, 0x28);

extern char fclStrYes[4], fclStrNo[3], fclStrMoney[6], fclStrTotal[6];
extern char fclStrOwn[4], fclStrFee[4];
extern const char fclGemsNeededText[16];
extern u32 D_0054FC68[2], D_0054FC78[2], D_0054FC80[2];

/* The encoded labels use their existing byte representation. */
fclGenericLabelData_t D_00330900 = {
    .common = { fclStrYes, fclStrNo, fclStrMoney, fclStrTotal, (const char *)D_0054FC80 },
    .inventory = { (const char *)D_0054FC78, fclStrOwn, (const char *)D_0054FC68,
                   fclGemsNeededText, fclStrFee },
};
