/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/11.
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
#define GPIO_MODE_OUT	1
#define GPIO_MODE_IN	0

ER	gpio_set_pin(UINT no, UINT mode);
ER	gpio_set_val(UINT no, UINT val);
UINT	gpio_get_val(UINT no);

/* PWM Control */
ER	pwm_set_pin(UINT no);
ER	pwm_set_wrap(UINT no, UW wrap);
ER	pwm_set_level(UINT no, UW level);
ER	pwm_set_enabled(UINT no, BOOL enable);

#endif	/* __BSP_LIBBSP_DEPEND_CPU_H__ */