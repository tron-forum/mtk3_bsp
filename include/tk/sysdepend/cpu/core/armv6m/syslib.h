/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/08.
 *
 *----------------------------------------------------------------------
 */

/*
 *	syslib.h
 *
 *	micro T-Kernel System Library  (ARMv6-M core depended)
 */

#ifndef __TK_SYSLIB_DEPEND_CORE_H__
#define __TK_SYSLIB_DEPEND_CORE_H__

#include <tk/errno.h>
#include <sys/sysdef.h>

/*----------------------------------------------------------------------*/
/*
 * CPU interrupt control for ARMv6-M.
 */

inline static void set_primask( UW pm )
{
	Asm("msr primask, %0":: "r"(pm));
}

inline static UW get_primask(void)
{
	UW	pm;
	Asm("mrs %0, primask": "=r"(pm));
	return pm;
}

IMPORT UW disint(void);			/* Disable interrupt */

#define	DI(intsts)	(intsts=disint())
#define	EI(intsts)	(set_primask(intsts))
#define isDI(intsts)	( (intsts) == 1 )

#define INTLEVEL_DI		(0)
#define INTLEVEL_EI		(1)

/*
 * Interrupt priority grouping
 */
#define INTPRI_GROUP(pri, subpri)	(((pri) << (8-INTPRI_BITWIDTH)) | (subpri))

/* ------------------------------------------------------------------------ */
/*
 * Convert to interrupt definition number
 *
 * For backward compatibility.
 * 	INTVEC has been obsoleted since micro T-Kernel 2.0.
 */
#define DINTNO(intvec)	(intvec)

#endif /* __TK_SYSLIB_DEPEND_CORE_H__ */
