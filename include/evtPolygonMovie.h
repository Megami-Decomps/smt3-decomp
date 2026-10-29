#ifndef EVT_POLYGON_MOVIE_H
#define EVT_POLYGON_MOVIE_H

#include "event.h"
#include "effect.h"

typedef struct evtPolygonMovieWork_t {
    u32 movieFlags;
    void *file04;
    sdfMemHandle_t *mem08;
    void *pm1data;
    void *header1;
    pmdEntry_t *entryTable1;
    char (*nameTable1)[32];
    s32 nameCount1;
    pmdStageTable_t *stageTable;
    s32 stageCount;
    void *stageF1;
    void *stageF2;
    void *stageTB;
    pmdUnitTable_t *unitTable;
    s32 unitCount;
    void *type9Data;
    pmdEffectTable_t *effectTable;
    s32 effectCount;
    void *type8Data;
    void *file4C;
    sdfMemHandle_t *mem50;
    void *pm2data;
    pmdHeader_t *header2;
    pmdEntry_t *entryTable2;
    char (*nameTable)[32];
    s32 nameCount2;
    pmdCutInfo_t *cutInfo;
    pmdFrameTable_t *frameTable;
    s32 frameCount;
    void *cameraTable;
    s32 cameraCount;
    pmdSceneLightTable_t *slightTable;
    s32 slightCount;
    pmdSceneFogTable_t *sfogTable;
    s32 sfogCount;
    dds3Effect_Blur_t *blur2Table;
    s32 blur2Count;
    dds3Effect_PartialMultiBlurParam_t *mblurTable;
    s32 mblurCount;
    dds3Effect_DistortionBlurParam_t *dblurTable;
    s32 dblurCount;
    dds3Effect_Filter_t *filterTable;
    s32 filterCount;
    dds3Effect_PartialMultiFilterParam_t *mfilterTable;
    s32 mfilterCount;
    dds3Effect_RippleBlurParam_t *rblurTable;
    s32 rblurCount;
    s32 mesHandle;
    struct dds3ProcessID_t *processId;
    s32 eventNo;
    s32 cutNo;
    s32 nowFrame;
    dds3Basic_t **effectBasicTable;
} evtPolygonMovieWork_t;

#endif
