/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2022-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/05.
 *
 *----------------------------------------------------------------------
 */


/*
 *	ser_cnf_sysdep.h 
 *	Serial Device configuration file
 *		for RP2040
 */
#ifndef	__DEV_SER_CNF_RP2040_H__
#define	__DEV_SER_CNF_RP2040_H__

/* Interrupt priority */
#define	DEVCNF_UART0_INTPRI	2
#define	DEVCNF_UART1_INTPRI	2

/* Debug option
 *	Specify the device used by T-Monitor.
 *	  -1 : T-Monitor does not use serial devices
 */
#if USE_TMONITOR
#define	DEVCNF_SER_DBGUN	0		// Used by T-Monitor
#else 
#define	DEVCNF_SER_DBGUN	-1		// T-Monitor not executed
#endif

#endif	/* __DEV_SER_CNF_RP2040_H__ */