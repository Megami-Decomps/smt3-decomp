#include "MODMIDI.h"

/* MODMIDI.IRX, Sony's MIDI sequencer: it reads a bank's SCEIMidi block
 * and turns it into the channel messages the synthesiser plays.
 * See docs/irx-modules.md.
 *
 * The chunks still in assembly are the ones no C has been written for yet,
 * plus the import stubs and the import and export tables between them. */

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
/* FUNC midiSelectTrack */
int midiSelectTrack(sceMidiSlot_t *slot)
{
    sceMidiTrack_t *trk;
    int i;

    trk = &slot->trk;
    slot->tick = 0;
    trk->tick = 0;
    trk->frac = 0;
    trk->usPerTick = 0;
    trk->runStatus = 0;
    trk->deltaHeld = 0;
    trk->ctlLoopId = 0xFF;
    trk->ctlNrpn = 0xFF;
    trk->ctlLoopCnt = 0xFF;
    trk->ctlRpn = 0xFF;
    trk->cur = trk->smfData;
    for (i = 0; i < 16; i++) {
        trk->chVol[i] = 0x7F;
        trk->ch[i].prog = 0xFF;
        trk->ch[i].bank = 0xFF;
        trk->ch[i].cc1 = 0xFF;
        trk->ch[i].cc2 = 0xFF;
        trk->ch[i].cc5 = 0xFF;
        trk->ch[i].vol = 0xFF;
        trk->ch[i].pan = 0xFF;
        trk->ch[i].expr = 0xFF;
        trk->ch[i].sustain = 0xFF;
        trk->ch[i].portamento = 0xFF;
        trk->ch[i].bend = 0xFFFF;
    }
    midiTrackSeekAbsolute(trk);
    midiTrackReset(trk);
    return 1;
}
/* ENDF midiSelectTrack */
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_SelectMidi);
INCLUDE_ASM("MODMIDI", modmidi_e16);
/* FUNC func_00001FE4 */
void func_00001FE4(sceMidiSlot_t *slot, sceMidiTrack_t *trk, sceMidiVoiceTbl_t *vt, u32 ch)
{
    u32 vol;
    u32 msg;

    if (slot->flags & 2) {
        vol = midiChannelReset(trk, ch);
        msg = (vol << 16) | 0x7B0;
        midiSendChMsg(slot, trk, vt, ch | msg, 3);
    }
}
/* ENDF func_00001FE4 */
#ifdef NON_MATCHING
/* The two return -1 paths come out as one shared tail where retail has
 * two, so the branch arrangement differs. */
/* FUNC modmidi_sceMidi_MidiSetVolume */
int modmidi_sceMidi_MidiSetVolume(sceMidiWork_t *work, int slotNo, u8 ch, u8 vol)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    sceMidiVoiceTbl_t *vt;
    u8 i;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    vt = (sceMidiVoiceTbl_t *)&work->tbl->nVoiceWord;
    if (ch < 16) {
        trk->chExpr[ch] = vol;
        func_00001FE4(slot, trk, vt, ch);
        return 0;
    }
    if (ch != 0xFF) {
        return -1;
    }
    trk->vol = vol;
    if (slot->flags & 2) {
        for (i = 0; i < 16; i++) {
            func_00001FE4(slot, trk, vt, i);
        }
    }
    return 0;
}
/* ENDF modmidi_sceMidi_MidiSetVolume */
#else
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_MidiSetVolume);
#endif
#ifdef NON_MATCHING
/* Two instructions longer: gcc reloads trk from its stack slot after the
 * chVol store instead of keeping it in a register. */
/* FUNC modmidi_e18 */
int modmidi_e18(sceMidiWork_t *work, int slotNo, u8 ch, s8 vol)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    sceMidiVoiceTbl_t *vt;
    u8 v;
    u8 i;

    v = vol;
    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    vt = (sceMidiVoiceTbl_t *)&work->tbl->nVoiceWord;
    if (vol < 0) {
        v = 0x7F;
    }
    if (ch < 16) {
        trk->chVol[ch] = v;
        trk->ch[ch].vol = v;
        func_00001FE4(slot, trk, vt, ch);
        return 0;
    }
    if (ch != 0xFF) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        trk->chVol[i] = v;
        trk->ch[i].vol = v;
        func_00001FE4(slot, trk, vt, i);
    }
    return 0;
}
/* ENDF modmidi_e18 */
#else
INCLUDE_ASM("MODMIDI", modmidi_e18);
#endif
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
/* FUNC midiSongStopNotes */
void midiSongStopNotes(sceMidiWork_t *work, int slotNo, u32 msg, int len)
{
    sceMidiTbl_t *tbl;
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;
    sceMidiVoiceTbl_t *vt;
    u32 *seg;
    u32 ofs;
    u32 ch;

    tbl = work->tbl;
    /* The pair at sceMidiSeq_t+8 is a {count, pointer} segment of its own. */
    ofs = (slotNo << 4) | 8;
    seg = (u32 *)(ofs + (u32)tbl->seq);
    slot = (sceMidiSlot_t *)seg[1];
    if (slot != NULL) {
        trk = &slot->trk;
        vt = (sceMidiVoiceTbl_t *)&tbl->nVoiceWord;
        msg &= 0xFFFFF0;
        for (ch = 0; ch < 16; ch++) {
            midiSendChMsg(slot, trk, vt, msg | ch, len);
        }
    }
}
/* ENDF midiSongStopNotes */
INCLUDE_ASM("MODMIDI", D_00002548);
/* FUNC midiSongApplyVolume */
#ifdef NON_MATCHING
/* Two instructions longer: gcc keeps trk->fadeLeft in $a0 and has to save
 * work in another register. */
void midiSongApplyVolume(sceMidiWork_t *work, int slotNo, sceMidiTrack_t *trk)
{
    int v;
    int t;

    if (trk->fadeLeft != 0) {
        v = (trk->curMidi - trk->nextMidi) * trk->fadeLeft / trk->fadeTicks +
            trk->nextMidi;
    } else {
        v = trk->curMidi;
    }
    t = v * trk->songVol;
    v = t / 128;
    if (trk->unk12F != v) {
        modmidi_sceMidi_MidiSetVolume(work, slotNo, 0xFF, v);
        trk->unk12F = v;
    }
}
#else
INCLUDE_ASM("MODMIDI", midiSongApplyVolume);
#endif
/* ENDF midiSongApplyVolume */
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
/* FUNC func_000031DC */
int func_000031DC(sceMidiWork_t *work, int slotNo, u8 vol)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    trk->songVol = vol;
    if (slot->flags & 0x200) {
        midiSongApplyVolume(work, slotNo, trk);
    }
    return 0;
}
/* ENDF func_000031DC */
/* FUNC modmidi_e10 */
int modmidi_e10(sceMidiWork_t *work, int slotNo, u8 val)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    midiSongSetTempo(trk, val);
    if (slot->flags & 0x200) {
        midiSongApplyVolume(work, slotNo, trk);
    }
    return 0;
}
/* ENDF modmidi_e10 */
/* FUNC modmidi_e11 */
int modmidi_e11(sceMidiWork_t *work, int slotNo, u8 val)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    midiSongSetFlags(trk, val);
    if (slot->flags & 0x200) {
        modmidi_e19(work, slotNo, trk->unk133);
    }
    return 0;
}
/* ENDF modmidi_e11 */
/* FUNC modmidi_sceMidi_SongSetAbsolute */
int modmidi_sceMidi_SongSetAbsolute(sceMidiWork_t *work, int slotNo, u16 speed)
{
    sceMidiSlot_t *slot;
    sceMidiTrack_t *trk;

    if (midiSongLookup(work, slotNo, &slot, &trk) == 0) {
        return -1;
    }
    trk->songSpeed = speed;
    if (slot->flags & 0x200) {
        modmidi_sceMidi_MidiSetAbsolute(work, slotNo, trk->songSpeed);
    }
    return 0;
}
/* ENDF modmidi_sceMidi_SongSetAbsolute */
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
