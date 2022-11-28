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
 *	libbsp_pwm.c
 *	PWM control (RP2040)
 */

#include <sys/machine.h>
#ifdef CPU_RP2040

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <bsp/libbsp.h>

/*
 * GPIO Control
 */
ER gpio_set_pin(UINT no, UINT mode)
{
	if(no >= GPIO_NUM) return E_PAR;

	switch(mode) {
	case GPIO_MODE_OUT:
		out_w(GPIO_OE_SET, 1<<no);		// Enable output
		clr_w(GPIO(no), GPIO_IE);		// Disable input
		clr_w(GPIO(no), GPIO_OD);		// Enable output
		break;
	case GPIO_MODE_IN:
		out_w(GPIO_OE_CLR, 1<<no);		// Disable output
		set_w(GPIO(no), GPIO_OD);		// Disable output
		set_w(GPIO(no), GPIO_IE);		// Enable input
		break;
	default:
		return E_PAR;
	}
	out_w(GPIO_CTRL(no), GPIO_CTRL_FUNCSEL_SIO);

	return E_OK;
}

ER gpio_set_val(UINT no, UINT val)
{
	if(no >= GPIO_NUM) return E_PAR;

	if(val > 0) {
		out_w(GPIO_OUT_SET, 1<<no);
	} else {
		out_w(GPIO_OUT_CLR, 1<<no);
	}
	return E_OK;
}

UINT gpio_get_val(UINT no)
{
	if(no >= GPIO_NUM) return E_PAR;

	return (in_w(GPIO_IN) & 1<<no)?1:0;
}

/*
 * PWM(GPIO) Control
 */
#define	PWM_BASE	0x40050000

#define	PWM_CHn_CSR(n)	(PWM_BASE + 0x00 + (n*0x14))
#define	PWM_CHn_DIV(n)	(PWM_BASE + 0x04 + (n*0x14))
#define	PWM_CHn_CTR(n)	(PWM_BASE + 0x08 + (n*0x14))
#define	PWM_CHn_CC(n)	(PWM_BASE + 0x0C + (n*0x14))
#define	PWM_CHn_TOP(n)	(PWM_BASE + 0x10 + (n*0x14))

#define	PWM_CH_CSR_EN		(1<<0)
#define	PWM_GET_CHNO(no)	((no>>1)& 0x07)

ER pwm_set_pin(UINT no)
{
	if(no >= GPIO_NUM) return E_PAR;

	clr_w(GPIO(no), GPIO_OD);			// Enable output
	set_w(GPIO(no), GPIO_IE);			// Enable input
	out_w(GPIO_CTRL(no), GPIO_CTRL_FUNCSEL_PWM);	// Set function

	return E_OK;
}

ER pwm_set_wrap(UINT no, UW wrap)
{
	if(no >= GPIO_NUM) return E_PAR;

	out_w(PWM_CHn_TOP(PWM_GET_CHNO(no)), wrap);
	return E_OK;
}

ER pwm_set_level(UINT no, UW level)
{
	UINT	ch;
	UW	cc;

	if(no >= GPIO_NUM) return E_PAR;

	ch = PWM_GET_CHNO(no);
	cc = in_w(PWM_CHn_CC(ch));
	if(no & 0x01) {	/* Chan B */
		out_w(PWM_CHn_CC(ch), (cc & 0x0000FFFF)| (level<<16));
	} else {	/* Chan A */
		out_w(PWM_CHn_CC(ch), (cc & 0xFFFF0000)| level);
	}
	return E_OK;
}

ER pwm_set_enabled(UINT no, BOOL enable)
{
	if(no >= GPIO_NUM) return E_PAR;

	if(enable) {
		set_w(PWM_CHn_CSR(PWM_GET_CHNO(no)), PWM_CH_CSR_EN);
	} else {
		clr_w(PWM_CHn_CSR(PWM_GET_CHNO(no)), PWM_CH_CSR_EN);
	}
	return	E_OK;
}

#endif /* CPU_RP2040 */