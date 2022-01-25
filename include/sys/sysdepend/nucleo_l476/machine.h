/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (Nucleo-64 STM32L476 depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- Nucleo-64 STM32L467 (CPU: STM32L476) definition ----- */

#define NUCLEO_L476		1		/* Target system : Nucleo-64 STM32L467 */
#define CPU_STM32L4		1		/* Target CPU : STM32L4 series */
#define CPU_STM32L476		1		/* Target CPU : STM32L476 */
#define CPU_CORE_ARMV7M		1		/* Target CPU-Core type : ARMv7-M */
#define CPU_CORE_ACM4		1		/* Target CPU-Core : ARM Cortex-M4 */

#define TARGET_DIR		nucleo_l476	/* Sysdepend-Directory name */

/*
 **** CPU-depeneded profile (STM32L4)
 */
#include <sys/sysdepend/cpu/stm32l4/machine.h>


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
