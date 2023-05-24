/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef NUCLEO_H723

/*
 *	hw_setting.c (Nucleo-144 STM32H723)
 *	startup / shoutdown processing for hardware
 *		PB0  : GPIO Out(LED Green)
 *		PB14 : GPIO Out(LED Red)
 *		PE1  : GPIO Out(LED Yellow)
 *		PC13 : GPIO In(Push Button)
 *		PD8  : USART3 TX
 *		PD9  : USART3 RX
 */

#include <kernel.h>
#include <tm/tmonitor.h>

#include "sysdepend.h"

/* 
 * Setup register data 
 */
typedef struct {
	UW	addr;
	UW	data;
} T_SETUP_REG;

/*
 * Setup module clock
 */
LOCAL const T_SETUP_REG modclk_tbl[] = {

#if !USE_SDEV_DRV	// Do not use sample device driver
	{RCC_AHB4ENR,		0x0000001E},	// GPIOB,C,D,E enable
	{RCC_APB1LENR,		0x0004000F},	// USART3, TIM2-TIM5 enable
	{RCC_APB4ENR,		0x00000002},	// SYSCFG enable

#else			// Use the sample device driver
	{RCC_AHB4ENR,		0x0000001F},	// GPIOA,B,C,D,E enable
	{RCC_APB1LENR,		0x0004000F},	// USART3, TIM2-TIM5 enable
	{RCC_APB4ENR,		0x00000002},	// SYSCFG enable

#endif /* !USE_SDEV_DRV */

	{0, 0}
};

/* 
 * Setup pin functions Tadle
 */
LOCAL const T_SETUP_REG pinfnc_tbl[] = {

#if !USE_SDEV_DRV	// Do not use device sample driver
	/* GPIOB Setting   PB0,PB14:LED PB9,PB8:I2C */
	{GPIO_MODER(B),		0xDFFAFEBD},	// GPIOB mode
	{GPIO_OTYPER(B),	0x00000000},	// GPIOB output type
	{GPIO_OSPEEDR(B),	0x000000C0},	// GPIOB output speed
	{GPIO_PUPDR(B),		0x00000100},	// GPIOB Pull-up/down
	{GPIO_AFRH(B),		0x00000044},	// GPIOB alternate function high

	/* GPIOC Setting   PC13:Button */
	{GPIO_MODER(C),		0xF3FFFFFF},	// GPIOC mode
	{GPIO_OTYPER(C),	0x00000000},	// GPIOC output type
	{GPIO_OSPEEDR(C),	0x00000000},	// GPIOC output speed
	{GPIO_PUPDR(C),		0x00000000},	// GPIOC Pull-up/down

	/* GPIOD Setting   PD8:USART3 TX  PD9:USART3 RX */
	{GPIO_MODER(D),		0xFFFAFFFF},	// GPIOD mode
	{GPIO_OTYPER(D),	0x00000000},	// GPIOD output type
	{GPIO_OSPEEDR(D),	0x00000000},	// GPIOD output speed
	{GPIO_PUPDR(D),		0x00000000},	// GPIOD Pull-up/down
	{GPIO_AFRH(D),		0x00000077},	// GPIO alternate function

	/* GPIOE Setting   PE1:LED */
	{GPIO_MODER(E),		0xFFFFFFF7},	// GPIOE mode
	{GPIO_OTYPER(E),	0x00000000},	// GPIOE output type
	{GPIO_OSPEEDR(E),	0x00000000},	// GPIOE output speed
	{GPIO_PUPDR(E),		0x00000000},	// GPIOE Pull-up/down

#else			// Use the device sample driver

	/* GPIOA Setting   PA3: Arduino A0 */
	{GPIO_MODER(A),		0xABFFFFFF},	// GPIOA mode
	{GPIO_OTYPER(A),	0x00000000},	// GPIOA output type
	{GPIO_OSPEEDR(A),	0x0C000000},	// GPIOA output speed
	{GPIO_PUPDR(A),		0x64000000},	// GPIOA Pull-up/down

	/* GPIOB Setting   PB0,PB14:LED  PB9:I2C1_SDA  PB8:I2C1_SCL */
	{GPIO_MODER(B),		0xDFFAFEBD},	// GPIOB mode
	{GPIO_OTYPER(B),	0x00000300},	// GPIOB output type
	{GPIO_OSPEEDR(B),	0x000000C0},	// GPIOB output speed
	{GPIO_PUPDR(B),		0x00000100},	// GPIOB Pull-up/down
	{GPIO_AFRH(B),		0x00000044},	// GPIO alternate function

	/* GPIOC Setting   PC13:Button */
	{GPIO_MODER(C),		0xF3FFFFFF},	// GPIOC mode
	{GPIO_OTYPER(C),	0x00000000},	// GPIOC output type
	{GPIO_OSPEEDR(C),	0x00000000},	// GPIOC output speed
	{GPIO_PUPDR(C),		0x00000000},	// GPIOC Pull-up/down

	/* GPIOD Setting   PD8: USART3 TX  PD9  : USART3 RX */
	{GPIO_MODER(D),		0xFFFAFFFF},	// GPIOD mode
	{GPIO_OTYPER(D),	0x00000000},	// GPIOD output type
	{GPIO_OSPEEDR(D),	0x00000000},	// GPIOD output speed
	{GPIO_PUPDR(D),		0x00000000},	// GPIOD Pull-up/down
	{GPIO_AFRH(D),		0x00000077},	// GPIO alternate function

	/* GPIOE Setting   PE1:LED */
	{GPIO_MODER(E),		0xFFFFFFF7},	// GPIOE mode
	{GPIO_OTYPER(E),	0x00000000},	// GPIOE output type
	{GPIO_OSPEEDR(E),	0x00000000},	// GPIOE output speed
	{GPIO_PUPDR(E),		0x00000000},	// GPIOE Pull-up/down

	/* SYSCFG Setting   PC3:Analog switch open */
	{SYSCFG_PMCR,		0x08000000},	// SYSCFG Peripheral mode

#endif /* USE_SDEV_DRV */

	{0, 0}
};

/*
 * Startup Device
 */
EXPORT void knl_startup_hw(void)
{
	const T_SETUP_REG	*p;

	/* Startup module clock */
	for(p = modclk_tbl; p->addr != 0; p++) {
		*(_UW*)(p->addr) = p->data;
		while(*(_UW*)(p->addr) != p->data);	/* Delay after an RCC peripheral clock enabling */
	}

	/* Startup clock */
	startup_clock(CLKATR_HSE | CLKATR_HSEBYP | CLKATR_PLL1);

	/* Setup Pin Function */
	for(p = pinfnc_tbl; p->addr != 0; p++) {
		*(_UW*)(p->addr) = p->data;
	}
}

#if USE_SHUTDOWN
/*
 * Shutdown device
 */
EXPORT void knl_shutdown_hw( void )
{
	disint();
	while(1);
}
#endif /* USE_SHUTDOWN */

/*
 * Re-start device
 *	mode = -1		reset and re-start	(Reset -> Boot -> Start)
 *	mode = -2		fast re-start		(Start)
 *	mode = -3		Normal re-start		(Boot -> Start)
 */
EXPORT ER knl_restart_hw( W mode )
{
	switch(mode) {
	case -1: /* Reset and re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESET & RESTART >>\n");
		return E_NOSPT;
	case -2: /* fast re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM FAST RESTART >>\n");
		return E_NOSPT;
	case -3: /* Normal re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESTART >>\n");
		return E_NOSPT;
	default:
		return E_PAR;
	}
}


#endif /* NUCLEO_H723 */