#ifndef SDF_FRAME_H
#define SDF_FRAME_H

/* Frame timers, written in the retail logic tick: one tick per two vblanks,
 * dds3VblanksPerFrame's default. Each macro expands to its argument, so this
 * build is unchanged. A host that runs more ticks per second defines them
 * first, scaling a duration in ticks up and a per-tick step down by the same
 * ratio, so a timer lasts as long and a walk covers as much ground. */

/* A duration in ticks, an integer. A count of vblanks scales by the same
 * ratio, since a host keeps two vblanks to a tick. */
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

/* A factor applied once per tick, such as a velocity's decay. A host raises
 * it to the power it scales a step by, so the decay over a second holds. */
#ifndef SDF_FRAME_DECAY
#define SDF_FRAME_DECAY(k) (k)
#endif

/* The retail tick a host tick count falls in, an integer: the inverse of
 * SDF_FRAMES, for a key or a parity that must advance at the retail rate. */
#ifndef SDF_FRAME_TICK
#define SDF_FRAME_TICK(age) (age)
#endif

/* True on the first host tick of each retail tick, given a count of host
 * ticks from 0. Gates a step that cannot be scaled, such as a record into a
 * history ring or an integer ramp, so it runs at the retail rate. */
#ifndef SDF_FRAME_DUE
#define SDF_FRAME_DUE(age) 1
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
