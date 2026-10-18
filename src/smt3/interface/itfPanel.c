#include "interface.h"

/* A color row is also a four-component vector for the template scaler. */
typedef union
{
    u32 components[4];
    struct
    {
        s32 red;
        s32 green;
        s32 blue;
        s32 alpha;
    } rgba;
} itfPanelColor_t;
typedef itfPanelColor_t itfDrawColor_t;
typedef itfPanelVertex_t itfDrawVertex_t;
typedef struct
{
    itfPanelVertex_t vertices[8];
    itfPanelColor_t colors[2];
} itfPanelNormalData_t;
typedef struct
{
    itfPanelVertex_t vertices[40];
    itfPanelColor_t colors[8];
} itfPanelMesWinData_t;
typedef struct
{
    itfPanelVertex_t vertices[32];
    itfPanelColor_t colors[6];
} itfPanelSelWinData_t;
typedef struct
{
    itfPanelVertex_t vertices[4];
    itfPanelColor_t colors[4];
} itfPanelPlainData_t;
typedef struct
{
    itfPanelVertex_t vertices[6];
    itfPanelColor_t colors[2];
} itfPanelLeftFadeData_t;
/* Kind 8 owns a distinct 0x64-byte buffer, independently selected by the
 * native allocation, layout-initializer and color-initializer tables. */
typedef struct itfPanelFacilityData_t {
    sdfTexHandle_t *texture;
    itfDrawVertex_t vertices[8];
    itfDrawColor_t colors[2];
} itfPanelFacilityData_t;
/* Kind 7's allocation is exactly this texture pointer; its factory transports
 * the pointer through the existing u32 value argument. */
typedef struct itfPanelTextureData_t {
    sdfTexHandle_t *texture;
} itfPanelTextureData_t;

void itfPnlRelease(itfPanel_t *sprite);

extern void sdfMemFreeDirect(sdfMemHandle_t *allocation);

void itfPnlSetPosition(itfPanel_t *panel, s32 left, s32 top, s32 right, s32 bottom, s32 renderValue);

void itfPnlSetPositionOffset(itfPanel_t *panel, s32 leftDelta, s32 topDelta, s32 rightDelta, s32 bottomDelta, s32 renderValueDelta);

void itfPnlSetColor(itfPanel_t *panel, s32 redValue, s32 greenValue, s32 blueValue, s32 alphaValue);

void itfPnlDraw(itfPanel_t *panel, sdfGrOt_t *packetOwner);

extern s32 sdfGrAllocPacketBuff(s32 size);

#define ITF_PANEL_PACKET_LIST_BYTES 0x20

extern void sdfDrawTagInit(sdfDrawTag_t *list);

void DrawTestAlphaPrim(sdfDrawTag_t *list);

void itfPnlDrawTag(itfPanel_t *panel, sdfDrawTag_t *list);

void CreateVertexNormal(itfPanelVertex_t *vertices, s32 x0, s32 y0, s32 x1, s32 y1);

#define ITF_PANEL_COLUMN_COUNT 4

#define ITF_PANEL_COLUMN_INSET 0x200

void CreateVertexMesWin(itfPanelVertex_t *vertices, s32 x0, s32 y0, s32 x1, s32 y1);

extern s32 D_00326508[4];

void CreateVertexSelWin(itfPanelVertex_t *vertices, s32 x0, s32 y0, s32 x1, s32 y1);

#define ITF_PANEL_ROW_COUNT 2

void CreateVertexPlain(itfPanelVertex_t *v, s32 x0, s32 y0, s32 x1, s32 y1);

void CreateVertexLeftFade(itfPanelVertex_t *v, s32 x0, s32 y0, s32 x1, s32 y1);

#define ITF_PANEL_CORNER_COLUMN_COUNT 3

#define ITF_PANEL_CORNER_STRIP_WIDTH 0x5A0

void CreateVertexFacility(itfPanelFacilityData_t *base, s32 x0, s32 y0, s32 x1, s32 y1);

extern void itfMiscSetRatioRGB(s32 *output, s32 scaleX, s32 scaleY, s32 scaleZ, s32 w, const s32 *input, s32 count);

extern s32 D_00326518[];

extern s32 D_00326578[];

extern s32 D_003265B8[];

#define ITF_PANEL_FULL_COLOR_SCALE 0x80

extern s32 D_003265C8[];

void CreateColorFacility(itfPanelFacilityData_t *base);

void DrawNormal(itfPanel_t *panel, sdfDrawTag_t *command);

#define ITF_PANEL_WIDE_PASS_COUNT 3

extern void itfMiscDrawRectanglePrim(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command);

extern u8 D_003265F8[];

#define ITF_PANEL_QUAD_INDEX_COUNT 4

extern u8 D_00326608[];

void DrawMesWin(itfPanel_t *panel, sdfDrawTag_t *command);

#define ITF_PANEL_SEVEN_COLOR_COUNT 7

extern void itfMiscDrawTriangleStripPrim(void *vertices, void *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

#define ITF_PANEL_WIDE_INDEX_COUNT 8

#define ITF_PANEL_SEVEN_SIDE_PASS_COUNT 6

extern void itfMiscDrawLineStripPrim(void *, void *, u8 *, u8 *, s32, u32, sdfDrawTag_t *);

extern u8 D_00326618[], D_00326630[], D_00326648[];

void DrawSelWin(itfPanel_t *panel, sdfDrawTag_t *command);

#define ITF_PANEL_FIVE_COLOR_COUNT 5

#define ITF_PANEL_FIVE_SIDE_PASS_COUNT 2

extern u8 D_0054F480[8], D_0054F488[8];

extern u8 D_00326660[], D_00326678[], D_00326690[];

void DrawPlain(itfPanel_t *panel, sdfDrawTag_t *command);

extern u8 D_0054F490[8];

extern u8 D_0054F498[8];

extern u8 D_003266A0[];

extern u8 D_003266B0[];

void DrawPera1(itfPanel_t *panel, sdfDrawTag_t *command);

extern u8 D_0054F4A0[8];

extern u8 D_0054F4A8[8];

void DrawLeftFade(itfPanel_t *panel, sdfDrawTag_t *command);

extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *command, s32 index, s32 flag);

extern u8 D_0054F4B0[8];

extern u8 D_0054F4B8[8];

void DrawSpkWin2(itfPanel_t *panel, sdfDrawTag_t *command);

extern void itfMiscDrawSpritePrim(void *, void *, void *, s32, sdfTexHandle_t *, s32, sdfDrawTag_t *);

/*END itfDrawFramedPanelWithTextureStrips*/
extern itfDrawColor_t D_003266C0;

extern void itfMiscDrawSpritePrim2(itfDrawVertex_t *, itfDrawColor_t *, itfDrawColor_t *, u32, s32, sdfDrawTag_t *);

extern itfDrawColor_t D_003266D0;

void DrawFacility(itfPanel_t *panel, sdfDrawTag_t *command);

extern u8 D_003266E0[];

extern u8 D_003266F0[];

extern itfDrawColor_t D_003264A0;

extern itfDrawColor_t D_003264B0;

extern itfDrawColor_t D_003264C0;

extern itfDrawColor_t D_003264D0;

#define ITF_PANEL_STATE_PACKET_BYTES 0x40

extern u32 sdf2DMakeStdPacket(u32, s32);

extern void sdfDrawTagLink(sdfDrawTag_t *list, u32 packetAddress);

extern void (*D_00326428[])();
extern void (*CreateColorTable[])();
extern void (*DrawTable[])(itfPanel_t *, sdfDrawTag_t *);

static inline void panelSetVec4(u32 *vec, u32 red, u32 green, u32 blue, u32 alpha) {
    vec[0] = red;
    vec[1] = green;
    vec[2] = blue;
    vec[3] = alpha;
}

INCLUDE_ASM(const s32, "interface/itfPanel", itfPnlRegist2);

/* A sprite with a retained payload owns a secondary allocation. Release that
 * before its primary allocation; NULL is a no-op. */
void itfPnlRelease(itfPanel_t *sprite) {
    if (sprite != NULL) {
        if (sprite->payload != NULL) {
            sdfMemFreeDirect(sprite->payloadAllocation);
        }
        sdfMemFreeDirect(sprite->allocation);
    }
}

/* Store rectangle corners and the caller's render value before notifying the
 * kind-specific handler with context only. No kind-range or context checks. */
void itfPnlSetPosition(itfPanel_t *panel, s32 left, s32 top, s32 right, s32 bottom, s32 renderValue) {
    panel->renderValue = renderValue;
    panel->left = left;
    panel->top = top;
    panel->right = right;
    panel->bottom = bottom;
    if (D_00326428[panel->kind] != 0) {
        D_00326428[panel->kind](panel->payload);
    }
}

/* Add independent corner/render-value deltas, then notify with the updated
 * corners as well as context. */
void itfPnlSetPositionOffset(itfPanel_t *panel, s32 leftDelta, s32 topDelta, s32 rightDelta, s32 bottomDelta, s32 renderValueDelta) {
    void (*handler)() = D_00326428[panel->kind];

    panel->left += leftDelta;
    panel->top += topDelta;
    panel->right += rightDelta;
    panel->bottom += bottomDelta;
    panel->renderValue += renderValueDelta;
    if (handler != 0) {
        handler(panel->payload, panel->left, panel->top, panel->right, panel->bottom);
    }
}

/* Store the RGBA words in packet order, then notify with context.
 * The panel renderers consume these channels directly. */
void itfPnlSetColor(itfPanel_t *panel, s32 redValue, s32 greenValue, s32 blueValue, s32 alphaValue) {
    panel->red = redValue;
    panel->green = greenValue;
    panel->blue = blueValue;
    panel->alpha = alphaValue;
    if (CreateColorTable[panel->kind] != 0) {
        CreateColorTable[panel->kind](panel->payload);
    }
}

/* Build the panel list and append it to the SDK draw-surface pool. */
void itfPnlDraw(itfPanel_t *panel, sdfGrOt_t *packetOwner) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(ITF_PANEL_PACKET_LIST_BYTES);

    sdfDrawTagInit(packetList);
    DrawTestAlphaPrim(packetList);
    itfPnlDrawTag(panel, packetList);
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))packetOwner->regFunc)(packetOwner, packetList);
}

/* Invoke the indexed panel handler; panel and kind are trusted. */
void itfPnlDrawTag(itfPanel_t *panel, sdfDrawTag_t *list) {
    DrawTable[panel->kind](panel, list);
}

/* Emit top/bottom vertices for four columns; collapse overlapping inner columns to the midpoint.
   Coordinates and inset remain in the renderer's native integer units. */
void CreateVertexNormal(itfPanelVertex_t *vertices, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 columnX[ITF_PANEL_COLUMN_COUNT];
    s32 columnIndex;

    columnX[0] = x0;
    columnX[1] = x0 + ITF_PANEL_COLUMN_INSET;
    columnX[2] = x1 - ITF_PANEL_COLUMN_INSET;
    columnX[3] = x1;
    if (columnX[2] < columnX[1]) {
        columnX[1] = ((x1 - x0) >> 1) + x0;
        columnX[2] = columnX[1];
    }
    for (columnIndex = 0; columnIndex < ITF_PANEL_COLUMN_COUNT; columnIndex++) {
        vertices->x = columnX[columnIndex];
        vertices->y = y0;
        vertices++;
        vertices->x = columnX[columnIndex];
        vertices->y = y1;
        vertices++;
    }
}

/* Emit the frame grid and three table-offset rows above and below it. */
void CreateVertexMesWin(itfPanelVertex_t *vertices, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 xs[ITF_PANEL_COLUMN_COUNT];
    s32 ys[4];
    s32 i;
    s32 j;

    xs[0] = x0 - 0x100;
    xs[1] = x0 + ITF_PANEL_COLUMN_INSET;
    xs[2] = x1 - ITF_PANEL_COLUMN_INSET;
    xs[3] = x1 + 0x100;
    ys[0] = y0 - 0x80;
    ys[1] = y0 + 0x80;
    ys[2] = y1 - 0x80;
    ys[3] = y1 + 0x80;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[i];
            vertices++;
        }
    }
    xs[0] = x0 - 0x100;
    xs[1] = x0 + 0x400;
    xs[2] = x1 - 0x400;
    xs[3] = x1 + 0x100;
    ys[0] = y0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[0] + D_00326508[i];
            vertices++;
        }
    }
    ys[0] = y1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[0] - D_00326508[i];
            vertices++;
        }
    }
}

void CreateVertexSelWin(itfPanelVertex_t *vertices, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 xs[ITF_PANEL_COLUMN_COUNT];
    s32 ys[4];
    s32 i;
    s32 j;

    xs[0] = x0 - 0x100;
    xs[1] = x0 + ITF_PANEL_COLUMN_INSET;
    xs[2] = x1 - ITF_PANEL_COLUMN_INSET;
    xs[3] = x1 + 0x100;
    ys[0] = y0 - 0x80;
    ys[1] = y0 + 0x80;
    ys[2] = y1 - 0x80;
    ys[3] = y1 + 0x80;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[i];
            vertices++;
        }
    }
    xs[0] = x0 - 0x100;
    xs[1] = x0 + 0x400;
    xs[2] = x1 - 0x400;
    xs[3] = x1 + 0x100;
    ys[0] = y0;
    for (i = 0; i < ITF_PANEL_ROW_COUNT; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[0] + i * 8;
            vertices++;
        }
    }
    ys[0] = y1;
    for (i = 0; i < ITF_PANEL_ROW_COUNT; i++) {
        for (j = 0; j < ITF_PANEL_COLUMN_COUNT; j++) {
            vertices->x = xs[j];
            vertices->y = ys[0] - i * 8;
            vertices++;
        }
    }
}

/* Write the rectangle corners in top-left, top-right, bottom-right, bottom-left order. */
void CreateVertexPlain(itfPanelVertex_t *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    itfPanelVertex_t *p;

    v->x = x0;
    v->y = y0;
    p = v + 1;
    p->x = x1;
    p->y = y0;
    p = v + 2;
    p->x = x1;
    p->y = y1;
    p = v + 3;
    p->x = x0;
    p->y = y1;
}

/* Write two rows of three columns; the middle column is the right edge minus a fixed strip width. */
void CreateVertexLeftFade(itfPanelVertex_t *v, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 columnX[ITF_PANEL_CORNER_COLUMN_COUNT];
    s32 rowY[ITF_PANEL_ROW_COUNT];
    s32 rowIndex;
    s32 columnIndex;

    columnX[0] = x0;
    columnX[1] = x1 - ITF_PANEL_CORNER_STRIP_WIDTH;
    columnX[2] = x1;
    rowY[0] = y0;
    rowY[1] = y1;
    for (rowIndex = 0; rowIndex < ITF_PANEL_ROW_COUNT; rowIndex++) {
        for (columnIndex = 0; columnIndex < ITF_PANEL_CORNER_COLUMN_COUNT; columnIndex++) {
            v->x = columnX[columnIndex];
            v->y = rowY[rowIndex];
            v++;
        }
    }
}

/* Write four fixed horizontal columns with inset top/bottom coordinates.
   x0 and x1 are intentionally unused; do not replace the fixed horizontal positions. */
void CreateVertexFacility(itfPanelFacilityData_t *base, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 columnX[ITF_PANEL_COLUMN_COUNT];
    s32 rowY[ITF_PANEL_ROW_COUNT];
    itfDrawVertex_t *vertices = base->vertices;
    s32 columnIndex;
    columnX[0] = 0x1A0;
    columnX[1] = 0x480;
    columnX[2] = 0x1B70;
    columnX[3] = 0x1E50;
    rowY[0] = y0 + 0x160;
    rowY[1] = y1 - 0x140;
    for (columnIndex = 0; columnIndex < ITF_PANEL_COLUMN_COUNT; columnIndex++) {
        vertices->x = columnX[columnIndex];
        vertices->y = rowY[0];
        vertices++;
        vertices->x = columnX[columnIndex];
        vertices->y = rowY[1];
        vertices++;
    }
}

/* Populate RGBA rows at bytes 0x40/0x50; the first row is transparent. */
void CreateColorNormal(itfPanelNormalData_t *data, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = data->colors[0].components;

    panelSetVec4(colorRow, red, green, blue, 0);
    colorRow = data->colors[1].components;
    panelSetVec4(colorRow, red, green, blue, alpha);
}

/* Write the two color rows, then scale six template rows; the first fourth component is one. */
void CreateColorMesWin(itfPanelMesWinData_t *data, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = data->colors[0].components;

    panelSetVec4(colorRow, red, green, blue, 1);
    colorRow = data->colors[1].components;
    panelSetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB((s32 *)data->colors[2].components, red, green, blue, alpha, D_00326518, 6);
}

/* Write the two color rows, then scale four template rows; the first fourth component is one. */
void CreateColorSelWin(itfPanelSelWinData_t *data, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = data->colors[0].components;

    panelSetVec4(colorRow, red, green, blue, 1);
    colorRow = data->colors[1].components;
    panelSetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB((s32 *)data->colors[2].components, red, green, blue, alpha, D_00326578, 4);
}

/* Set the base color, one alpha-scaled gradient row, and two rows at full color scale. */
void CreateColorPlain(itfPanelPlainData_t *data, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = data->colors[0].components;

    panelSetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB((s32 *)data->colors[1].components, red, green, blue, alpha, D_003265B8, 1);
    itfMiscSetRatioRGB((s32 *)data->colors[2].components, red, green, blue, ITF_PANEL_FULL_COLOR_SCALE, D_003265C8, 2);
}

/* Set the four color words at byte 0x20, consumed as a color record by the quad path. */
void CreateColorPera1(itfPanelPlainData_t *data, s32 red, s32 green, s32 blue, s32 alpha) {
    u32 *color = data->colors[0].components;

    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;
}

/* Populate RGBA rows at bytes 0x30/0x40; the first row is transparent. */
void CreateColorLeftFade(itfPanelLeftFadeData_t *data, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = data->colors[0].components;

    panelSetVec4(colorRow, red, green, blue, 0);
    colorRow = data->colors[1].components;
    panelSetVec4(colorRow, red, green, blue, alpha);
}

/* Set both panel colors to the same blue tint with zero alpha. */
void CreateColorFacility(itfPanelFacilityData_t *base) {
    u32 *colorRow = base->colors[0].components;

    panelSetVec4(colorRow, 0x73, 0x87, 0xFF, 0);
    colorRow = base->colors[1].components;
    panelSetVec4(colorRow, 0x73, 0x87, 0xFF, 0);
}

/* Draw the panel's three flat quads from one vertex/color buffer; each quad uses its own 4-byte index rows. */
void DrawNormal(itfPanel_t *panel, sdfDrawTag_t *command) {
    itfPanelNormalData_t *data = (itfPanelNormalData_t *)panel->payload;
    itfPanelVertex_t *drawBuffer = data->vertices;
    itfPanelColor_t *colors = data->colors;
    s32 quadIndex;

    colors[1].rgba.alpha = panel->alpha;
    for (quadIndex = 0; quadIndex < ITF_PANEL_WIDE_PASS_COUNT; quadIndex++) {
        itfMiscDrawRectanglePrim(drawBuffer, colors, &D_003265F8[quadIndex * ITF_PANEL_QUAD_INDEX_COUNT], &D_00326608[quadIndex * ITF_PANEL_QUAD_INDEX_COUNT], panel->renderValue, command);
    }
}

/* Update seven alpha words, then emit three wide and six side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawMesWin(itfPanel_t *panel, sdfDrawTag_t *command) {
    itfPanelMesWinData_t *data = (itfPanelMesWinData_t *)panel->payload;
    itfPanelVertex_t *drawBuffer = data->vertices;
    itfPanelColor_t *colors = data->colors + 1;
    s32 alpha = panel->alpha;
    s32 rowIndex;

    for (rowIndex = 0; rowIndex < ITF_PANEL_SEVEN_COLOR_COUNT; rowIndex++, colors++) {
        colors->rgba.alpha = alpha;
    }
    colors = data->colors;
    for (rowIndex = 0; rowIndex < ITF_PANEL_WIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawTriangleStripPrim(drawBuffer, colors, &D_00326618[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], &D_00326630[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], ITF_PANEL_WIDE_INDEX_COUNT, panel->renderValue, command);
    }
    for (rowIndex = 0; rowIndex < ITF_PANEL_SEVEN_SIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawLineStripPrim(drawBuffer + 16 + rowIndex * 4, colors, D_0054F480, &D_00326648[rowIndex * ITF_PANEL_QUAD_INDEX_COUNT], ITF_PANEL_QUAD_INDEX_COUNT, panel->renderValue, command);
    }
}

/* Update five alpha words, then emit three wide and two side strips.
   The first color row is deliberately excluded from the alpha update. */
void DrawSelWin(itfPanel_t *panel, sdfDrawTag_t *command) {
    itfPanelSelWinData_t *data = (itfPanelSelWinData_t *)panel->payload;
    itfPanelVertex_t *drawBuffer = data->vertices;
    itfPanelColor_t *colors = data->colors + 1;
    s32 alpha = panel->alpha;
    s32 rowIndex;

    for (rowIndex = 0; rowIndex < ITF_PANEL_FIVE_COLOR_COUNT; rowIndex++, colors++) {
        colors->rgba.alpha = alpha;
    }
    colors = data->colors;
    for (rowIndex = 0; rowIndex < ITF_PANEL_WIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawTriangleStripPrim(drawBuffer, colors, &D_00326660[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], &D_00326678[rowIndex * ITF_PANEL_WIDE_INDEX_COUNT], ITF_PANEL_WIDE_INDEX_COUNT, panel->renderValue, command);
    }
    for (rowIndex = 0; rowIndex < ITF_PANEL_FIVE_SIDE_PASS_COUNT; rowIndex++) {
        itfMiscDrawLineStripPrim(drawBuffer + 16 + rowIndex * 4, colors, D_0054F488, &D_00326690[rowIndex * ITF_PANEL_QUAD_INDEX_COUNT], ITF_PANEL_QUAD_INDEX_COUNT, panel->renderValue, command);
    }
}

void DrawPlain(itfPanel_t *panel, sdfDrawTag_t *command) {
    itfPanelVertex_t vertices[12];
    itfPanelPlainData_t *data = (itfPanelPlainData_t *)panel->payload;
    itfPanelVertex_t *buf = data->vertices;
    itfPanelColor_t *colors = data->colors;
    itfPanelVertex_t *input = data->vertices;
    s32 i;

    for (i = 0; i < 3; i++) {
        s32 xOffset = i * 16;
        s32 yOffset = i * 8;
        itfPanelVertex_t *out = &vertices[i * 4];

        out[0].x = input[0].x + xOffset;
        out[0].y = input[0].y + yOffset;
        out[1].x = input[1].x - xOffset;
        out[1].y = input[1].y + yOffset;
        out[2].x = input[2].x - xOffset;
        out[2].y = input[2].y - yOffset;
        out[3].x = input[3].x + xOffset;
        out[3].y = input[3].y - yOffset;
    }

    itfMiscDrawRectanglePrim(buf, colors, D_0054F490, D_0054F498, panel->renderValue, command);
    for (i = 0; i < 3; i++) {
        itfMiscDrawLineStripPrim(vertices, colors, &D_003266A0[i * 5], &D_003266B0[i * 5], 5, panel->renderValue, command);
    }
}

/* Update the first color row's alpha and emit one indexed quad. */
INCLUDE_ASM(const s32, "interface/itfPanel", DrawPera1);

/* Bracket the six-index panel draw with table-state two and its native reset to zero. */
void DrawLeftFade(itfPanel_t *panel, sdfDrawTag_t *command) {
    itfPanelLeftFadeData_t *data = (itfPanelLeftFadeData_t *)panel->payload;
    itfPanelVertex_t *drawBuffer = data->vertices;
    itfPanelColor_t *colors = data->colors;

    colors[1].rgba.alpha = panel->alpha;
    itfMiscDrawAlphaPrim(command, 2, 0);
    itfMiscDrawTriangleStripPrim(drawBuffer, colors, D_0054F4B0, D_0054F4B8, 6, panel->renderValue, command);
    itfMiscDrawAlphaPrim(command, 0, 0);
}

INCLUDE_ASM(const s32, "interface/itfPanel", DrawMesWin2);

/* Bind the texture for the left cap, then draw the center and reversed right
 * cap. Insets are captured before the first submission; Y bounds are live. */
void DrawSpkWin2(itfPanel_t *panel, sdfDrawTag_t *command) {
    itfDrawVertex_t rect[2];
    itfDrawColor_t *color = (itfDrawColor_t *)&panel->red;
    itfPanelTextureData_t *payload = (itfPanelTextureData_t *)panel->payload;
    s32 leftInset = panel->left + 0x3E0;
    s32 rightInset = panel->right - 0x3E0;

    rect[0].x = panel->left;
    rect[0].y = panel->top;
    rect[1].x = leftInset;
    rect[1].y = panel->bottom;
    itfMiscDrawSpritePrim(rect, &D_003266C0, color, panel->renderValue,
                                  payload->texture, 0, command);

    rect[0].x = leftInset;
    rect[0].y = panel->top;
    rect[1].x = rightInset;
    rect[1].y = panel->bottom;
    itfMiscDrawSpritePrim2(rect, &D_003266D0, color, panel->renderValue, 0, command);

    rect[0].x = panel->right;
    rect[0].y = panel->top;
    rect[1].x = rightInset;
    rect[1].y = panel->bottom;
    itfMiscDrawSpritePrim2(rect, &D_003266C0, color, panel->renderValue, 0, command);
}

/* Draw the three inset flat quads, then split a tall texture into top, stretch
 * and bottom bands. The short path uses the live rectangle in the panel. */
void DrawFacility(itfPanel_t *panel, sdfDrawTag_t *command) {
    itfDrawVertex_t rect[2];
    itfPanelFacilityData_t *payload = (itfPanelFacilityData_t *)panel->payload;
    itfDrawVertex_t *vertices = payload->vertices;
    itfDrawColor_t *colors = payload->colors;
    s32 i;
    s32 top;
    s32 bottom;

    colors[1].components[3] = (panel->alpha * 0x20) >> 7;
    for (i = 0; i < 3; i++) {
        itfMiscDrawRectanglePrim(vertices, colors, &D_003266E0[i * 4],
                        &D_003266F0[i * 4], panel->renderValue, command);
    }
    bottom = panel->bottom;
    top = panel->top;
    if (bottom - top >= 0x569) {
        s32 topEnd = top + 0x2B4;
        s32 bottomStart = bottom - 0x2B4;
        s32 *color = &panel->red;

        rect[0].x = panel->left;
        rect[0].y = top;
        rect[1].x = panel->right;
        rect[1].y = topEnd;
        itfMiscDrawSpritePrim(rect, &D_003264A0, color, panel->renderValue,
                                      payload->texture, 0, command);
        rect[0].y = topEnd;
        rect[1].y = bottomStart;
        itfMiscDrawSpritePrim(rect, &D_003264B0, color, panel->renderValue,
                                      payload->texture, 0, command);
        rect[0].y = bottomStart;
        rect[1].y = panel->bottom;
        itfMiscDrawSpritePrim(rect, &D_003264C0, color, panel->renderValue,
                                      payload->texture, 0, command);
    } else {
        itfMiscDrawSpritePrim(&panel->left, &D_003264D0,
                                      &panel->red, panel->renderValue,
                                      payload->texture, 0, command);
    }
}

/* Append two GS A+D state writes: TEST_1 (0x47), then ALPHA_1 (0x42). */
void DrawTestAlphaPrim(sdfDrawTag_t *list) {
    s32 packet = sdfGrAllocPacketBuff(ITF_PANEL_STATE_PACKET_BYTES);
    u64 *stateWords = (u64 *)sdf2DMakeStdPacket(packet, ITF_PANEL_STATE_PACKET_BYTES);

    stateWords[4] = 0x5101B;
    stateWords[5] = 0x47;
    stateWords[6] = 0x44;
    stateWords[7] = 0x42;
    sdfDrawTagLink(list, packet);
}
INCLUDE_SDATA(const s32, "interface/itfPanel", D_0054F480);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_0054F488);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_0054F490);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_0054F498);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_0054F4A0);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_0054F4A8);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_0054F4B0);

INCLUDE_SDATA(const s32, "interface/itfPanel", D_0054F4B8);

