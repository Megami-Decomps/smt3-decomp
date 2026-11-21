#include "common.h"

typedef struct dds3StartMenuEntry_t {
    const char *label;
    void (*initialize)(void);
} dds3StartMenuEntry_t;

SIZE_CHECK(dds3StartMenuEntrySize, dds3StartMenuEntry_t, 8);

extern const char dds3StartMenuGameStart[16];
extern const char dds3StartMenuNewBattleTest[16];
extern const char dds3StartMenuBattleFieldTest[24];
extern const char dds3StartMenuModelCollisionEditor[24];
extern const char dds3StartMenuKernelTest[16];
extern const char dds3StartMenuEventTest[16];
extern const char dds3StartMenuEventViewer[16];
extern char D_0054E938[8];
extern const char dds3StartMenuMessageScriptTest[24];
extern const char dds3StartMenuCampTest[16];
extern const char dds3StartMenuCombineTest[16];
extern const char dds3StartMenuShopTest[16];
extern const char dds3StartMenuRagTest[16];
extern const char dds3StartMenuRecoverTest[16];
extern const char dds3StartMenuTerminalTest[16];
extern const char dds3StartMenuResult2Test[16];
extern const char dds3StartMenuNameEntryTest[16];
extern const char dds3StartMenuModelViewer[16];
extern const char dds3StartMenuSoundTest[16];
extern const char dds3StartMenuFontTest[16];
extern const char dds3StartMenuParticleEditor[16];
extern const char dds3StartMenuSpriteViewer[16];
extern const char dds3StartMenuMagatuhiEditor[16];
extern const char dds3StartMenuPuzzleBoyGame[16];
extern const char dds3StartMenuMovieViewer[16];
extern const char dds3StartMenuStaffTest[16];
extern const char dds3StartMenuEndSaveTest[16];
extern char D_0054E930[8];
extern const char dds3StartMenuModeSelect[16];
extern char D_0054E928[8];

extern void InitTitleProcess(void);
extern void InitNewBattleTestProcess(void);
extern void func_001042F0(void);
extern void InitModelCollisionEditorProcess(void);
extern void InitKernelTestProcess(void);
extern void InitEventTestProcess(void);
extern void InitEventViewerProcess(void);
extern void func_00104370(void);
extern void InitMessageScriptTestProcess(void);
extern void InitCampTestProcess(void);
extern void InitCombineTestProcess(void);
extern void InitShopTestProcess(void);
extern void InitRagTestProcess(void);
extern void InitRecoverTestProcess(void);
extern void InitTerminalTestProcess(void);
extern void InitResult2TestProcess(void);
extern void InitNameTestProcess(void);
extern void func_001044E0(void);
extern void InitSoundTestProcess(void);
extern void InitFontTestProcess(void);
extern void InitParticleEditorProcess(void);
extern void InitSpriteViewerProcess(void);
extern void InitMagatuhiEditorProcess(void);
extern void InitPuzzleBoyProcess(void);
extern void InitMovieViewerTestProcess(void);
extern void InitStaffProcess(void);
extern void InitEndSaveProcess(void);
extern void InitPipeProcess(void);
extern void InitModeSelectProcess(void);
extern void InitTgsProcess(void);

/* Retained menu data; the retail start-menu process does not dispatch it. */
dds3StartMenuEntry_t dds3StartMenuTable[30] = {
    { dds3StartMenuGameStart, InitTitleProcess },
    { dds3StartMenuNewBattleTest, InitNewBattleTestProcess },
    { dds3StartMenuBattleFieldTest, func_001042F0 },
    { dds3StartMenuModelCollisionEditor, InitModelCollisionEditorProcess },
    { dds3StartMenuKernelTest, InitKernelTestProcess },
    { dds3StartMenuEventTest, InitEventTestProcess },
    { dds3StartMenuEventViewer, InitEventViewerProcess },
    { D_0054E938, func_00104370 },
    { dds3StartMenuMessageScriptTest, InitMessageScriptTestProcess },
    { dds3StartMenuCampTest, InitCampTestProcess },
    { dds3StartMenuCombineTest, InitCombineTestProcess },
    { dds3StartMenuShopTest, InitShopTestProcess },
    { dds3StartMenuRagTest, InitRagTestProcess },
    { dds3StartMenuRecoverTest, InitRecoverTestProcess },
    { dds3StartMenuTerminalTest, InitTerminalTestProcess },
    { dds3StartMenuResult2Test, InitResult2TestProcess },
    { dds3StartMenuNameEntryTest, InitNameTestProcess },
    { dds3StartMenuModelViewer, func_001044E0 },
    { dds3StartMenuSoundTest, InitSoundTestProcess },
    { dds3StartMenuFontTest, InitFontTestProcess },
    { dds3StartMenuParticleEditor, InitParticleEditorProcess },
    { dds3StartMenuSpriteViewer, InitSpriteViewerProcess },
    { dds3StartMenuMagatuhiEditor, InitMagatuhiEditorProcess },
    { dds3StartMenuPuzzleBoyGame, InitPuzzleBoyProcess },
    { dds3StartMenuMovieViewer, InitMovieViewerTestProcess },
    { dds3StartMenuStaffTest, InitStaffProcess },
    { dds3StartMenuEndSaveTest, InitEndSaveProcess },
    { D_0054E930, InitPipeProcess },
    { dds3StartMenuModeSelect, InitModeSelectProcess },
    { D_0054E928, InitTgsProcess },
};
