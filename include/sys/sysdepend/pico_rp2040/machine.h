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
 *	machine.h
 *
 *	Machine type definition (RaspberryPi Pico depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- Nucleo-64 STM32L467 (CPU: STM32L476) definition ----- */

#define PICO_RP2040		1		/* Target system : RaspberryPi Pico */
#define CPU_RP2040		1		/* Target CPU : RP2040 */
#define CPU_CORE_ARMV6M		1		/* Target CPU-Core type : ARMv6-M */
#define CPU_CORE_ACM0P		1		/* Target CPU-Core : ARM Cortex-M0+ */

#define TARGET_DIR		pico_rp2040	/* Sysdepend-Directory name */
#define	TARGET_CPU_DIR		rp2040		/* Sysdepend-CPU-Directory name */

/*
 **** CPU-depeneded profile (RP2040)
 */
#include <sys/sysdepend/cpu/rp2040/machine.h>


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
