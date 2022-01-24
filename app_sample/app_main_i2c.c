#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <tk/device.h>

#include "gesture.h"

#define	I2C_SADR			(0x73)
#define	PAJ7620_REGITER_BANK_SEL	(0xEF)
#define	PAJ7620_ADDR_GES_PS_DET_FLAG_0	(0x43)

LOCAL void task1(INT, void*);
T_CTSK	ctsk1 = {
	.tskatr		= TA_HLNG | TA_RNG3,
	.task		= task1,
	.itskpri	= 10,
	.stksz		= 1024,
};
ID	tskid1;

void task1(INT stacd, void *exinf)
{
	UW	val;

	tm_printf((UB*)"Start Task1\n");

	gesture_sensor_init(0);

	while(1) {
		gesture_sensor_get(&val);

		if(val & GES_RIGHT_FLAG ) {
			tm_printf((UB*)"Right\n");
		} else if(val & GES_LEFT_FLAG) {
			tm_printf((UB*)"Left\n");
		} else if(val & GES_UP_FLAG) {
			tm_printf((UB*)"Up\n");
		} else if(val & GES_DOWN_FLAG) {
			tm_printf((UB*)"Down\n");
		}

		if(val & GES_FORWARD_FLAG) {
			tm_printf((UB*)"Forward\n");
		} else if(val & GES_BACKWARD_FLAG) {
			tm_printf((UB*)"Balckward\n");
		}
		if(val & GES_CLOCKWISE_FLAG) {
			tm_printf((UB*)"Clockwise\n");
		} else if(val & GES_COUNT_CLOCKWISE_FLAG) {
			tm_printf((UB*)"Anti-Cockwise\n");
		}

		tk_dly_tsk(500);
	}
	tk_ext_tsk();
}

/* ----------------------------------------------------------
 *
 * User-Main Definition (Run on initial task)
 *
 */
EXPORT INT usermain( void )
{
	T_RVER	rver;

	tm_putstring((UB*)"Start User-main program.\n");

	tk_ref_ver(&rver);		/* Get the OS Version. */

	tm_printf((UB*)"Make Code: %04x  Product ID: %04x\n", rver.maker, rver.prid);
	tm_printf((UB*)"Product Ver. %04x\nProduct Num. %04x %04x %04x %04x\n", 
			rver.prver, rver.prno[0],rver.prno[1],rver.prno[2],rver.prno[3]);

	tskid1 = tk_cre_tsk(&ctsk1);
	if(tskid1 > E_OK) {
		tk_sta_tsk(tskid1, 0);
	} else {
		tm_printf((UB*)"Task Create Error %d\n", tskid1);
	}

	tk_slp_tsk(TMO_FEVR);
	return 0;
}
