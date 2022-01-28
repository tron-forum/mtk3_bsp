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

/*
 *      tkernel_map.ld
 *	micro T-Kernel linker script for Nucleo STM32H723
 */

ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
	ITCMRAM (xrw)	: ORIGIN = 0x00000000,   LENGTH = 64K
	DTCMRAM (xrw)	: ORIGIN = 0x20000000,   LENGTH = 128K
	ROM      (rx)	: ORIGIN = 0x08000000,   LENGTH = 1024K
	RAM_D1  (xrw)	: ORIGIN = 0x24000000,   LENGTH = 320K
	RAM_D2  (xrw)	: ORIGIN = 0x30000000,   LENGTH = 32K
	RAM_D3  (xrw)	: ORIGIN = 0x38000000,   LENGTH = 16K
}

SECTIONS {
	.text : {
		. = ALIGN(8);
		__vector_org = .;
		*(.vector)
		__start = .;
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata.*)
	} >ROM
	. = ALIGN(4);
	__data_org = .;

	vector 0x24000000 (NOLOAD) : {
		__vector_start = .;
		*(.data_vector)
		__vector_end = .;
	} >RAM_D1
	
	.data : AT(__data_org) {
		__data_start = .;
		*(.data)
		*(.data.*)
		. = ALIGN(4);
		__data_end = .;
	} >RAM_D1
	
	.bss ALIGN(4) (NOLOAD) : {
		__bss_start = .;
		PROVIDE (__noinit_start = .);
		*(.noinit)
		PROVIDE (__noinit_end = .);
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
		__end = .;
	} >RAM_D1
}

