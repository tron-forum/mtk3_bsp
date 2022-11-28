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
 *	Machine type definition (ARMv6-M core depended)
 */

#ifndef __SYS_MACHINE_CORE_H__
#define __SYS_MACHINE_CORE_H__

/*
 * CPU_xxxx		CPU type
 * ALLOW_MISALIGN	1 if access to misalignment data is allowed 
 * BIGENDIAN		1 if big endian 
 */

/* ----- ARMv6-M definition ----- */

#define ALLOW_MISALIGN		0
#define INT_BITWIDTH		32

/*
 * Endianness
 */
#define BIGENDIAN		0	/* Default (Little Endian) */

#endif /* __SYS_MACHINE_CORE_H__ */
