#include "fldEveHitStorage.h"

SIZE_CHECK(FldRoomStateSize, fldRoomState_t, 0x130);

fldRoomState_t fldEveHitTable[FIELD_ROOM_RECORD_COUNT]
    __attribute__((section(".bss"), aligned(16)));
