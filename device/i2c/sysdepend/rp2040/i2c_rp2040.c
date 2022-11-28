/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RP2040

#include <tk/tkernel.h>
#include "../../i2c.h"
#include "../../../include/dev_def.h"
#if DEV_IIC_ENABLE
/*
 *	i2c_rp2040.c
  *	I2C device driver
 *	System-dependent definition for RP2040
 */

/*
 **** This program is beta version.
 **** It operates by polling without using interrupts.
 */

/*----------------------------------------------------------------------
 * Device register base address
*/
const LOCAL UW ba[DEV_I2C_UNITNM] = { I2C0_BASE, I2C1_BASE };

/*----------------------------------------------------------------------
 * Device data
 **** This data is not used in beta version
*/
#if 0
const LOCAL struct {
	UINT	intno;		// Interrupt number
	PRI	intpri;		// Interrupt priority
	TMO	timout;		// I2C timeout time
} ll_devdat[DEV_I2C_UNITNM] = {
	{	/* I2C0 */
		.intno		= INTNO_I2C0,
		.intpri		= DEVCNF_I2C0_INTPRI,
		.timout		= DEVCNF_I2C0_TMO,
	},
	{	/* I2C1 */
		.intno		= INTNO_I2C1,
		.intpri		= DEVCNF_I2C1_INTPRI,
		.timout		= DEVCNF_I2C1_TMO,
	},
};
#endif

/*----------------------------------------------------------------------
 * Device control data
*/
typedef struct {
	ID	wait_tskid;	/* Wait Task ID */
//	UW	state;		/* Operating state */
	UW	sadr;		/* Slave address */
	ER	ioerr;		/* Communication error */
	UW	abort_reason;
	UW	sdat_num;	/* Number of send data */
	UW	rdat_num;	/* Number of receive data */
	UB	*sbuf;		/* Send buffer */
	UB	*rbuf;		/* Receive buffer */
} T_I2C_LLDCB;
LOCAL T_I2C_LLDCB	ll_devcb[DEV_I2C_UNITNM];

/*-------------------------------------------------------
 * Interrupt handler
 ******* This handler is not used in beta version
 */
#if 0
LOCAL void i2c_inthdr( UINT intno )
{
	ClearInt(intno);
}
#endif
/*----------------------------------------------------------------------
 * Execution of communication
 */
LOCAL ER i2c_trans(INT unit, T_I2C_LLDCB *p_cb)
{
	UW	cmd, sts;
	UINT	state	= I2C_STS_START;
	UW	abort;
	ER	err	= E_OK;

	p_cb->ioerr		= E_OK;
	p_cb->wait_tskid	= 0;

	out_w(I2C_TAR(unit), p_cb->sadr & I2C_TAR_7BIT_ADR);		// Set slave address
	out_w(I2C_ENABLE(unit), I2C_ENABLE_ENABLE);			// Enable I2C

	if(p_cb->sdat_num) {
		state = I2C_STS_SEND;
	} else if(p_cb->rdat_num) {
		state = I2C_STS_RECV;
	} else {
		err = E_PAR;
		state = I2C_STS_STOP;
	}

	out_w(I2C_CLR_INTR(unit), 0);	// Clear all Interrupt Registers
	in_w(I2C_CLR_TX_ABRT(unit));	// Clear Transmit abort

	while(state != I2C_STS_STOP) {
		switch(state){
		case I2C_STS_SEND:
			cmd = *(p_cb->sbuf)++ & I2C_DATA_CMD_DAT;
			if(--(p_cb->sdat_num)) {
				cmd |= I2C_DATA_CMD_CMD_WRITE;
			} else {	// Last data
				if(p_cb->rdat_num) {
					cmd |= I2C_DATA_CMD_CMD_WRITE | I2C_DATA_CMD_RESTART;
					state = I2C_STS_RECV;
				} else {
					cmd |= I2C_DATA_CMD_CMD_WRITE | I2C_DATA_CMD_STOP;
					state = I2C_STS_STOP;
				}
			}
			out_w(I2C_DATA_CMD(unit), cmd);

			while(1) {
				sts = in_w(I2C_RAW_INTR_STAT(unit));
				if(sts & I2C_RAW_INTR_STAT_TX_EMPTY) break;
			}

			break;
		case I2C_STS_RECV:
			if(--(p_cb->rdat_num)) {
				cmd = I2C_DATA_CMD_CMD_READ;
			} else {	// Last data
				cmd = I2C_DATA_CMD_CMD_READ | I2C_DATA_CMD_STOP;
				state = I2C_STS_STOP;
			}
			out_w(I2C_DATA_CMD(unit), cmd);

			while(1) {
				abort = in_w(I2C_TX_ABRT_SOURCE(unit));
				if(abort != 0) {
					p_cb->abort_reason = abort;
					p_cb->ioerr = err = E_IO;
					state = I2C_STS_STOP;
					break;
				}
				sts = in_w(I2C_RXFLR(unit));
				if(sts != 0) break;
			}
			*(p_cb->rbuf)++ = in_w(I2C_DATA_CMD(unit)) & I2C_DATA_CMD_DAT;
			break;
		default:
			break;
		}
	}
	out_w(I2C_ENABLE(unit), 0);				// Disable I2C

	return err;
}

/*----------------------------------------------------------------------
 * Set communication parameters
 */
Inline void set_com_start(UW unit, UW sadr, UW sdat_num, UW rdat_num, UB *sbuf, UB *rbuf)
{
//	ll_devcb[unit].state	= I2C_STS_START;
	ll_devcb[unit].sadr	= sadr;		/* Slave address */
	ll_devcb[unit].sdat_num	= sdat_num;	/* Number of send data */
	ll_devcb[unit].rdat_num	= rdat_num;	/* Number of receive data */
	ll_devcb[unit].sbuf	= sbuf;		/* Send buffer */
	ll_devcb[unit].rbuf	= rbuf;		/* Receive buffer */
}


/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT W dev_i2c_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	T_I2C_EXEC	*p_ex;
	ER		err	= E_OK;

	switch(cmd) {
	case LLD_I2C_OPEN:
		/* Initial Configuration */
		out_w(I2C_ENABLE(unit), 0);						// Disable I2C

		out_w(I2C_CON(unit), I2C_CON_SLAVE_DISABLE | I2C_CON_MASTER_MODE	// Master Mode
			| I2C_CON_RESTART_EN | I2C_CON_TX_EMPTY_CTRL
			| I2C_CON_7BIT_ADR | I2C_CON_SPEED_FAST);			// 7bit Address & Fast Speed

		out_w(I2C_RX_TL(unit), 0);
		out_w(I2C_TX_TL(unit), 0);
		out_w(I2C_DMA_CR(unit), I2C_DMA_CR_RDMAE | I2C_DMA_CR_TDMAE);

		/* Set Baudrate (400Kbts)*/
		out_w(I2C_FS_SCL_HCNT(unit), 126);
		out_w(I2C_FS_SCL_LCNT(unit), 187);
		out_w(I2C_FS_SPKLEN(unit), 187/16);
		out_w(I2C_SDA_HOLD(unit), 38);

		/* I2C interrupt enable */
//		EnableInt(ll_devdat[unit].intno, ll_devdat[unit].intpri);
		break;

	case LLD_I2C_CLOSE:
		/* I2C interrupt disable */
//		DisableInt(ll_devdat[unit].intno);
		break;

	case LLD_I2C_READ:
		set_com_start( unit, p1, 0, p2, NULL, (UB*)pp);
		err = i2c_trans(unit, &ll_devcb[unit]);
		if(err >= E_OK) err = p1 - ll_devcb[unit].sdat_num;
		break;

	case LLD_I2C_WRITE:
		set_com_start( unit, p1, p2, 0, (UB*)pp, NULL);
		err = i2c_trans(unit, &ll_devcb[unit]);
		if(err >= E_OK) err = p1 - ll_devcb[unit].sdat_num;
		break;

	case LLD_I2C_EXEC:
		p_ex = (T_I2C_EXEC*)pp;
		set_com_start( unit, p_ex->sadr, p_ex->snd_size, p_ex->rcv_size, p_ex->snd_data, p_ex->rcv_data);
		err = i2c_trans(unit, &ll_devcb[unit]);
		if(err >= E_OK) err = p_ex->snd_size + p_ex->rcv_size;
		break;
	}
	
	return (W)err;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_i2c_llinit( T_I2C_DCB *p_dcb)
{
//	T_DINT	dint;
	UW	unit;
	ER	err	= E_OK;

	unit = p_dcb->unit;

	switch(unit) {
	case DEV_I2C_0:

#if DEVCNF_I2C_RESET
		/* Reset I2C module*/
		set_w( RESETS_RESET, RESETS_RESET_I2C0);
		clr_w( RESETS_RESET, RESETS_RESET_I2C0);
		while((in_w(RESETS_RESET_DONE)&(RESETS_RESET_I2C0))==0);
#endif

#if DEVCNF_I2C_SETPINFUNC
		out_w(GPIO_CTRL(8), GPIO_CTRL_FUNCSEL_I2C);
		out_w(GPIO(8), GPIO_IE | GPIO_DRIVE_4MA | GPIO_PUE | GPIO_SHEMITT);
		out_w(GPIO_CTRL(9), GPIO_CTRL_FUNCSEL_I2C);
		out_w(GPIO(9), GPIO_IE | GPIO_DRIVE_4MA | GPIO_PUE | GPIO_SHEMITT);
#endif
		break;
	case DEV_I2C_1:
#if DEVCNF_I2C_RESET
		/* Reset I2C module*/
		set_w( RESETS_RESET, RESETS_RESET_I2C1);
		clr_w( RESETS_RESET, RESETS_RESET_I2C1);
		while((in_w(RESETS_RESET_DONE)&(RESETS_RESET_I2C1))==0);
#endif

#if DEVCNF_I2C_SETPINFUNC
		out_w(GPIO_CTRL(6), GPIO_CTRL_FUNCSEL_I2C);
		out_w(GPIO(6), GPIO_IE | GPIO_DRIVE_4MA | GPIO_PUE | GPIO_SHEMITT);
		out_w(GPIO_CTRL(7), GPIO_CTRL_FUNCSEL_I2C);
		out_w(GPIO(7), GPIO_IE | GPIO_DRIVE_4MA | GPIO_PUE | GPIO_SHEMITT);
		break;
#endif
	default:
		return E_PAR;
	}

	out_w(I2C_ENABLE(unit), 0);			// Disable I2C

	/* Interrupt handler definition */
//	dint.intatr	= TA_HLNG;
//	dint.inthdr	= i2c_inthdr;
	
//	err = tk_def_int(ll_devdat[unit].intno, &dint);

	return err;
}

#endif		/* DEV_IIC_ENABLE */
#endif		/* CPU_RP2040 */