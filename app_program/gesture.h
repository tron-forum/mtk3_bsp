/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00
 *
 *	PAJ7620 Gesture sensor control
 *----------------------------------------------------------------------
 */

IMPORT	ER gesture_sensor_init(ID);
IMPORT	ER gesture_sensor_start(void);
IMPORT	ER gesture_sensor_stop(void);
IMPORT	ER gesture_sensor_get(ID, UW*);

#define	GES_RIGHT_FLAG			(1<<0)
#define	GES_LEFT_FLAG			(1<<1)
#define	GES_UP_FLAG			(1<<2)
#define	GES_DOWN_FLAG			(1<<3)
#define	GES_FORWARD_FLAG		(1<<4)
#define	GES_BACKWARD_FLAG		(1<<5)
#define	GES_CLOCKWISE_FLAG		(1<<6)
#define	GES_COUNT_CLOCKWISE_FLAG	(1<<7)
