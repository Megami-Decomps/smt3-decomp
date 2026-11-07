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

/* A countdown decremented at the end of each tick and acted on at 0, set
 * mid-tick. Its value less one counts the ticks still run, so a host scales
 * that part and keeps 1 meaning "ends with this tick". */
#ifndef SDF_FRAMES_COUNTDOWN
#define SDF_FRAMES_COUNTDOWN(n) (n)
#endif

/* An advance per tick, in whatever unit the counter moves. */
#ifndef SDF_FRAME_STEP
#define SDF_FRAME_STEP(d) (d)
#endif

/* Runs where a floor face's edge band reached a sphere, before the sphere is
 * moved to that clearance. Below retail's step the move pulls a sphere above
 * the floor into the next face, which pushes it back each tick, so a host
 * that shortens the step keeps it where it is. Expands to nothing here. */
#ifndef SDF_FRAME_FLOOR_EDGE
#define SDF_FRAME_FLOOR_EDGE(clearance, distance, signedDistance)
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

/* Runs one statement on the first host tick of each retail tick. */
#ifndef SDF_FRAME_WHEN
#define SDF_FRAME_WHEN(age, stmt) stmt
#endif

/* The bits of a key mask, read only on the first host tick of each retail
 * tick, so a held key repeats at the retail rate. */
#ifndef SDF_FRAME_MASK
#define SDF_FRAME_MASK(age, bits) (bits)
#endif

/* Brackets the reads of positions recorded once per retail tick by parts
 * that follow a moving mark: pos is the mark at the last retail tick and
 * posOld the one before. A host running more ticks than retail may move the
 * followers between retail ticks, reading the mark itself from root's node
 * mark, and must put pos and posOld back at END so the next record still
 * spans one retail tick. Retail reads the recorded positions, so both expand
 * to nothing. */
#ifndef SDF_FRAME_FOLLOW_BEGIN
#define SDF_FRAME_FOLLOW_BEGIN(age, pos, posOld, root, mark)
#endif
#ifndef SDF_FRAME_FOLLOW_END
#define SDF_FRAME_FOLLOW_END(pos, posOld)
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
