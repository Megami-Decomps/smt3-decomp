extern char shpBuyText[8];
extern char shpSellText[8];

/* shopSellTitle is the second slot of this writable native pair. */
char *shopBuyTitle[2] __attribute__((section(".data"))) = {
    shpBuyText,
    shpSellText,
};
