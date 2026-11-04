#ifndef SDF_BUILD_HOOK_H
#define SDF_BUILD_HOOK_H

#include "common.h"

/* Optional observers of the engine's packet builders and draw producers. A host
 * that wants them sets the pointers; all stay NULL otherwise. Off the matching
 * build the macros below expand to the retail statement alone. */
#ifdef NON_MATCHING

typedef struct sdfBuildHook_t
{
    /* Replaces sdfModelLinkOt wholesale when set; aRa is its caller. */
    void (*linkOt)(void *aOt, void *aRoot, u32 aRa);
    /* Open and close bracket a producer; they nest, close ends the latest open. */
    void (*open)(const char *aFn, u32 aRa, const u32 *aArgs);
    void (*close)(void);
    /* A builder wrote or returned a packet; aRa is 0 where the packet is returned. */
    void (*packet)(const char *aFn, const char *aType, u32 aPacket, u32 aRa);
} sdfBuildHook_t;

extern sdfBuildHook_t sdfBuildHook;

#define SDF_BUILD_LINK_OT(aOt, aRoot) \
    do { \
        if (sdfBuildHook.linkOt != NULL) { \
            sdfBuildHook.linkOt(aOt, aRoot, (u32)__builtin_return_address(0)); \
            return; \
        } \
    } while (0)
#define SDF_BUILD_OPEN(aFn, args...) \
    do { \
        u32 buildArgs_[8] = { args }; \
        if (sdfBuildHook.open != NULL) sdfBuildHook.open(aFn, (u32)__builtin_return_address(0), buildArgs_); \
    } while (0)
#define SDF_BUILD_CLOSE() \
    do { if (sdfBuildHook.close != NULL) sdfBuildHook.close(); } while (0)
#define SDF_BUILD_PACKET(aFn, aType, aPacket) \
    do { if (sdfBuildHook.packet != NULL) sdfBuildHook.packet(aFn, aType, (u32)(aPacket), (u32)__builtin_return_address(0)); } while (0)
/* return aExpr, recording the returned packet first; the _CLOSE form also ends a producer. */
#define SDF_BUILD_RETURN(aFn, aType, aExpr) \
    do { \
        __typeof__(aExpr) built_ = (aExpr); \
        if (sdfBuildHook.packet != NULL) sdfBuildHook.packet(aFn, aType, (u32)built_, 0); \
        return built_; \
    } while (0)
#define SDF_BUILD_RETURN_CLOSE(aFn, aType, aExpr) \
    do { \
        __typeof__(aExpr) built_ = (aExpr); \
        if (sdfBuildHook.packet != NULL) sdfBuildHook.packet(aFn, aType, (u32)built_, 0); \
        if (sdfBuildHook.close != NULL) sdfBuildHook.close(); \
        return built_; \
    } while (0)

#else

#define SDF_BUILD_LINK_OT(aOt, aRoot)
#define SDF_BUILD_OPEN(aFn, args...)
#define SDF_BUILD_CLOSE()
#define SDF_BUILD_PACKET(aFn, aType, aPacket)
#define SDF_BUILD_RETURN(aFn, aType, aExpr) return aExpr
#define SDF_BUILD_RETURN_CLOSE(aFn, aType, aExpr) return aExpr

#endif

#endif
