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
	UB	buf[]	= {'T','E','S','T','\n','\r'};
	SZ	asz;
	ER	err;
	ID	dd;

	err = dev_init_ser(1);
	if(err < E_OK) tm_printf((UB*)"Init error %d\n", err);

	dd = tk_opn_dev((UB*)"serb", TD_UPDATE);
	if(dd < E_OK) tm_printf((UB*)"Open error %d\n", dd);

	err = tk_swri_dev(dd, 0, buf, sizeof(buf), NULL);
	if(err < E_OK) tm_printf((UB*)"Write Eroor %d\n", err);

	for(INT i = 0; i < 10; i++) {
		for(INT j = 0; j <10; j++) {
			buf[0] = '0' + j;
			err = tk_swri_dev(dd, 0, buf, 1, NULL);
			if(err < E_OK) tm_printf((UB*)"Write Error %d\n", err);
		}
		buf[0] = '\n';
		tk_swri_dev(dd, 0, buf, 1, NULL);
	}

	for(INT i = 0; i < 10; i++) {
		err = tk_srea_dev(dd, 0, buf, 1, &asz);
		if(err < E_OK) tm_printf((UB*)"Read error %d\n", dd);
		tk_swri_dev(dd, 0, buf, 1, NULL);
	}

	tk_dly_tsk(1);
	err = tk_cls_dev(dd, 0);
	if(dd < E_OK) tm_printf((UB*)"Close error %d\n", dd);

	tk_dly_tsk(500);
	tm_printf((UB*)"\nEnd task\n");
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
