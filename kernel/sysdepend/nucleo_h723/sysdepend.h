/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdepend.h (Nucleo-64 STM32H723)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_TARGET_SYSDEPEND_
#define _SYSDEPEND_TARGET_SYSDEPEND_

#include "../cpu/stm32h7/sysdepend.h"

/*
 *    Clock Setting (cpu_clock.c)
 */
IMPORT void startup_clock(ATR clkatr);
#define	CLKATR_HSI		0x00000001
#define	CLKATR_CSI		0x00000002
#define	CLKATR_HSE		0x00000004
#define	CLKATR_HSEBYP		0x00000008
#define CLKATR_HSI48		0x00000010

#define	CLKATR_PLL1		0x00000020
#define	CLKATR_PLL2		0x00000040
#define	CLKATR_PLL3		0x00000080

IMPORT void shutdown_clock(void);

#endif /* _SYSDEPEND_TARGET_SYSDEPEND_ */