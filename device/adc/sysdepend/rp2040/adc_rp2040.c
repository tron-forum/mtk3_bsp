/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RP2040

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "../../adc.h"
#include "../../../include/dev_def.h"
#if DEV_ADC_ENABLE
/*
 *	dev_adc_rp2040.c
 *	A/D converter device driver
 *	System dependent processing for RP2040
 */

/*----------------------------------------------------------------------
 * Device control data
*/
LOCAL struct {
	ID	wait_tskid;
	UW	*buf;
	SZ	asz;
} ll_devcb;

/*----------------------------------------------------------------------
 * Interrupt handler
 */
void adc_inthdr( UINT intno)
{
	*ll_devcb.buf = in_w(ADC_FIFO) & 0x0FFF;
	tk_wup_tsk(ll_devcb.wait_tskid);

	ClearInt(intno);
}

/*----------------------------------------------------------------------
 * A/D convert
 */
LOCAL UW adc_convert( INT ch, INT size, UW *buf )
{
	ER	err;

	if(ch<0 || ch>(ADC_CH_NUM-1)) return E_PAR;
	if(size != 1) return E_PAR;

	while((in_w(ADC_CS)&ADC_CS_READY)==0);

	ll_devcb.wait_tskid = tk_get_tid();
	ll_devcb.buf = buf;

	out_w(ADC_INTE,1);					// Interrupt Enable
	out_w(ADC_CS, ch<<ADC_CS_AINSEL_POS|ADC_CS_EN);
	set_w(ADC_CS, ADC_CS_STRAT_ONCE);

	err = tk_slp_tsk(DEVCNF_ADC_TMOSCAN);

	return err;
}


/*----------------------------------------------------------------------
 * A/DC open
 */
LOCAL ER adc_open(void)
{
	ER	err = E_OK;

	out_w(ADC_DIV, ADC_DIV_INI);				// Clock divider

	out_w(ADC_FCS, 1<<ADC_FCS_THRESH_POS | ADC_FCS_EN);	// Set FIFO
	EnableInt(INTNO_ADC, DEVCNF_ADC_INTPRI);

	return err;
}

/*----------------------------------------------------------------------
 * A/DC close
 */
LOCAL void adc_close(void)
{
	DisableInt(INTNO_ADC);
}

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT W dev_adc_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	W	rtn	= (W)E_OK;

	switch(cmd) {
	case LLD_ADC_OPEN:	/* Open　A/DC */
		rtn = (W)adc_open();
		break;

	case LLD_ADC_CLOSE:	/* Close　A/DC */
		adc_close();
		break;
	
	case LLD_ADC_READ:	/* Read A/DC data */
		rtn = adc_convert( p1, p2, pp);
		break;
	
	case LLD_ADC_RSIZE:	/* Get read data size */
		rtn = 1;
		break;
	}
	
	return rtn;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_adc_llinit( T_ADC_DCB *p_dcb)
{
	const T_DINT	dint = {
		.intatr	= TA_HLNG,
		.inthdr	= adc_inthdr
	};

	ER	err;

/* Release A/DC reset */
#if DEVCONF_ADC_REL_RESET
	clr_w(RESETS_RESET, RESETS_RESET_ADC);	// Release reset
#endif

/* Initialize analog input pins */
#if DEVCONF_ADC_PIN_INIT_0
	out_w(GPIO_CTRL(26),GPIO_CTRL_FUNCSEL_NULL);
	clr_w(GPIO(26), GPIO_IE|GPIO_PUE|GPIO_PDE);
#endif

#if DEVCONF_ADC_PIN_INIT_1
	out_w(GPIO_CTRL(27),GPIO_CTRL_FUNCSEL_NULL);
	clr_w(GPIO(27), GPIO_IE|GPIO_PUE|GPIO_PDE);
#endif

#if DEVCONF_ADC_PIN_INIT_2
	out_w(GPIO_CTRL(28),GPIO_CTRL_FUNCSEL_NULL);
	clr_w(GPIO(28), GPIO_IE|GPIO_PUE|GPIO_PDE);
#endif

#if DEVCONF_ADC_PIN_INIT_3
	out_w(GPIO_CTRL(29),GPIO_CTRL_FUNCSEL_NULL);
	clr_w(GPIO(29), GPIO_IE|GPIO_PUE|GPIO_PDE);
#endif

	/* Enable A/DC */
	set_w(ADC_CS, ADC_CS_EN);
	while(!(in_w(ADC_CS)&ADC_CS_READY));

	err = tk_def_int( INTNO_ADC, &dint);

	return err;
}

#endif		/* DEV_ADC_ENABLE */
#endif		/* CPU_RP2040 */