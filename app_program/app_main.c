#include <tk/tkernel.h>
#include <tm/tmonitor.h>

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
			}
			pdata = data;
		}
		tk_dly_tsk(100);
	}
}


void tsk2(INT stacd, void *exinf)
{
	while(1) {
		tk_dly_tsk(1000);
		out_b(PORTD_PODR, in_b(PORTD_PODR)^(1<<6));
	}

}

void tsk3(INT stacd, void *exinf)
{
	while(1) {
		tk_dly_tsk(2000);
		out_b(PORTD_PODR, in_b(PORTD_PODR)^(1<<7));
	}

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