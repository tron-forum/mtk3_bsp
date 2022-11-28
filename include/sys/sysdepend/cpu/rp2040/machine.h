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
 *	machine_depend.h
 *
 *	Machine type definition (RP2040 depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_CPU_H__
#define __SYS_SYSDEPEND_MACHINE_CPU_H__

/*
 **** CPU core-depeneded profile (ARMv6M(ARM Cortex-M0+))
 */

#include "../core/armv6m/machine.h"

/*
 **** CPU-depeneded profile (RP2040)
 */

#define	TARGET_CPU_DIR		rp2040		/* Sysdepend-CPU-Directory name */


#endif /* __SYS_SYSDEPEND_MACHINE_CPU_H__ */
