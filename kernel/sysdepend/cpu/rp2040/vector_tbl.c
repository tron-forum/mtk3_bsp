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
 *	vector_tbl.h (RP2040)
 *	Exception/Interrupt Vector Table
 */

#include "kernel.h"
#include "../../sysdepend.h"

/* ------------------------------------------------------------------------ */
/*
 * Exception/Interrupt Vector Table
 * 
 */

void (* const vector_tbl[])()  __attribute__((section(".vector"))) = {
	(void(*)()) (INITIAL_SP),		/* 0: Top of Stack */
	Reset_Handler,				/* 1: Reset Handler */
	NMI_Handler,				/* 2: NMI Handler */
	HardFault_Handler,			/* 3: Hard Fault Handler */
	0,					/* 4: Reserved */
	0,					/* 5: Reserved */
	0,					/* 6: Reserved */
	0,					/* 7: Reserved */
	0,					/* 8: Reserved */
	0,					/* 9: Reserved */
	0,					/* 10: Reserved */
	Svcall_Handler,				/* 11: Svcall */
	0,					/* 10: Reserved */
	0,					/* 13: Reserved */
	knl_dispatch_entry,			/* 14: Pend SV */
	knl_systim_inthdr,			/* 15: Systick */

	/* External Interrupts */
	Default_Handler,			/* IRQ 0 */
	Default_Handler,			/* IRQ 1 */
	Default_Handler,			/* IRQ 2 */
	Default_Handler,			/* IRQ 3 */
	Default_Handler,			/* IRQ 4 */
	Default_Handler,			/* IRQ 5 */
	Default_Handler,			/* IRQ 6 */
	Default_Handler,			/* IRQ 7 */
	Default_Handler,			/* IRQ 8 */
	Default_Handler,			/* IRQ 9 */
	Default_Handler,			/* IRQ 10 */
	Default_Handler,			/* IRQ 11 */
	Default_Handler,			/* IRQ 12 */
	Default_Handler,			/* IRQ 13 */
	Default_Handler,			/* IRQ 14 */
	Default_Handler,			/* IRQ 15 */
	Default_Handler,			/* IRQ 16 */
	Default_Handler,			/* IRQ 17 */
	Default_Handler,			/* IRQ 18 */
	Default_Handler,			/* IRQ 19 */
	Default_Handler,			/* IRQ 20 */
	Default_Handler,			/* IRQ 21 */
	Default_Handler,			/* IRQ 22 */
	Default_Handler,			/* IRQ 23 */
	Default_Handler,			/* IRQ 24 */
	Default_Handler,			/* IRQ 25 */
	Default_Handler,			/* IRQ 26 */
	Default_Handler,			/* IRQ 27 */
	Default_Handler,			/* IRQ 28 */
	Default_Handler,			/* IRQ 29 */
	Default_Handler,			/* IRQ 30 */
	Default_Handler,			/* IRQ 31 */
};

#endif /* CPU_RP2040 */
