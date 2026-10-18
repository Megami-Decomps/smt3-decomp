#include "common.h"
#include "kernel.h"

extern char D_0053BC98[];
extern char D_0053BEF0[];
extern char D_0054F908[];
extern char D_0054F9B8[];
extern char D_0053C5B8[];
extern char D_0053C5D0[];
extern char D_0053C5E8[];
extern char D_0054FAB0[];
/* Shared by the float and integer menus after the preceding menu label. */
extern char D_0053BCC0[]; /* "VALUE CHANGE." */

extern u16 skyanime;
extern u16 skyanow;
extern u16 skyamax;
extern s16 skyatarget;
extern s16 skyabase_par;
extern s32 fldbokasi;
extern u16 evt_file_minor;
extern u16 D_0054F824;

extern s32 printf(const char *fmt, ...);
extern s32 sprintf(char *buf, const char *fmt, ...);
extern s32 p2sdLoadSMGFile(s32 index);
extern s32 p2sdLoadCheckMidi(s32 index);
extern void p2sdPlayBGM(s32 index);
extern void p2sdSetBgmTrack(s32 index);
extern void p2sdPlayBGMFade(s32 index);
extern void p2sdStopBGM(s32 index);
extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern s16 fldSkyGetAlphaOfs(void);
extern void fldSkySetAlphaOfs(s32 alpha);
extern void fldSkySetAlpha(s32 alpha);
extern void fldSetFilterAnime(s32 anime);
extern void fldSkySetFilter(s32 r, s32 g, s32 b);
extern void fldSkyDrawFilter(void);
extern void fldDrawFrameSprite(void);
extern void rsetOtOfs(s32 ofs);
extern s32 sdfDevPrintf(s32 x, s32 y, u32 color, s32 flag, const char *format, ...);
extern s32 sceWrite(s32 fd, const void *buffer, s32 size);
extern void p2sdStopSe(s32 sound);
extern void sdfDrawTagLink(void *list, u32 tag);
extern dds3ProcessID_t *dds3SearchProcessName(char *name);
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);

void evtUpdateSkyAlpha(void);
s32 evtSkyProcess1(dds3ProcessID_t *id);
void evtSkyShutdown(dds3ProcessID_t *id);
s32 FrameVarProcess(dds3ProcessID_t *id);
s32 evtGetSoundIndex(s32 eventId, s32 variation);

void evtSetBokasi(s32 bokasi)
{
    fldbokasi = bokasi;
}

void evtSetSkyAlpha(s32 time, s32 alpha)
{
    s16 current;

    current = fldSkyGetAlphaOfs();
    if (current != alpha) {
        if (time == 0) {
            fldSkySetAlphaOfs(alpha);
            skyanime = 0;
        } else {
            skyamax = time;
            skyatarget = current;
            skyabase_par = alpha;
            skyanime = 1;
            skyanow = 0;
        }
    }
}

u16 evtCheckSkyAlphaAnime(void)
{
    return skyanime;
}

void evtUpdateSkyAlpha(void)
{
    if (skyanime != 0) {
        skyanow++;
        fldSkySetAlphaOfs(skyatarget +
                          (s32)((f32)(skyabase_par - skyatarget) * ((f32)skyanow / (f32)skyamax)));
        if (skyanow >= skyamax) {
            skyanime = 0;
        }
    }
}

s32 evtSkyProcess1(dds3ProcessID_t *id)
{
    evtUpdateSkyAlpha();
    fldSkyDrawFilter();
    if (fldbokasi != 0) {
        rsetOtOfs(0x47);
        fldDrawFrameSprite();
    }
    return 0;
}

void evtSkyShutdown(dds3ProcessID_t *id)
{
    skyanime = 0;
    fldbokasi = 0;
}

void evtExitSky(void)
{
    dds3ProcessID_t *id;

    id = dds3SearchProcessName(D_0054F908);
    if (id != NULL) {
        dds3KillProcess(id, 1);
    }
}

void evtInitSky(void)
{
    fldSetFilterAnime(0);
    fldSkySetAlpha(0x80);
    fldSkySetAlphaOfs(0);
    fldSkySetFilter(0, 1, 0);
    dds3StartProcess(D_0054F908, 0x2B0E, 1, 1, evtSkyProcess1, (dds3ProcessFunc_t)evtSkyShutdown, NULL);
}

INCLUDE_ASM(const s32, "code_001F4518", FrameVarProcess);

void evtInitFrameVar(void)
{
    dds3StartProcess(D_0053BC98, 0x2AF9, 1, 1, FrameVarProcess, NULL, NULL);
}

INCLUDE_ASM(const s32, "code_001F4518", evtMenuDisp);

s32 evtFloatChangeHead(void *list, s32 x, s32 y)
{
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, D_0053BCC0));
    return 2;
}

INCLUDE_RODATA(const s32, "code_001F4518", D_0053BCB0);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053BCC0);

INCLUDE_ASM(const s32, "code_001F4518", evtFloatChangeItem);

INCLUDE_ASM(const s32, "code_001F4518", evtFloatChangeMenu);

s32 evtIntChangeHead(void *list, s32 x, s32 y)
{
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, D_0053BCC0));
    return 2;
}

INCLUDE_RODATA(const s32, "code_001F4518", D_0053BD10);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053BD20);

INCLUDE_RODATA(const s32, "code_001F4518", jtbl_0053BD30);

INCLUDE_ASM(const s32, "code_001F4518", evtIntChangeItem);

INCLUDE_ASM(const s32, "code_001F4518", evtIntChangeMenu);

s32 evtFrameChangeHead(void *list, s32 x, s32 y)
{
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

INCLUDE_ASM(const s32, "code_001F4518", evtFrameChangeItem);

INCLUDE_ASM(const s32, "code_001F4518", evtFrameChangeMenu);

INCLUDE_ASM(const s32, "code_001F4518", evtMainMenuItem);

INCLUDE_ASM(const s32, "code_001F4518", evtMainMenu);

INCLUDE_ASM(const s32, "code_001F4518", evtSelectHead);

INCLUDE_ASM(const s32, "code_001F4518", evtSelectItem);

INCLUDE_ASM(const s32, "code_001F4518", evtSelectMenu);

INCLUDE_ASM(const s32, "code_001F4518", evtFileNameHead);

INCLUDE_ASM(const s32, "code_001F4518", evtFileNameItem);

INCLUDE_ASM(const s32, "code_001F4518", evtFileNameMenu);

s32 evtFileName2Head(void *list, s32 x, s32 y)
{
    char name[32];

    sprintf(name, D_0053BEF0, D_0054F824, evt_file_minor);
    sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, D_0054F9B8, name));
    return 2;
}

INCLUDE_RODATA(const s32, "code_001F4518", D_0053BEF0);

INCLUDE_ASM(const s32, "code_001F4518", evtFileName2Item);

INCLUDE_ASM(const s32, "code_001F4518", evtFileName2Menu);

INCLUDE_ASM(const s32, "code_001F4518", evtObjectListSub);

INCLUDE_ASM(const s32, "code_001F4518", evtObjectListItem);

INCLUDE_ASM(const s32, "code_001F4518", evtOjbectListMenu);

INCLUDE_ASM(const s32, "code_001F4518", evtFrameListHead);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053C230);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053C240);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053C250);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053C260);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053C270);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053C280);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053C290);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053C2A0);

INCLUDE_ASM(const s32, "code_001F4518", evtFrameListItemSub);

INCLUDE_ASM(const s32, "code_001F4518", evtFrameListItem);

INCLUDE_ASM(const s32, "code_001F4518", evtFrameListMenu);

INCLUDE_ASM(const s32, "code_001F4518", evtBasicListItem);

INCLUDE_ASM(const s32, "code_001F4518", evtBasicListMenu);

INCLUDE_ASM(const s32, "code_001F4518", evtEventPosItem);

INCLUDE_ASM(const s32, "code_001F4518", evtEventPosMenu);

INCLUDE_ASM(const s32, "code_001F4518", mfilter_Process);

INCLUDE_ASM(const s32, "code_001F4518", evtViewerExit);

INCLUDE_ASM(const s32, "code_001F4518", func_001F79A8);

INCLUDE_ASM(const s32, "code_001F4518", evtViewerDispatch);

s32 evtWriteRecord(s32 fd, const void *buffer, s32 size)
{
    return sceWrite(fd, buffer, size);
}

INCLUDE_ASM(const s32, "code_001F4518", evtCountObjType10);

INCLUDE_ASM(const s32, "code_001F4518", evtCountObjType11);

INCLUDE_ASM(const s32, "code_001F4518", evtCountObjType13);

INCLUDE_ASM(const s32, "code_001F4518", evtCountObjType14);

INCLUDE_ASM(const s32, "code_001F4518", evtCountObjType15);

INCLUDE_ASM(const s32, "code_001F4518", evtCountObjType23);

INCLUDE_ASM(const s32, "code_001F4518", evtCountObjType16);

INCLUDE_ASM(const s32, "code_001F4518", evtCountObjType17);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveViewerHeader);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveViewerInfo);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveFrameList);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveObjectList);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveObjType2);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveObjType10);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveObjType11);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveObjType13);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveObjType14);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveObjType15);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveObjType23);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveObjType16);

INCLUDE_ASM(const s32, "code_001F4518", evtSaveObjType17);

INCLUDE_ASM(const s32, "code_001F4518", evtMakeViewerPath);

INCLUDE_ASM(const s32, "code_001F4518", evtMakeMotionName);

INCLUDE_ASM(const s32, "code_001F4518", evtViewerShutdown);

s32 evtGetSoundIndex(s32 eventId, s32 variation)
{
    s32 index;

    index = 0xC7;
    if (eventId != 0x31F) {
        index = eventId - 0x259;
        if (eventId >= 0x320) {
            index = (eventId < 0x384) ? (eventId - 0x258) : (eventId - 0x29E);
        }
    }
    return ((index + 0x100) << 16) + variation;
}

s32 evtLoadEventBgm(s32 eventId)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    soundIndex = evtGetSoundIndex(eventId, 0);
    p2sdLoadSMGFile(soundIndex);
    return soundIndex;
}

s32 evtCheckEventBgmLoaded(s32 eventId)
{
    if ((u32)(eventId - 0x258) >= 0x100) {
        return 1;
    }
    return p2sdLoadCheckMidi(evtGetSoundIndex(eventId, 0)) == 1;
}

s32 evtPlayEventBgm(s32 eventId, s32 variation)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    soundIndex = evtGetSoundIndex(eventId, variation);
    printf(D_0053C5B8, soundIndex);
    p2sdPlayBGM(soundIndex);
    return soundIndex;
}

void evtTransEventBgm(s32 eventId, s32 variation)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return;
    }
    soundIndex = -1;
    if (variation >= 0) {
        soundIndex = evtGetSoundIndex(eventId, variation);
    }
    printf(D_0053C5D0, soundIndex);
    p2sdSetBgmTrack(soundIndex);
}

s32 evtFadeInEventBgm(s32 eventId, s32 variation)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    soundIndex = evtGetSoundIndex(eventId, variation);
    printf(D_0053C5E8, soundIndex);
    p2sdPlayBGMFade(soundIndex);
    return soundIndex;
}

s32 evtStopEventBgm(s32 eventId, s32 variation)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    soundIndex = evtGetSoundIndex(eventId, variation);
    p2sdStopBGM(soundIndex);
    return soundIndex;
}

s32 evtPlaySe(s32 eventId, s32 variation)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    soundIndex = evtGetSoundIndex(eventId, variation);
    p2sdPlaySE(soundIndex, 0x7F, 0x3F);
    return soundIndex;
}

s32 func_001FA158(s32 eventId, s32 variation)
{
    s32 sound;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtGetSoundIndex(eventId, variation);
    p2sdStopSe(sound);
    return sound;
}

void evtMakeEventPackName(s32 index, char *buffer)
{
    sprintf(buffer, D_0054FAB0, index);
}
INCLUDE_RODATA(const s32, "code_001F4518", D_0053C5B8);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053C5D0);

INCLUDE_RODATA(const s32, "code_001F4518", D_0053C5E8);

INCLUDE_SDATA(const s32, "code_001F4518", skyanime);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F902);

INCLUDE_SDATA(const s32, "code_001F4518", fldbokasi);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F908);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F910);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F918);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F920);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F928);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F930);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F938);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F940);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F948);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F950);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F958);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F960);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F968);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F970);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F978);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F980);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F988);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F990);

INCLUDE_SDATA(const s32, "code_001F4518", evtMenuDisp_cnt);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F99C);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9A0);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9A8);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9B0);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9B8);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9C0);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9C8);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9D0);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9D8);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9E0);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9E8);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9F0);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054F9F8);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA00);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA08);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA10);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA18);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA20);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA28);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA30);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA38);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA40);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA48);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA50);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA58);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA60);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA68);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA70);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA78);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA80);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA88);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA90);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FA98);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FAA0);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FAA8);

INCLUDE_SDATA(const s32, "code_001F4518", D_0054FAB0);

