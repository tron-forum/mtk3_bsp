/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/04.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef RTB_RX65N

/*
 *	devinit.c (Renesas Target Board for RX65N)
 *	Device-Dependent Initialization
 */

#include <sys/sysdef.h>
#include <tm/tmonitor.h>
#include <tk/device.h>

#include "kernel.h"
#include "sysdepend.h"

/* ------------------------------------------------------------------------ */

/*
 * Initialization before micro T-Kernel starts
 */

EXPORT ER knl_init_device( void )
{
	return E_OK;
}

/* ------------------------------------------------------------------------ */
/*
 * Start processing after T-Kernel starts
 *	Called from the initial task contexts.
 */
EXPORT ER knl_start_device( void )
{
#if USE_GROUP_INT	// Use group interrupt
	EnableInt(INTNO_GROUPBE0, INTPRI_GROUPBE0);
	EnableInt(INTNO_GROUPBL0, INTPRI_GROUPBL0);
	EnableInt(INTNO_GROUPBL1, INTPRI_GROUPBL1);
	EnableInt(INTNO_GROUPBL2, INTPRI_GROUPBL2);
	EnableInt(INTNO_GROUPAL0, INTPRI_GROUPAL0);
	EnableInt(INTNO_GROUPAL1, INTPRI_GROUPAL1);
#endif /* USE_GROUP_INT */

#if USE_SDEV_DRV	// Use sample driver
	ER	err;

	/* A/D Converter "adca" */
	#if DEVCNF_USE_ADC
		err = dev_init_adc(0);
		if(err < E_OK) return err;
	#endif

	/* RIIC0 "iica" */
	#if DEVCNF_USE_IIC
		err = dev_init_i2c(0);
		if(err < E_OK) return err;
	#endif

#endif	/* USE_SDEV_DRV */

	return E_OK;
}

#if USE_SHUTDOWN
/* ------------------------------------------------------------------------ */
/*
 * System finalization
 *	Called just before system shutdown.
 *	Execute finalization that must be done before system shutdown.
 */
EXPORT ER knl_finish_device( void )
{
	return E_OK;
}

#endif /* USE_SHUTDOWN */

#endif /* RTB_RX65N */
