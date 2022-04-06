/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/04.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RX65N

#include <tk/tkernel.h>
#include "../../i2c.h"
#include "../../../include/dev_def.h"
#if DEV_IIC_ENABLE
/*
 *	i2c_rx65n.c
  *	I2C device driver
 *	System-dependent definition for RX65N
 */

/*----------------------------------------------------------------------
 * Device register base address
*/
const LOCAL UW ba[DEV_I2C_UNITNM] = { RIIC0_BASE, RIIC1_BASE, RIIC2_BASE };

#define	RIIC_ICCR1(x)		(ba[x] + ICCR1)		/* I2C bus cintrol reg. 1 */
#define	RIIC_ICCR2(x)		(ba[x] + ICCR2)		/* I2C bus cintrol reg. 2 */
#define RIIC_ICMR1(x)		(ba[x] + ICMR1)		/* I2C bus mode reg. 1 */
#define RIIC_ICMR2(x)		(ba[x] + ICMR2)		/* I2C bus mode reg. 2 */
#define RIIC_ICMR3(x)		(ba[x] + ICMR3)		/* I2C bus mode reg. 3 */
#define	RIIC_ICFER(x)		(ba[x] + ICFER)		/* I2C bus function enable reg */
#define	RIIC_ICSER(x)		(ba[x] + ICSER)		/* I2C bus Status enable reg. */
#define	RIIC_ICIER(x)		(ba[x] + ICIER)		/* I2C bus Interrupt enable reg. */
#define	RIIC_ICSR1(x)		(ba[x] + ICSR1)		/* I2C bus Status reg. 1 */
#define	RIIC_ICSR2(x)		(ba[x] + ICSR2)		/* I2C bus Status reg. 2 */

#define	RIIC_SARL(x,y)		(ba[x] + SARL(y))	/* Slave address reg. L0~2 */
#define	RIIC_SARU(x,y)		(ba[x] + SARU(y))	/* Slave address reg. U0~2 */

#define	RIIC_ICBRL(x)		(ba[x] + ICBRL)		/* I2C bus bit rate Low reg. */
#define	RIIC_ICBRH(x)		(ba[x] + ICBRH)		/* I2C bus bit rate High reg. */
#define	RIIC_ICDRT(x)		(ba[x] + ICDRT)		/* I2C bus transmit data reg. */
#define	RIIC_ICDRR(x)		(ba[x] + ICDRR)		/* I2C bus receive data reg. */

/*----------------------------------------------------------------------
 * Device data
*/
const LOCAL struct {
	UINT	intno_rxi;	// Interrupt number (RXI)
	UINT	intno_txi;	// Interrupt number (TXI)
	PRI	intpri;		// Interrupt priority
	TMO	timout;		// I2C timeout time
} ll_devdat[DEV_I2C_UNITNM] = {
	{
		.intno_rxi	= INTNO_RIIC0_RXI,
		.intno_txi	= INTNO_RIIC0_TXI,
		.intpri		= DEVCNF_I2C0_INTPRI,
		.timout		= DEVCNF_I2C0_TMO,
	},
	{
		.intno_rxi	= INTNO_RIIC1_RXI,
		.intno_txi	= INTNO_RIIC1_TXI,
		.intpri		= DEVCNF_I2C1_INTPRI,
		.timout		= DEVCNF_I2C1_TMO,
	},
	{
		.intno_rxi	= INTNO_RIIC2_RXI,
		.intno_txi	= INTNO_RIIC2_TXI,
		.intpri		= DEVCNF_I2C2_INTPRI,
		.timout		= DEVCNF_I2C2_TMO,
	},
};

/*----------------------------------------------------------------------
 * Device control data
*/
typedef struct {
	ID	wait_tskid;	/* Wait Task ID */
	UW	state;		/* Operating state */
	UW	sadr;		/* Slave address */
	ER	ioerr;		/* Communication error */
	UW	sdat_num;	/* Number of send data */
	UW	rdat_num;	/* Number of receive data */
	UB	*sbuf;		/* Send buffer */
	UB	*rbuf;		/* Receive buffer */
} T_I2C_LLDCB;

T_I2C_LLDCB	ll_devcb[DEV_I2C_UNITNM];

/*-------------------------------------------------------
 * Interrupt handler
 *-------------------------------------------------------
 */
/*
 * TXI: Transmit data empty interrupt
 */
LOCAL void i2c_txi_inthdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	INT		unit;
	UB		reg;

	for ( unit = 0; unit < DEV_I2C_UNITNM; unit++ ) {
		if ( ll_devdat[unit].intno_txi == intno ) {
			p_cb = &ll_devcb[unit];
			break;
		}
	}
	if(unit >= DEV_I2C_UNITNM) {
		ClearInt(intno);	// Clear interrupt
		return;
	}

	switch(p_cb->state) {
	case I2C_STS_START:
		if(p_cb->sdat_num > 0 ) {	/* Send */
			out_b(RIIC_ICDRT(unit), ll_devcb[unit].sadr);
			p_cb->state = I2C_STS_SEND;
		} else {			/* Receive */
			out_b(RIIC_ICDRT(unit), (p_cb->sadr|1));
			reg = in_b(RIIC_ICIER(unit));
			out_b(RIIC_ICIER(unit), (reg | RIIC_ICIER_RIE) & ~RIIC_ICIER_TIE);
			p_cb->state = I2C_STS_RECV | I2C_STS_TOP;
		}
		break;

	case I2C_STS_SEND:
		if(--(p_cb->sdat_num) <= 0) {
			if(p_cb->rdat_num == 0) {
				p_cb->state = I2C_STS_STOP;
			} else {
				p_cb->state = I2C_STS_RESTART;
			}
			reg = in_b(RIIC_ICIER(unit));
			out_b(RIIC_ICIER(unit), (reg | RIIC_ICIER_TEIE) & ~RIIC_ICIER_TIE);
		}
		out_b(RIIC_ICDRT(unit), *(p_cb->sbuf)++);
		break;
	}
}

LOCAL void i2c_rxi_inthdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	INT		unit;
	UB		reg;

	for ( unit = 0; unit < DEV_I2C_UNITNM; unit++ ) {
		if ( ll_devdat[unit].intno_rxi == intno ) {
			p_cb = &ll_devcb[unit];
			break;
		}
	}
	if(unit >= DEV_I2C_UNITNM) {
		ClearInt(intno);	// Clear interrupt
		return;
	}

	switch(p_cb->state) {
	case I2C_STS_RECV | I2C_STS_TOP:
		if(p_cb->rdat_num <= 2) {	// Last data -2
			*(UB*)RIIC_ICMR3(unit) |= RIIC_ICMR3_WAIT;
		}
		if(p_cb->rdat_num == 1) {	//Next is the final data
			*(UB*)RIIC_ICMR3(unit) |= RIIC_ICMR3_ACKBT;
			p_cb->state = I2C_STS_RECV | I2C_STS_LAST;
		} else {
			p_cb->state = I2C_STS_RECV;
		}
		in_b(RIIC_ICDRR(unit));	// Dummy read
		break;

	case I2C_STS_RECV:
		if(p_cb->rdat_num == 3) {	// Last data -2
			*(UB*)RIIC_ICMR3(unit) |= RIIC_ICMR3_WAIT;
		}
		if(p_cb->rdat_num == 2) {	//Next is the final data
			*(UB*)RIIC_ICMR3(unit) |= RIIC_ICMR3_ACKBT;
			p_cb->state = I2C_STS_RECV | I2C_STS_LAST;
		}
		*(p_cb->rbuf)++ = in_b(RIIC_ICDRR(unit));
		p_cb->rdat_num--;
		break;

	case I2C_STS_RECV | I2C_STS_LAST:
		*(UB*)RIIC_ICSR2(unit) &= ~RIIC_ICSR2_STOP;
		reg = in_b(RIIC_ICIER(unit));
		out_b(RIIC_ICIER(unit), (reg | RIIC_ICIER_SPIE) & ~RIIC_ICIER_RIE);
		*(UB*)RIIC_ICCR2(unit) |= RIIC_IICR2_SP;
		*(p_cb->rbuf)++ = in_b(RIIC_ICDRR(unit));
		*(UB*)RIIC_ICMR3(unit) &= ~RIIC_ICMR3_WAIT;
		p_cb->state = I2C_STS_STOP;
		break;
	}
}

/*
 * TEI: Transmission end interrupt
 */
LOCAL void i2c_tei_inthdr( UINT unit )
{
	UB	reg;

	switch(ll_devcb[unit].state) {
	case I2C_STS_STOP:
		reg = in_b(RIIC_ICIER(unit));
		out_b(RIIC_ICIER(unit), (reg & ~RIIC_ICIER_TEIE) |RIIC_ICIER_SPIE);
		*(UB*)RIIC_ICCR2(unit) |= RIIC_IICR2_SP;	// Stop condition
		break;
	
	case I2C_STS_RESTART:
		out_b( RIIC_ICIER(unit) , RIIC_ICIER_TIE | RIIC_ICIER_NAKIE);	/* Enable TXI & NAKI */
		*(UB*)RIIC_ICCR2(unit) |= RIIC_IICR2_RS;	// Rester condition
		ll_devcb[unit].state = I2C_STS_START;
		break;
	}
}

LOCAL void i2c_eei_inthdr( UINT unit )
{
	UB	reg;

	reg = in_b(RIIC_ICSR2(unit));

	if(reg & RIIC_ICSR2_STOP) {		/* Detects stop condition */
		if (ll_devcb[unit].state != I2C_STS_STOP) {
			ll_devcb[unit].ioerr = E_IO;
		}
		out_b( RIIC_ICIER(unit), 0);
		if(ll_devcb[unit].wait_tskid) {
			tk_wup_tsk(ll_devcb[unit].wait_tskid);
			ll_devcb[unit].wait_tskid = 0;
		}
	} else {
		ll_devcb[unit].ioerr = E_IO;
		ll_devcb[unit].state = I2C_STS_STOP;
		*(UB*)RIIC_ICSR2(unit) &= ~RIIC_ICSR2_STOP;
		out_b( RIIC_ICIER(unit), RIIC_ICIER_SPIE);
		*(UB*)RIIC_ICCR2(unit) |= RIIC_IICR2_SP;	/* Stop condition */
	}
}

LOCAL void i2c_xei_inthdr( UINT intno )
{
	UW	reg;

	reg = in_w(ICU_GRPBL1);
	if(reg & (1<<13)) {
		i2c_tei_inthdr(DEV_I2C_UNIT0);
	} else if(reg & (1<<14)) {
		i2c_eei_inthdr(DEV_I2C_UNIT0);
	} else if(reg & (1<<28)) {
		i2c_tei_inthdr(DEV_I2C_UNIT1);
	} else if(reg & (1<<29)) {
		i2c_eei_inthdr(DEV_I2C_UNIT1);
	} else if(reg & (1<<15)) {
		i2c_tei_inthdr(DEV_I2C_UNIT2);
	} else if(reg & (1<<16)) {
		i2c_eei_inthdr(DEV_I2C_UNIT2);
	}
	ClearInt(intno);
}

/*----------------------------------------------------------------------
 * Execution of communication
 */
LOCAL ER i2c_trans( UW unit)
{
	UB		reg;
	ER		err;

	if( in_b(RIIC_ICCR2(unit)) & RIIC_IICR2_BBSY) {
		ll_devcb[unit].ioerr = E_BUSY;		/* I2C bus busy */
		return E_BUSY;
	}

	ll_devcb[unit].ioerr		= E_OK;
	ll_devcb[unit].wait_tskid 	= tk_get_tid();

	out_b( RIIC_ICIER(unit) , RIIC_ICIER_TIE | RIIC_ICIER_NAKIE);	/* Enable TXI & NAKI */
//	out_b( RIIC_ICIER , 0xFF);	/* Enable TXI & NAKI */
	*(UB*)RIIC_ICCR2(unit) |= RIIC_IICR2_ST;		/* start condition */

	err = tk_slp_tsk(ll_devdat[unit].timout);
	if ( ll_devcb[unit].ioerr != E_OK ) {
		err = ll_devcb[unit].ioerr;
	}

	out_b( RIIC_ICIER(unit), 0);
	reg = in_b(RIIC_ICSR2(unit));
	out_b( RIIC_ICSR2(unit), reg & ~RIIC_ICSR2_STOP & ~RIIC_ICSR2_NACKF);

	return err;
}

/*----------------------------------------------------------------------
 * Set communication start parameters
 */
Inline void set_com_start( T_I2C_LLDCB *p_cb, 
				UW sadr, UW sdat_num, UW rdat_num, UB *sbuf, UB *rbuf)
{
	p_cb->state	= I2C_STS_START;
	p_cb->sadr	= sadr<<1;	/* Slave address */
	p_cb->sdat_num	= sdat_num;	/* Number of send data */
	p_cb->rdat_num	= rdat_num;	/* Number of receive data */
	p_cb->sbuf	= sbuf;		/* Send buffer */
	p_cb->rbuf	= rbuf;		/* Receive buffer */
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
		/* I2C Device reset */
		out_b( RIIC_ICCR1(unit), RIIC_ICCR1_IICRST | RIIC_ICCR1_SOWP);
		out_b( RIIC_ICCR1(unit), RIIC_ICCR1_IICRST| RIIC_ICCR1_ICE | RIIC_ICCR1_SOWP);

		/* I2C Initial setting */
		out_b( RIIC_ICSER(unit), 0);
		out_b( RIIC_ICMR1(unit), DEVCNF_I2C_CKS<<4 );
		out_b( RIIC_ICBRH(unit), RIIC_ICBRH_INI);
		out_b( RIIC_ICBRL(unit), RIIC_ICBRL_INI);

		/* Interrupt initialize */
		out_b( RIIC_ICIER(unit), 0);
		out_b( RIIC_ICSR2(unit), 0);
		EnableInt( ll_devdat[unit].intno_rxi, ll_devdat[unit].intpri);
		EnableInt( ll_devdat[unit].intno_txi, ll_devdat[unit].intpri);
		EnableInt( INTNO_GROUPBL1, ll_devdat[unit].intpri);

		/* Release reset */
		out_b( RIIC_ICCR1(unit),  RIIC_ICCR1_ICE | RIIC_ICCR1_SOWP);
		break;

	case LLD_I2C_CLOSE:
		/* Disable I2C interrupt */
		DisableInt(ll_devdat[unit].intno_rxi);
		DisableInt(ll_devdat[unit].intno_txi);
		DisableInt(INTNO_GROUPBL1);
		break;

	case LLD_I2C_READ:
		set_com_start( &ll_devcb[unit], p1, 0, p2, NULL, (UB*)pp);
		err = i2c_trans(unit);
		if(err >= E_OK) err = p2 - ll_devcb[unit].sdat_num;
		break;

	case LLD_I2C_WRITE:
		set_com_start( &ll_devcb[unit], p1, p2, 0, (UB*)pp, NULL);
		err = i2c_trans(unit);
		if(err >= E_OK) err = p2 - ll_devcb[unit].sdat_num;
		break;

	case LLD_I2C_EXEC:
		p_ex = (T_I2C_EXEC*)pp;
		set_com_start( &ll_devcb[unit], (p_ex->sadr), p_ex->snd_size, p_ex->rcv_size, p_ex->snd_data, p_ex->rcv_data);
		err = i2c_trans(unit);
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
	T_DINT	dint = {
		.intatr = TA_HLNG,
	};
	UW	unit;
	ER	err;

	unit = p_dcb->unit;

#if DEVCONF_I2C_INIT_MSTP		// Initialize module stop
	UINT	sts;

	if(in_w(MSTPCRB) & (1<<21)) {
		DI(sts);
		out_h(SYSTEM_PRCR, 0xA502);		// Disable Register Protect

		/* Release module stop */
		switch (unit) {
		case DEV_I2C_UNIT0:
			*(UW*)(MSTPCRB) &= ~(1<<21);
			break;
		case DEV_I2C_UNIT1:
			*(UW*)(MSTPCRB) &= ~(1<<20);
			break;
		case DEV_I2C_UNIT2:
			*(UW*)(MSTPCRC) &= ~(1<<17);
			break;
		default:
			return E_SYS;
			break;
		}

		out_h(SYSTEM_PRCR, 0xA500);		// Enable Register protect
		EI(sts);
	}
#endif	/* DEVCONF_I2C_INIT_MSTP */

	out_b(RIIC_ICCR1(unit), in_b(RIIC_ICCR1(unit))&~RIIC_ICCR1_ICE);	// Disable RIIC

	/* Interrupt handler definition */
	dint.inthdr	= i2c_rxi_inthdr;
	err = tk_def_int(ll_devdat[unit].intno_rxi, &dint);

	dint.inthdr	= i2c_txi_inthdr;
	err = tk_def_int(ll_devdat[unit].intno_txi, &dint);

	dint.inthdr	= i2c_xei_inthdr;
	err = tk_def_int(INTNO_GROUPBL1, &dint);	// GROUPBL1

#if DEVCONF_I2C_INIT_GRPI
	UW	reg;

	reg = in_w(ICU_GENBL1);
	switch(unit) {
	case DEV_I2C_UNIT0:
		reg |= (1<<13)|(1<<14);
		break;
	case DEV_I2C_UNIT1:
		reg |= (1<<28)|(1<<29);
		break;
	case DEV_I2C_UNIT2:
		reg |= (1<<15)|(1<<16);
		break;
	}
	out_w(ICU_GENBL1, reg);
#endif

	return err;
}

#endif		/* DEV_IIC_ENABLE */
#endif		/* CPU_RX65N */