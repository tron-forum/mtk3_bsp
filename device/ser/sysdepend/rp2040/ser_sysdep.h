/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2022-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/11.
 *
 *----------------------------------------------------------------------
 */
/*
 *	ser_sysdep.h
 *	Serial communication device driver
 *	System-dependent definition for RP2040
 */

#ifndef	__DEV_SER_RP2040_H__
#define	__DEV_SER_RP2040_H__

#define	DEV_SER_UNITNM	(2)	/* Number of devive channels */
#define DEV_SER_UNIT0	(0)	/* Ch.0 - UART0 */
#define DEV_SER_UNIT1	(1)	/* Ch.1 - UART1 */

/*
 * UART registers
 */

/* Base address for each channel */
#define	UART0_BASE	(0x40034000UL)
#define	UART1_BASE	(0x40038000UL)

#define	UARTx_DR		(0x000)	// Data Register
#define	UARTx_RSR		(0x004)	// Receive Status Register/Error Clear Register
#define	UARTx_FR		(0x018)	// Flag Register
#define	UARTx_ILPR		(0x020)	// IrDA Low-Power Counter Register
#define	UARTx_IBRD		(0x024)	// Integer Baud Rate Register
#define	UARTx_FBRD		(0x028)	// Fractional Baud Rate Register
#define	UARTx_LCR_H		(0x02c)	// Line Control Register
#define	UARTx_CR		(0x030)	// Control Register
#define	UARTx_IFLS		(0x034)	// Interrupt FIFO Level Select Register
#define	UARTx_IMSC		(0x038)	// Interrupt Mask Set/Clear Register
#define	UARTx_RIS		(0x03c)	// Raw Interrupt Status Register
#define	UARTx_MIS		(0x040)	// Masked Interrupt Status Register
#define	UARTx_ICR		(0x044)	// Interrupt Clear Register
#define	UARTx_DMACR		(0x048)	// DMA Control Register
#define	UARTx_PERIPHID0		(0xFE0)	// UARTPeriphID0 Register
#define	UARTx_PERIPHID1		(0xFE4)	// UARTPeriphID1 Register
#define	UARTx_PERIPHID2		(0xFE8)	// UARTPeriphID2 Register
#define	UARTx_PERIPHID3		(0xFEc)	// UARTPeriphID3 Register
#define	UARTx_PCELLID0		(0xFF0)	// UARTPCellID0 Register
#define	UARTx_PCELLID1		(0xFF4)	// UARTPCellID1 Register
#define	UARTx_PCELLID2		(0xFF8)	// UARTPCellID2 Register
#define	UARTx_PCELLID3		(0xFFc)	// UARTPCellID3 Register

/* Register bit definition */
#define	UART_DR_ERR		(0x0F00)	// Error Bits
#define	UART_DR_DATA		(0x00FF)	// Receive & Trnsmit Data

#define	UART_FR_RI		(1<<8)	// Ring indicator.
#define	UART_FR_TXFE		(1<<7)	// Transmit FIFO empty.
#define	UART_FR_RXFF		(1<<6)	// Receive FIFO full.
#define	UART_FR_TXFF		(1<<5)	// Transmit FIFO full.
#define	UART_FR_RXFE		(1<<4)	// Receive FIFO empty.
#define	UART_FR_BUSY		(1<<3)	// UART busy.
#define	UART_FR_DCD		(1<<2)	// Data carrier detect.
#define	UART_FR_DSR		(1<<1)	// Data set ready.
#define	UART_FR_CTS		(1<<0)	// Clear to send.

#define	UART_LCR_H_SPS		(1<<7)	// Stick parity select
#define	UART_LCR_H_WLEN		(3<<5)	// Word length
#define	UART_LCR_H_FEN		(1<<4)	// Enable FIFOs (0:disable)
#define	UART_LCR_H_STP2		(1<<3)	// Two stop bits (1:enable)
#define	UART_LCR_H_EPS		(1<<2)	// Even parity select (0: odd 1:even)
#define	UART_LCR_H_PEN		(1<<1)	// Parity enable (0: no parity)
#define	UART_LCR_H_BRK		(1<<0)	// Send break (1: break)

#define	UART_LCR_H_WLEN_8	(3<<5)	// Word length 8
#define	UART_LCR_H_WLEN_7	(2<<5)	// Word length 7
#define	UART_LCR_H_WLEN_6	(1<<5)	// Word length 6
#define	UART_LCR_H_WLEN_5	(0<<5)	// Word length 5

#define	UART_CR_CTSEN		(1<<15)	// CTS hardware flow control enable.
#define	UART_CR_RTSEN		(1<<14)	// RTS hardware flow control enable.
#define	UART_CR_OUT2		(1<<13)	// For DTE this can be used as Ring Indicator (RI).
#define	UART_CR_OUT1		(1<<12)	// For DTE this can be used as Data Carrier Detect (DCD).
#define	UART_CR_RTS		(1<<11)	// Request to send.(nUARTRTS is LOW)
#define	UART_CR_DTR		(1<<10)	// Data transmit ready.(nUARTDTR is LOW.)
#define	UART_CR_RXE		(1<<9)	// Receive enable.
#define	UART_CR_TXE		(1<<8)	// Transmit enable.
#define	UART_CR_LBE		(1<<7)	// Loopback enable.
#define	UART_CR_SIRLP		(1<<2)	// SIR low-power IrDA mode.
#define	UART_CR_SIREN		(1<<1)	// SIR enable.
#define	UART_CR_UARTEN		(1<<0)	// UART enable.

#define	UART_IMSC_OEIM		(1<<10)	// Overrun error interrupt mask.
#define	UART_IMSC_BEIM		(1<<9)	// Break error interrupt mask.
#define	UART_IMSC_PEIM		(1<<8)	// Parity error interrupt mask.
#define	UART_IMSC_FEIM		(1<<7)	// Framing error interrupt mask.
#define	UART_IMSC_RTIM		(1<<6)	// Receive timeout interrupt mask.
#define	UART_IMSC_TXIM		(1<<5)	// Transmit interrupt mask.
#define	UART_IMSC_RXIM		(1<<4)	// Receive interrupt mask.
#define	UART_IMSC_DSRMIM	(1<<3)	// nUARTDSR modem interrupt mask.
#define	UART_IMSC_DCDMIM	(1<<2)	// nUARTDCD modem interrupt mask.
#define	UART_IMSC_CTSMIM	(1<<1)	// nUARTCTS modem interrupt mask.
#define	UART_IMSC_RIMIM		(1<<0)	// nUARTRI modem interrupt mask.

#define	UART_IMSC_EI		(0xFFFFFFF0)

#define	UART_RIS_OE		(1<<10)	// Overrun error interrupt status.
#define	UART_RIS_BE		(1<<9)	// Break error interrupt status.
#define	UART_RIS_PE		(1<<8)	// Parity error interrupt status.
#define	UART_RIS_FE		(1<<7)	// Framing error interrupt status.
#define	UART_RIS_RT		(1<<6)	// Receive timeout interrupt status.
#define	UART_RIS_TX		(1<<5)	// Transmit interrupt status.
#define	UART_RIS_RX		(1<<4)	// Receive interrupt status.
#define	UART_RIS_DSRM		(1<<3)	// nUARTDSR modem interrupt status.
#define	UART_RIS_DCDM		(1<<2)	// nUARTDCD modem interrupt status.
#define	UART_RIS_CTSM		(1<<1)	// nUARTCTS modem interrupt status.
#define	UART_RIS_RIM		(1<<0)	// nUARTRI modem interrupt status.

#define	UART_MIS_OE		(1<<10)	// Overrun error interrupt status.
#define	UART_MIS_BE		(1<<9)	// Break error interrupt status.
#define	UART_MIS_PE		(1<<8)	// Parity error interrupt status.
#define	UART_MIS_FE		(1<<7)	// Framing error interrupt status.
#define	UART_MIS_RT		(1<<6)	// Receive timeout interrupt status.
#define	UART_MIS_TX		(1<<5)	// Transmit interrupt status.
#define	UART_MIS_RX		(1<<4)	// Receive interrupt status.
#define	UART_MIS_DSRM		(1<<3)	// nUARTDSR modem interrupt status.
#define	UART_MIS_DCDM		(1<<2)	// nUARTDCD modem interrupt status.
#define	UART_MIS_CTSM		(1<<1)	// nUARTCTS modem interrupt status.
#define	UART_MIS_RIM		(1<<0)	// nUARTRI modem interrupt status.

/*Initial register value when using debug */
#define	UART_IBRD_DEBUG		(67)
#define	UART_FBRD_DEBUG		(52)
#define	UART_LCR_H_DEBUG	(0x00000070)
#define UART_CR_DEBUG		(0x00000301)	/* USART enable */

/* USART interrupt number */
#define	INTNO_UART0		20
#define	INTNO_UART1		21

#endif	/* __DEV_SER_RP2040_H__ */