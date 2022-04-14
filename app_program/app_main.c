#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "gesture.h"

ID	flgid1;

void tsk1(INT stacd, void *exinf)
{
	UB	data, pdata;

	pdata = in_w(PORTB_PIDR) & (1<<1);
	while(1) {
		data = in_w(PORTB_PIDR) & (1<<1);
		if(data != pdata) {
			if(!data) {
				pdata = data; // dummy
				out_b(PORTD_PODR, in_b(PORTD_PODR)^(1<<7));
			}
			pdata = data;
		}
		tk_dly_tsk(100);
	}
}


void tsk2(INT stacd, void *exinf)
{
	ID	dd_i2c;
	UW	val;
	ER	err;

	dd_i2c = tk_opn_dev((UB*)"iica", TD_UPDATE);

	err = gesture_sensor_init(dd_i2c);
	if(err < E_OK) {
		tm_printf((UB*)"ERROR\n");
	} else{
		tm_printf((UB*)"OK\n");
	}

#if 0
	if(err < E_OK) {
		err = gesture_sensor_init(dd_i2c);
		if(err < E_OK) {
			err = gesture_sensor_init(dd_i2c);
		}
	}
#endif

	while(1) {
		// Gesture sensor
		err = gesture_sensor_get(dd_i2c, &val);
		if(err<E_OK) tm_printf((UB*)"Sensor Read error %d\n", err);

		if(val != 0) {
			tm_printf((UB*)"sense %x\n", val);
		}
#if 0
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
#endif
		tk_dly_tsk(500);
	}
}

void tsk3(INT stacd, void *exinf)
{
	ID	dd;
	ER	err;
	UW	data;
	SZ	asz;

	dd = tk_opn_dev((UB*)"adca", TD_UPDATE);
	while(1) {
		err = tk_srea_dev(dd, 0, &data, 1, &asz);
		tk_dly_tsk(500);
	}

	tk_exd_tsk();	/* Exit task */
}

const T_CTSK	ctsk1 = {
	.tskatr		= TA_HLNG | TA_RNG3,
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= tsk1,
};

const T_CTSK	ctsk2 = {
	.tskatr		= TA_HLNG | TA_RNG3,
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= tsk2,
};

const T_CTSK	ctsk3 = {
	.tskatr		= TA_HLNG | TA_RNG3,
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= tsk3,
};

/* usermain関数 */
EXPORT INT usermain(void)
{
	ID	tskid1, tskid2, tskid3;

	tskid1 = tk_cre_tsk(&ctsk1);
	tk_sta_tsk(tskid1, 0);

	tskid2 = tk_cre_tsk(&ctsk2);
	tk_sta_tsk(tskid2, 0);

	tskid3 = tk_cre_tsk(&ctsk3);
	tk_sta_tsk(tskid3, 0);

	tk_slp_tsk(TMO_FEVR);
	return 0;
}