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
	ID	dd;
	UW	data[2];
	SZ	asz;
	ER	err;

	err = dev_init_adc(0);
	if(err < E_OK) {
		tm_printf((UB*)"ADC init error%d(%x)\n", err, err);
	} else {
		tm_printf((UB*)"ADC-1 init OK\n");
	}

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
