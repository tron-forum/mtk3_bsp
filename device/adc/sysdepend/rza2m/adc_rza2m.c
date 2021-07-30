﻿/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.00.05.B0
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/08.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RZA2M

#include "../../../include/dev_def.h"
#if DEV_ADC_ENABLE

/*
 *	dev_adc_rza2m.c
 *	A/D converter device driver
 *	System dependent processing for RZ/A2M
 */
#include <tk/tkernel.h>
#include "../../adc.h"

/*----------------------------------------------------------------------
 * Device control data
*/
LOCAL struct {
	ID	wait_tskid;
} ll_devcb;


/*----------------------------------------------------------------------
 * Interrupt handler
 */
void adc_inthdr( UINT intno)
{
	ClearInt(intno);
	tk_wup_tsk(ll_devcb.wait_tskid);
	EndOfInt(intno);
}

/*----------------------------------------------------------------------
 * A/D convert
 */
LOCAL UW adc_convert( INT start, INT size, UW *buf )
{
	UH	chset = 0;
	INT	end, ch;
	ER	err;

	ll_devcb.wait_tskid = tk_get_tid();
	tk_can_wup(TSK_SELF);

	if(start >= ADC_CH_NUM) return E_PAR;
	end = start + size -1;
	if( end >= ADC_CH_NUM) return E_PAR;

	/* Set target channel */
	for(INT ch = start; ch <= end; ch++) {
		chset |= (1<<ch);
	}
	out_h(ADANSA0, chset);

	out_h(ADCSR, ADCSR_ADST | ADCSR_ADIE);		// Start Covert

	err = tk_slp_tsk(DEVCNF_ADC_TMOSCAN);
	if(err != E_OK) return (UW)err;

	for(ch = start; ch <= end; ch++) {	/* Read data */
		*buf++ = (UW)in_h(ADDR(ch));
	}
	return (UW)size;
}


/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT W dev_adc_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	UW	rtn = (UW)E_OK;

	switch(cmd) {
	case LLD_ADC_OPEN:	/* Open　A/DC */
		EnableInt(INTNO_S12ADI0, DEVCNF_ADC_INTPRI);
		break;

	case LLD_ADC_CLOSE:	/* Close　A/DC */
		DisableInt(INTNO_S12ADI0);
		break;
	
	case LLD_ADC_READ:	/* Read A/DC data */
		rtn = adc_convert(p1, p2, pp);
		break;
	
	case LLD_ADC_RSIZE:	/* Get read data size */
		if(p1  >= ADC_CH_NUM) return (UW)E_PAR;
		rtn = ADC_CH_NUM - p1;
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
		.inthdr	= adc_inthdr,
	};
	ER	err;

#if DEVCONF_ADC_INIT_MSTP		// Initialize module stop
	UB	stbc;

	stbc = in_b(CPG_STBCR5);
	out_b(CPG_STBCR5, stbc & 0x7F);
	stbc = in_b(CPG_STBCR5);	// dummy read
	
#endif	/* DEVCONF_ADC_INIT_MSTP */

#if DEVCONF_ADC_INIT_PIN		// Initialize I/O pin
	UH	pdr;
	UB	cnf;

	/* Pin function selection */
	out_b(PORT_PWPR, 0);
	out_b(PORT_PWPR, PORT_PWPR_PFSWE);		/* Allow writing to PFS */

	cnf = DEVCONF_ENA_AN00_07;
	pdr = in_h(PORT5_PDR);
	for(INT i = 0; i < 8; i++, cnf<<=1) {
		if(cnf & 1) {
			pdr &= ~(0x03<<(i*2));
			out_b(PORT5n_PFS(i), PORT_PFS_PSEL(1));
		}
	}

	out_b(PORT_PWPR, PORT_PWPR_B0WI);		/* Prohibit writing to PFS */

	out_h(PORT5_PDR, pdr);
	*(UB*)PORT5_PMR |= cnf;

#endif		/* DEVCONF_ADC_INIT_PIN */

	/* ADC device initialize */
	out_h(ADCSR, 0);		// ADC stop, Single scan mode

	// Set sampling rate
	out_b(ADSSTR(0), DEVCNF_ADSSTR0_INI);
	out_b(ADSSTR(1), DEVCNF_ADSSTR1_INI);
	out_b(ADSSTR(2), DEVCNF_ADSSTR2_INI);
	out_b(ADSSTR(3), DEVCNF_ADSSTR3_INI);
	out_b(ADSSTR(4), DEVCNF_ADSSTR4_INI);
	out_b(ADSSTR(5), DEVCNF_ADSSTR5_INI);
	out_b(ADSSTR(6), DEVCNF_ADSSTR6_INI);
	out_b(ADSSTR(7), DEVCNF_ADSSTR7_INI);

	/* Interrupt handler definition */
	err = tk_def_int(INTNO_S12ADI0, &dint);

	return err;
}

#endif		/* DEV_ADC_ENABLE */
#endif		/* CPU_RZA2M */