#include "itf_mes_pool.h"

/* Owned runtime pool; itfMesMngInitializeManager initializes the live nodes. */
itfMesManager_t itfMesMngManager __attribute__((section(".bss")));
