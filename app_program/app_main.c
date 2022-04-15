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

#include <tk/tkernel.h>
#include <tk/device.h>
#include <tm/tmonitor.h>

/* ---------------------------------------------------------
 * Sample User Program
 * ---------------------------------------------------------
 * 
 * Entry routine for the user application.
 * At this point, Initialize and start the user application.
 *
 * Entry routine is called from the initial task for Kernel,
 * so system call for stopping the task should not be issued 
 * from the contexts of entry routine.
 * We recommend that:
 * (1)'usermain()' only generates the user initial task.
 * (2)initialize and start the user application by the user
 * initial task.
 */

#if USE_TMONITOR
#define TM_PUTSTRING(a)	tm_putstring(a)

void print_err( UB* str, ER err)
{
	tm_printf(str, err);
}

#else
#define TM_PUTSTRING(a)

void print_err( UB* str, INT par) {}

#endif /* USE_TMONITOR */


/* ----------------------------------------------------------
 *
 * Task-1 A/D
 *
 */
void tsk1(INT stacd, void *exinf)
{
	ID	dd;
	ER	err;
	UW	data;
	SZ	asz;

	TM_PUTSTRING((UB*)"Start Task-1\n");
	dd = tk_opn_dev((UB*)"adca", TD_UPDATE);
	while(1) {
		err = tk_srea_dev(dd, 0, &data, 1, &asz);
		tm_printf((UB*)"A/D = %d\n", data);
		tk_dly_tsk(500);
	}

	tk_exd_tsk();	/* Exit task */
}

/* ---------------------------------------------------------
 *
 * Task-2 -- I2C
 *
 */

ID	dd_i2c;

/*-------------------------------------------------------------*/
/* EEPROM R/W                                                  */
/*-------------------------------------------------------------*/
#define EEPROM_ADDR		0x53
#define	EEPROM_SIZE		256
#define	EEPROM_PAGE_SIZE		16

ER read_eeprom( ID dd_eeprom, W addr, void *buf, SZ size)
{
	static UB	data[EEPROM_SIZE];
	SZ		asz;
	ER		err;

	if(addr < 0 || addr > EEPROM_ADDR) return E_PAR;
	if((addr + size -1) >= EEPROM_SIZE ) return E_PAR;

	data[0] = addr;
	err = tk_swri_dev(dd_eeprom, EEPROM_ADDR, data, 1, &asz);
	if(err != E_OK) return err;
	tk_dly_tsk(1);

	while(size > 0) {
		if(size > EEPROM_PAGE_SIZE) {
			err = tk_srea_dev(dd_eeprom, EEPROM_ADDR, data, EEPROM_PAGE_SIZE, &asz);
			if(err != E_OK) return err;
			knl_memcpy(buf, data, EEPROM_PAGE_SIZE);
			size -= EEPROM_PAGE_SIZE; buf += EEPROM_PAGE_SIZE; addr += EEPROM_PAGE_SIZE;
		} else {
			err = tk_srea_dev(dd_eeprom, EEPROM_ADDR, data, size, &asz);
			if(err != E_OK) return err;
			knl_memcpy(buf, data, size);
			size = 0;
		}
		tk_dly_tsk(1);
	}

	return err;
}

ER write_eeprom( ID dd_eeprom, W addr, void *buf, SZ size)
{
	static UB	data[EEPROM_SIZE];
	SZ		asz;
	ER		err;

	if(addr < 0 || addr > EEPROM_ADDR) return E_PAR;
	if((addr + size -1) >= EEPROM_SIZE ) return E_PAR;

	while(size > 0) {
		data[0] = addr;
		if(size > EEPROM_PAGE_SIZE) {
			knl_memcpy(&data[1], buf, EEPROM_PAGE_SIZE);
			err = tk_swri_dev(dd_eeprom, EEPROM_ADDR, data, EEPROM_PAGE_SIZE+1, &asz);
			if(err != E_OK) return err;
			size -= EEPROM_PAGE_SIZE; buf += EEPROM_PAGE_SIZE; addr += EEPROM_PAGE_SIZE;
		} else {
			knl_memcpy(&data[1], buf, size);
			err = tk_swri_dev(dd_eeprom, EEPROM_ADDR, data, size+1, &asz);
			if(err != E_OK) return err;
			size = 0;
		}
		tk_dly_tsk(1);
	}
	return err;
}

LOCAL void test_eeprom(ID dd_i2c)
{
	UB	wd[10], rd[10];
	ER	err;

	err = read_eeprom(dd_i2c, 0, rd, 5);
	for(INT i = 0; i < 5; i++) {
		tm_printf((UB*)"%x ", rd[i]);
	}
	tm_putchar('\n');

	for(INT i = 0; i < 5; i++) {
		wd[i] = 'a'+i;
	}
	err = write_eeprom(dd_i2c, 0, wd, 5);
	err = read_eeprom(dd_i2c, 0, rd, 5);
	for(INT i = 0; i < 5; i++) {
		tm_printf((UB*)"%c ", rd[i]);
	}
	tm_putchar('\n');

	for(INT i = 0; i < 5; i++) {
		wd[i] = 'A'+i;
	}
	err = write_eeprom(dd_i2c, 0, wd, 5);
	err = read_eeprom(dd_i2c, 0, rd, 5);
	for(INT i = 0; i < 5; i++) {
		tm_printf((UB*)"%c ", rd[i]);
	}
	tm_putchar('\n');
}

/*--------------------------------------------------------------*/
/* Gesture Sensor						*/
/*--------------------------------------------------------------*/
#include "gesture.h"

LOCAL void test_i2c(ID dd_i2c)
{
	UW	val;
	ER	err;


	err = gesture_sensor_init(dd_i2c);
	if(err >= E_OK) tm_printf((UB*)"Sensor Init OK\n");
	else {
		tm_printf((UB*)"Retry\n");
		err = gesture_sensor_init(dd_i2c);
		if(err < E_OK) tm_printf((UB*)"Sensor Init ERROR %d\n", err);
	}

	while(1) {
		// Gesture sensor
		err = gesture_sensor_get(dd_i2c, &val);
		if(err<E_OK) tm_printf((UB*)"Sensor Read error %d\n", err);

		if(val & GES_RIGHT_FLAG ) {
			tm_printf((UB*)"Right\n");
		} else if(val & GES_LEFT_FLAG) {
			tm_printf((UB*)"Left\n");
		} else if(val & GES_UP_FLAG) {
			tm_printf((UB*)"Up\n");
		} else if(val & GES_DOWN_FLAG) {
			tm_printf((UB*)"Down\n");
		}

		if(val & GES_FORWARD_FLAG) {
			tm_printf((UB*)"Forward\n");
		} else if(val & GES_BACKWARD_FLAG) {
			tm_printf((UB*)"Balckward\n");
		}
		if(val & GES_CLOCKWISE_FLAG) {
			tm_printf((UB*)"Clockwise\n");
		} else if(val & GES_COUNT_CLOCKWISE_FLAG) {
			tm_printf((UB*)"Anti-Cockwise\n");
		}
		tk_dly_tsk(500);
	}
}

void tsk2(INT stacd, void *exinf)
{
	TM_PUTSTRING((UB*)"Start Task-2\n");

	dd_i2c = tk_opn_dev((UB*)"iica", TD_UPDATE);
	if(dd_i2c<E_OK) tm_printf((UB*)"Open Error %d\n", dd_i2c);

	test_eeprom(dd_i2c);
	test_i2c(dd_i2c);

	tk_exd_tsk();	/* Exit Task */
}


void tsk3(INT stacd, void *exinf)
{
	static UB buf[] = {'T', 'E', 'S', 'T', '\n'};;
	UB	rdata;
	ID	dd;
	SZ	asz;
	ER	err;

	TM_PUTSTRING((UB*)"Start Task-3\n");

	dd = tk_opn_dev((UB*)"seri", TD_UPDATE);
	if(dd<E_OK) tm_printf((UB*)"Open Error %d\n", dd);

	err = tk_swri_dev( dd, 0, buf, sizeof(buf), &asz);
	if(err<E_OK) tm_printf((UB*)"Send Error %d\n", err);

	for(INT i = 0; i<10; i++) {
		tk_srea_dev( dd, 0, &rdata, 1, &asz);
		tk_swri_dev( dd, 0, &rdata, 1, &asz);
	}

	tk_exd_tsk();	/* Exit Task */
}

const T_CTSK	ctsk1	= {0, (TA_HLNG | TA_RNG3), &tsk1, 10, 1024, 0};
const T_CTSK	ctsk2	= {0, (TA_HLNG | TA_RNG3), &tsk2, 11, 1024, 0};
const T_CTSK	ctsk3	= {0, (TA_HLNG | TA_RNG3), &tsk3, 11, 1024, 0};

/* ----------------------------------------------------------
 *
 * User-Main Definition (Run on initial task)
 *
 */

EXPORT INT usermain( void )
{
	T_RVER	rver;
	ID	id1, id2, id3;

	TM_PUTSTRING((UB*)"Start User-main program.\n");

	tk_ref_ver(&rver);		/* Get the OS Version. */

#if USE_TMONITOR
	tm_printf((UB*)"Make Code: %04x  Product ID: %04x\n", rver.maker, rver.prid);
	tm_printf((UB*)"Product Ver. %04x\nProduct Num. %04x %04x %04x %04x\n", 
			rver.prver, rver.prno[0],rver.prno[1],rver.prno[2],rver.prno[3]);
#endif

	id1 = tk_cre_tsk(&ctsk1);
	tk_sta_tsk(id1, 0);

	id2 = tk_cre_tsk(&ctsk2);
//	tk_sta_tsk(id2, 0);

	id3 = tk_cre_tsk(&ctsk3);
//	tk_sta_tsk(id3, 0);

	tk_slp_tsk(TMO_FEVR);

	return 0;
}
