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

/*
 *	sysdepend.h (ARMv6-M)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_CPU_CORE_SYSDEPEND_
#define _SYSDEPEND_CPU_CORE_SYSDEPEND_

/*
 *    Reset Handler (vector_tbl.c/reset_hdl.c)
 */
IMPORT void Reset_Handler(void);		/* Reset Handler */

/*
 *    Exception Handler (vector_tbl.c/exc_hdr.c)
 */
IMPORT void NMI_Handler(void);			/* NMI Handler */
IMPORT void HardFault_Handler(void);		/* Hard Fault Handler */
IMPORT void Svcall_Handler(void);		/* Svcall */

IMPORT void Default_Handler(void);		/* Default Handler (Undefine Interrupt) */

/*
 * Dispatcher (dispatch.S)
 */
IMPORT void knl_dispatch_entry(void);		/* dispatch entry */
IMPORT void knl_dispatch_to_schedtsk(void);	/* force dispatch */

/*
 * Interrupt Control (interrupt.c)
 */
IMPORT	UW exchdr_tbl[];			/* Exception handler table (RAM) */

IMPORT void knl_systim_inthdr(void);		/* System-timer Interrupt handler */


/*
 * Task context block
 */
typedef struct {
	void	*ssp;		/* System stack pointer */
} CTXB;

/*
 *	Control register operation
 */

Inline UW knl_get_ipsr(void)
{
	UW	ipsr;
	Asm("mrs %0, ipsr": "=r"(ipsr));
	return ipsr;
}

Inline UW knl_get_xpsr(void)
{
	UW	xpsr;
	Asm("mrs %0, psr": "=r"(xpsr));
	return xpsr;
}

#endif /* _SYSDEPEND_CPU_CORE_SYSDEPEND_ */
