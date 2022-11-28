/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef PICO_RP2040

/*
 *	cpu_clock.c (RaspberryPi Pico)
 *	Clock Setting
 */

#include <tk/tkernel.h>
#include "sysdepend.h"

/* 
 * PLL initialization
 */
LOCAL void init_pll(UW pll, UINT refdiv, UINT vco_mhz, UINT post_div1, UINT post_div2)
{
	UW	fbdiv, pdiv;
	UW	pll_reset;

	fbdiv	= vco_mhz / (XOSC_MHz / refdiv);
	pdiv = (post_div1 << PLL_PRIM_POSTDIV1_LSB)|(post_div2 << PLL_PRIM_POSTDIV2_LSB);

	pll_reset = (pll==PLL_USB_BASE)? RESETS_RESET_PLLUSB: RESETS_RESET_PLLSYS;
	set_w(RESETS_RESET, pll_reset);
	clr_w(RESETS_RESET, pll_reset);

	out_w(pll+PLL_CS, refdiv);
	out_w(pll+PLL_FBDIV_INT, fbdiv);

	clr_w(pll+PLL_PWR, (PLL_PWR_PD | PLL_PWR_VCOPD));
	while (!(in_w(pll+PLL_CS) & PLL_CS_LOCK)) ;

	out_w(pll+PLL_PRIM, pdiv);
	clr_w(pll+PLL_PWR, PLL_PWR_POSTDIVPD);
}

/* 
 * Peripheral clock setting
 */
LOCAL void clock_config(UINT clock_kind, UW clksrc, UW src_freq, UW freq)
{
	UW	clock;
	UW	div;

	if (freq > src_freq) return;

	clock = CLOCKS_BASE + (clock_kind * 0xC);

	div = (UW)(((UD)src_freq << 8) / freq);
	if (div > in_w(clock+CLK_x_DIV)) out_w(clock+CLK_x_DIV, div);

	clr_w(clock+CLK_x_CTRL, CLK_CTRL_ENABLE);	// Disable clock
	for( _UW dely_cyc = 3; dely_cyc--;) ;

	out_w(clock+CLK_x_CTRL, (in_w(clock+CLK_x_CTRL) & CLK_SYS_CTRL_AUXSRC) | (clksrc << 5));

	set_w(clock+CLK_x_CTRL, CLK_CTRL_ENABLE);	// Enable clock
	out_w(clock+CLK_x_DIV, div);
}

/*
 *  Startup System Clock
 */
EXPORT void startup_clock(ATR clkatr)
{
	UW	div;

	out_w(CLK_SYS_RESUS_CTRL, 0);

	/* XOSC settings */
	out_w(XOSC_CTRL, XOSC_CTRL_FRANG_1_15MHZ);
	out_w(XOSC_STARTUP, XOSC_STARTUP_DELAY);
	set_w(XOSC_CTRL, XOSC_CTRL_ENABLE);
	while((in_w(XOSC_STATUS) & XOSC_STATUS_STABLE)==0);

	clr_w(CLK_SYS+CLK_x_CTRL, CLK_SYS_CTRL_SRC);
	while(in_w(CLK_SYS+CLK_x_SELECTED) != 0x1);
	clr_w(CLK_REF+CLK_x_CTRL, CLK_REF_CTRL_SRC);
	while(in_w(CLK_REF+CLK_x_SELECTED) != 0x1);

	/* PLL settings */
	init_pll(PLL_SYS_BASE, 1, 1500, 6, 2);	// PLL SYS 125MHz
	init_pll(PLL_USB_BASE, 1, 480, 5, 2);	// PLL USB 48MHz

	/* CLK_REF settings */
	div = (XOSC_MHz << 8) / XOSC_MHz;
	if (div > in_w(CLK_REF+CLK_x_DIV)) out_w(CLK_REF+CLK_x_DIV, div);

	out_w(CLK_REF+CLK_x_CTRL, (in_w(CLK_REF+CLK_x_CTRL) & ~CLK_REF_CTRL_SRC) | 2 );	// clock source = xosc
	while(!(in_w(CLK_REF+CLK_x_SELECTED)&(1<<2)));

	out_w(CLK_REF+CLK_x_DIV, div);

	/* CLK SYS settings */
	div = (TMCLK_MHz << 8) / TMCLK_MHz;
	if (div > in_w(CLK_SYS+CLK_x_DIV)) out_w(CLK_SYS+CLK_x_DIV, div);

	clr_w(CLK_SYS+CLK_x_CTRL, CLK_SYS_CTRL_SRC);	// clock source = clk_ref
	while(!(in_w(CLK_SYS+CLK_x_SELECTED) & 0x1));

	out_w(CLK_SYS+CLK_x_CTRL, (in_w(CLK_SYS+CLK_x_CTRL) & ~CLK_SYS_CTRL_AUXSRC));	// auxsrc = 0(pll_sys)
	out_w(CLK_SYS+CLK_x_CTRL, (in_w(CLK_SYS+CLK_x_CTRL) & ~CLK_SYS_CTRL_SRC) | 1 );	// clock source = aux
	while(!(in_w(CLK_SYS+CLK_x_SELECTED)&(1<<1)));

	out_w(CLK_SYS+CLK_x_DIV, div);

	/* CLK PERI (UART+SPI) settings */
	if(clkatr & CLKATR_PREI) {
		clock_config( CLK_KIND_PERI, CLK_PERI_SRC, CLK_PLL_SYS_FREQ, CLK_PERI_FREQ);
	}

	/* CLK_USB settings */
	if(clkatr & CLKATR_USB) {
		clock_config( CLK_KIND_USB, CLK_USB_SRC, CLK_PLL_USB_FREQ, CLK_USB_FREQ);
	}

	/* CLK_ADC settings */
	if(clkatr & CLKATR_ADC) {
		clock_config( CLK_KIND_ADC, CLK_ADC_SRC, CLK_PLL_USB_FREQ, CLK_ADC_FREQ);
	}

	/* CLK_RTC settings */
	if(clkatr & CLKATR_RTC) {
		clock_config( CLK_KIND_RTC, CLK_RTC_SRC, CLK_PLL_USB_FREQ, CLK_RTC_FREQ);
	}
}

EXPORT void shutdown_clock(void)
{

}

#endif /* PICO_RP2040 */