#ifndef FCL_SHOP_PRICE_DATA_H
#define FCL_SHOP_PRICE_DATA_H

#include "common.h"

typedef struct
{
    u16 id;
    u16 price;
} shpPriceItem_t;

typedef struct
{
    /* 0x00 */ u16 base;
    /* 0x02 */ shpPriceItem_t items[32];
} shpPriceRow_t;

SIZE_CHECK(ShpPriceItemSize, shpPriceItem_t, 4);
SIZE_CHECK(ShpPriceRowSize, shpPriceRow_t, 130);

extern shpPriceRow_t shpPriceTable[8];

#endif
