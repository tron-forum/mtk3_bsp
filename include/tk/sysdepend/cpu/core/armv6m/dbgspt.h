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
 *	dbgspt_depend.h
 *
 *	micro T-Kernel Debugger Support  Definition (ARMv6-M core depended)
 */

#ifndef __TK_DBGSPT_DEPEND_CORE_H__
#define __TK_DBGSPT_DEPEND_CORE_H__

/*
 * System call/extension SVC caller information 
 */
typedef struct td_calinf {
	void	*ssp;		/* System stack pointer */
	void	*r11;		/* Frame pointer when calling */
} TD_CALINF;

#endif /* __TK_DBGSPT_DEPEND_CORE_H__ */
