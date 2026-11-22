#ifndef DAT_ITEM_BANK_H
#define DAT_ITEM_BANK_H
#include "datSagasi.h"

typedef struct {
    u8 useFlags;
    u8 actionFlags;
    u16 skill;
    u32 price;
} datItemEntry_t;

typedef struct {
    datItemEntry_t items[144];
    datTakaraSagasi_t sagasi;
} datItemBankRecords_t;

/* The command inventory scan reads the last 16 row positions from the
 * adjacent search data. Keep the native field boundaries and expose the
 * bank's unsigned object representation for those fixed-width reads. */
typedef union {
    datItemBankRecords_t records;
    u16 halfwords[sizeof(datItemBankRecords_t) / sizeof(u16)];
    u32 words[sizeof(datItemBankRecords_t) / sizeof(u32)];
} datItemBank_t;

extern datItemBank_t datItemTable;
#define DAT_ITEM_USE(i)   (((const u8 *)&datItemTable)[(i) * 8])
#define DAT_ITEM_FLAG(i)  (((const u8 *)&datItemTable)[(i) * 8 + 1])
#define DAT_ITEM_SKILL(i) (datItemTable.halfwords[(i) * 4 + 1])
#define DAT_ITEM_PRICE(i) (datItemTable.words[(i) * 2 + 1])
#endif
