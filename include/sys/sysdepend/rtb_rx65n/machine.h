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

/*
 *	machine.h
 *
 *	Machine type definition (Renesas Target Board for RX65N depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- Renesas Target Board for RX65N (CPU: RX65N) definition ----- */

#define RTB_RX65N		1		/* Target system : Target Board */
#define CPU_RX65N		1		/* Target CPU : Renesas RX65N */
#define CPU_CORE_RXV2		1		/* Target CPU-Core : RX200 series*/

#define TARGET_DIR		rtb_rx65n	/* Sysdepend-Directory name */

/*
 **** CPU-depeneded profile (RX65N)
 */
#include "../cpu/rx65n/machine.h"


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
