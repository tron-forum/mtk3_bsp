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

#ifndef __BSP_LIBBSP_H__
#define __BSP_LIBBSP_H__

/* System dependencies */
#define LIBBSP_PATH_(a)		#a
#define LIBBSP_PATH(a)		LIBBSP_PATH_(a)
#define LIBBSP_SYSDEP()		LIBBSP_PATH(sysdepend/TARGET_DIR/libbsp.h)
#include LIBBSP_SYSDEP()

#endif /* __BSP_LIBBSP_H__ */
