#include "common.h"
#include "kernel.h"

extern char D_0054F908[];

extern char D_0054F9B8[];




extern char D_0054FAB0[];

extern u16 skyanime;

extern s32 fldbokasi;

extern s32 printf(const char *fmt, ...);

extern s32 sprintf(char *buf, const char *fmt, ...);

extern s32 p2sdLoadSMGFile(s32 index);

extern s32 p2sdLoadCheckMidi(s32 index);

extern void p2sdPlayBGM(s32 index);

extern void p2sdSetBgmTrack(s32 index);

extern void p2sdPlayBGMFade(s32 index);

extern void p2sdStopBGM(s32 index);

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern void p2sdStopSe(s32 sound);

s32 evtGetSoundIndex(s32 eventId, s32 variation);

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
    printf("Event BGM play :%08X\n", soundIndex);
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
    printf("Event BGM trans :%08X\n", soundIndex);
    p2sdSetBgmTrack(soundIndex);
}

s32 evtFadeInEventBgm(s32 eventId, s32 variation)
{
    s32 soundIndex;

    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    soundIndex = evtGetSoundIndex(eventId, variation);
    printf("Event BGM fade in play :%08X\n", soundIndex);
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

