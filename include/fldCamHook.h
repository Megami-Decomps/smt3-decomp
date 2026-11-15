#ifndef FLD_CAM_HOOK_H
#define FLD_CAM_HOOK_H

/* Field camera control points. Each macro expands to retail's code, so this
 * build is unchanged. A host that steers the camera itself defines them first. */

/* Whether the right stick takes over: retail peeks with the chase camera and
 * stops the player walking while cond holds. */
#ifndef FLD_CAM_PEEK
#define FLD_CAM_PEEK(cond) (cond)
#endif

/* Nonzero skips the player step's automatic camera turns (the diagonal walk
 * turn and the ease back behind the player); retail never skips them. */
#ifndef FLD_CAM_TURN
#define FLD_CAM_TURN() 0
#endif

/* The follow camera's position eye, before collision pulls it toward pivot,
 * the point it is pulled toward. Retail places it as computed. */
#ifndef FLD_CAM_PLACE
#define FLD_CAM_PLACE(eye, pivot) ((void)0)
#endif

#endif
