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

void ptmrhdr(void *exinf)
{
	static UW	si = 0;

	if( (++si) % 100 == 0) {
		tk_wup_tsk(tskid1);
	}
}

void task1(INT stacd, void *exinf)
{
	T_DPTMR	dptmr;
	SYSTIM	systim;
	UW	old;
	ER	ercd;

	tm_printf((UB*)"*** System Timer\n");
	tk_get_tim(&systim);
	old = systim.lo;
	for( INT i= 0; i <10; i++) {
		tk_dly_tsk(100);
		tk_get_tim(&systim);
		tm_printf((UB*)"%u\n", systim.lo - old);
		old = systim.lo;
	}

	tm_printf((UB*)"*** Physical Timer\n");
	for(INT tno = 1; tno <= TK_MAX_PTIMER + 1; tno++) {
		tm_printf((UB*)"\nStart ptim-%d\n", tno);

		dptmr.ptmratr = TA_HLNG;
		dptmr.ptmrhdr = ptmrhdr;
		DefinePhysicalTimerHandler(tno, &dptmr);

		ercd = StartPhysicalTimer( tno, 10000, TA_CYC_PTMR);
		if(ercd != E_OK) {
			tm_printf((UB*)"Err %d\n", ercd);
			continue;
		}

		tk_get_tim(&systim);
		old = systim.lo;
		for(INT i = 0; i < 10; i++) {
			tk_slp_tsk(TMO_FEVR);
			tk_get_tim(&systim);
			tm_printf((UB*)"%u\n", systim.lo - old);
			old = systim.lo;
		}
		StopPhysicalTimer(tno);
		DefinePhysicalTimerHandler(tno, 0);
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
