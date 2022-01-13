/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/12
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (Nucleo-144 STM32H723 depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_H__
#define __SYS_SYSDEF_DEPEND_H__


/* CPU-dependent definition */
#include <sys/sysdepend/cpu/stm32h7/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Power & Clock control definition
 */

/* PWR register initial value */
#define	PWR_D3CR_INIT		(0x00)		// VOS Scale 0

/* RCC register initial value */
#define	RCC_CFGR_MCO2_VAL	(0x0)		// sys_ck
#define	RCC_CFGR_MCO2PRE_VAL	(0x0)		// disabled
#define	RCC_CFGR_MCO1_VAL	(0x0)		// hsi_clk
#define	RCC_CFGR_MCO1PRE_VAL	(0x0)		// disabeld
#define	RCC_CFGR_RTCPRE_VAL	(0x0)		// no clock
#define	RCC_CFGR_SW_VAL		(0x3)		// pll1_p_ck

#define	RCC_CFGR_INIT_VAL	((RCC_CFGR_MCO2_VAL<<29) \
				|(RCC_CFGR_MCO2PRE_VAL<<25) \
				|(RCC_CFGR_MCO1_VAL<<22) \
				|(RCC_CFGR_MCO1PRE_VAL<<18) \
				|(RCC_CFGR_RTCPRE_VAL<<8) \
				|RCC_CFGR_SW_VAL)

#define RCC_PLLCKSELR_INIT1	(0x02020202)	// SRC = hse_ck, M = 4
#define RCC_PLLCKSELR_INIT2	(0x02020042)	// SRC = hse_ck, M = 4
#define	RCC_PLL1DIVR_INIT	(0x01030112)	// N = 275, P = 1, Q = 4, R = 2
#define	RCC_PLL1FRACR_INIT	(0x00000000)	// FRACN = 0
#define	RCC_PLLCFGR_INIT	(0x01FF0004)	// PLL1RGE = 1, PLLVCOSEL = 0

#define	RCC_D1CFGR_INIT		(0x00000048)	// HPRE = div2, D1PPRE = div2, D1CPRE = div1
#define	RCC_D1CFGR_PPRE		(0x00000040)	// D1PPRE = div2
#define	RCC_D1CFGR_HPRE		(0x00000008)	// HPRE = div2

#define	RCC_D2CFGR_INIT		(0x00000440)	// D2PPRE1 = div2, D2PPRE2 = div2
#define	RCC_D3CFGR_INIT		(0x00000040)	// D3PPRE = div2

#define	FLASH_ACR_INIT		(0x00000033)	// WRHIGHFREQ = 3, LATENCY = 3

/* Clock frequency 　*/
/* D1 Comain */
#define	SYSCLK			(550)		/* System clock (CPU Clock) */
#define TMCLK			(SYSCLK)	/* System timer clock input (MHz) */
#define TMCLK_KHz		(TMCLK * 1000)	/* System timer clock input (kHz) */
#define	HCLK			(SYSCLK/2)	/* Peripheral clock (AHB3) */
#define	D1PCLK1			(SYSCLK/4)	/* Peripheral clock (APB3) */

/* D2 Domain */
#define	HCLK12			(SYSCLK/2)	/* Peripheral clock (AHB1&2) */
#define	PCLK1			(SYSCLK/4)	/* Peripheral clock (APB1) */
#define	PCLK2			(SYSCLK/4)	/* Peripheral clock (APB2) */

/* D3 Domain */
#define	HCLK4			(SYSCLK/2)	/* Peripheral clock (AHB4) */
#define	D3PCLK1			(SYSCLK/4)	/* Peripheral clock (APB4) */

/*
 * Maximum value of Power-saving mode switching prohibition request.
 * Use in tk_set_pow API.
 */
#define LOWPOW_LIMIT	0x7fff		/* Maximum number for disabling */

#endif /* __TK_SYSDEF_DEPEND_H__ */
