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
#define	RCC_CFGR_SW_INIT	(0x3)		// System clock switch  3 = pll1_p_ck

#define RCC_PLLCKSELR_INIT	(0x02020042)	// PLL SRC = hse_ck, M = 4
#define	RCC_PLL1DIVR_INIT	(0x01030112)	// N = 275, P = 1, Q = 4, R = 2
#define	RCC_PLL1FRACR_INIT	(0x00000000)	// FRACN = 0
#define	RCC_PLLCFGR_INIT	(0x01FF0004)	// PLL1RGE = 1, PLL1VCOSEL = 0

#define RCC_PLL2DIVR_INIT	(0x01010117)	// PLL2 N = 24, P = 2, Q = 2, R = 2
#define	RCC_PLL2FRACR_INIT	(0x00000000)	// FRACN = 0

#define RCC_PLLCKSELR_DIVM2_INIT	(1)	// Prescaler for PLL2
#define RCC_PLLCFGR_PLL2RGE_INIT	(3)	// 8 ~ 16 MHz
#define RCC_PLLCFGR_PLL2VCOSEL_INIT	(0)	// Wide VCO range

#define RCC_PLL3DIVR_INIT	(0x01010280)	// PLL3 N = 129, P = 2, Q = 2, R = 2
#define	RCC_PLL3FRACR_INIT	(0x00000000)	// FRACN = 0

#define RCC_PLLCKSELR_DIVM3_INIT	(32)	// Prescaler for PLL3
#define RCC_PLLCFGR_PLL3RGE_INIT	(0)	// PLL3 input frequency range
#define RCC_PLLCFGR_PLL3VCOSEL_INIT	(0)	// Wide VCO range

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
