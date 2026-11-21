#include "common.h"

extern char sdfDmaTagRefeName[];
extern char sdfDmaTagCntName[];
extern char sdfDmaTagNextName[];
extern char sdfDmaTagRefName[];
extern char sdfDmaTagRefsName[];
extern char sdfDmaTagCallName[];
extern char sdfDmaTagRetName[];
extern char sdfDmaTagEndName[];
extern char sdfVifUnpackS_32Name[];
extern char sdfVifUnpackS_16Name[];
extern char sdfVifUnpackS_8Name[];
extern char sdfVifUnpackV2_32Name[];
extern char sdfVifUnpackV2_16Name[];
extern char sdfVifUnpackV2_8Name[];
extern char sdfVifUnpackV3_32Name[];
extern char sdfVifUnpackV3_16Name[];
extern char sdfVifUnpackV3_8Name[];
extern char sdfVifUnpackV4_32Name[];
extern char sdfVifUnpackV4_16Name[];
extern char sdfVifUnpackV4_8Name[];
extern char sdfVifUnpackV4_5Name[];
extern char sdfVifFlagsNoneName[];
extern char sdfVifFlagsMName[];
extern char sdfVifFlagsUName[];
extern char sdfVifFlagsMuName[];
extern char sdfVifFlagsRName[];
extern char sdfVifFlagsMrName[];
extern char sdfVifFlagsRuName[];
extern char sdfVifFlagsMruName[];
extern char sdfGsMemFreeName[];
extern char sdfGsMemFrameName[];
extern char sdfGsMemTexName[];
extern char sdfGsMemClutName[];

/* The eight DMA tag names precede the sixteen VIF unpack-format names. */
char *sdfDmaTagNameTable[24] = {
    sdfDmaTagRefeName, sdfDmaTagCntName,
    sdfDmaTagNextName, sdfDmaTagRefName,
    sdfDmaTagRefsName, sdfDmaTagCallName,
    sdfDmaTagRetName, sdfDmaTagEndName,
    sdfVifUnpackS_32Name, sdfVifUnpackS_16Name,
    sdfVifUnpackS_8Name, NULL,
    sdfVifUnpackV2_32Name, sdfVifUnpackV2_16Name,
    sdfVifUnpackV2_8Name, NULL,
    sdfVifUnpackV3_32Name, sdfVifUnpackV3_16Name,
    sdfVifUnpackV3_8Name, NULL,
    sdfVifUnpackV4_32Name, sdfVifUnpackV4_16Name,
    sdfVifUnpackV4_8Name, sdfVifUnpackV4_5Name,
};

/* Names for the three VIF unpack flags. */
char *sdfGrVifUnpackNames[8] = {
    sdfVifFlagsNoneName, sdfVifFlagsMName,
    sdfVifFlagsUName, sdfVifFlagsMuName,
    sdfVifFlagsRName, sdfVifFlagsMrName,
    sdfVifFlagsRuName, sdfVifFlagsMruName,
};

/* Bits per unpacked element in native format-index order. */
s32 sdfGrVifUnpackBits[16] = {
    32, 16, 8, 0,
    64, 32, 16, 0,
    96, 48, 24, 0,
    128, 64, 32, 16,
};

/* GS memory type names, separately retained for the two list traversals. */
char *sdfGsMemTypeNames[4] = {
    sdfGsMemFreeName, sdfGsMemFrameName,
    sdfGsMemTexName, sdfGsMemClutName,
};

char *sdfGsMemTypeNamesTail[4] = {
    sdfGsMemFreeName, sdfGsMemFrameName,
    sdfGsMemTexName, sdfGsMemClutName,
};
