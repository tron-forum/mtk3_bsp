/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
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
 *	adc_sysdep.h
 *	A/D converter device driver
 *	System-dependent definition for RP2040
 */

#ifndef	__DEV_ADC_RP2040_H__
#define	__DEV_ADC_RP2040_H__

#define	DEV_ADC_UNITNM	(1)	/* Number of devive units */

/* NUmber of A/DC chanels */
#define	ADC_CH_NUM	(5)


/*----------------------------------------------------------------------
 * A/DC registers
 */

#define ADC_BASE		(0x4004C000)

#define	ADC_CS			(ADC_BASE+0x00)		// ADC Control and Status
#define	ADC_RESULT		(ADC_BASE+0x04)		// Result of most recent ADC conversion
#define	ADC_FCS			(ADC_BASE+0x08)		// FIFO control and status
#define	ADC_FIFO		(ADC_BASE+0x0C)		// Conversion result FIFO
#define	ADC_DIV			(ADC_BASE+0x10)		// Clock divider
#define	ADC_INTR		(ADC_BASE+0x14)		// Raw Interrupts
#define	ADC_INTE		(ADC_BASE+0x18)		// Interrupt Enable
#define	ADC_INTF		(ADC_BASE+0x1C)		// Interrupt Force
#define	ADC_INTS		(ADC_BASE+0x20)		// Interrupt status after masking & forcing

#define	ADC_CS_EN		(1<<0)			// Power on ADC and enable its clock.
#define	ADC_CS_TS_EN		(1<<1)			// Power on temperature sensor.
#define	ADC_CS_STRAT_ONCE	(1<<2)			// Start a single conversion.
#define	ADC_CS_START_MANY	(1<<3)			// Continuously perform conversions.
#define	ADC_CS_READY		(1<<8)			// ADC is ready to start a new conversion.
#define	ADC_CS_ERR		(1<<9)			// The most recent ADC conversion encountered an error.
#define	ADC_CS_ERR_STICKY	(1<<10)			// Some past ADC conversion encountered an error.
#define	ADC_CS_AINSEL		(0x00007000)		// Select analog mux input. Updated automatically in roundrobin mode.
#define	ADC_CS_AINSEL_POS	12
#define	ADC_CS_RROBIN		(0x001F0000)		// Round-robin sampling.

#define ADC_FCS_THRESH		(0x0F000000)
#define ADC_FCS_THRESH_POS	24
#define ADC_FCS_LEVEL		(0x000F0000)
#define ADC_FCS_EN		1
/*
 * A/D converter interrupt number
*/
#define	INTNO_ADC		22

#endif		/* __DEV_ADC_RP2040_H__ */
