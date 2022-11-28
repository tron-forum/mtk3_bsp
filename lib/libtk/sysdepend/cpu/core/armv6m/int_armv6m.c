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
#ifdef CPU_CORE_ARMV6M

/*
 *	int_armv6m.c
 *
 *	Interrupt controller (ARMv6-M)
 */

#include <tk/tkernel.h>

#include "int_armv6m.h"

/*----------------------------------------------------------------------*/
/*
 * CPU Interrupt Control for ARMv6-M.
 *
 */
EXPORT UW disint(void)
{
	UW	intsts = get_primask();
	set_primask(1);

	return intsts;
}

/*----------------------------------------------------------------------*/
/*
 * Interrupt controller (NVIC) Control
 *
 */

/*
 * Enable interrupt for NVIC
 */
EXPORT void EnableInt_nvic( UINT intno, INT level )
{
	UINT	imask;
	
	DI(imask);
	/* Set interrupt priority level. */
	*(_UB*)(NVIC_IPR(intno)) = (UB)INTPRI_GROUP(level, 0);

	/* Enables the specified interrupt. */
	*(_UW*)(NVIC_ISER(intno)) = (0x01U << (intno % 32));

	EI(imask);
}

#endif /* CPU_CORE_ARMV6M */
