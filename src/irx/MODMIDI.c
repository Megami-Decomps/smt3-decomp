#include "MODMIDI.h"

/* MODMIDI.IRX, Sony's MIDI sequencer: it reads a bank's SCEIMidi block
 * and turns it into the channel messages the synthesiser plays.
 * See docs/irx-modules.md.
 *
 * Every chunk is still assembly: the module has no C yet, and the names are
 * the addresses until the IDB has them. */

INCLUDE_ASM("MODMIDI", modmidi_e26);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_Init);
INCLUDE_ASM("MODMIDI", midiParseHeader);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_Load);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_Unload);
INCLUDE_ASM("MODMIDI", D_000006A4);
INCLUDE_ASM("MODMIDI", modmidi_e5);
INCLUDE_ASM("MODMIDI", D_00000784);
/* FUNC midiLoopTableAdd */
sceMidiLoop_t *midiLoopTableAdd(sceMidiLoop_t *tbl, u8 id)
{
    int i;

    for (i = 7; i != -1; i--, tbl++) {
        if (tbl->id == id) {
            return tbl;
        }
    }
    return NULL;
}
/* ENDF midiLoopTableAdd */
INCLUDE_ASM("MODMIDI", midiScanLoops);
INCLUDE_ASM("MODMIDI", midiTrackSeekAbsolute);
/* FUNC midiReadVarLen */
int midiReadVarLen(sceMidiTrack_t *trk)
{
    u8 *p;
    int v;
    u8 c;

    p = trk->cur;
    v = 0;
    do {
        c = *p++;
        v = (v << 7) + (c & 0x7F);
    } while (c & 0x80);
    trk->cur = p;
    return v;
}
/* ENDF midiReadVarLen */
INCLUDE_ASM("MODMIDI", midiTrackReset);
INCLUDE_ASM("MODMIDI", midiReadDeltaTime);
INCLUDE_ASM("MODMIDI", midiParseMetaEvent);
/* FUNC midiOutputPush */
sceMidiVoice_t *midiOutputPush(u32 index, sceMidiVoiceTbl_t *vt)
{
    if (index >= vt->nVoiceWord) {
        return NULL;
    }
    return &vt->voice[index];
}
/* ENDF midiOutputPush */
INCLUDE_ASM("MODMIDI", midiParseExcEvent);
INCLUDE_ASM("MODMIDI", midiSendChMsg);
INCLUDE_ASM("MODMIDI", midiChannelReset);
INCLUDE_ASM("MODMIDI", midiPlayEnv);
INCLUDE_ASM("MODMIDI", midiStopAllNotes);
INCLUDE_ASM("MODMIDI", midiTrackServiceTick);
INCLUDE_ASM("MODMIDI", midiApplyPlayState);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_MidiPlaySwitch);
INCLUDE_ASM("MODMIDI", midiSelectTrack);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_SelectMidi);
INCLUDE_ASM("MODMIDI", modmidi_e16);
INCLUDE_ASM("MODMIDI", func_00001FE4);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_MidiSetVolume);
INCLUDE_ASM("MODMIDI", modmidi_e18);
INCLUDE_ASM("MODMIDI", modmidi_e19);
INCLUDE_ASM("MODMIDI", modmidi_e20);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_MidiSetAbsolute);
INCLUDE_ASM("MODMIDI", modmidi_e22);
INCLUDE_ASM("MODMIDI", modmidi_e23);
INCLUDE_ASM("MODMIDI", modmidi_e24);
INCLUDE_ASM("MODMIDI", midiSongStopNotes);
INCLUDE_ASM("MODMIDI", D_00002548);
INCLUDE_ASM("MODMIDI", midiSongApplyVolume);
INCLUDE_ASM("MODMIDI", midiSongLookup);
/* FUNC midiSongSelect */
void midiSongSelect(sceMidiSlot_t *slot, u8 **songCur)
{
    int i;

    slot->flags &= ~0x700;
    songCur[1] = songCur[0];
    /* songCur is &trk->songData, so index 7 + 2 * i is repeat[i].cursor. */
    for (i = 7; i >= 0; i--) {
        songCur[7 + i * 2] = NULL;
    }
}
/* ENDF midiSongSelect */
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_SelectSong);
/* FUNC midiSongSetTempo */
void midiSongSetTempo(sceMidiTrack_t *trk, int val)
{
    if (val < 0) {
        val = 0;
    } else if (val > 0x80) {
        val = 0x80;
    }
    trk->curMidi = val;
}
/* ENDF midiSongSetTempo */
/* FUNC midiSongSetLoopCount */
void midiSongSetLoopCount(sceMidiTrack_t *trk, int val)
{
    if (val < 0) {
        val = 0;
    } else if (val > 0x7F) {
        val = 0x7F;
    }
    trk->loopCount = val;
}
/* ENDF midiSongSetLoopCount */
/* FUNC midiSongSetFlags */
void midiSongSetFlags(sceMidiTrack_t *trk, int val)
{
    if (val < 0xA) {
        val = 0xA;
    } else if (val > 0xFF) {
        val = 0xFF;
    }
    trk->unk133 = val;
}
/* ENDF midiSongSetFlags */
INCLUDE_ASM("MODMIDI", midiSongApplyState);
INCLUDE_ASM("MODMIDI", midiSongRepeat);
INCLUDE_ASM("MODMIDI", midiPlaySong);
INCLUDE_ASM("MODMIDI", modmidi_e8);
INCLUDE_ASM("MODMIDI", midiSongServiceTick);
INCLUDE_ASM("MODMIDI", func_000031DC);
INCLUDE_ASM("MODMIDI", modmidi_e10);
INCLUDE_ASM("MODMIDI", modmidi_e11);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_SongSetAbsolute);
INCLUDE_ASM("MODMIDI", midiSearchPlay);
INCLUDE_ASM("MODMIDI", modmidi_e13);
INCLUDE_ASM("MODMIDI", D_000036C0);
/* FUNC modmidi_e3 */
void modmidi_e3(void)
{
}
/* ENDF modmidi_e3 */
INCLUDE_ASM("MODMIDI", D_00003748);
INCLUDE_ASM("MODMIDI", intrman_CpuSuspendIntr);
INCLUDE_ASM("MODMIDI", intrman_CpuResumeIntr);
INCLUDE_ASM("MODMIDI", D_00003774);
INCLUDE_ASM("MODMIDI", loadcore_ReleaseLibraryEntries);
INCLUDE_ASM("MODMIDI", D_00003798);
INCLUDE_ASM("MODMIDI", stdio_printf);
INCLUDE_ASM("MODMIDI", D_000037BC);
INCLUDE_ASM("MODMIDI", sysclib_look_ctype_table);
INCLUDE_ASM("MODMIDI", sysclib_strlen);
INCLUDE_ASM("MODMIDI", sysclib_strncmp);
INCLUDE_ASM("MODMIDI", sysclib_strtol);
INCLUDE_ASM("MODMIDI", D_000037F8);
