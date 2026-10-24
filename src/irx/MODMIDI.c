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
/* FUNC modmidi_e5 */
int modmidi_e5(sceMidiWork_t *work)
{
    sceMidiTbl_t *tbl;
    sceMidiVoiceTbl_t *vt;
    sceMidiSlot_t *slot;
    int unit;

    tbl = work->tbl;
    vt = (sceMidiVoiceTbl_t *)&tbl->nVoiceWord;
    for (unit = 1; unit < tbl->nSeqWord; unit += 2) {
        slot = (sceMidiSlot_t *)((void **)tbl->seq)[unit * 2 + 1];
        if (slot == NULL) {
            continue;
        }
        if (slot->flags & 2) {
            midiTrackServiceTick(slot, vt);
        }
        if (slot->flags & 0x100) {
            midiSongServiceTick(work, unit / 2, slot);
        }
    }
    return 0;
}
/* ENDF modmidi_e5 */
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
/* FUNC midiTrackSeekAbsolute */
u32 midiTrackSeekAbsolute(sceMidiTrack_t *trk)
{
    u32 us;

    us = (trk->tempo << 7) / trk->division;
    us = (us << 8) / trk->speed;
    trk->usPerTick = us;
    return us;
}
/* ENDF midiTrackSeekAbsolute */
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
/* FUNC midiTrackReset */
int midiTrackReset(sceMidiTrack_t *trk)
{
    u8 *p;
    int v;
    u8 c;

    v = 0;
    if (trk->deltaHeld != 0) {
        trk->deltaHeld = 0;
    } else {
        p = trk->cur;
        do {
            c = *p++;
            v = (v << 7) + (c & 0x7F);
        } while (c & 0x80);
        trk->cur = p;
        trk->tick += v;
    }
    return v;
}
/* ENDF midiTrackReset */
/* FUNC midiReadDeltaTime */
int midiReadDeltaTime(sceMidiTrack_t *trk)
{
    int v;

    trk->cur--;
    v = midiReadVarLen(trk);
    trk->cur += v;
    return v;
}
/* ENDF midiReadDeltaTime */
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
/* FUNC midiChannelReset */
int midiChannelReset(sceMidiTrack_t *trk, u8 ch)
{
    u32 prod;
    u32 v;

    prod = trk->chVol[ch] * trk->chExpr[ch] * trk->vol;
    v = prod >> 14;
    if (v >= 0x80) {
        v = 0x7F;
    }
    return v;
}
/* ENDF midiChannelReset */
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
/* FUNC modmidi_e19 */
int modmidi_e19(sceMidiWork_t *work, int slotNo, u8 bpm)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    trk->tempo = 60000000U / bpm;
    midiTrackSeekAbsolute(trk);
    return 0;
}
/* ENDF modmidi_e19 */
/* FUNC modmidi_e20 */
int modmidi_e20(sceMidiWork_t *work, int slotNo)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    u32 bpm;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0 || slot->midiNo == -1 ||
        trk->tempo == 0) {
        return 0;
    }
    bpm = 60000000 / trk->tempo;
    if (bpm < 20) {
        bpm = 20;
    } else if (bpm > 255) {
        bpm = 255;
    }
    return (u8)bpm;
}
/* ENDF modmidi_e20 */
/* FUNC modmidi_sceMidi_MidiSetAbsolute */
int modmidi_sceMidi_MidiSetAbsolute(sceMidiWork_t *work, int slotNo, u16 speed)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    trk->speed = speed;
    midiTrackSeekAbsolute(trk);
    return 0;
}
/* ENDF modmidi_sceMidi_MidiSetAbsolute */
/* FUNC modmidi_e22 */
int modmidi_e22(sceMidiWork_t *work, int slotNo)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return 0;
    }
    if (slot->midiNo == -1) {
        return 0;
    }
    return (u16)trk->speed;
}
/* ENDF modmidi_e22 */
/* FUNC modmidi_e23 */
int modmidi_e23(sceMidiWork_t *work, int slotNo, int tempo)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    trk->tempo = tempo;
    midiTrackSeekAbsolute(trk);
    return 0;
}
/* ENDF modmidi_e23 */
/* FUNC modmidi_e24 */
int modmidi_e24(sceMidiWork_t *work, int slotNo)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return 0;
    }
    if (slot->midiNo == -1) {
        return 0;
    }
    return trk->tempo;
}
/* ENDF modmidi_e24 */
INCLUDE_ASM("MODMIDI", midiSongStopNotes);
INCLUDE_ASM("MODMIDI", D_00002548);
INCLUDE_ASM("MODMIDI", midiSongApplyVolume);
/* FUNC midiSongLookup */
int midiSongLookup(sceMidiWork_t *work, int slotNo, sceMidiSlot_t **pslot, sceMidiTrack_t **ptrk)
{
    sceMidiTbl_t *tbl;
    sceMidiSlot_t *slot;
    u32 unit;

    if (work != NULL) {
        tbl = work->tbl;
        /* nSeqWord counts eight-byte units, so a slot owns units 2n and
         * 2n+1 and its sceMidiSlot_t pointer is the odd one's second word. */
        unit = slotNo * 2 + 1;
        if (unit < tbl->nSeqWord) {
            slot = (sceMidiSlot_t *)((void **)tbl->seq)[unit * 2 + 1];
            *pslot = slot;
            if (pslot == NULL) {
                return 0;
            }
            *ptrk = &slot->trk;
            return 1;
        }
    }
    return 0;
}
/* ENDF midiSongLookup */
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
