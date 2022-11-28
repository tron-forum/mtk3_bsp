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
 *	micro T-Kernel System Library  (RP2040 depended)
 */

#ifndef __TK_SYSLIB_CPU_DEPEND_H__
#define __TK_SYSLIB_CPU_DEPEND_H__

#include "../core/armv6m/syslib.h"

/* ------------------------------------------------------------------------ */
/*
 * Interrupt Control
 */

/*
 * Interrupt number
 */
#define MIN_NVIC_INTNO	0		/* Minimum value of NVIC interrupt number */
#define MAX_NVIC_INTNO	(N_INTVEC-1)	/* Maximum value of NVIC interrupt number */

/*
 * Interrupt mode ( Use SetIntMode )
 */
#define IM_EDGE		0x0000		/* Edge trigger */
#define IM_HI		0x0002		/* Interrupt at rising edge */
#define IM_LOW		0x0001		/* Interrupt at falling edge */
#define IM_BOTH		0x0003		/* Interrupt at both edge */


/* ------------------------------------------------------------------------ */
/*
 * I/O port access
 *	for memory mapped I/O
 */
Inline void out_w( UW port, UW data )
{
	*(_UW*)port = data;
}
Inline void out_h( UW port, UH data )
{
	*(_UH*)port = data;
}
Inline void out_b( UW port, UB data )
{
	*(_UB*)port = data;
}

Inline UW in_w( UW port )
{
	return *(_UW*)port;
}
Inline UH in_h( UW port )
{
	return *(_UH*)port;
}
Inline UB in_b( UW port )
{
	return *(_UB*)port;
}

/* ------------------------------------------------------------------------ */
/*
 * I / O register logical operation
 *	RP2040 specific function
 */
#define OP_CLR	0x3000
Inline void clr_w(UW adr, UW data)
{
	*(_UW*)(adr + OP_CLR) = data;
}

#define OP_SET	0x2000
Inline void set_w(UW adr, UW data)
{
    *(_UW*)(adr + OP_SET) = data;
}

#define OP_XOR	0x1000
Inline void xor_w(UW adr, UW data)
{
    *(_UW*)(adr + OP_XOR) = data;
}

#endif /* __TK_SYSLIB_DEPEND_H__ */
