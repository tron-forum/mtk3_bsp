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
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (RaspberryPi Pico)
 */

#include <tk/typedef.h>
#include <sys/sysdef.h>

#if USE_TMONITOR
#include "../../libtm.h"

#ifdef PICO_RP2040
#if TM_COM_SERIAL_DEV

/* UART register definition (Use UART0) */
#define UART_BASE              0x40034000

#define UART_DR		(*(_UW*)(UART_BASE+0x000))
#define UART_FR		(*(_UW*)(UART_BASE+0x018))
#define UART_IBRD	(*(_UW*)(UART_BASE+0x024))
#define UART_FBRD	(*(_UW*)(UART_BASE+0x028))
#define UART_LCR_H	(*(_UW*)(UART_BASE+0x02C))
#define UART_CR		(*(_UW*)(UART_BASE+0x030))

#define	FR_TXFE		(1<<7)	// Transmit FIFO empty
#define	FR_RXFE		(1<<4)	// Receive FIFO empty

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while ((UART_FR & FR_TXFE) == 0 );
		UART_DR = *b;
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	for( ; size > 0; size--, buf++ ){
		while ( (UART_FR & FR_RXFE) != 0 );
		*buf = UART_DR & 0xff;
	}
}

EXPORT	void	tm_com_init(void)
{
	UART_IBRD	= 67;			/* Baud rate setting */
	UART_FBRD	= 52;
	UART_LCR_H	= 0x70;			/* Communication data format setting */
	UART_CR 	= (1<<9)|(1<<8)|(1<<0);	/* Communication enabled */
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* PICO_RP2040 */
#endif /* USE_TMONITOR */