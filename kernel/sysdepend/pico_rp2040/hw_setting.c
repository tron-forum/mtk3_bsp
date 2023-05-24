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
 *	hw_setting.c (RaspberryPi Pico)
 *	startup / shoutdown processing for hardware
 *	
 *	Pin function Setting (for RasoberryPi Pico)
 *		P25  : GPIO out (LED)
 *		P0   : UART0-TX
 *		P1   : UART0-RX
 */

#include <kernel.h>
#include <tm/tmonitor.h>

#include "sysdepend.h"

/* 
 * Setup register data 
 */
typedef struct {
	UW	addr;
	UW	data;
} T_SETUP_REG;

/*
 * Release peripheral’s reset
 */
LOCAL const UW module_tbl[] = {
	RESETS_RESET_IO_BANK0,		/* IO_BANK0 */
	RESETS_RESET_PADS_BANK0,	/* PADS_BANK0 */
	RESETS_RESET_UART0,		/* UART0 */

#if USE_PTMR
	RESETS_RESET_PWM,		/* PWM */
#endif	/* USE_PTMR */

#if USE_SDEV_DRV	// Do not use sample device driver
	RESETS_RESET_ADC,		/* A/DC */
	RESETS_RESET_I2C0,		/* I2C0 */
#endif /* USE_SDEV_DRV */
	0
};

/* 
 * Setup pin functions Tadle
 */
LOCAL const T_SETUP_REG pinfnc_tbl[] = {
	/* P25 : LED */
	{GPIO_CTRL(25),	GPIO_CTRL_FUNCSEL_SIO},		/* P25 GPIO */
	{GPIO_OE, (1<<25)},				/* P25 output enable */
	/* P0,P1 : UART0 */
	{GPIO_CTRL(0),	GPIO_CTRL_FUNCSEL_UART},	/* P0 UART0-TX */
	{GPIO_CTRL(1),	GPIO_CTRL_FUNCSEL_UART},	/* P1 UART0-RX */

#if USE_SDEV_DRV	// Do not use sample device driver
	/* P26 : ADC0 */
	{GPIO_CTRL(26),	GPIO_CTRL_FUNCSEL_NULL},
	{GPIO(26), GPIO_DRIVE_4MA | GPIO_SHEMITT},	/* Disable input & pull-up & pull-down */

	/* P27 : ADC1 */
	{GPIO_CTRL(27),	GPIO_CTRL_FUNCSEL_NULL},
	{GPIO(27), GPIO_DRIVE_4MA | GPIO_SHEMITT},	/* Disable input & pull-up & pull-down */

	/* P28 : ADC2 */
	{GPIO_CTRL(28),	GPIO_CTRL_FUNCSEL_NULL},
	{GPIO(28), GPIO_DRIVE_4MA | GPIO_SHEMITT},	/* Disable input & pull-up & pull-down */

	/* P8 : I2C0_SDA */
	{GPIO_CTRL(8),	GPIO_CTRL_FUNCSEL_I2C},
	{GPIO(8), GPIO_IE | GPIO_DRIVE_4MA | GPIO_PUE | GPIO_SHEMITT},	/* Pull-up */

	/* P9 : I2C0_SCL */
	{GPIO_CTRL(9),	GPIO_CTRL_FUNCSEL_I2C},
	{GPIO(9), GPIO_IE | GPIO_DRIVE_4MA | GPIO_PUE | GPIO_SHEMITT},	/* Pull-up */

#endif /* USE_SDEV_DRV */
	{0, 0}
};

/*
 * Startup Device
 */
EXPORT void knl_startup_hw(void)
{
	const T_SETUP_REG	*p;
	UW	rst;

	/* Startup System Clock */
	startup_clock(CLKATR_USB | CLKATR_ADC | CLKATR_RTC | CLKATR_PREI);

	for(INT i = 0; (rst = module_tbl[i]); i++) {
		set_w(RESETS_RESET, rst);
		clr_w(RESETS_RESET, rst);
		while((in_w(RESETS_RESET_DONE) & rst)==0);
	}

	for(p = pinfnc_tbl; p->addr != 0; p++) {
		out_w(p->addr, p->data);
	}
}

#if USE_SHUTDOWN
/*
 * Shutdown device
 */
EXPORT void knl_shutdown_hw( void )
{
	disint();
	while(1);
}
#endif /* USE_SHUTDOWN */

/*
 * Re-start device
 *	mode = -1		reset and re-start	(Reset -> Boot -> Start)
 *	mode = -2		fast re-start		(Start)
 *	mode = -3		Normal re-start		(Boot -> Start)
 */
EXPORT ER knl_restart_hw( W mode )
{
	switch(mode) {
	case -1: /* Reset and re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESET & RESTART >>\n");
		return E_NOSPT;
	case -2: /* fast re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM FAST RESTART >>\n");
		return E_NOSPT;
	case -3: /* Normal re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESTART >>\n");
		return E_NOSPT;
	default:
		return E_PAR;
	}
}


#endif /* PICO_RP2040 */