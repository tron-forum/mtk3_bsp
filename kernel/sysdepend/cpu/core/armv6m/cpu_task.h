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
 *	cpu_task.h (ARMv6-M)
 *	CPU-Dependent Task Start Processing
 */

#ifndef _SYSDEPEND_CPU_CORE_CPUTASK_
#define _SYSDEPEND_CPU_CORE_CPUTASK_

/*
 * System stack configuration at task startup
 */
typedef struct {
	UW	exp_ret;	/* Exception return */
	UW	r_[8];		/* R4-R11 */
	// Exception entry stack
	UW	r[4];		/* R0-R3 */
	UW	ip;		/* R12 */
	void	*lr;		/* lr */
	void	*pc;		/* pc */
	UW	xpsr;		/* xpsr */
} SStackFrame;

/*
 * Size of system stack area destroyed by 'make_dormant()'
 * In other words, the size of area required to write by 'knl_setup_context().'
 */
#define DORMANT_STACK_SIZE	( sizeof(VW) * 7 ) /* To 'R4' position */

/*
 * Create stack frame for task startup
 *	Call from 'make_dormant()'
 */
Inline void knl_setup_context( TCB *tcb )
{
	SStackFrame	*ssp;

	ssp = tcb->isstack;
	ssp--;

	/* CPU context initialization */
	ssp->exp_ret	= 0xFFFFFFF9;
	ssp->lr		= 0;
	ssp->xpsr	= 0x01000000;		/* Initial SR */
	ssp->pc = (void*)((UW)tcb->task & ~0x00000001UL);	/* Task startup address */

	tcb->tskctxb.ssp = ssp;		/* System stack pointer */
}

/*
 * Set task startup code
 *	Called by 'tk_sta_tsk()' processing.
 */
Inline void knl_setup_stacd( TCB *tcb, INT stacd )
{
	SStackFrame	*ssp;
	
	ssp = tcb->tskctxb.ssp;

	ssp->r[0] = stacd;
	ssp->r[1] = (VW)tcb->exinf;
}

/*
 * Delete task contexts
 */
Inline void knl_cleanup_context( TCB *tcb )
{
}

#endif /* _SYSDEPEND_CPU_CORE_CPUTASK_ */
