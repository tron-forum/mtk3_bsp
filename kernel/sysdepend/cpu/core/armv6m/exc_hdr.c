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
 *	exc_hdr.c (ARMv6-M)
 *	Exception handler
 */

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <kernel.h>
#include "../../../sysdepend.h"

#if (USE_EXCEPTION_DBG_MSG && USE_TMONITOR)
	#define EXCEPTION_DBG_MSG(a)	tm_putstring((UB*)a)
#else
	#define EXCEPTION_DBG_MSG(a)
#endif

/*
 * NMI handler
 */
WEAK_FUNC EXPORT void NMI_Handler(void)
{
	EXCEPTION_DBG_MSG("NMI\n");
	while(1);
}

/*
 * Hard fault handler
 */
WEAK_FUNC EXPORT void HardFault_Handler(void)
{
#if (USE_EXCEPTION_DBG_MSG  && USE_TMONITOR)

	ID	ctskid;

	if(knl_ctxtsk != NULL) {
		ctskid = knl_ctxtsk->tskid;
	} else {
		ctskid = 0;
	}
	tm_printf((UB*)"*** Hard fault ***  ctxtsk:%d\n", ctskid);
#endif
	while(1);
}

/*
 * Svcall
 */
WEAK_FUNC EXPORT void Svcall_Handler(void)
{
	EXCEPTION_DBG_MSG("SVCall\n");
	while(1);
}

/*
 * Default Handler
 */
WEAK_FUNC EXPORT void Default_Handler(void)
{
#if (USE_EXCEPTION_DBG_MSG  && USE_TMONITOR)
	INT	i;
	_UW	*icpr;

	icpr = (_UW*)NVIC_ICPR_BASE;

	EXCEPTION_DBG_MSG("Undefine Exceptio ICPR: ");
	for(i=0; i < 8; i++) {
		tm_printf((UB*)"%x ", *icpr++);
	}
	EXCEPTION_DBG_MSG("\n");
#endif
	while(1);
}

#endif /* CPU_CORE_ARMV6M */
