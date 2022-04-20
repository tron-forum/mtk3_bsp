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
 *	sysdef.h
 *
 *	System dependencies definition (Renesas Target Board for RX65N depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_H__
#define __SYS_SYSDEF_DEPEND_H__


/* CPU-dependent definition */
#include "../cpu/rx65n/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * System Clock Setting 
 *
 */
#define	MHz		(UW)(1000000UL)

#define	SYSCLK_ICLK	(UW)(120*MHz)
#define	SYSCLK_PCLKA	(UW)(120*MHz)
#define	SYSCLK_PCLKB	(UW)(60*MHz)
#define	SYSCLK_PCLKC	(UW)(60*MHz)
#define	SYSCLK_PCLKD	(UW)(60*MHz)

/* System timer related definition */
#define PCLK_DIV	(2)		/* PCLK Devistion = PCLKB of SCKCR */
#define CMCR_DIV	(32)		/* CMT Clock = PCLK/CMCR_DIV */
#define CMCR_CKS_PLCK	(0x0001)	/* CMCR.CKS = PCLK/32 */

/* ------------------------------------------------------------------------ */
/*
 * Maximum value of Power-saving mode switching prohibition request.
 * Use in tk_set_pow API.
 */
#define LOWPOW_LIMIT	0x7fff		/* Maximum number for disabling */

#endif /* __TK_SYSDEF_DEPEND_H__ */
