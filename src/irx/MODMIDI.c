#include "iop.h"

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
INCLUDE_ASM("MODMIDI", midiLoopTableAdd);
INCLUDE_ASM("MODMIDI", midiScanLoops);
INCLUDE_ASM("MODMIDI", midiTrackSeekAbsolute);
INCLUDE_ASM("MODMIDI", midiReadVarLen);
INCLUDE_ASM("MODMIDI", midiTrackReset);
INCLUDE_ASM("MODMIDI", midiReadDeltaTime);
INCLUDE_ASM("MODMIDI", midiParseMetaEvent);
INCLUDE_ASM("MODMIDI", midiOutputPush);
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
INCLUDE_ASM("MODMIDI", midiSongSelect);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_SelectSong);
INCLUDE_ASM("MODMIDI", midiSongSetTempo);
INCLUDE_ASM("MODMIDI", midiSongSetLoopCount);
INCLUDE_ASM("MODMIDI", midiSongSetFlags);
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
INCLUDE_ASM("MODMIDI", modmidi_e3);
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
