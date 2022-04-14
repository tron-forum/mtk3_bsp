/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2021-2021 by Ken Sakamura.
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
 *	System dependencies definition (Renesas Starter Kit+ for RX65N depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_H__
#define __SYS_SYSDEF_DEPEND_H__


/* CPU-dependent definition */
#include "../cpu/rx65n/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Maximum value of Power-saving mode switching prohibition request.
 * Use in tk_set_pow API.
 */
#define LOWPOW_LIMIT	0x7fff		/* Maximum number for disabling */

#endif /* __TK_SYSDEF_DEPEND_H__ */
