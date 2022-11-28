/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
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
 *	sysdef.h
 *
 *	System dependencies definition (RP2040 depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__

#include "../../../machine.h"

/* CPU Core-dependent definition */
#include "../core/armv6m/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM) 64KB*4bank + 4KB*2bank
 */

#define INTERNAL_RAM_START      0x20000000
#define INTERNAL_RAM_SIZE       (64*1024*4)

#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END

/* ------------------------------------------------------------------------ */
/*
 * System Timer clock
 */

/* Settable interval range (millisecond) */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50

/* ------------------------------------------------------------------------ */
/*
 * Number of Interrupt vectors
 */
#define N_INTVEC		32	/* Number of Interrupt vectors */
#define	N_SYSVEC		16	/* Number of System Exceptions */

/*
 * The number of the implemented bit width for priority value fields.
 */
#define INTPRI_BITWIDTH		2


/* ------------------------------------------------------------------------ */
/*
 * Interrupt Priority Levels
 */
#define INTPRI_MAX_EXTINT_PRI	1	/* Highest Ext. interrupt level */
#define INTPRI_SVC		0	/* SVCall */
#define INTPRI_SYSTICK		0	/* SysTick */
#define INTPRI_PENDSV		2	/* PendSV */

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0

/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU		0
#define CPU_HAS_DPS		0

/*
 *  Number of coprocessors to use. Depends on user configuration
 */
#define NUM_COPROCESSOR		0

/* ------------------------------------------------------------------------ */
/*
 * APB Peripherals
 */
/* Clocks */
#define CLOCKS_BASE             0x40008000
#define CLK_GPOUT0              (CLOCKS_BASE+0x00)
#define CLK_GPOUT1              (CLOCKS_BASE+0x0C)
#define CLK_GPOUT2              (CLOCKS_BASE+0x18)
#define CLK_GPOUT3              (CLOCKS_BASE+0x24)
#define CLK_REF	                (CLOCKS_BASE+0x30)
#define CLK_SYS	                (CLOCKS_BASE+0x3C)
#define CLK_PERI                (CLOCKS_BASE+0x48)
#define CLK_USB	                (CLOCKS_BASE+0x54)
#define CLK_ADC	                (CLOCKS_BASE+0x60)
#define CLK_RTC	                (CLOCKS_BASE+0x6C)
#define CLK_SYS_RESUS_CTRL      (CLOCKS_BASE+0x78)
#define CLK_SYS_RESUS_STATUS    (CLOCKS_BASE+0x7C)

#define	CLK_x_CTRL              (0x00)
#define	CLK_x_DIV               (0x04)
#define	CLK_x_SELECTED          (0x08)

#define CLK_SYS_CTRL_SRC        (0x00000001)
#define CLK_SYS_CTRL_AUXSRC     (0x000000e0)
#define CLK_REF_CTRL_SRC        (0x00000003)
#define CLK_CTRL_ENABLE	        (0x00000800)

#define CLK_KIND_GPOUT0         0
#define CLK_KIND_GPOUT1         1
#define CLK_KIND_GPOUT2         2
#define CLK_KIND_GPOUT3         3
#define CLK_KIND_REF            4
#define CLK_KIND_SYS            5
#define CLK_KIND_PERI           6
#define CLK_KIND_USB            7
#define CLK_KIND_ADC            8
#define CLK_KIND_RTC            9

/* Reset Controler */
#define RESETS_BASE             0x4000C000
#define RESETS_RESET            (RESETS_BASE+0x0)
#define RESETS_WDSEL            (RESETS_BASE+0x4)
#define RESETS_RESET_DONE       (RESETS_BASE+0x8)

#define RESETS_RESET_USBCTRL	(0x01000000)
#define RESETS_RESET_UART1	(0x00800000)
#define RESETS_RESET_UART0	(0x00400000)
#define RESETS_RESET_TIMER	(0x00200000)
#define RESETS_RESET_TBMAN	(0x00100000)
#define RESETS_RESET_SYSINFO	(0x00080000)
#define RESETS_RESET_SYSCFG	(0x00040000)
#define RESETS_RESET_SPI1	(0x00020000)
#define RESETS_RESET_SPI0	(0x00010000)
#define RESETS_RESET_RTC	(0x00008000)
#define RESETS_RESET_PWM	(0x00004000)
#define RESETS_RESET_PLLUSB	(0x00002000)
#define RESETS_RESET_PLLSYS	(0x00001000)
#define RESETS_RESET_PIO1	(0x00000800)
#define RESETS_RESET_PIO0	(0x00000400)
#define RESETS_RESET_PADS_QSPI	(0x00000200)
#define RESETS_RESET_PADS_BANK0	(0x00000100)
#define RESETS_RESET_JTAG	(0x00000080)
#define RESETS_RESET_IO_QSPI	(0x00000040)
#define RESETS_RESET_IO_BANK0	(0x00000020)
#define RESETS_RESET_I2C1	(0x00000010)
#define RESETS_RESET_I2C0	(0x00000008)
#define RESETS_RESET_DMA	(0x00000004)
#define RESETS_RESET_BUSCTRL	(0x00000002)
#define RESETS_RESET_ADC	(0x00000001)

/* GPIO */
#define	GPIO_NUM		30

#define IO_BANK0_BASE		0x40014000
#define	GPIO_CTRL(n)		(IO_BANK0_BASE+0x04+(n*8))

#define	GPIO_CTRL_FUNCSEL_XIP	0
#define	GPIO_CTRL_FUNCSEL_SPI	1
#define	GPIO_CTRL_FUNCSEL_UART	2
#define	GPIO_CTRL_FUNCSEL_I2C	3
#define	GPIO_CTRL_FUNCSEL_PWM	4
#define	GPIO_CTRL_FUNCSEL_SIO	5
#define	GPIO_CTRL_FUNCSEL_PIO0	6
#define	GPIO_CTRL_FUNCSEL_PIO1	7
#define	GPIO_CTRL_FUNCSEL_GPCK	8
#define	GPIO_CTRL_FUNCSEL_USB	9
#define	GPIO_CTRL_FUNCSEL_NULL	31

#define	IO_QSPI_BASE		0x40018000

#define PADS_BANK0_BASE		0x4001c000
#define	GPIO(n)			(PADS_BANK0_BASE+0x4+(n*4))

#define	GPIO_OD			(1<<7)
#define	GPIO_IE			(1<<6)
#define	GPIO_DRIVE_2MA		(0<<4)
#define	GPIO_DRIVE_4MA		(1<<4)
#define	GPIO_DRIVE_8MA		(2<<4)
#define	GPIO_DRIVE_12MA		(3<<4)
#define	GPIO_PUE		(1<<3)
#define	GPIO_PDE		(1<<2)
#define	GPIO_SHEMITT		(1<<1)
#define	GPIO_SLEWDAST		(1<<0)

/* Crystal Oscillator(XOSC) */
#define XOSC_BASE               0x40024000
#define XOSC_CTRL               (XOSC_BASE+0x00)
#define XOSC_STATUS             (XOSC_BASE+0x04)
#define XOSC_STARTUP            (XOSC_BASE+0x0C)

#define	XOSC_CTRL_ENABLE        (0x00FAB000)
#define	XOSC_CTRL_DISABLE       (0x00D1E000)
#define	XOSC_CTRL_FRANG_1_15MHZ (0x00000AA0)

#define	XOSC_STATUS_STABLE      (0x80000000)

/* PLL */
#define PLL_SYS_BASE            (0x40028000)
#define PLL_USB_BASE            (0x4002C000)

#define PLL_CS                  (0x00)
#define	PLL_PWR                 (0x04)
#define	PLL_FBDIV_INT           (0x08)
#define	PLL_PRIM                (0x0C)

#define	PLL_CS_LOCK             (1<<31)
#define	PLL_PWR_PD              (1<<0)
#define	PLL_PWR_VCOPD           (1<<5)
#define	PLL_PWR_POSTDIVPD       (1<<3)
#define PLL_PRIM_POSTDIV1_LSB   (16)
#define PLL_PRIM_POSTDIV2_LSB   (12)

/* IOPORT (SIO:Single-cycle IO) */
#define SIO_BASE                0xD0000000

#define	GPIO_IN			(SIO_BASE+0x004)

#define	GPIO_OUT		(SIO_BASE+0x008)
#define GPIO_OUT_SET		(SIO_BASE+0x014)
#define GPIO_OUT_CLR		(SIO_BASE+0x018)
#define GPIO_OUT_XOR		(SIO_BASE+0x01C)

#define GPIO_OE			(SIO_BASE+0x020)
#define GPIO_OE_SET		(SIO_BASE+0x024)
#define GPIO_OE_CLR		(SIO_BASE+0x028)
#define GPIO_OE_XOR		(SIO_BASE+0x02C)

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
