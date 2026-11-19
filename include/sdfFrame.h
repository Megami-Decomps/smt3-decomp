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

/* SDF_FRAME_FRACTION of a share f that is not a constant. Retail's value is f
 * as written, where 1 - (1 - f) would not fold. */
#ifndef SDF_FRAME_SHARE
#define SDF_FRAME_SHARE(f) (f)
#endif

/* A turn by step, a share of the distance left that is never under floor.
 * Retail's value is step. */
#ifndef SDF_FRAME_CHASE
#define SDF_FRAME_CHASE(step, left, floor) (step)
#endif

/* A mover's tick: SDF_FRAME_MOVE_FROM names the vector it starts from,
 * SDF_FRAME_MOVE and SDF_FRAME_MOVE_HIT give axis i's value x without and with
 * a collision result, and SDF_FRAME_MOVE_END runs after the collision test with
 * the mover's basic and vector. Retail's values are x, and FROM and END do
 * nothing. */
#ifndef SDF_FRAME_MOVE_FROM
#define SDF_FRAME_MOVE_FROM(v) do { } while (0)
#endif
#ifndef SDF_FRAME_MOVE
#define SDF_FRAME_MOVE(i, x) (x)
#endif
#ifndef SDF_FRAME_MOVE_HIT
#define SDF_FRAME_MOVE_HIT(i, x) (x)
#endif
#ifndef SDF_FRAME_MOVE_END
#define SDF_FRAME_MOVE_END(basic, v) do { } while (0)
#endif

/* An advance per tick, in whatever unit the counter moves. */
#ifndef SDF_FRAME_STEP
#define SDF_FRAME_STEP(d) (d)
#endif

/* A float counter x raised or lowered by the step per tick d to a limit that
 * retail's whole steps land on exactly. A host scales d and stops x at the
 * limit. */
#ifndef SDF_FRAME_STEP_UP
#define SDF_FRAME_STEP_UP(x, d, max) ((x) + (d))
#endif
#ifndef SDF_FRAME_STEP_DOWN
#define SDF_FRAME_STEP_DOWN(x, d, min) ((x) - (d))
#endif

/* Runs one statement on every host tick of a retail tick but its last, such as
 * holding a state that retail keeps for a whole tick. Expands to nothing here. */
#ifndef SDF_FRAME_BEFORE_LAST
#define SDF_FRAME_BEFORE_LAST(stmt)
#endif

/* Runs one statement on the last host tick of each retail tick only, such as
 * clearing a draw request that a step made once per retail tick. */
#ifndef SDF_FRAME_LAST
#define SDF_FRAME_LAST(stmt) stmt
#endif

/* The field player's step and the field camera's update, which a host may run
 * its own way, and a statement that runs on each host tick the step ran. */
#ifndef SDF_FRAME_PLAYER_CALC
#define SDF_FRAME_PLAYER_CALC() fldPlayerCalc()
#endif
#ifndef SDF_FRAME_CAM_MAIN
#define SDF_FRAME_CAM_MAIN() fldCamMain()
#endif
#ifndef SDF_FRAME_WHEN_STEPPED
#define SDF_FRAME_WHEN_STEPPED(stmt) stmt
#endif
/* fldPlayerPosRot, which copies the player's pose as shown into the field
 * state. A host that shows poses between ticks may keep the state at R. */
#ifndef SDF_FRAME_POS_ROT
#define SDF_FRAME_POS_ROT() fldPlayerPosRot()
#endif

/* The stage a sequence's switch runs: a host that runs it on every host tick
 * may hold every stage but the steady one to one step a retail tick. */
#ifndef SDF_FRAME_STAGE
#define SDF_FRAME_STAGE(stage, steady) (stage)
#endif

/* Whether a battle action process runs its sequence's check on this host tick,
 * given its seq and stat. A host may hold it to one check a retail tick. */
#ifndef SDF_FRAME_ACTION_CHECK
#define SDF_FRAME_ACTION_CHECK(seq, stat) 1
#endif

/* Whether the kernel calls process id's step function on this host tick. A
 * host may hold a function the process switched to until the next retail
 * tick's first host tick, so a chain of one-shot steps takes a tick a step. */
#ifndef SDF_FRAME_PROCESS_RUN
#define SDF_FRAME_PROCESS_RUN(id) 1
#endif

/* A countdown below zero that a host keeps until the next retail tick's first
 * host tick, skipped like a paused one, so it ends where retail ends it. */
#ifndef SDF_FRAME_EXPIRED
#define SDF_FRAME_EXPIRED(count) 0
#endif

/* One step of a script process: the interpreter up to its next yield. A host
 * may skip it on some host ticks, returning idle. */
#ifndef SDF_FRAME_SCRIPT_STEP
#define SDF_FRAME_SCRIPT_STEP(id, step, idle) (step)
#endif
/* A script command reads the pad. Expands to nothing here. */
#ifndef SDF_FRAME_SCRIPT_READS_PAD
#define SDF_FRAME_SCRIPT_READS_PAD()
#endif

/* An event unit's step (its script-driven move, turn, timers and motion sets),
 * which a host may run its own way. */
#ifndef SDF_FRAME_UNIT_STEP
#define SDF_FRAME_UNIT_STEP(basic, stmt) stmt
#endif

/* An advance per tick added to an integer. A host splits d into whole steps
 * that add up to exactly d over each retail tick, where a rounded
 * SDF_FRAME_STEP would stall or run fast. */
#ifndef SDF_FRAME_STEP_INT
#define SDF_FRAME_STEP_INT(d) (d)
#endif

/* A test retail makes once per tick before a step SDF_FRAME_STEP_INT splits.
 * A host answers it on the first host tick of each retail tick and keeps that
 * answer, keyed by key, for the tick, so the split step runs whole. */
#ifndef SDF_FRAME_TICK_TEST
#define SDF_FRAME_TICK_TEST(key, cond) (cond)
#endif

/* The stage a switch run each tick takes, kept at key. Retail runs a stage set
 * on one tick from the next. A host that runs the switch each host tick answers
 * -1, matching no case, for a stage first seen after its retail tick's first
 * host tick, until the next retail tick. */
#ifndef SDF_FRAME_STAGE_HELD
#define SDF_FRAME_STAGE_HELD(key, stage) (stage)
#endif

/* SDF_FRAME_STEP_INT counted from an object's own age, a count of host ticks
 * from 0, so the steps keep the object's phase. */
#ifndef SDF_FRAME_STEP_INT_AGE
#define SDF_FRAME_STEP_INT_AGE(age, d) (d)
#endif

/* A factor applied once per tick, such as a velocity's decay. A host raises
 * it to the power it scales a step by, so the decay over a second holds. */
#ifndef SDF_FRAME_DECAY
#define SDF_FRAME_DECAY(k) (k)
#endif

/* A divisor applied once per tick, as x += (target - x) / d: the decay
 * 1 - 1/d, so a host's SDF_FRAME_DECAY scales it. Folds to d here. */
#ifndef SDF_FRAME_DIVISOR
#define SDF_FRAME_DIVISOR(d) (1.0f / (1.0f - SDF_FRAME_DECAY(1.0f - 1.0f / (d))))
#endif

/* A share of the remaining gap closed once per tick, as x += (target - x) * f
 * with f in [0, 1]: the decay 1 - f, so a host's SDF_FRAME_DECAY scales it.
 * Folds to f here. */
#ifndef SDF_FRAME_FRACTION
#define SDF_FRAME_FRACTION(f) (1.0f - SDF_FRAME_DECAY(1.0f - (f)))
#endif
/* The retail tick a host tick count falls in, an integer: the inverse of
 * SDF_FRAMES, for a key or a parity that must advance at the retail rate. */
#ifndef SDF_FRAME_TICK
#define SDF_FRAME_TICK(age) (age)
#endif

/* True on the first host tick of each retail tick. Gates a step that cannot
 * be scaled, such as a record into a history ring or an integer ramp, so it
 * runs at the retail rate. */
#ifndef SDF_FRAME_DUE
#define SDF_FRAME_DUE() 1
#endif

/* SDF_FRAME_DUE counted from an object's own age, a count of host ticks from
 * 0, so the step keeps the object's phase. */
#ifndef SDF_FRAME_DUE_AGE
#define SDF_FRAME_DUE_AGE(age) 1
#endif

/* The random state for a draw that code retail runs once per tick makes each
 * host tick, such as a jitter drawn while drawing. A host answers the shared
 * stream on one host tick of each retail tick and a stream of its own on the
 * rest, so the shared stream advances at the retail rate. */
#ifndef SDF_FRAME_RAND
#define SDF_FRAME_RAND() ((void *)0)
#endif

/* SDF_FRAME_RAND keyed by a count of host ticks an owner keeps from 0. */
#ifndef SDF_FRAME_RAND_AGE
#define SDF_FRAME_RAND_AGE(age) ((void *)0)
#endif

/* SDF_FRAME_RAND for a draw from the random state at state rather than the
 * shared stream, such as the battle stream: a host answers state on one host
 * tick of each retail tick and a stream of its own on the rest. */
#ifndef SDF_FRAME_RAND_STATE
#define SDF_FRAME_RAND_STATE(state) (state)
#endif
#ifndef SDF_FRAME_RAND_STATE_AGE
#define SDF_FRAME_RAND_STATE_AGE(age, state) (state)
#endif

/* Runs one statement on the first host tick of each retail tick. */
#ifndef SDF_FRAME_WHEN
#define SDF_FRAME_WHEN(stmt) stmt
#endif

/* SDF_FRAME_WHEN counted from an object's own age, a count of host ticks from
 * 0. */
#ifndef SDF_FRAME_WHEN_AGE
#define SDF_FRAME_WHEN_AGE(age, stmt) stmt
#endif

/* The bits of a key mask, read only on the first host tick of each retail
 * tick, so a held key repeats at the retail rate. */
#ifndef SDF_FRAME_MASK
#define SDF_FRAME_MASK(bits) (bits)
#endif

/* A turn by share of the angle left, made once per retail tick by the object
 * at key. Retail's value is angle * share. */
#ifndef SDF_FRAME_TURN
#define SDF_FRAME_TURN(key, angle, share) ((angle) * (share))
#endif

/* A step d added to a value once per retail tick, keyed by the value's
 * address, and that value as drawn on a host tick. A host draws it less the
 * part of d this retail tick has yet to run, so it moves d/n a host tick and is
 * the stepped value on the tick's last. Retail's values are d and x. */
#ifndef SDF_FRAME_TICK_STEP
#define SDF_FRAME_TICK_STEP(key, d) (d)
#endif
#ifndef SDF_FRAME_TICK_DRAWN
#define SDF_FRAME_TICK_DRAWN(key, x) (x)
#endif

/* A value x stepped by d once per tick, after it is read, and the step: a host
 * steps it by d on a retail tick's first host tick and by 0 on the rest. HELD is
 * x as retail holds it for the tick, for a test made once per tick. DRAWN is x as
 * drawn on a host tick: less the part of the tick still to run, so it moves d/n a
 * host tick and is retail's value on the tick's last, and never under floor.
 * Retail's values are d and x. */
#ifndef SDF_FRAME_DUE_STEP
#define SDF_FRAME_DUE_STEP(d) (d)
#endif
#ifndef SDF_FRAME_DUE_HELD
#define SDF_FRAME_DUE_HELD(x, d) (x)
#endif
#ifndef SDF_FRAME_DUE_DRAWN
#define SDF_FRAME_DUE_DRAWN(x, d, floor) (x)
#endif

/* A countdown x just lowered by one, counted in host ticks. A host holds it at 1
 * until a retail tick's last host tick, so what waits for 0 starts on a retail
 * tick's first host tick. Retail's value is x. */
#ifndef SDF_FRAME_DELAY_END
#define SDF_FRAME_DELAY_END(x) (x)
#endif

/* An age in host ticks, counted from 0 on a retail tick's first host tick, as
 * the time to draw: a host takes off n - 1, never going under 0, so what is read
 * from it reaches retail's value on the tick's last host tick. Retail's value is
 * age. */
#ifndef SDF_FRAME_DRAWN_AGE
#define SDF_FRAME_DRAWN_AGE(age) (age)
#endif

/* The test age < end for a step made after the draw, with age in host ticks
 * from a retail tick's first host tick. A host steps while the drawn age is
 * from n to under end, so the next draw is retail's on a tick's last host tick.
 * Retail's value is age < end. */
#ifndef SDF_FRAME_STEP_AFTER_DRAW
#define SDF_FRAME_STEP_AFTER_DRAW(age, end) ((age) < (end))
#endif

/* A step d made after the draw where no test holds the step, with age in host
 * ticks from a tick's first host tick. A host adds d / n while the drawn age is
 * n or more, so the next draw is the original's on a tick's last host tick. The
 * original adds d. */
#ifndef SDF_FRAME_STEP_AFTER_DRAW_BY
#define SDF_FRAME_STEP_AFTER_DRAW_BY(age, d) (d)
#endif

/* The n floats at dst, which their owner scaled about origin by ratio on this
 * retail tick, as drawn on a host tick: origin + (dst - origin) * ratio to the
 * power of the share of the tick run, less one. Retail draws dst as it is. */
#ifndef SDF_FRAME_SCALE_DRAWN
#define SDF_FRAME_SCALE_DRAWN(dst, origin, ratio, n)
#endif

/* Sets the pose of each pipe game model placed from the pipe's path nodes on
 * this retail tick to where the game's rules put it at this host tick, from
 * the path position and offsets the tick moved from. Expands to nothing here. */
#ifndef SDF_FRAME_PATH_POSES
#define SDF_FRAME_PATH_POSES()
#endif

/* Four floats the pipe camera sets once per retail tick from the path: a host
 * points at their value at this host tick's path position. Retail points at
 * src. */
#ifndef SDF_FRAME_PATH_VEC
#define SDF_FRAME_PATH_VEC(src) (src)
#endif

/* A value drawn each host tick from state its owner updates once per retail
 * tick, given a key no other interpolated value uses. A host draws it between its values at the ends of the last two retail
 * ticks by the share of this tick run, so it moves every host tick and lands
 * on the retail value at the tick's last host tick. The state is unchanged,
 * so retail draws the value itself. */
#ifndef SDF_FRAME_INTERPOLATE
#define SDF_FRAME_INTERPOLATE(key, value) (value)
#endif

/* SDF_FRAME_INTERPOLATE of a value read before its owner updates it in the
 * same retail tick, so the value changes between that tick's host ticks. A
 * host samples it on the first host tick of each retail tick only. */
#ifndef SDF_FRAME_INTERPOLATE_DUE
#define SDF_FRAME_INTERPOLATE_DUE(key, value) (value)
#endif

/* Forgets what SDF_FRAME_INTERPOLATE drew for key, where its owner starts the
 * value over, so the next draw starts at the new value instead of moving from
 * the old one. Expands to nothing here. */
#ifndef SDF_FRAME_INTERPOLATE_RESET
#define SDF_FRAME_INTERPOLATE_RESET(key)
#endif

/* SDF_FRAME_INTERPOLATE_RESET of key when cond holds, such as a camera cut.
 * Expands to nothing here, cond included. */
#ifndef SDF_FRAME_INTERPOLATE_RESET_IF
#define SDF_FRAME_INTERPOLATE_RESET_IF(cond, key)
#endif

/* Sets lvalue to SDF_FRAME_INTERPOLATE(key, value). Expands to nothing here. */
#ifndef SDF_FRAME_INTERPOLATE_SET
#define SDF_FRAME_INTERPOLATE_SET(key, lvalue, value)
#endif

/* A value its owner moves once per retail tick a share of the way to target,
 * from before the tick's step to after it. A host draws the approach itself at
 * the share of the tick run, from the first host tick's from and to. Retail's
 * value is to. */
#ifndef SDF_FRAME_APPROACH
#define SDF_FRAME_APPROACH(key, from, to, target) (to)
#endif

/* Sets lvalue, which holds from, to SDF_FRAME_APPROACH(key, lvalue, to,
 * target). Expands to nothing here. */
#ifndef SDF_FRAME_APPROACH_SET
#define SDF_FRAME_APPROACH_SET(key, lvalue, to, target)
#endif

/* A value drawn after its owner moves it once per retail tick a share of the
 * way to target. A host draws the approach from the value at the end of the
 * last retail tick to this one at the share of the tick run, given a key no
 * other drawn value uses. Retail draws the value. */
#ifndef SDF_FRAME_APPROACH_DRAW
#define SDF_FRAME_APPROACH_DRAW(key, value, target) (value)
#endif

/* SDF_FRAME_APPROACH_DRAW of a value that changes between a retail tick's
 * host ticks, read on its first host tick only. */
#ifndef SDF_FRAME_APPROACH_DRAW_DUE
#define SDF_FRAME_APPROACH_DRAW_DUE(key, value, target) (value)
#endif

/* The battle camera's eye, target and view direction, three floats each, after
 * its shot steps once per retail tick; cut is true on a shot's first tick. A
 * host rewrites them to the shot at the share of the tick run. Expands to
 * nothing here, cut included. */
#ifndef SDF_FRAME_CAMERA_SHOW
#define SDF_FRAME_CAMERA_SHOW(cut, eye, target, direction)
#endif

/* The battle camera's field of view for the host tick SDF_FRAME_CAMERA_SHOW
 * drew. Retail's value is fovy. */
#ifndef SDF_FRAME_CAMERA_FOVY
#define SDF_FRAME_CAMERA_FOVY(fovy) (fovy)
#endif

/* A battle camera shot's frame count, one per retail tick, where the shot reads
 * it as a time. A host gives the time of the host tick drawn. Retail's value is
 * frame. */
#ifndef SDF_FRAME_CAMERA_TIME
#define SDF_FRAME_CAMERA_TIME(frame) (frame)
#endif

/* Sets the n floats at dst to SDF_FRAME_INTERPOLATE of the n floats at src, the
 * i-th keyed by key and i. Expands to nothing here. */
#ifndef SDF_FRAME_INTERPOLATE_VEC
#define SDF_FRAME_INTERPOLATE_VEC(key, dst, src, n)
#endif

/* Four floats drawn each host tick from the four at src, which their owner
 * updates once per retail tick: a host points at a copy whose first n lanes
 * are SDF_FRAME_INTERPOLATE of src's, keyed by src. Retail points at src. */
#ifndef SDF_FRAME_INTERPOLATE_PTR
#define SDF_FRAME_INTERPOLATE_PTR(src, n) (src)
#endif

/* Sets basic's position to the vector at pos, which its owner sets once per
 * retail tick. A host sets SDF_FRAME_INTERPOLATE of it, keyed by pos, and sets
 * it again at each SDF_FRAME_INTERPOLATE_POSES on the rest of the retail tick. */
#ifndef SDF_FRAME_INTERPOLATE_POSE
#define SDF_FRAME_INTERPOLATE_POSE(basic, pos) dds3SetPosPoseSet((basic), (pos))
#endif

/* Sets basic's rotation to the quaternion at rot, which its owner sets once per
 * retail tick. A host interpolates it the short way round, keyed by rot, and
 * sets it again at each SDF_FRAME_INTERPOLATE_POSES as it does positions. */
#ifndef SDF_FRAME_INTERPOLATE_ROT
#define SDF_FRAME_INTERPOLATE_ROT(basic, rot) dds3SetRotPoseSet((basic), (rot))
#endif

/* Sets again each position SDF_FRAME_INTERPOLATE_POSE and each rotation
 * SDF_FRAME_INTERPOLATE_ROT set in this retail tick.
 * Expands to nothing here. */
#ifndef SDF_FRAME_INTERPOLATE_POSES
#define SDF_FRAME_INTERPOLATE_POSES()
#endif

/* Brackets the reads of positions recorded once per retail tick by parts
 * that follow a moving mark: pos is the mark at the last retail tick and
 * posOld the one before. A host running more ticks than retail may move the
 * followers between retail ticks, reading the mark itself from root's node
 * mark, and must put pos and posOld back at END so the next record still
 * spans one retail tick. BEGIN takes the age of the part's owner, a count
 * of host ticks from 0. Retail reads the recorded positions, so both expand to
 * nothing. */
#ifndef SDF_FRAME_FOLLOW_BEGIN_AGE
#define SDF_FRAME_FOLLOW_BEGIN_AGE(age, pos, posOld, root, mark)
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
