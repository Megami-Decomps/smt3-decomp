#include "common.h"

extern char D_00550EA8[8];
extern const char cmpTextMarogareh[16];
extern const char cmpTextWadatsumi[16];
extern char D_005507C8[8];
extern const char cmpTextIyomante[16];
extern const char cmpTextShiranui[16];
extern char D_005507C0[8];
extern char D_005507B8[8];
extern const char cmpTextNarukami[16];
extern const char cmpTextAnathema[16];
extern char D_005507B0[8];
extern char D_005507A8[8];
extern const char cmpTextMurakumo[16];
extern char D_005507A0[8];
extern char D_00550798[8];
extern char D_00550790[8];
extern char D_00550788[8];
extern const char cmpTextKamurogi[16];
extern char D_00550780[8];
extern char D_00550778[8];
extern char D_00550770[8];
extern char D_00550768[8];
extern char D_00550760[8];
extern char D_00550758[8];
extern char D_00550750[8];
extern const char cmpTextMasakados[16];
extern const char D_005430A8[24];

/* The established table ABI uses char pointers. These borrows keep the original
 * string storage and are only read by the message/front-end consumers. */
#define READONLY_TEXT(name) ((char *)(name))

/* Magatama display names in native index order. */
char *pstr[30] = {
    D_00550EA8, READONLY_TEXT(cmpTextMarogareh), READONLY_TEXT(cmpTextWadatsumi),
    D_005507C8, READONLY_TEXT(cmpTextIyomante), READONLY_TEXT(cmpTextShiranui),
    D_005507C0, D_005507B8, READONLY_TEXT(cmpTextNarukami),
    READONLY_TEXT(cmpTextAnathema), D_005507B0, D_005507A8,
    READONLY_TEXT(cmpTextMurakumo), D_005507A0, D_00550798,
    D_00550790, D_00550788, READONLY_TEXT(cmpTextKamurogi),
    D_00550780, D_00550778, D_00550770,
    D_00550768, D_00550760, D_00550758,
    D_00550750, READONLY_TEXT(cmpTextMasakados), READONLY_TEXT(D_005430A8),
    READONLY_TEXT(D_005430A8), READONLY_TEXT(D_005430A8), READONLY_TEXT(D_005430A8),
};
