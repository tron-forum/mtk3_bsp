/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03.B0
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/01.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (Nucleo-64 STM32L467 depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_H__
#define __SYS_SYSDEF_DEPEND_H__


/* CPU-dependent definition */
#include <sys/sysdepend/cpu/stm32l4/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Clock control definition
 */

/* RCC register initial value */
#define	RCC_CFGR_INIT		(0x00000000)	// SYSCLK = PLL ,SYSCLK,HCLK not divided. 
#define	RCC_PLLCFGR_INIT	(0x00000A00)	// M = 1, N = 10, P = 7, Q = 2, R =2
#define	RCC_PLLSAI1CFGR_INIT	(0x00000800)	// N = 8, P = 7, Q = 2, R =2
#define	RCC_PLLSAI2CFGR_INIT	(0x00000800)	// N = 8, P = 7, R =2

/* Clock frequency 　*/
#define	SYSCLK			(80)		/* System clock */
#define	HCLK			(SYSCLK)	/* Peripheral clock (AHB) */
#define	PCLK1			(HCLK)		/* Peripheral clock (APB1) */
#define	PCLK2			(HCLK)		/* Peripheral clock (APB2) */
#define TMCLK			(HCLK)		/* System timer clock input (MHz) */
#define TMCLK_KHz		(TMCLK * 1000)	/* System timer clock input (kHz) */

/*
 * Maximum value of Power-saving mode switching prohibition request.
 * Use in tk_set_pow API.
 */
#define LOWPOW_LIMIT	0x7fff		/* Maximum number for disabling */

#endif /* __TK_SYSDEF_DEPEND_H__ */
