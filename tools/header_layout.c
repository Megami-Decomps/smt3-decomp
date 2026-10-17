/* Compile-only check of include/: every verified size and offset. ee-gcc 2.96
 * has no _Static_assert, so each claim sizes an array by it: a false condition
 * gives a negative length and the compile fails. Run by tools/test_headers.py.
 * Evidence for each number is in include/README.md. */
#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "scr.h"

#define ASSERT_AT(line, cond) typedef char static_assert_##line[(cond) ? 1 : -1]
#define SZ(line, type, bytes) ASSERT_AT(line, sizeof(type) == (bytes))
#define OFF(line, type, field, at) \
    ASSERT_AT(line, ((char *)&((type *)0)->field - (char *)(type *)0) == (at))

/* libsdf.h */
SZ(l01, sdfVector2_t, 8);
SZ(l02, sdfVector3_t, 12);
SZ(l03, sdfVector4_t, 16);
SZ(l04, sdfMatrix4_t, 64);
SZ(l05, sdfColor_t, 16);
SZ(l06, sdfColor32_t, 4);
SZ(l07, sdfDmaTag_t, 16);
SZ(l08, sdfGifTag_t, 16);
SZ(l09, sdfGrTagStdReg_t, 16);
SZ(l10, sdfGrPacketHeader_t, 32);
SZ(l11, sdfChipCB_tag, 12);
SZ(l12, sdfChipPageInfo_tag, 24);
SZ(l13, sdfChipRecord_t, 16);
SZ(l14, sdfMemHandle_t, 16);
SZ(l15, sdfMemRecord_t, 16);
SZ(l16, sdfGsMemSlice_t, 28);
SZ(l17, sdfTexHandle_t, 64);
SZ(l18, sdfTexPacket_t, 64);
SZ(l19, sdfDrawTag_t, 32);
SZ(l20, sdfPreDrawHook_t, 16);
SZ(l21, sdfPreDrawTag_t, 16);
SZ(l22, sdfModelPacketHeader_t, 16);
SZ(l23, sdfMatUvwParm_t, 24);
SZ(l24, sdfMat_t, 72);
SZ(l25, sdfMatPacket_t, 160);
SZ(l26, sdf3DLight_t, 64);

OFF(l30, sdfDmaTag_t, id, 3);
OFF(l31, sdfDmaTag_t, addr, 4);
OFF(l32, sdfGrPacketHeader_t, giftag, 16);
OFF(l33, sdfChipCB_tag, chipsPerPage, 10);
OFF(l34, sdfChipPageInfo_tag, alloc, 8);
OFF(l35, sdfChipPageInfo_tag, owner, 12);
OFF(l36, sdfMemHandle_t, ptr, 8);
OFF(l37, sdfMemHandle_t, stat, 12);
OFF(l38, sdfMemHandle_t, mlock, 14);
OFF(l39, sdfGsMemSlice_t, type, 8);
OFF(l40, sdfGsMemSlice_t, addr, 12);
OFF(l41, sdfGsMemSlice_t, size, 16);
OFF(l42, sdfTexHandle_t, width, 12);
OFF(l43, sdfTexHandle_t, height, 14);
OFF(l44, sdfTexHandle_t, pixelGsHandle, 16);
OFF(l45, sdfTexHandle_t, clutGsHandle, 20);
OFF(l46, sdfTexHandle_t, numOfCLUT, 24);
OFF(l47, sdfTexHandle_t, CLUTFormat, 25);
OFF(l48, sdfTexHandle_t, pixelFormat, 26);
OFF(l49, sdfTexHandle_t, numOfMipmap, 27);
OFF(l50, sdfTexHandle_t, packet, 40);
OFF(l51, sdfTexHandle_t, clutColor, 56);
OFF(l52, sdfDrawTag_t, head, 4);
OFF(l53, sdfDrawTag_t, tail, 8);
OFF(l54, sdfDrawTag_t, z, 12);
OFF(l55, sdfDrawTag_t, lightPacket, 16);
OFF(l56, sdfDrawTag_t, fogPacket, 20);
OFF(l57, sdfDrawTag_t, texPacket, 24);
OFF(l58, sdfModelPacketHeader_t, otIndex, 5);
OFF(l59, sdfModelPacketHeader_t, size, 6);
OFF(l60, sdfModelPacketHeader_t, memHandle, 12);
OFF(l61, sdfMatUvwParm_t, tile, 8);
OFF(l62, sdfMatUvwParm_t, angle, 16);
OFF(l63, sdfMat_t, duty, 6);
OFF(l64, sdfMat_t, packet, 8);
OFF(l65, sdfMat_t, ambient, 16);
OFF(l66, sdfMat_t, diffuse, 20);
OFF(l67, sdfMat_t, multiTexDiffuse, 24);
OFF(l68, sdfMat_t, uvwParm, 56);
OFF(l69, sdfMat_t, multiTexUvwParm, 60);
OFF(l70, sdfMat_t, fdBias, 64);
OFF(l71, sdfMat_t, fdSoften, 68);
OFF(l72, sdfMatPacket_t, vifcode1, 24);
OFF(l73, sdfMatPacket_t, vifcode2, 52);
OFF(l74, sdfMatPacket_t, tex1Data, 56);
OFF(l75, sdfMatPacket_t, texTm, 104);

/* kernel.h */
SZ(k01, dds3ProcessID_t, 80);
OFF(k10, dds3ProcessID_t, namehash, 24);
OFF(k11, dds3ProcessID_t, flag, 28);
OFF(k12, dds3ProcessID_t, priority, 32);
OFF(k13, dds3ProcessID_t, starttime, 36);
OFF(k14, dds3ProcessID_t, runtime, 40);
OFF(k15, dds3ProcessID_t, starttimer, 44);
OFF(k16, dds3ProcessID_t, shutdowntimer, 46);
OFF(k17, dds3ProcessID_t, processfunc, 48);
OFF(k18, dds3ProcessID_t, shutdownfunc, 52);
OFF(k19, dds3ProcessID_t, workdata, 56);
OFF(k20, dds3ProcessID_t, next, 60);
OFF(k21, dds3ProcessID_t, prev, 64);
OFF(k22, dds3ProcessID_t, parent, 68);
OFF(k23, dds3ProcessID_t, child, 72);
OFF(k24, dds3ProcessID_t, sibling, 76);

/* basic.h */
SZ(b01, dds3Basic_t, 68);
SZ(b02, dds3BasicFunction_t, 24);
SZ(b03, dds3PoseSet_t, 208);
SZ(b04, dds3BasicArrayHandle_t, 16);
SZ(b05, dds3PathBasicControl_t, 36);
OFF(b10, dds3Basic_t, name, 8);
OFF(b11, dds3Basic_t, type, 12);
OFF(b12, dds3Basic_t, basicfunction, 16);
OFF(b13, dds3Basic_t, workdata, 24);
OFF(b14, dds3Basic_t, poseset, 28);
OFF(b15, dds3Basic_t, next, 32);
OFF(b16, dds3Basic_t, prev, 36);
OFF(b17, dds3Basic_t, color, 52);
OFF(b18, dds3BasicFunction_t, destructor, 4);
OFF(b19, dds3BasicFunction_t, mainprocuder, 8);
OFF(b20, dds3BasicFunction_t, drawprocuder, 12);
OFF(b21, dds3PoseSet_t, pos, 64);
OFF(b22, dds3PoseSet_t, rot, 80);
OFF(b23, dds3PoseSet_t, scl, 96);
OFF(b24, dds3PoseSet_t, flag, 192);
OFF(b25, dds3PoseSet_t, r, 196);
OFF(b26, dds3PoseSet_t, frameFall, 200);
OFF(b27, dds3BasicArrayHandle_t, nowindex, 4);
OFF(b28, dds3BasicArrayHandle_t, count, 6);
OFF(b29, dds3BasicArrayHandle_t, next, 8);
OFF(b30, dds3BasicArrayHandle_t, prev, 12);

/* scr.h */
SZ(s01, scrHeader_t, 32);
SZ(s02, scrTypeTable_t, 16);
SZ(s03, scrProcTable_t, 32);
SZ(s04, scrLabelTable_t, 32);
SZ(s05, scrCode_u, 4);
SZ(s06, scrStackData_u, 4);
SZ(s07, scrCommandTable_t, 8);
SZ(s08, scrProcessWork_s, 244);
OFF(s10, scrHeader_t, FileSize, 4);
OFF(s11, scrHeader_t, MagicCode, 8);
OFF(s12, scrHeader_t, TypeTableCount, 16);
OFF(s13, scrHeader_t, LocalIntCount, 20);
OFF(s14, scrHeader_t, LocalFloatCount, 22);
OFF(s15, scrTypeTable_t, ItemCount, 8);
OFF(s16, scrTypeTable_t, ItemAddress, 12);
OFF(s17, scrProcTable_t, CodeAddress, 24);
OFF(s18, scrLabelTable_t, CodeAddress, 24);
OFF(s19, scrCommandTable_t, paranum, 4);
OFF(s20, scrProcessWork_s, CP, 24);
OFF(s21, scrProcessWork_s, SP, 28);
OFF(s22, scrProcessWork_s, StackType, 32);
OFF(s23, scrProcessWork_s, StackData, 60);
OFF(s24, scrProcessWork_s, Header, 172);
OFF(s25, scrProcessWork_s, TypeTable, 176);
OFF(s26, scrProcessWork_s, ProcTable, 180);
OFF(s27, scrProcessWork_s, LabelTable, 184);
OFF(s28, scrProcessWork_s, Code, 188);
OFF(s29, scrProcessWork_s, MessageData, 192);
OFF(s30, scrProcessWork_s, StringData, 196);
OFF(s31, scrProcessWork_s, StartProc, 200);
OFF(s32, scrProcessWork_s, MesHandle, 204);
OFF(s33, scrProcessWork_s, ScriptTimer, 208);
OFF(s34, scrProcessWork_s, CommandTimer, 212);
OFF(s35, scrProcessWork_s, MemHandle, 216);
OFF(s36, scrProcessWork_s, LocalInt, 220);
OFF(s37, scrProcessWork_s, LocalFloat, 224);
OFF(s38, scrProcessWork_s, ProcessID, 228);
OFF(s39, scrProcessWork_s, prev, 232);
OFF(s40, scrProcessWork_s, next, 236);
OFF(s41, scrProcessWork_s, data, 240);

/* Keep the object non-empty so ee-as has something to assemble. */
const s32 headerCheck = 1;
