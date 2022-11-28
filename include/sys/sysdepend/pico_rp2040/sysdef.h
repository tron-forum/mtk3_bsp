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
 *	sysdef.h
 *
 *	System dependencies definition (RaspberryPi Pico depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_H__
#define __SYS_SYSDEF_DEPEND_H__


/* CPU-dependent definition */
#include <sys/sysdepend/cpu/rp2040/sysdef.h>

/* ------------------------------------------------------------------------ */
/*
 * Clock control definition
 */

/* RCC register initial value */

/* Clock frequency 　*/
#define	KHz			(1000)
#define	MHz			(KHz*1000)

#define	SYSCLK			(125)		/* System clock */
#define	TMCLK_MHz		(125)
#define	TMCLK_KHz		(TMCLK_MHz*1000)

#define	XOSC_MHz		(12)
#define	XOSC_KHz		(XOSC_MHz*1000)
#define XOSC_STARTUP_DELAY	((XOSC_KHz + 128) / 256)

#define	CLK_PLL_SYS_FREQ	(TMCLK_MHz*MHz)
#define	CLK_PLL_USB_FREQ	(48*MHz)

#define	CLK_USB_FREQ		(48*MHz)
#define	CLK_ADC_FREQ		(48*MHz)
#define	CLK_RTC_FREQ		(46875)
#define	CLK_PERI_FREQ		(125*MHz)

#define	CLK_USB_SRC		0	// clksrc_pll_usb
#define	CLK_ADC_SRC		0	// clksrc_pll_usb
#define	CLK_RTC_SRC		0	// clksrc_pll_usb
#define	CLK_PERI_SRC		0	// clk_sys


/*
 * Maximum value of Power-saving mode switching prohibition request.
 * Use in tk_set_pow API.
 */
#define LOWPOW_LIMIT	0x7fff		/* Maximum number for disabling */

#endif /* __TK_SYSDEF_DEPEND_H__ */
