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

/*
 *	sysdepend.h (RaspberryPi Pico)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_TARGET_SYSDEPEND_
#define _SYSDEPEND_TARGET_SYSDEPEND_

#include "../cpu/rp2040/sysdepend.h"

/*
 *    Clock Setting (cpu_clock.c)
 */
IMPORT void startup_clock(ATR clkatr);
#define	CLKATR_PREI		0x00000001
#define	CLKATR_USB		0x00000002
#define	CLKATR_ADC		0x00000004
#define	CLKATR_RTC		0x00000008

#endif /* _SYSDEPEND_TARGET_SYSDEPEND_ */