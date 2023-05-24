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
 *	i2c_cnf_sysdep.h 
 *	I2C device configuration file
 *		for RP2040
 */
#ifndef	__DEV_I2C_CNF_RP2040_H__
#define	__DEV_I2C_CNF_RP2040_H__

/* Device initialization */
#define	DEVCNF_I2C_RESET	TRUE	// Reset I2C module
#define	DEVCNF_I2C_SETPINFUNC	TRUE	// Set I/O pin function

/* Register initial value */

// Interrupt priority
#define	DEVCNF_I2C0_INTPRI	2
#define	DEVCNF_I2C1_INTPRI	2

/* Communication timeout time */
#define	DEVCNF_I2C0_TMO		1000
#define	DEVCNF_I2C1_TMO		1000

#endif		/* __DEV_I2C_CNF_RP2040_H__ */
