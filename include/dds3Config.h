#ifndef DDS3_CONFIG_H
#define DDS3_CONFIG_H

/* The config screen. Each macro expands to retail's code, so this build is
 * unchanged. A host that adds pages of its own options defines them first. */

/* Runs once the screen's work is set up and its cursor holds retail's rows. */
#ifndef DDS3_CONFIG_OPEN
#define DDS3_CONFIG_OPEN(work) ((void)0)
#endif

/* Draws the rows into ordering table ot. stmt draws retail's rows; a host
 * showing a page of its own draws that instead. */
#ifndef DDS3_CONFIG_ROWS
#define DDS3_CONFIG_ROWS(ot, stmt) stmt
#endif

/* The pressed button the input switch acts on, for the cursor at row index. A
 * host acting on the button itself answers 0, which no case takes. */
#ifndef DDS3_CONFIG_BUTTON
#define DDS3_CONFIG_BUTTON(button, index) (button)
#endif

#endif
