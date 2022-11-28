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
 *	profile.h
 *
 *	Service Profile (RaspberryPi Pico depended)
 */

#ifndef __SYS_DEPEND_PROFILE_H__
#define __SYS_DEPEND_PROFILE_H__

/*
 **** CPU-depeneded profile (RP2040)
 */
#include <sys/sysdepend/cpu/rp2040/profile.h>

/*
 **** Target-depeneded profile (RaspberryPi Pico)
 */

/*
 * Power management
 */
#define TK_SUPPORT_LOWPOWER	FALSE		/* Support of power management */



#endif /* __SYS_DEPEND_PROFILE_H__ */
