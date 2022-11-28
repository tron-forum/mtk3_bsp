/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RP2040

/*
 *	int_rp2040.c
 *
 *	Interrupt controller (RP2040)
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#include "../core/armv6m/int_armv6m.h"

/*----------------------------------------------------------------------*/
/*
 * Interrupt control API
 * 
 */
/*
 * Enable interrupt 
 */
EXPORT void EnableInt( UINT intno, INT level )
{
	if( intno <= MAX_NVIC_INTNO) {
		EnableInt_nvic( intno, level);
	}
}

/*
 * Disable interrupt 
 */
EXPORT void DisableInt( UINT intno )
{
	if( intno <= MAX_NVIC_INTNO) {
		DisableInt_nvic( intno);
	}
}

/*
 * Clear interrupt
 */
EXPORT void ClearInt(UINT intno)
{
	if( intno <= MAX_NVIC_INTNO) {
		ClearInt_nvic( intno);
	}
}

/*
 * Issue EOI to interrupt controller
 */
EXPORT void EndOfInt(UINT intno)
{
	/* No opetarion. */
}

/*
 * Check active state
 */
EXPORT BOOL CheckInt( UINT intno )
{
	BOOL rtncd;

	if( intno <= MAX_NVIC_INTNO) {
		rtncd = CheckInt_nvic( intno);
	} else {
		rtncd = FALSE;
	}
	return rtncd;
}

#endif /* CPU_RP2040 */