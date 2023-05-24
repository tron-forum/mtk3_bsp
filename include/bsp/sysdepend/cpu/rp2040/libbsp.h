/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
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
 *	libbsp.h
 *	System dependencies definition (RP2040 depended)
 */

#ifndef __BSP_LIBBSP_DEPEND_CPU_H__
#define __BSP_LIBBSP_DEPEND_CPU_H__

/* GPIO Contrl */
#define GPIO_MODE_OUT	1			// GPIO output mode
#define GPIO_MODE_IN	0			// GPIO input mode

ER	gpio_set_pin(UINT no, UINT mode);	// Set GPIO pin
ER	gpio_set_val(UINT no, UINT val);	// Set output value
UINT	gpio_get_val(UINT no);			// Get Input value

/* PWM Control */
ER	pwm_set_pin(UINT no);			// Set PWM output pin
ER	pwm_set_wrap(UINT no, UW wrap);		// Set counter wrap value
ER	pwm_set_cc(UINT no, UW cc);		// Set counter compare value
ER	pwm_set_enabled(UINT no, BOOL enable);	// Enable/Disable chanel

#endif	/* __BSP_LIBBSP_DEPEND_CPU_H__ */