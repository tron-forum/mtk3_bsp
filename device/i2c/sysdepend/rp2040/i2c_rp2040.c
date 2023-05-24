/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/03.
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

/*----------------------------------------------------------------------
 * Device register base address
*/
const LOCAL UW ba[DEV_I2C_UNITNM] = { I2C0_BASE, I2C1_BASE };

/*----------------------------------------------------------------------
 * Device data
 **** This data is not used in beta version
*/
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

/*----------------------------------------------------------------------
 * Device control data
*/
typedef struct {
	ID	wait_tskid;	// Wait Task ID
	UW	state;		// Operating state
	UW	sadr;		// Slave address
	ER	ioerr;		// Communication error
	UW	err_reason;	// Error reason
	UW	sdat_num;	// Number of send data
	UW	rdat_num;	// Number of receive data
	UB	*sbuf;		// Send buffer
	UB	*rbuf;		// Receive buffer
} T_I2C_LLDCB;
LOCAL T_I2C_LLDCB	ll_devcb[DEV_I2C_UNITNM];

#define I2C_INT_ERROR	(I2C_INT_TX_ABRT|I2C_INT_TX_OVER|I2C_INT_RX_OVER|I2C_INT_RX_UNDER)

/*-------------------------------------------------------
 * Interrupt handler
 */
LOCAL void i2c_inthdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	UW		i2c_sts, cmd;
	INT		unit;
	BOOL		abort	= FALSE;
	BOOL		wup	= FALSE;

	for ( unit = 0; unit < DEV_I2C_UNITNM; unit++ ) {
		if ( ll_devdat[unit].intno == intno ) {
			p_cb = &ll_devcb[unit];
			break;
		}
	}
	if(unit >= DEV_I2C_UNITNM) {
		ClearInt(intno);	// Clear interrupt
		return;
	}

	i2c_sts = in_w(I2C_INTR_STAT(unit));
	switch ( p_cb->state ) {
	case I2C_STS_SEND:
		if(i2c_sts & I2C_INT_TX_EMPTY) {
			cmd = *(p_cb->sbuf)++ & I2C_DATA_CMD_DAT;
			if(--(p_cb->sdat_num)) {
				cmd |= I2C_DATA_CMD_CMD_WRITE;
			} else {	// Last data
				clr_w(I2C_INTR_MASK(unit), I2C_INT_TX_EMPTY);
				if(p_cb->rdat_num) {
					cmd |= I2C_DATA_CMD_CMD_WRITE | I2C_DATA_CMD_RESTART;
					p_cb->state = I2C_STS_START;
				} else {
					cmd |= I2C_DATA_CMD_CMD_WRITE | I2C_DATA_CMD_STOP;
					p_cb->state = I2C_STS_STOP;
				}
				wup = TRUE;
			}
			out_w(I2C_DATA_CMD(unit), cmd);			
		} else {	// Error
			abort = TRUE;	// Error
		}
		break;
	case I2C_STS_RECV:
		if(i2c_sts & I2C_INT_RX_FULL) {
			*(p_cb->rbuf)++ = in_w(I2C_DATA_CMD(unit)) & I2C_DATA_CMD_DAT;
			if(--(p_cb->rdat_num)) {
				cmd = I2C_DATA_CMD_CMD_READ;
				out_w(I2C_DATA_CMD(unit), cmd);
			} else {	// Last data
				clr_w(I2C_INTR_MASK(unit), I2C_INT_RX_FULL);
				p_cb->state = I2C_STS_STOP;
				wup = TRUE;
			}
		} else {
			abort = TRUE;	// Error
		}
		break;
	default:
		abort = TRUE;	// Error
	}

	if(abort) {
		out_w(I2C_INTR_MASK(unit), 0);	// Mask all innterrupts
		p_cb->ioerr = E_IO;
		p_cb->err_reason = i2c_sts;
		p_cb->state = I2C_STS_STOP;
		wup = TRUE;
	}

	out_w(I2C_CLR_INTR(unit), 0);	// Clear all Interrupt Registers
	ClearInt(intno);		// Clear interrupt

	if(wup) {
		if(p_cb->wait_tskid) {
			tk_wup_tsk(p_cb->wait_tskid);
			p_cb->wait_tskid = 0;
		}
	}
}
/*----------------------------------------------------------------------
 * Execution of communication
 */
LOCAL ER i2c_trans(INT unit, T_I2C_LLDCB *p_cb)
{
	UW	cmd;
	UINT	imask;
	ER	err;

	p_cb->ioerr = err = E_OK;

	out_w(I2C_TAR(unit), p_cb->sadr & I2C_TAR_7BIT_ADR);	// Set slave address
	out_w(I2C_INTR_MASK(unit), 0);				// Mask all innterrupts
	out_w(I2C_ENABLE(unit), I2C_ENABLE_ENABLE);		// Enable I2C
	out_w(I2C_CLR_INTR(unit), 0);				// Clear all Interrupt Registers
	in_w(I2C_CLR_TX_ABRT(unit));				// Clear Transmit abort

	while (p_cb->state != I2C_STS_STOP) {
		DI(imask);
		p_cb->wait_tskid = tk_get_tid();
		if(p_cb->sdat_num > 0 ) {	/* Send */
			set_w(I2C_INTR_MASK(unit), I2C_INT_TX_EMPTY|I2C_INT_ERROR);
			p_cb->state = I2C_STS_SEND;
		} else {			/* Receive */
			if(p_cb->rdat_num > 1) {
				cmd = I2C_DATA_CMD_CMD_READ;
			} else {	// Last data
				cmd = I2C_DATA_CMD_CMD_READ | I2C_DATA_CMD_STOP;
			}
			set_w(I2C_INTR_MASK(unit), I2C_INT_RX_FULL|I2C_INT_ERROR);
			out_w(I2C_DATA_CMD(unit), cmd);				
			p_cb->state = I2C_STS_RECV;
		}
		EI(imask);

		if(p_cb->ioerr != E_OK) {
			err = p_cb->ioerr;
			break;
		}

		err = tk_slp_tsk(ll_devdat[unit].timout);
		if ( err < E_OK ) break;
		else err = p_cb->ioerr;
	}
	out_w(I2C_ENABLE(unit), 0);				// Disable I2C

	return err;
}

/*----------------------------------------------------------------------
 * Set communication parameters
 */
Inline void set_com_start(UW unit, UW sadr, UW sdat_num, UW rdat_num, UB *sbuf, UB *rbuf)
{
	ll_devcb[unit].state	= I2C_STS_START;
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

	p_ex = (T_I2C_EXEC*)pp;

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
		EnableInt(ll_devdat[unit].intno, ll_devdat[unit].intpri);
		break;

	case LLD_I2C_CLOSE:
		/* I2C interrupt disable */
		DisableInt(ll_devdat[unit].intno);
		break;

	case LLD_I2C_READ:
		set_com_start( unit, p1, 0, p2, NULL, (UB*)pp);
		err = i2c_trans(unit, &ll_devcb[unit]);
		if(err >= E_OK) err = p_ex->rcv_size - ll_devcb[unit].rdat_num;
		break;

	case LLD_I2C_WRITE:
		set_com_start( unit, p1, p2, 0, (UB*)pp, NULL);
		err = i2c_trans(unit, &ll_devcb[unit]);
		if(err >= E_OK) err = p_ex->snd_size - ll_devcb[unit].sdat_num;
		break;

	case LLD_I2C_EXEC:
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
	T_DINT	dint;
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
	dint.intatr	= TA_HLNG;
	dint.inthdr	= i2c_inthdr;
	
	err = tk_def_int(ll_devdat[unit].intno, &dint);

	return err;
}

#endif		/* DEV_IIC_ENABLE */
#endif		/* CPU_RP2040 */