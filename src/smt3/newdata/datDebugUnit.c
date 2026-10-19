#include "common.h"
#include "nb.h"
#include "effBattle.h"

INCLUDE_ASM(const s32, "newdata/datDebugUnit", DispList);

INCLUDE_ASM(const s32, "newdata/datDebugUnit", DispUnit1);

INCLUDE_ASM(const s32, "newdata/datDebugUnit", datDebugUnitProcess);

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

