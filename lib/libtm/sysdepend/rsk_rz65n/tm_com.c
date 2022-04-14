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
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (Renesas Stsrter Kit+ for RX65N)
 */

#include <tk/tkernel.h>
#include <sys/sysdef.h>

#if USE_TMONITOR
#include "../../libtm.h"

#ifdef RSK_RX65N
#if TM_COM_SERIAL_DEV

#define	PCLK		(60000000UL)		/* P Clock(60MHz) */
#define	ICLK		(120000000UL)		/* I Clock(120MHz) */
#define RS_BAUD_RATE	(115200)		/* 115.2kbps */

#define	SCI8_SMR	(0x0008A100UL)		/* Serial mode register */
#define SCI8_BRR	(0x0008A101UL)		/* Bit rate register */
#define	SCI8_SCR	(0x0008A102UL)		/* Serial control register */
#define	SCI8_TDR	(0x0008A103UL)		/* Transmit data register */
#define	SCI8_SSR	(0x0008A104UL)		/* Serial status register */
#define	SCI8_RDR	(0x0008A105UL)		/* Receive data register */
#define	SCI8_SCMR	(0x0008A106UL)		/* Smartcard mode register */
#define	SCI8_SEMR	(0x0008A107UL)		/* Serial Extended mode register */

#define	SCR_TIE		(0x80)			/* Transmit interrupt enable */
#define	SCR_RIE		(0x40)			/* Receive interrupt ebable */
#define	SCR_TE		(0x20)			/* Transmit enable */
#define	SCR_RE		(0x10)			/* Receive enable */

#define	SSR_TDRE	(0x80)			/* Transmit data empty */
#define SSR_RDRF	(0x40)			/* Receive data full */
#define	SSR_ERR		(0x38)			/* Error */

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while( (in_b(SCI8_SSR) & SSR_TDRE) == 0);
		out_b( SCI8_TDR, *b );
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	UB	ssr;

	for( ; size > 0; size--, buf++ ){
		while(1) {
			ssr = in_b(SCI8_SSR);
			if( (ssr & SSR_RDRF) != 0) {
				*buf = in_b( SCI8_RDR );
				break;
			} else if( ssr & SSR_ERR ) {	/* check Communication error */
				out_b( SCI8_SSR, ssr & ~SSR_ERR);
			}
		}
	}
}

EXPORT	void	tm_com_init(void)
{
	_UW	dly;

	/* Initialize SCI8 */
	out_b(SCI8_SCR, 0x00);			// TE = 0 RE = 0
	out_b(SCI8_SMR, 0x00);			// Data8 Stop1
	out_b(SCI8_SCMR, 0x72);			// LSB first
	out_b(SCI8_SEMR, 0x50);			// set ABCS
	out_b(SCI8_BRR, 0x40);			// 115.2kbps

	dly = ICLK/5/RS_BAUD_RATE;
	while ((dly--) != 0) ;

	out_b(SCI8_SCR, 0x30);				// TE = 1 RE = 1
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* STK_RC65N */
#endif /* USE_TMONITOR */