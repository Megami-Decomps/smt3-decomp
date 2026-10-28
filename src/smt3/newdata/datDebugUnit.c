#include "common.h"
#include "nb.h"
#include "effBattle.h"

INCLUDE_ASM(const s32, "newdata/datDebugUnit", DispList);

INCLUDE_ASM(const s32, "newdata/datDebugUnit", DispUnit1);

extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void *sdfDevPrintf(s32 x, s32 y, s32 z, s32 attr, const char *format, ...);
extern void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);
extern void DispList(s32 x, s32 y, sdfDrawTag_t *tag);
extern void DispUnit1(s32 unit, s32 mode, s32 entry, sdfDrawTag_t *tag);
extern sdfGrOt_t D_002F6938;
const char D_005431C8[] = "     ID     LV  HP       MP     STAT";

s32 datDebugUnitProcess(dds3ProcessID_t *id)
{
    sdfDrawTag_t *unitList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTag_t *panelList;
    s32 i;
    s32 entry;

    sdfDrawTagInit(unitList);
    DispList(2, 10, unitList);
    sdfDrawTagLink(unitList, (u32)sdfDevPrintf(0x7180, 0x7E40, 0xFEFFFF, 4, D_005431C8));

    for (i = 0, entry = 15; i < 16; i++, entry++) {
        DispUnit1(i, 2, entry, unitList);
    }

    entry++;
    panelList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(panelList);
    dds3ConsPanelTagLink(panelList, 0x18, 0x72, 0x25, entry - 9);
    D_002F6938.regFunc(&D_002F6938, panelList);
    D_002F6938.regFunc(&D_002F6938, unitList);
    return 0;
}

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_005431F0);

INCLUDE_ASM(const s32, "newdata/datDebugUnit", datDebugDrawSelectMenu);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_00543230);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_00543240);

INCLUDE_RODATA(const s32, "newdata/datDebugUnit", D_00543250);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EB0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EB8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EC0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EC8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550ED0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550ED8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EE0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EE8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EF0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550EF8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F00);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F08);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F10);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F18);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F20);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F28);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F30);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F38);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F40);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F48);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F50);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F54);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F58);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F60);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F68);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F70);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F78);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F7C);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F80);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F88);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F90);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550F98);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FA0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FA8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FB0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FB8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FC0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FC8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FD0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FD8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FE0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FE8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FF0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00550FF8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551000);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551008);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551010);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551018);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551020);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551028);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551030);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551038);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551040);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551048);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551050);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551058);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551060);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551068);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551070);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551078);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551080);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551088);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551090);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551098);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510A0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510A8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510B0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510B8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510C0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510C8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultTimer);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultTobiwarpLabel);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultState);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultBestScore);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultStep);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultCounter);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510E8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510F0);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_005510F8);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbTobiwarpLabelIdx);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", D_00551100);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultTex1);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultTex2);

INCLUDE_SDATA(const s32, "newdata/datDebugUnit", nbResultTexHandle);

