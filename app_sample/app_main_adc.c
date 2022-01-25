#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <tk/device.h>

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
	UW	data[2];
	SZ	asz;
	ER	err;

#ifdef NUCLEO_L476
	ID	dd;

	dd = tk_opn_dev((UB*)"adca", TD_UPDATE);
	if(dd < E_OK) {
		tm_printf((UB*)"ADC open error %d(%x)\n", dd, dd);
	} else {
		tm_printf((UB*)"ADC open OK\n");
	}

	while(1) {
		err = tk_srea_dev(dd, 5, data, 1, &asz);
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A0 %d(%x)  ", data[0], data[0]);
		}

		err = tk_srea_dev(dd, 6, data, 1, &asz);
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A1 %d(%x)  ", data[0], data[0]);
		}
		tk_dly_tsk(500);

		err = tk_srea_dev(dd, 5, data, 2, &asz);
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A0 %d(%x)  A1 %d(%x)", data[0], data[0], data[1], data[1]);
		}
		tm_putchar('\n');

		tk_dly_tsk(500);
	}
#elif NUCLEO_H723
	ID	dd2, dd3;

	err = dev_init_adc(2);
	if(err < E_OK) {
		tm_printf((UB*)"ADC init error%d(%x)\n", err, err);
	} else {
		tm_printf((UB*)"ADC-3 init OK\n");
	}

	dd2 = tk_opn_dev((UB*)"adca", TD_UPDATE);
	if(dd2 < E_OK) {
		tm_printf((UB*)"ADC open error %d(%x)\n", dd2, dd2);
	} else {
		tm_printf((UB*)"ADC open OK\n");
	}
	dd3 = tk_opn_dev((UB*)"adcc", TD_UPDATE);
	if(dd3 < E_OK) {
		tm_printf((UB*)"ADC open error %d(%x)\n", dd3, dd3);
	} else {
		tm_printf((UB*)"ADC open OK\n");
	}

	while(1) {
		err = tk_srea_dev(dd2, 15, &data, 1, &asz);	// A0:  PA3  ADC12_INP15
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A0 %d(%x)  ", data, data);
		}

		err = tk_srea_dev(dd2, 10, &data, 1, &asz);	// A1:  PC0  ADC123_INP10
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A1 %d(%x)  ", data, data);
		}

		err = tk_srea_dev(dd3, 1, &data, 1, &asz);	// A2:  PC3  ADC3_INP1
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A2 %d(%x)  ", data, data);
		}

		err = tk_srea_dev(dd2, 5, &data, 1, &asz);	// A3:  PB1  ADC12_INP5
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A3 %d(%x)\n", data, data);
		}
		tk_dly_tsk(1000);
	}

#endif
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
