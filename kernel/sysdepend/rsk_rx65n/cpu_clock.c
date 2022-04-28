/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/04.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef RSK_RX65N

/*
 *	cpu_clock.c (Renesas Starter Kit+ for RX65N)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

/*
 *  Startup System Clock
 *    Used Main Clock(24MHz), Uesd PLL/UPLL, System Clock 120MHz
 *    ICLK:120MHz, PCLKA:120MHz, PCLKB:60MHz, PCLKC:60MHz, PCLKD:60MHz, FCLK:60MHz, BCLK:60MHz, UCLK:40MHz
 */
EXPORT void startup_clock(void)
{
	out_h(SYSTEM_PRCR, 0xA503);			/* Register Protect Disable */

	out_b(SYSTEM_MOFCR, 0x00);			/* Set Main Clock (24MHz XTAL) */
	out_b(SYSTEM_MOSCWTCR, 0x5C);			/* Set Main CLock Wait Time */
	out_b(SYSTEM_MOSCCR, 0x00);			/* Enable Main Clock */
	while(!(in_b(SYSTEM_OSCOVFSR) & 0x01));		/* Wait Main Clock Stabilization */

	out_b(SYSTEM_ROMWT, 0x02);			/* ROM wait 2 (120MHz) */

	out_h(SYSTEM_PLLCR, 0x1301);			/* PLL 24MHz/2*10=120MHz */
	out_b(SYSTEM_PLLCR2, 0x00);			/* Enable PLL */
	while(!(in_b(SYSTEM_OSCOVFSR) & 0x04));		/* Wait PLL Stabilization */

	out_w(SYSTEM_SCKCR, 0x10010111);		/* ICLK=PCLKA:120MHz,FCLK=BCLK=PCLKB=PCLKC=PCLKD=60MHz */
	out_h(SYSTEM_SCKCR2, 0x0021);			/* UCLK:40MHz */
	out_h(SYSTEM_SCKCR3, 0x0400);			/* Select PLL */

	out_b(SYSTEM_LOCOCR, 0x01);			/* Disable LOCO */
	out_b(RTC_RCR3, 0x06);				/* Disable Sub Clock */
	out_b(SYSTEM_SOSCCR, 0x01);			/* Disable Sub Clock */
	while(!(in_b(SYSTEM_OSCOVFSR) & 0x02));		/* Wait Sub Clock Stoped */

	out_h(SYSTEM_PRCR, 0xA500);			/* Register protect Enable */

	return;
}

EXPORT void shutdown_clock(void)
{
}

#endif /* CPU_RX65N */