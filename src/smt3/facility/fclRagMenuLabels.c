#include "common.h"
#include "fclRag.h"

extern const char ragTradeItemsText[16];
extern const char ragTradeElementalsText[24];
extern const char ragTradeMitamaText[24];
extern char ragMenuTextTalk[5];
extern char ragMenuTextLeave[6];

/* fclRagUpdateInit passes exactly these five entries to the cursor. */
const char *ragMenuText[5] = {
    ragTradeItemsText,
    ragTradeElementalsText,
    ragTradeMitamaText,
    ragMenuTextTalk,
    ragMenuTextLeave,
};
