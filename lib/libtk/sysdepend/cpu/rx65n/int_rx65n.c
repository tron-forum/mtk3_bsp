/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06.B0
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/04.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RX65N

/*
 *	int.c
 *
 *	Interrupt controll (RX65N )
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

/* 
 * Set Software Configurable Interrupt (PERIA & PERIB)
 *	intno: Interrupt number  128~207(PERIB), 208~255(PERIA)
 *	fctno: Interrupt factor number
 */
EXPORT ER SetPERI(UINT intno, UINT fctno)
{
	if((intno < 128) || (intno >255)) return E_PAR;		// Interrupt number is out of range
	if(in_b(ICU_SLIPRCR)!=0) return E_RONLY;		// Changes are prohibited

	out_b(ICU_IER(intno), (in_b(ICU_IER(intno)) & ~(ICU_IER_IEN(intno))));

	if(intno >= 128 && intno <= 143) {
		out_b(ICU_SLIBXR(intno), fctno);
	} else if(intno >= 144 && intno <= 207) {
		out_b(ICU_SLIBR(intno), fctno);
	} else {
		out_b(ICU_SLIAR(intno), fctno);
	}

	out_b(ICU_IR(intno), 0);
	out_b(ICU_IER(intno), (in_b(ICU_IER(intno)) | (ICU_IER_IEN(intno))));

	return E_OK;
}

#endif /* CPU_RX65N */