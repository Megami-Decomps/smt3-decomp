#ifndef SDF_FRAME_H
#define SDF_FRAME_H

/* Frame timers, written in the retail logic tick: one tick per two vblanks,
 * dds3VblanksPerFrame's default. Each macro expands to its argument, so this
 * build is unchanged. A host that runs more ticks per second defines them
 * first, scaling a duration in ticks up and a per-tick step down by the same
 * ratio, so a timer lasts as long and a walk covers as much ground. */

/* A duration in ticks, an integer. */
#ifndef SDF_FRAMES
#define SDF_FRAMES(n) (n)
#endif

/* A duration in ticks held in a float. Its counters step by exactly 1.0f and
 * are compared with ==, so a host must answer a whole number. */
#ifndef SDF_FRAMES_F
#define SDF_FRAMES_F(n) (n)
#endif

/* An advance per tick, in whatever unit the counter moves. */
#ifndef SDF_FRAME_STEP
#define SDF_FRAME_STEP(d) (d)
#endif

/* Storage for a tick count. A host may widen one only where C defines the
 * storage: a symbol the data sections define keeps its retail width. */
#ifndef SDF_FRAME_U8
#define SDF_FRAME_U8 u8
#endif
#ifndef SDF_FRAME_U16
#define SDF_FRAME_U16 u16
#endif
typedef SDF_FRAME_U8 sdfFrameU8_t;
typedef SDF_FRAME_U16 sdfFrameU16_t;

#endif
