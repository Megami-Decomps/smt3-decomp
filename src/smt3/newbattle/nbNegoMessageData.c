#include "nbNegoMessageData.h"

extern struct itfMesResource_t itfMesEmbedded_3FB2F0;
extern struct itfMesResource_t itfMesEmbedded_403388;
extern struct itfMesResource_t itfMesEmbedded_405038;
extern struct itfMesResource_t itfMesEmbedded_4065B8;
extern struct itfMesResource_t itfMesEmbedded_40A840;
extern struct itfMesResource_t itfMesEmbedded_412630;
extern struct itfMesResource_t itfMesEmbedded_414418;
extern struct itfMesResource_t itfMesEmbedded_4152F0;
extern struct itfMesResource_t itfMesEmbedded_419458;
extern struct itfMesResource_t itfMesEmbedded_420410;
extern struct itfMesResource_t itfMesEmbedded_4220A8;
extern struct itfMesResource_t itfMesEmbedded_423540;
extern struct itfMesResource_t itfMesEmbedded_427D68;
extern struct itfMesResource_t itfMesEmbedded_42F080;
extern struct itfMesResource_t itfMesEmbedded_430CA8;
extern struct itfMesResource_t itfMesEmbedded_4321F8;
extern struct itfMesResource_t itfMesEmbedded_436750;
extern struct itfMesResource_t itfMesEmbedded_43E5C0;
extern struct itfMesResource_t itfMesEmbedded_4405A0;
extern struct itfMesResource_t itfMesEmbedded_441D18;
extern struct itfMesResource_t itfMesEmbedded_447F80;
extern struct itfMesResource_t itfMesEmbedded_44F2C8;
extern struct itfMesResource_t itfMesEmbedded_451130;
extern struct itfMesResource_t itfMesEmbedded_452490;
extern struct itfMesResource_t itfMesEmbedded_456718;
extern struct itfMesResource_t itfMesEmbedded_45DFB8;
extern struct itfMesResource_t itfMesEmbedded_45FF20;
extern struct itfMesResource_t itfMesEmbedded_461438;
extern struct itfMesResource_t itfMesEmbedded_4666B0;
extern struct itfMesResource_t itfMesEmbedded_46E318;
extern struct itfMesResource_t itfMesEmbedded_4702D8;
extern struct itfMesResource_t itfMesEmbedded_471780;
extern struct itfMesResource_t itfMesEmbedded_476188;
extern struct itfMesResource_t itfMesEmbedded_47D868;
extern struct itfMesResource_t itfMesEmbedded_47F928;
extern struct itfMesResource_t itfMesEmbedded_480E28;
extern struct itfMesResource_t itfMesEmbedded_4851A0;
extern struct itfMesResource_t itfMesEmbedded_48D428;
extern struct itfMesResource_t itfMesEmbedded_48F438;
extern struct itfMesResource_t itfMesEmbedded_490AE8;
extern struct itfMesResource_t itfMesEmbedded_494D70;
extern struct itfMesResource_t itfMesEmbedded_49CD88;
extern struct itfMesResource_t itfMesEmbedded_49ECF8;
extern struct itfMesResource_t itfMesEmbedded_4A03B0;
extern struct itfMesResource_t itfMesEmbedded_4A4368;
extern struct itfMesResource_t itfMesEmbedded_4AB4F0;
extern struct itfMesResource_t itfMesEmbedded_4AD458;
extern struct itfMesResource_t itfMesEmbedded_4AE180;
extern struct itfMesResource_t itfMesEmbedded_4AF810;
extern struct itfMesResource_t itfMesEmbedded_4B7110;
extern struct itfMesResource_t itfMesEmbedded_4B8DC0;
extern struct itfMesResource_t itfMesEmbedded_4B9960;
extern struct itfMesResource_t itfMesEmbedded_4BAFC8;
extern struct itfMesResource_t itfMesEmbedded_4BF9D0;
extern struct itfMesResource_t itfMesEmbedded_4C0C30;
extern struct itfMesResource_t itfMesEmbedded_4C1618;
extern struct itfMesResource_t itfMesEmbedded_4C2CA8;
extern struct itfMesResource_t itfMesEmbedded_4C9D60;
extern struct itfMesResource_t itfMesEmbedded_4CBA10;
extern struct itfMesResource_t itfMesEmbedded_4CC4A8;
extern struct itfMesResource_t itfMesEmbedded_4CDAD8;
extern struct itfMesResource_t itfMesEmbedded_4D4FE0;
extern struct itfMesResource_t itfMesEmbedded_4D6C90;
extern struct itfMesResource_t itfMesEmbedded_4D7758;
extern struct itfMesResource_t itfMesEmbedded_4D8DE8;
extern struct itfMesResource_t itfMesEmbedded_4DF880;
extern struct itfMesResource_t itfMesEmbedded_4E1480;
extern struct itfMesResource_t itfMesEmbedded_4E2710;

/* Negotiation voices index four independent message banks. Several voice
 * slots deliberately reuse the same native resources. */
nbNegoMsgEntry_t negoMsgTable[31] = {
    /*  0 */ { &itfMesEmbedded_419458, &itfMesEmbedded_4220A8, &itfMesEmbedded_420410, &itfMesEmbedded_423540 },
    /*  1 */ { &itfMesEmbedded_427D68, &itfMesEmbedded_430CA8, &itfMesEmbedded_42F080, &itfMesEmbedded_4321F8 },
    /*  2 */ { &itfMesEmbedded_436750, &itfMesEmbedded_4405A0, &itfMesEmbedded_43E5C0, &itfMesEmbedded_441D18 },
    /*  3 */ { &itfMesEmbedded_4666B0, &itfMesEmbedded_4702D8, &itfMesEmbedded_46E318, &itfMesEmbedded_471780 },
    /*  4 */ { &itfMesEmbedded_476188, &itfMesEmbedded_47F928, &itfMesEmbedded_47D868, &itfMesEmbedded_480E28 },
    /*  5 */ { &itfMesEmbedded_40A840, &itfMesEmbedded_414418, &itfMesEmbedded_412630, &itfMesEmbedded_4152F0 },
    /*  6 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /*  7 */ { &itfMesEmbedded_3FB2F0, &itfMesEmbedded_405038, &itfMesEmbedded_403388, &itfMesEmbedded_4065B8 },
    /*  8 */ { &itfMesEmbedded_456718, &itfMesEmbedded_45FF20, &itfMesEmbedded_45DFB8, &itfMesEmbedded_461438 },
    /*  9 */ { &itfMesEmbedded_447F80, &itfMesEmbedded_451130, &itfMesEmbedded_44F2C8, &itfMesEmbedded_452490 },
    /* 10 */ { &itfMesEmbedded_4851A0, &itfMesEmbedded_48F438, &itfMesEmbedded_48D428, &itfMesEmbedded_490AE8 },
    /* 11 */ { &itfMesEmbedded_4A4368, &itfMesEmbedded_4AD458, &itfMesEmbedded_4AB4F0, &itfMesEmbedded_4AE180 },
    /* 12 */ { &itfMesEmbedded_4AF810, &itfMesEmbedded_4B8DC0, &itfMesEmbedded_4B7110, &itfMesEmbedded_4B9960 },
    /* 13 */ { &itfMesEmbedded_4BAFC8, &itfMesEmbedded_4C0C30, &itfMesEmbedded_4BF9D0, &itfMesEmbedded_4C1618 },
    /* 14 */ { &itfMesEmbedded_4C2CA8, &itfMesEmbedded_4CBA10, &itfMesEmbedded_4C9D60, &itfMesEmbedded_4CC4A8 },
    /* 15 */ { &itfMesEmbedded_4CDAD8, &itfMesEmbedded_4D6C90, &itfMesEmbedded_4D4FE0, &itfMesEmbedded_4D7758 },
    /* 16 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 17 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 18 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 19 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 20 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 21 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 22 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 23 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 24 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 25 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 26 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 27 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 28 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 29 */ { &itfMesEmbedded_494D70, &itfMesEmbedded_49ECF8, &itfMesEmbedded_49CD88, &itfMesEmbedded_4A03B0 },
    /* 30 */ { &itfMesEmbedded_4D8DE8, &itfMesEmbedded_4E1480, &itfMesEmbedded_4DF880, &itfMesEmbedded_4E2710 },
};
