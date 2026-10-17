#include "common.h"

INCLUDE_ASM(const s32, "interface/frFont", frSetWhiteSpaceW);

INCLUDE_ASM(const s32, "interface/frFont", frSwapColor);

INCLUDE_ASM(const s32, "interface/frFont", frConvertVal2N);

INCLUDE_ASM(const s32, "interface/frFont", frCreateDrawEnvPacket);

INCLUDE_ASM(const s32, "interface/frFont", frCreateDrawFontPacket_20550848);

INCLUDE_ASM(const s32, "interface/frFont", frCreateUserFontPacket);

INCLUDE_ASM(const s32, "interface/frFont", frCopyFontClutToVram);

INCLUDE_ASM(const s32, "interface/frFont", frClearTexBuf);

INCLUDE_ASM(const s32, "interface/frFont", frLoadFontData);

INCLUDE_ASM(const s32, "interface/frFont", frInitFontTable);

INCLUDE_ASM(const s32, "interface/frFont", frDestroyFontTable);

INCLUDE_ASM(const s32, "interface/frFont", frAddFontStyle);

INCLUDE_ASM(const s32, "interface/frFont", frDelFontStyle);

INCLUDE_ASM(const s32, "interface/frFont", frLoadStyleAll);

INCLUDE_ASM(const s32, "interface/frFont", frDelStyleAll);

INCLUDE_ASM(const s32, "interface/frFont", frSetFontClut);

INCLUDE_ASM(const s32, "interface/frFont", frFlushGsFontTable);

INCLUDE_ASM(const s32, "interface/frFont", frDestroyFontRequest);

INCLUDE_ASM(const s32, "interface/frFont", frDestroyFontRequestNow);

INCLUDE_ASM(const s32, "interface/frFont", frDestroyFontRequestAfter);

INCLUDE_ASM(const s32, "interface/frFont", frGetGsFontNum);

INCLUDE_ASM(const s32, "interface/frFont", frGetTextHandleNum);

INCLUDE_ASM(const s32, "interface/frFont", frGetFontHandleNum);

INCLUDE_ASM(const s32, "interface/frFont", frGetYShiftLev);

INCLUDE_ASM(const s32, "interface/frFont", frGetXShiftLev);

INCLUDE_ASM(const s32, "interface/frFont", frSetUserImage);

INCLUDE_ASM(const s32, "interface/frFont", frLinkUserImage);

INCLUDE_ASM(const s32, "interface/frFont", frTranslateUserData);

INCLUDE_ASM(const s32, "interface/frFont", frDecodeFontImage);

INCLUDE_ASM(const s32, "interface/frFont", frTranslateFont2GS);

INCLUDE_ASM(const s32, "interface/frFont", frInitTextHandle);

INCLUDE_ASM(const s32, "interface/frFont", frInitFontHandle);

INCLUDE_ASM(const s32, "interface/frFont", frSetFont);

INCLUDE_ASM(const s32, "interface/frFont", frLinkFontString);

INCLUDE_ASM(const s32, "interface/frFont", frSetFontString);

INCLUDE_ASM(const s32, "interface/frFont", frSetDrawSpeed);

INCLUDE_ASM(const s32, "interface/frFont", frSetFontSpace);

INCLUDE_ASM(const s32, "interface/frFont", frSetFontSize);

INCLUDE_ASM(const s32, "interface/frFont", frSetDrawPosition);

INCLUDE_ASM(const s32, "interface/frFont", frSetPriority);

INCLUDE_ASM(const s32, "interface/frFont", frSetFontColorNo);

INCLUDE_ASM(const s32, "interface/frFont", frSetFontColor);

INCLUDE_ASM(const s32, "interface/frFont", frEnableMode);

INCLUDE_ASM(const s32, "interface/frFont", frDisableMode);

INCLUDE_ASM(const s32, "interface/frFont", frSetFontDrawOT);

INCLUDE_ASM(const s32, "interface/frFont", frEraseEffect);

INCLUDE_ASM(const s32, "interface/frFont", frDrawEffect);

INCLUDE_ASM(const s32, "interface/frFont", frDrawFont);

INCLUDE_ASM(const s32, "interface/frFont", frDrawFontEx);

INCLUDE_ASM(const s32, "interface/frFont", frFlipPacketBuf);

INCLUDE_ASM(const s32, "interface/frFont", frCombineFontRequest);

INCLUDE_ASM(const s32, "interface/frFont", frCombineFontRequestEx);

INCLUDE_ASM(const s32, "interface/frFont", frSetExtDic);

INCLUDE_ASM(const s32, "interface/frFont", frDestroyExtDic);

INCLUDE_ASM(const s32, "interface/frFont", frStrLen);

INCLUDE_ASM(const s32, "interface/frFont", frGetStrWidth);

INCLUDE_ASM(const s32, "interface/frFont", frGetStrWidth2);

INCLUDE_ASM(const s32, "interface/frFont", frPreCalcStrWidth);

INCLUDE_ASM(const s32, "interface/frFont", frGetDefaultFontWidth);

INCLUDE_ASM(const s32, "interface/frFont", frGetFontWidth);

INCLUDE_ASM(const s32, "interface/frFont", frGetDefaultFontHeight);

INCLUDE_ASM(const s32, "interface/frFont", frGetFontHeight);

INCLUDE_ASM(const s32, "interface/frFont", frSetMessageDefStepY);

INCLUDE_ASM(const s32, "interface/frFont", frSetMessageStepY);

INCLUDE_ASM(const s32, "interface/frFont", MessageNumSearch);

INCLUDE_ASM(const s32, "interface/frFont", frCreateVariableRequest);

INCLUDE_ASM(const s32, "interface/frFont", frCreateNewHandle);

INCLUDE_ASM(const s32, "interface/frFont", frMessageReturnCode);

INCLUDE_ASM(const s32, "interface/frFont", frPushTagChar);

INCLUDE_ASM(const s32, "interface/frFont", frAnalyzeTag);

INCLUDE_ASM(const s32, "interface/frFont", frGetTagArgCount);

INCLUDE_ASM(const s32, "interface/frFont", frCopyTagArgs);

INCLUDE_ASM(const s32, "interface/frFont", frCalcStringHash);

INCLUDE_ASM(const s32, "interface/frFont", frAnalyzeMessage);

INCLUDE_ASM(const s32, "interface/frFont", MessageRequest);

INCLUDE_ASM(const s32, "interface/frFont", MessageRequestEx);

INCLUDE_ASM(const s32, "interface/frFont", MessageRequestEx2);

INCLUDE_ASM(const s32, "interface/frFont", MessageRequestNS);

INCLUDE_ASM(const s32, "interface/frFont", frGetMessageResult);

INCLUDE_ASM(const s32, "interface/frFont", frSetVariableRequest);

INCLUDE_ASM(const s32, "interface/frFont", frDumpFontTexture);

INCLUDE_RODATA(const s32, "interface/frFont", D_00535968);

INCLUDE_RODATA(const s32, "interface/frFont", D_00535978);

INCLUDE_RODATA(const s32, "interface/frFont", D_00535988);

INCLUDE_SDATA(const s32, "interface/frFont", gMessageRequestResult);

INCLUDE_SDATA(const s32, "interface/frFont", gMessageSpaceX);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F409);

INCLUDE_SDATA(const s32, "interface/frFont", gMessageStepY);

INCLUDE_SDATA(const s32, "interface/frFont", frDefaultFontHandle);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F414);

INCLUDE_SDATA(const s32, "interface/frFont", gModeFlag);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F41D);

INCLUDE_SDATA(const s32, "interface/frFont", gFontDrawOT);

INCLUDE_SDATA(const s32, "interface/frFont", frSetFontString_style);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F425);

INCLUDE_SDATA(const s32, "interface/frFont", frDrawFontWork);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F42C);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F430);

INCLUDE_SDATA(const s32, "interface/frFont", D_0054F438);

