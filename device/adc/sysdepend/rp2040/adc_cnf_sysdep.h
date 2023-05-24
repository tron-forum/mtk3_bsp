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
 *	adc_cnf_sysdep.h 
 *	A/D converter device driver configuration file
 *		for RP2040
 */
#ifndef	__DEV_ADC_CNF_RP2040_H__
#define	__DEV_ADC_CNF_RP2040_H__

/* 
 * Release A/DC reset
 *	Enable when not performed in the OS initialization process.
 */
#define	DEVCONF_ADC_REL_RESET	FALSE

/*
 * Device control data
 */
#define	ADC_DIV_INI		0	// Clock divider

/* 
 * Initialize analog input pins
 *	Enable when not performed in the OS initialization process.
 */
#define	DEVCONF_ADC_PIN_INIT_0	FALSE
#define	DEVCONF_ADC_PIN_INIT_1	FALSE
#define	DEVCONF_ADC_PIN_INIT_2	FALSE
#define	DEVCONF_ADC_PIN_INIT_3	FALSE

/* Interrupt t priority */
#define	DEVCNF_ADC_INTPRI	2

/* A/D conversion timeout time */
#define	DEVCNF_ADC_TMOSCAN	1000

#endif		/* __DEV_ADC_CNF_RP2040_H__ */
