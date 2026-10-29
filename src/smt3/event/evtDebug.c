#include "common.h"
#include "scr.h"
#include "basic.h"
#include "newdata.h"
#include "kernel.h"
#include "libsdf.h"

typedef void (*evtOtRegFunc_t)(sdfGrOt_t *, sdfDrawTag_t *);

extern f32 gMoonCnt;

extern dds3ProcessID_t *moonprocess_id;

extern s32 evtDebugState;

extern s32 evtSceneDispWork;

extern u32 dds3BackColor;

extern u8 dds3PadButtonState[];

extern sdfGrOt_t sdfGrOtTable[];


extern s32 sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);

extern void dds3ConsPanelTagLink(sdfDrawTag_t *, s32, s32, s32, s32);

extern void *sdfDevPrintf(u32 aX, u32 aY, u32 aZ, s32 aAttr, const char *aStr, ...);

#include "sdk_libc.h"

extern s32 sdfFileCheckExist(char *aFilename);


extern void scrAllReleaseScript(void);

extern void evtFreeStage(void);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 start, s32 shutdown,
                                         dds3ProcessFunc_t process, dds3ProcessFunc_t finish, void *data);
extern s32 dds3KillProcessName(char *name, s32 shutdown);

extern void evtBitAllOff(void);

extern void evtDebugPrintCamera(void);

s32 EventDebugProcess1(dds3ProcessID_t *id);

extern dds3Basic_t *dds3GetActiveScene(void);
extern dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *scene);
extern dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *scene, dds3Basic_t *camera);
extern f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *camera);
extern void dds3SetScreenFovy(f32 fovy);
extern void *dds3FontReqAllocDrawTag(void);
extern const char D_0054F790[];
extern const char *D_0054F788;
extern u32 evtDebugCameraPage;
extern sdfGrOt_t D_002F6978;

void evtDebugPrintCamera(void)
{
    dds3Basic_t *scene;
    dds3Basic_t *camera;
    dds3Basic_t *nextCamera;
    const char *cameraName;
    sdfDrawTag_t *tag;

    scene = dds3GetActiveScene();
    if (scene == NULL) {
        return;
    }
    if ((s8)dds3PadButtonState[0x2D] < 0) {
        if (evtDebugCameraPage != 0) {
            evtDebugCameraPage = 0;
        } else {
            evtDebugCameraPage = 1;
        }
    }
    if (evtDebugCameraPage == 0) {
        return;
    }

    camera = dds3GetActiveCameraSceneBasic(scene);
    if (camera == NULL) {
        return;
    }
    if ((s8)dds3PadButtonState[0x20] < 0) {
        nextCamera = camera->next;
        if (nextCamera != NULL) {
            camera = nextCamera;
        } else {
            dds3SceneBasic_t *sceneWork = scene->workdata;
            camera = sceneWork->basictable[4].firstbasic;
        }
        dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
        dds3SetScreenFovy(dds3CameraObjectBasicGetFovy(camera));
    } else if ((s8)dds3PadButtonState[0x22] < 0) {
        nextCamera = camera->prev;
        if (nextCamera != NULL) {
            camera = nextCamera;
        } else {
            dds3SceneBasic_t *sceneWork = scene->workdata;
            camera = sceneWork->basictable[4].lastbasic;
        }
        dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), camera);
        dds3SetScreenFovy(dds3CameraObjectBasicGetFovy(camera));
    }

    cameraName = D_0054F788;
    if (camera->name != NULL) {
        cameraName = camera->name;
    }
    tag = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x8680, 0x79C0, 0x00FF0000, 5,
                                     D_0054F790, cameraName));
    D_002F6978.regFunc(&D_002F6978, tag);
}

/* After launching a test script, keep the test task idle and return 0. */
s32 evtDebugCameraStep(dds3ProcessID_t *id)
{
    evtDebugPrintCamera();
    return 0;
}

/* Return the same update callback installed by evtInitEventTest. */
void *evtGetDebugProcess(void)
{
    return (void *)EventDebugProcess1;
}

s32 EventDebugProcess1(dds3ProcessID_t *id)
{
    char path[0x40];
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;

    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    dds3ConsPanelTagLink(tag, 0x84, 0x54, 0x15, 10);
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7840, 0x7BA0, 0xFEFFFF, 0, "Event Debug"));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7A80, 0x7C60, 0xFEFFFF, 6, "E%03d.BF", evtDebugState));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7900, 0x7D20, 0xFEFFFF, 0, "L,R  = EVENT SELECT"));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7900, 0x7D80, 0xFEFFFF, 0, "RR   = ENTER"));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7900, 0x7DE0, 0xFEFFFF, 0, "RU   = SET E500"));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7900, 0x7E40, 0xFEFFFF, 0, "RL   = SET E600"));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7900, 0x7EA0, 0xFEFFFF, 0, "RD   = RESET FLAG"));
    sdfDrawTagLink(tag, (u32)sdfDevPrintf(0x7900, 0x7F00, 0xFEFFFF, 0, "L1R1 = +-100"));
    ot = &sdfGrOtTable[81];
    ((evtOtRegFunc_t)ot->regFunc)(ot, tag);

    if ((s8)dds3PadButtonState[0x21] < 0) {
        sprintf(path, "/event/e%03d/e%03d/scr/e%03d.bf",
                evtDebugState - evtDebugState % 10, evtDebugState, evtDebugState);
        if (sdfFileCheckExist(path) != 0) {
            dds3BackColor = 0x80000000;
            scrStartScriptByName(0x3EB, path, 0);
            return (s32)evtDebugCameraStep;
        }
    }
    if ((s8)dds3PadButtonState[0x23] < 0) {
        evtBitAllOff();
        return (s32)evtGetDebugProcess;
    }
    if (dds3PadButtonState[0x25] & 2) {
        evtDebugState++;
    }
    if ((dds3PadButtonState[0x24] & 2) && evtDebugState > 500) {
        evtDebugState--;
    }
    if ((s8)dds3PadButtonState[0x22] < 0) {
        evtDebugState = 500;
    }
    if ((s8)dds3PadButtonState[0x20] < 0) {
        evtDebugState = 600;
    }
    if ((dds3PadButtonState[0x28] & 2) && evtDebugState > 500) {
        evtDebugState -= 100;
        if (evtDebugState < 500) {
            evtDebugState = 500;
        }
    }
    if (dds3PadButtonState[0x2A] & 2) {
        evtDebugState += 100;
    }
    return 0;
}

/* Test-task teardown destroys the named script processes. */
void EventDebugShutdown(dds3ProcessID_t *id)
{
    scrAllReleaseScript();
}

void evtInitEventTest(void)
{
    dds3BackColor = 0x80000000;
    dds3StartProcess("EventTest", 0x2AF9, 1, 1,
                    (dds3ProcessFunc_t)EventDebugProcess1,
                    (dds3ProcessFunc_t)EventDebugShutdown, NULL);
}

void evtChkEndProcess(void)
{
    dds3KillProcessName("EventTest", 1);
    dds3KillProcessName("PolygonMovie", 0);
    evtFreeStage();
}

INCLUDE_RODATA(const s32, "event/evtDebug", D_0053B308);

INCLUDE_RODATA(const s32, "event/evtDebug", D_0053B318);

INCLUDE_RODATA(const s32, "event/evtDebug", D_0053B328);

INCLUDE_RODATA(const s32, "event/evtDebug", D_0053B338);

INCLUDE_RODATA(const s32, "event/evtDebug", D_0053B348);

#ifdef NON_MATCHING
typedef struct {
    const char *name;
    s32 type;
    union { u16 rowCount; s16 rows; };
    s16 count;
} evtSceneDispRow_t;

typedef struct {
    evtSceneDispRow_t rows[17];
} evtSceneDispRows_t;

extern const evtSceneDispRows_t D_0053B348;
extern s32 evtSceneDispTimer, evtSceneDispState;
extern s8 dds3PadBitTable[];
extern const char D_0054F810[], D_0054F818[];
extern sdfGrOt_t D_002F6938;
extern s32 dds3DebugBasic(dds3ProcessID_t *id);
extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);

s32 evtSceneDispProcess(dds3ProcessID_t *id)
{
    evtSceneDispRows_t rows = D_0053B348;
    evtSceneDispRow_t *row;
    sdfDrawTag_t *textTag, *panelTag;
    dds3Basic_t *scene, *basic;
    dds3SceneBasic_t *sceneWork;
    s32 rowIndex, line, x, column, attr, selected;
    s32 delta;

    textTag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    selected = 0;
    sdfDrawTagInit(textTag);
    scene = dds3GetActiveScene();
    if (scene == NULL) {
        rowIndex = 0;
        sdfDrawTagLink(textTag, (u32)sdfDevPrintf(0x7180, (4 - evtSceneDispWork) * 0x60 + 0x7900,
                                             0xFEFFFF, 0, "NO SCENE"));
        sceneWork = NULL;
        line = 3;
    } else {
        line = 2;
        rowIndex = 0;
        sceneWork = scene->workdata;
        for (row = rows.rows; row->type != 0; row++) {
            row->count = 0;
            row->rowCount = 0;
            attr = 0;
            if (evtSceneDispTimer == rowIndex) {
                if (evtSceneDispState != 0) {
                    attr = 5;
                } else {
                    attr = 4;
                    selected = 1;
                }
            }
            column = 0;
            sdfDrawTagLink(textTag, (u32)sdfDevPrintf(0x7180, (line - evtSceneDispWork) * 0x60 + 0x7900,
                                                 0xFEFFFF, attr, row->name));
            sdfDrawTagLink(textTag, (u32)sdfDevPrintf(0x7900, (line - evtSceneDispWork) * 0x60 + 0x7900,
                                                 0xFEFFFF, attr, D_0054F810,
                                                 sceneWork->basictable[row->type].linkcount));
            basic = sceneWork->basictable[row->type].firstbasic;
            if (basic != NULL) {
                x = 0x7CC0;
                do {
                    attr = 0xE;
                    if (evtSceneDispTimer == rowIndex && row->count == evtSceneDispState - 1) {
                        attr = 4;
                        selected = 1;
                    }
                    column++;
                    sdfDrawTagLink(textTag, (u32)sdfDevPrintf(x, (line - evtSceneDispWork) * 0x60 + 0x7900,
                                                         0xFEFFFF, attr, D_0054F818, basic->serialID));
                    x += 0x480;
                    if (column >= 4) {
                        x = 0x7CC0;
                        column = 0;
                        line++;
                        row->rowCount++;
                    }
                    basic = basic->next;
                    row->count++;
                } while (basic != NULL);
            }
            if (row->count > 0 && ((u16)row->count & 3) == 0) {
                line--;
                row->rowCount--;
            }
            line++;
            rowIndex++;
        }
    }
    panelTag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(panelTag);
    dds3ConsPanelTagLink(panelTag, 0x18, (2 - evtSceneDispWork) * 0xC, 0x27, line);
    D_002F6938.regFunc(&D_002F6938, panelTag);
    D_002F6938.regFunc(&D_002F6938, textTag);
    if (!selected) {
        evtSceneDispTimer = 0;
        evtSceneDispState = 0;
        return 0;
    }
    if (dds3PadBitTable[23] & 2) {
        if (evtSceneDispState == 0) {
            if (evtSceneDispTimer < rowIndex - 1) {
                delta = rows.rows[evtSceneDispTimer].rows;
                evtSceneDispWork += delta != 0 ? delta : 1;
                evtSceneDispTimer++;
            }
        } else {
            delta = evtSceneDispState + 4;
            *(volatile s32 *)&evtSceneDispState =
                delta <= rows.rows[evtSceneDispTimer].count ? delta : evtSceneDispState;
        }
    }
    if (dds3PadBitTable[22] & 2) {
        if (evtSceneDispState != 0) {
            delta = evtSceneDispState - 4;
            if (delta > 0) {
                evtSceneDispState = delta;
            }
        } else if (evtSceneDispTimer > 0) {
            evtSceneDispTimer--;
            delta = rows.rows[evtSceneDispTimer].rows;
            evtSceneDispWork -= delta != 0 ? delta : 1;
        }
    }
    if (dds3PadBitTable[21] & 2) {
        if (evtSceneDispState > 0) {
            delta = evtSceneDispState + 1;
            *(volatile s32 *)&evtSceneDispState =
                delta <= rows.rows[evtSceneDispTimer].count ? delta : evtSceneDispState;
        }
    }
    if (dds3PadBitTable[20] & 2) {
        if (evtSceneDispState > 0) {
            delta = evtSceneDispState - 1;
            *(volatile s32 *)&evtSceneDispState = delta > 0 ? delta : evtSceneDispState;
        }
    }
    if (dds3PadBitTable[17] < 0 && scene != NULL) {
        if (evtSceneDispState == 0) {
            if (rows.rows[evtSceneDispTimer].count > 0) {
                evtSceneDispState = 1;
            }
        } else {
            delta = evtSceneDispState - 1;
            basic = sceneWork->basictable[rows.rows[evtSceneDispTimer].type].firstbasic;
            while (delta > 0) {
                basic = basic->next;
                delta--;
            }
            dds3SetProcessData(id, basic);
            return (s32)dds3DebugBasic;
        }
    }
    if (dds3PadBitTable[19] < 0) {
        if (evtSceneDispState != 0) {
            evtSceneDispState = 0;
            return 0;
        }
        return -1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtDebug", evtSceneDispProcess);
#endif

void func_001EBC90(void)
{
    evtSceneDispWork = 0;
}

