#include "interface.h"

typedef void itfPanelDrawProc_t(itfpanel_t *panel, sdfDrawTag_t *tag);

extern itfPanelDrawProc_t DrawNormal;
extern itfPanelDrawProc_t DrawMesWin;
extern itfPanelDrawProc_t DrawSelWin;
extern itfPanelDrawProc_t DrawPlain;
extern itfPanelDrawProc_t DrawPera1;
extern itfPanelDrawProc_t DrawLeftFade;
extern itfPanelDrawProc_t DrawMesWin2;
extern itfPanelDrawProc_t DrawSpkWin2;
extern itfPanelDrawProc_t DrawFacility;

/* itfPnlDrawTag selects the draw procedure from the panel kind. */
itfPanelDrawProc_t *DrawTable[9] = {
    DrawNormal, DrawMesWin, DrawSelWin, DrawPlain, DrawPera1,
    DrawLeftFade, DrawMesWin2, DrawSpkWin2, DrawFacility,
};
