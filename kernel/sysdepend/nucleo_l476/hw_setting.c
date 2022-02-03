/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef NUCLEO_L476

/*
 *	hw_setting.c (Nucleo-64 STM32L476)
 *	startup / shoutdown processing for hardware
 *	
 *	Pin function Setting
 *		PA2  : USART2 TX
 *		PA3  : USART2 RX
 *		PA5  : GPIO Out(LED)
 *		PC13 : GPIO In(Push Button)
 *
 *		(USE_SDEV_DRV)	
 *		PA0  : A/DC12 IN5
 *		PA1  : A/DC12 IN6
 *		PA4  : A/DC12 IN9
 *		PB0  : A/DC12 IN15
 *		PB8  : I2C1 DCL
 *		PB9  : I2C1 SDA
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
	{RCC_AHB2ENR,		0x00000005},	// GPIOA,GPIOC enable
	{RCC_APB1ENR1,		0x0002000F},	// USART2, TIM2-TIM5 enable
	{RCC_APB2ENR,		0x00000001},	// SYSCFG enable

#else			// Use the sample device driver
	{RCC_AHB2ENR,		0x00000007},	// GPIOA, B, C enable
	{RCC_APB1ENR1,		0x0002000F},	// USART2, TIM2-TIM5 enable
	{RCC_APB2ENR,		0x00000001},	// SYSCFG enable
#endif /* !USE_SDEV_DRV */

	{0, 0}
};

/* 
 * Setup pin functions Tadle
 */
LOCAL const T_SETUP_REG pinfnc_tbl[] = {

#if !USE_SDEV_DRV	// Do not use device sample driver

	// Serial debug I/F : PA2 -> USART2_TX, PA3 -> USART2_RX
	// GPIOA out : PA5 -> LED
	{GPIO_MODER(A),		0xABFFF7AF},	// GPIOA mode
	{GPIO_OTYPER(A),	0x00000000},	// GPIOA output type
	{GPIO_OSPEEDR(A),	0x0C000000},	// GPIOA output speed
	{GPIO_PUPDR(A),		0x64000000},	// GPIOA Pull-up/down
	{GPIO_AFRL(A),		0x00007700},

	// GPIOC in   PC13 -> Push button
	{GPIO_MODER(C),		0xF3FFFFFF},	// GPIOC mode
	{GPIO_OTYPER(C),	0x00000000},	// GPIOC output type
	{GPIO_OSPEEDR(C),	0x00000000},	// GPIOC output speed
	{GPIO_PUPDR(C),		0x00000000},	// GPIOC Pull-up/down

#else			// Use the device sample driver

	// Serial debug I/F : PA2 -> USART2_TX, PA3 -> USART2_RX
	// GPIOA out : PA5
	// A/DC in : PA0, PA1, PA4
	{GPIO_MODER(A),		0xABFFF7AF},	// GPIOA mode
	{GPIO_OTYPER(A),	0x00000000},	// GPIOA output type
	{GPIO_OSPEEDR(A),	0x0C000050},	// GPIOA output speed
	{GPIO_PUPDR(A),		0x64000050},	// GPIOA Pull-up/down
	{GPIO_AFRL(A),		0x00007700},	// GPIOA Alternate function
	{GPIO_ASCR(A),		0x00000013},	// GPIOA Analog switch control

	// I2C I/F : PB8 -> I2C1_DCL, PB9 -> I2C1_SDA
	// A/DC in : PB0
	{GPIO_MODER(B),		0xFFFAFFBF},	// GPIOB mode
	{GPIO_OTYPER(B),	0x00000300},	// GPIOB output type
	{GPIO_OSPEEDR(B),	0x000F0000},	// GPIOB output speed
	{GPIO_PUPDR(B),		0x00050100},	// GPIOB Pull-up/down
	{GPIO_AFRL(B),		0x00000000},	// GPIOB Alternate function
	{GPIO_AFRH(B),		0x00000044},	// GPIOB Alternate function
	{GPIO_ASCR(B),		0x00000001},	// GPIOB Analog switch control

	// GPIOC in   PC13 -> Push button
	{GPIO_MODER(C),		0xF3FFFFFF},	// GPIOC mode
	{GPIO_OSPEEDR(C),	0x00000000},	// GPIOC output speed
	{GPIO_OTYPER(C),	0x00000000},	// GPIOC output type
	{GPIO_PUPDR(C),		0x00000000},	// GPIOC Pull-up/down

#endif /* USE_SDEV_DRV */

	{0, 0}
};

/*
 * Startup Device
 */
EXPORT void knl_startup_hw(void)
{
	const T_SETUP_REG	*p;

	/* Startup clock : Use HSI clock, Use PLL, Flash latency 4 */
	startup_clock(CLKATR_HSI | CLKATR_USE_PLL | CLKATR_USE_PLLSAI1 |CLKATR_USE_PLLSAI2 | CLKATR_LATENCY_4);

	/* Startup module clock */
	for(p = modclk_tbl; p->addr != 0; p++) {
		*(_UW*)(p->addr) = p->data;
		while(*(_UW*)(p->addr) != p->data);	/* Delay after an RCC peripheral clock enabling */
	}

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


#endif /* NUCLEO_L476 */