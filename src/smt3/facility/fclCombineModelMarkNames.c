#include "fclCombineMarkerNames.h"

extern const char cmbAkumaposName01[16];
extern const char cmbAkumaposName02[16];
extern const char cmbAkumaposName03[16];
extern const char cmbAkumaposName04[16];

/* The fourth adjacent pointer is for the initial camera marker. */
cmbMarkerNames_t cmbModelMarkNames = {
    { cmbAkumaposName01, cmbAkumaposName02, cmbAkumaposName03 },
    { cmbAkumaposName04 },
};
