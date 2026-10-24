#include "iop.h"

/* MODMIDI.IRX, Sony's MIDI sequencer: it reads a bank's SCEIMidi block
 * and turns it into the channel messages the synthesiser plays.
 * See docs/irx-modules.md.
 *
 * Every chunk is still assembly: the module has no C yet, and the names are
 * the addresses until the IDB has them. */

INCLUDE_ASM("MODMIDI", modmidi_e26);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_Init);
INCLUDE_ASM("MODMIDI", MidiParseHeader);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_Load);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_Unload);
INCLUDE_ASM("MODMIDI", D_000006A4);
INCLUDE_ASM("MODMIDI", modmidi_e5);
INCLUDE_ASM("MODMIDI", D_00000784);
INCLUDE_ASM("MODMIDI", MidiLoopTableAdd);
INCLUDE_ASM("MODMIDI", MidiScanLoops);
INCLUDE_ASM("MODMIDI", MidiTrackSeekAbsolute);
INCLUDE_ASM("MODMIDI", MidiReadVarLen);
INCLUDE_ASM("MODMIDI", MidiTrackReset);
INCLUDE_ASM("MODMIDI", MidiReadDeltaTime);
INCLUDE_ASM("MODMIDI", MidiParseMetaEvent);
INCLUDE_ASM("MODMIDI", MidiOutputPush);
INCLUDE_ASM("MODMIDI", MidiParseExcEvent);
INCLUDE_ASM("MODMIDI", MidiSendChMsg);
INCLUDE_ASM("MODMIDI", MidiChannelReset);
INCLUDE_ASM("MODMIDI", MidiPlayEnv);
INCLUDE_ASM("MODMIDI", MidiStopAllNotes);
INCLUDE_ASM("MODMIDI", MidiTrackServiceTick);
INCLUDE_ASM("MODMIDI", MidiApplyPlayState);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_MidiPlaySwitch);
INCLUDE_ASM("MODMIDI", MidiSelectTrack);
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
INCLUDE_ASM("MODMIDI", MidiSongStopNotes);
INCLUDE_ASM("MODMIDI", D_00002548);
INCLUDE_ASM("MODMIDI", MidiSongApplyVolume);
INCLUDE_ASM("MODMIDI", MidiSongLookup);
INCLUDE_ASM("MODMIDI", MidiSongSelect);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_SelectSong);
INCLUDE_ASM("MODMIDI", MidiSongSetTempo);
INCLUDE_ASM("MODMIDI", MidiSongSetLoopCount);
INCLUDE_ASM("MODMIDI", MidiSongSetFlags);
INCLUDE_ASM("MODMIDI", MidiSongApplyState);
INCLUDE_ASM("MODMIDI", MidiSongRepeat);
INCLUDE_ASM("MODMIDI", MidiPlaySong);
INCLUDE_ASM("MODMIDI", modmidi_e8);
INCLUDE_ASM("MODMIDI", MidiSongServiceTick);
INCLUDE_ASM("MODMIDI", func_000031DC);
INCLUDE_ASM("MODMIDI", modmidi_e10);
INCLUDE_ASM("MODMIDI", modmidi_e11);
INCLUDE_ASM("MODMIDI", modmidi_sceMidi_SongSetAbsolute);
INCLUDE_ASM("MODMIDI", MidiSearchPlay);
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
