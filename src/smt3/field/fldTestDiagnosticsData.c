#include "basic.h"

/* Field-event diagnostics and the field tester's following small data. */
char fldDataNgMsg[8] = "DataNG\n";
char fldMsg1KO[8] = "1KO\n";
const char fldMsgRe1KO[8] = "Re1KO\n";

s32 fldRunDanteSe_asise = 0;

SDATA_PAD(4);

char fldMsg2A[8] = "2-A\n";

dds3Basic_t *fldUnitControlHandle = NULL;
u32 fldPlayerUnit = 0;
dds3Basic_t *fldDanteUnit = NULL;
