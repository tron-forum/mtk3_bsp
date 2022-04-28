/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022.04.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef RSK_RX65N

/*
 *	hw_setting.c (Renesas Starter Kit+ for RX65N)
 *	startup / shoutdown processing for hardware
 *	
 *	Pin function Setting (for IoT-Engine Starter board)
 *		PJ1  : SCI8 RXD8
 *		PJ2  : SCI8 TXD8
 *
 *		(USE_SDEV_DRV)
 *		P12  : RIIC0 SCL0
 *		P13  : RIIC0 SDA0
 *		P40  : AN000
 */

#include "kernel.h"
#include <tm/tmonitor.h>

#include "sysdepend.h"

/* ------------------------------------------------------------------------ */
/* 
 * Setup register data 
 */
typedef struct {
	UW	addr;
	UW	data;
} T_SETUP_REG;

/* 
 * Setup module stop Tadle
 */
LOCAL const T_SETUP_REG mstop_tbl[] = {
#if !USE_SDEV_DRV	// Do not use sample device driver
	{ MSTPCRA, 0x46FF7FCF },	/* Enable EXDMAC, DMAC/DTC, CMT0-1, TMR0-3 */
	{ MSTPCRB, 0xFFFFFFFF },
	{ MSTPCRC, 0xF7FF0000 },	/* Enable SCI8, Disable Deep-Sleep mode, Enable RAM */
	{ MSTPCRD, 0xFFFFFF00 },

#else			// Use the sample device driver
	{ MSTPCRA, 0x46F57FCF },	/* Enable EXDMAC, DMAC/DTC, S12AD0, CMT0-1, TMR0-3 */
	{ MSTPCRB, 0xFFDFFFFF },	/* Enable RIIC0 */
	{ MSTPCRC, 0xF7FF0000 },	/* Enable SCI8, Disable Deep-Sleep mode, Enable RAM */
	{ MSTPCRD, 0xFFFFFF00 },

#endif /* !USE_SDEV_DRV */
	{0, 0}
};

/* 
 * Setup pin functions Tadle
 */
LOCAL const T_SETUP_REG pinfnc_tbl[] = {
	{MPC_PJnPFS(1), 0x0A},		/* PJ1 = SCI8 RXD8 */
	{MPC_PJnPFS(2), 0x0A},		/* PJ2 = SCI8 TXD8 */

#if USE_SDEV_DRV	// Use the sample device driver
	{MPC_P1nPFS(2), 0x0F},		/* P12 = RIIC0 SCL0 */
	{MPC_P1nPFS(3), 0x0F},		/* P13 = RIIC0 SDA0 */
	{MPC_P4nPFS(0), 0x80},		/* P40 = AN000 */

#endif /* USE_SDEV_DRV */
	{0, 0}
};

/* 
 * Setup port mode Tadle
 */
LOCAL const T_SETUP_REG portmode_tbl[] = {
	/* PJ1:SCI8 RXD8 PJ2:SCI8 TXD8 */
	{PORTJ_PMR, 0x06},		/* PJ1&PJ2 peripheral function. */
	{PORTJ_PODR, 0x04},		/* PJ2 = 'H' */
	{PORTJ_PDR, 0x04},		/* PJ2 output port */

#if USE_SDEV_DRV	// Use the sample device driver
	/*  P12:RIIC0_SCL0 P13:RIIC0_SDA0 */
	{PORT1_PMR, 0x0C},		/* P12&P13  peripheral function. */
	{PORT1_PCR, 0x0C},		/* P12&P13 Pull-up */
	/* P40:AN000 */
	{PORT4_PMR, 0x00},		/* P40  AN000 */
	{PORT4_PDR, 0x00},

#endif /* USE_SDEV_DRV */

	{0, 0}
};

/*
 * Startup hardware
 */
EXPORT void knl_startup_hw(void)
{
	const T_SETUP_REG	*p;

	/* Setup module stop */
	out_h(SYSTEM_PRCR, 0xA502);		/* Disable Register Protect */
	for(p = mstop_tbl; p->addr != 0; p++) {
		out_w(p->addr, p->data);
	}
	out_h(SYSTEM_PRCR, 0xA500);		/* Enable Register protect */

	/* Setup Pin Function */
	out_b(MPC_PWPR, 0);
	out_b(MPC_PWPR, MPC_PWMR_PFSWE);	/* Disable Register Protect */
	for(p = pinfnc_tbl; p->addr != 0; p++) {
		out_b(p->addr, (UB)p->data);
	}
	out_b(MPC_PWPR, MPC_PWMR_B0WI);		/* Enable Register protect */

	/* Setup port mode */
	for(p = portmode_tbl; p->addr != 0; p++) {
		out_b(p->addr, (UB)p->data);
	}	

	startup_clock();			/* Startup CPU Clock */
}

#if USE_SHUTDOWN
/*
 * Shutdown hardware
 */
EXPORT void knl_shutdown_hw( void )
{
	disint();
	shutdown_clock();

	while(1);
}
#endif /* USE_SHUTDOWN */

/*
 * Re-start hardware
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


#endif /* RSK_RX65N */