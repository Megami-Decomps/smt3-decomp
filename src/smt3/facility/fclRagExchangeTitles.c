#include "common.h"
#include "fclRag.h"

extern const char ragExchangeTitleItem[16];
extern const char ragExchangeTitleSoul[16];
extern const char ragExchangeTitleMitama[16];

/* The item, elemental, and Mitama lists each select one fixed heading. */
const char *ragExchangeTitle[3] = {
    ragExchangeTitleItem,
    ragExchangeTitleSoul,
    ragExchangeTitleMitama,
};
