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
 *	machine.h
 *
 *	Machine type definition (Nucleo-144 STM32H723 depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- Nucleo-144 STM32H723 (CPU: STM32H723) definition ----- */

#define NUCLEO_H723		1		/* Target system : Nucleo-144 STM32H723 */
#define CPU_STM32H7		1		/* Target CPU : STM32H7 series */
#define CPU_STM32H723		1		/* Target CPU : STM32H723 */
#define CPU_CORE_ARMV7M		1		/* Target CPU-Core type : ARMv7-M */
#define CPU_CORE_ACM7		1		/* Target CPU-Core : ARM Cortex-M7 */

#define TARGET_DIR		nucleo_h723	/* Sysdepend-Directory name */
#define	TARGET_CPU_DIR		stm32h7		/* Sysdepend-CPU-Directory name */

/*
 **** CPU-depeneded profile (STM32H7)
 */
#include <sys/sysdepend/cpu/stm32h7/machine.h>


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
