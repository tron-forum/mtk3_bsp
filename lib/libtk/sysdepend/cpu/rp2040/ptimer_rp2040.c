/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/3.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RP2040
/*
 *	ptimer_rp2040.c
 *
 *	Physical timer (for RP2040)
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#if USE_PTMR

typedef struct {
	UW	baddr;		// Register Base Address
	UINT	mode;		// Timer mode
	UW	div;		// clock division
	void	*exinf;		// Extended information
	FP	ptmrhdr;	// Timer Hnadler
} T_PTMRCB;

T_PTMRCB ptmrcb[TK_MAX_PTIMER] = {
	{PWM_CH0_BASE, 0, PTMR_DIV_CH0, NULL},
	{PWM_CH1_BASE, 0, PTMR_DIV_CH1, NULL},
	{PWM_CH2_BASE, 0, PTMR_DIV_CH2, NULL},
	{PWM_CH3_BASE, 0, PTMR_DIV_CH3, NULL},
	{PWM_CH4_BASE, 0, PTMR_DIV_CH4, NULL},
	{PWM_CH5_BASE, 0, PTMR_DIV_CH5, NULL},
	{PWM_CH6_BASE, 0, PTMR_DIV_CH6, NULL},
	{PWM_CH7_BASE, 0, PTMR_DIV_CH7, NULL},
};

#define	PWM_CH_CSR(n)	(ptmrcb[n].baddr + PWM_CHx_CSR)
#define	PWM_CH_DIV(n)	(ptmrcb[n].baddr + PWM_CHx_DIV)
#define	PWM_CH_CTR(n)	(ptmrcb[n].baddr + PWM_CHx_CTR)
#define	PWM_CH_TOP(n)	(ptmrcb[n].baddr + PWM_CHx_TOP)

LOCAL UW	ptmr_stat	= 0;

/*
 * Physical timer interrupt handler
 */
LOCAL void ptmr_inthdr( UINT intno)
{
	T_PTMRCB	*p_cb;
	UW		ints;
	UW		ptmrno;

	ints = in_w(PWM_INTS);
	for(ptmrno = 0; ptmrno <= 7; ptmrno++) {
		if(ints & (1<<ptmrno)) {
			p_cb = &ptmrcb[ptmrno];
			if( p_cb->ptmrhdr != NULL) {
				(*p_cb->ptmrhdr)( p_cb->exinf);	// Execute user handler.
			}
			if( p_cb->mode == TA_ALM_PTMR)  {
				clr_w(PWM_CH_CSR(ptmrno), PWM_CH_CSR_EN);	// Stop timer.
				clr_w(PWM_INTE, (1<<ptmrno));			// Disable interrupt
			}
			clr_w(PWM_INTR, (1<<ptmrno));		// Clear interrupt
		}
	}
	ClearInt(intno);
}

/*
 * Start of physical timer operation
 */
EXPORT ER StartPhysicalTimer( UINT ptmrno, UW limit, UINT mode)
{
	T_DINT		dint;
	ER		err;

	/* parameter check */
	if(( ptmrno == 0 || ptmrno > TK_MAX_PTIMER )
		|| ( limit == 0 ) || ( mode > TA_CYC_PTMR ))	return E_PAR;

	ptmrno--;
	if( limit > PTMR_MAX_CNT) {
		return E_PAR;
	}

	if(ptmr_stat & (1<<ptmrno)) {
		return E_PAR;		// Physical timer is already running.
	}

	ptmrcb[ptmrno].mode	= mode;

	/* Timer initialization */
	out_w(PWM_CH_CSR(ptmrno), 0);			// Stop timer.
	out_w(PWM_CH_DIV(ptmrno), ptmrcb[ptmrno].div);	// Set devision
	out_w(PWM_CH_TOP(ptmrno), limit);		// Set the counter upper limit.
	out_w(PWM_CH_CTR(ptmrno), 0);			// Clear counter.
	clr_w(PWM_INTR, (1<<ptmrno));			// Clear interrupt
	set_w(PWM_INTE, (1<<ptmrno));			// Enable interrupt

	/* Register interrupt handler */
	if(ptmr_stat == 0) {
		dint.intatr	= TA_HLNG;
		dint.inthdr	= ptmr_inthdr;
		err = tk_def_int( INTNO_PTIM, &dint);
		if(err != E_OK) {
			return err;
		}
		EnableInt( INTNO_PTIM, INTPRI_PTIM);	// Enable Interrupt.
	}
	ptmr_stat |= (1<<ptmrno);

	/* Start Physical Timer */
	out_w(PWM_CH_CSR(ptmrno), PWM_CH_CSR_EN);	// Start Timer.

	return E_OK;
}

EXPORT ER StopPhysicalTimer( UINT ptmrno )
{
	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	ptmrno--;

	if((ptmr_stat & (1<<ptmrno)) == 0) {
		return E_OK;		// Physical timer is already stopped.
	}

	clr_w(PWM_CH_CSR(ptmrno), PWM_CH_CSR_EN);	// Stop timer.
	clr_w(PWM_INTR, (1<<ptmrno));			// Clear interrupt
	clr_w(PWM_INTE, (1<<ptmrno));			// Disable interrupt

	ptmr_stat &= ~(1<<ptmrno);
	if(ptmr_stat == 0) {
		DisableInt( INTNO_PTIM);
	}

	return E_OK;
}

IMPORT ER GetPhysicalTimerCount( UINT ptmrno, UW *p_count )
{
	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	ptmrno--;

	/* Get Physical timer counter */
	*p_count = in_w(PWM_CH_CTR(ptmrno)) & PTMR_MAX_CNT;

	return E_OK;
}


EXPORT ER DefinePhysicalTimerHandler( UINT ptmrno, CONST T_DPTMR *pk_dptmr )
{
	UINT	intsts;

	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;
	if( pk_dptmr->ptmratr != TA_HLNG) return E_RSATR;

	ptmrno--;

	/* Set user Handler */
	DI(intsts);
	if(pk_dptmr != NULL) {
		ptmrcb[ptmrno].ptmrhdr	= pk_dptmr->ptmrhdr;
		ptmrcb[ptmrno].exinf	= pk_dptmr->exinf;
	} else {
		ptmrcb[ptmrno].ptmrhdr	= NULL;
	}
	EI(intsts);

	return E_OK;
}


EXPORT ER GetPhysicalTimerConfig(UINT ptmrno, T_RPTMR *pk_rptmr)
{
	UW		ptmrclk;

	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	ptmrclk = (CLK_PERI_FREQ / ((ptmrcb[--ptmrno].div>>4)&0x0FF));

	pk_rptmr->ptmrclk	= ptmrclk;
	pk_rptmr->maxcount	= PTMR_MAX_CNT;
	pk_rptmr->defhdr	= TRUE;

	return E_OK;
}

#endif	/* USE_PTMR */
#endif	/* CPU_RP2040 */