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

#include <tk/tkernel.h>
#include <kernel.h>

#ifdef RSK_RX65N

/*
 *	power_save.c (Renesas Starter Kit+ for RX65N)
 *	Power-Saving Function
 */

#include "sysdepend.h"

/*
 * Switch to power-saving mode
 */
EXPORT void low_pow( void )
{
}

/*
 * Move to suspend mode
 */
EXPORT void off_pow( void )
{
}


#endif /* RSK_RX65N */
