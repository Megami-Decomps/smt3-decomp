#include "datSagasi.h"

SIZE_CHECK(datSagasiItemSize, datTakaraSagasiItem_t, 0x4);
SIZE_CHECK(datSagasiGroupSize, datTakaraSagasiTable_t, 0x24);
SIZE_CHECK(datSagasiTableSize, datTakaraSagasi_t, 0x94);

/* Each group's last threshold is the upper bound for the draw. */
datTakaraSagasi_t datSagasiSkillTable = {
    0,
    {
        { 0, { { 1, 30 }, { 4, 50 }, { 10, 65 }, { 98, 75 },
               { 5, 85 }, { 97, 95 }, { 96, 98 }, { 11, 100 } } },
        { 0, { { 3, 20 }, { 1, 40 }, { 6, 60 }, { 4, 70 },
               { 9, 80 }, { 15, 90 }, { 5, 95 }, { 7, 100 } } },
        { 0, { { 3, 20 }, { 1, 40 }, { 6, 60 }, { 4, 70 },
               { 9, 80 }, { 15, 90 }, { 5, 95 }, { 7, 100 } } },
        { 0, { { 3, 20 }, { 1, 40 }, { 6, 60 }, { 4, 70 },
               { 9, 80 }, { 15, 90 }, { 5, 95 }, { 7, 100 } } },
    },
};
