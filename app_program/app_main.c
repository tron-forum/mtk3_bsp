#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <tk/device.h>

#if 0

#include "gesture.h"

/* --------------------------------------
 * Arduino Pin
 * D0:  PB7
 * D1:  PB6
 * D2:  PG14
 * D3:  PE13
 * D4:  PE14
 * D5:  PE11
 * D6:  PE9
 * D7:  PG12
 * D8:  PF3
 * D9:  PD15
 * D10: PD14
 * D11: PB5
 * D12: PA6
 * D13: PA5
 * 
 * SDA: PB9 I2C1_SDA
 * SCL: PB8 I2C1_SCL
 * 
 * A0:  PA3  ADC12_INP15
 * A1:  PC0  ADC123_INP10
 * A2:  PC3  ADC3_INP1
 * A3:  PB1  ADC12_INP5
 * A4:  PC2  ADC3_INP0
 * A5:  PF10 ADC3_INP6
 * 
 */

#define	I2C_SADR			(0x73)
#define	PAJ7620_REGITER_BANK_SEL	(0xEF)
#define	PAJ7620_ADDR_GES_PS_DET_FLAG_0	(0x43)

LOCAL void task1(INT, void*);
T_CTSK	ctsk1 = {
	.tskatr		= TA_HLNG | TA_RNG3,
	.task		= task1,
	.itskpri	= 10,
	.stksz		= 1024,
};
ID	tskid1;

LOCAL void task1(INT stacd, void *exinf)
{
	UW	pb,pe;
	ID	dd1,dd2,dd3;
	UW	data;
	UB	i2cdat;
	SZ	asz;
	ER	err;

	tm_printf((UB*)"Start Task1\n");

	err = dev_init_i2c(0);
	if(err < E_OK) {
		tm_printf((UB*)"I2C init error%d(%x)\n", err, err);
	} else {
		tm_printf((UB*)"I2C init OK\n");
	}

	dd1 = tk_opn_dev((UB*)"iica", TD_UPDATE);
	if(dd1 < E_OK) {
		tm_printf((UB*)"I2C open error %d(%x)\n", dd1, dd1);
	} else {
		tm_printf((UB*)"I2C open OK\n");
	}

	err = i2c_write_reg(dd1, I2C_SADR, PAJ7620_REGITER_BANK_SEL, 0);
	if(err < E_OK) {
		tm_printf((UB*)"I2C write error %d(%X)\n", err, err);
	}
	err = i2c_read_reg(dd1, I2C_SADR, PAJ7620_ADDR_GES_PS_DET_FLAG_0, &i2cdat);
	if(err < E_OK) {
		tm_printf((UB*)"I2C read error %d(%X)\n", err, err);
	} else {
		tm_printf((UB*)"I2C read data %d(%X)\n", i2cdat, i2cdat);
	}

	tk_ext_tsk();

#if 1
	err = dev_init_adc(0);
	if(err < E_OK) {
		tm_printf((UB*)"ADC init error%d(%x)\n", err, err);
	} else {
		tm_printf((UB*)"ADC-1 init OK\n");
	}
	err = dev_init_adc(2);
	if(err < E_OK) {
		tm_printf((UB*)"ADC init error%d(%x)\n", err, err);
	} else {
		tm_printf((UB*)"ADC-3 init OK\n");
	}
#endif
	dd2 = tk_opn_dev((UB*)"adca", TD_UPDATE);
	if(dd2 < E_OK) {
		tm_printf((UB*)"ADC open error %d(%x)\n", dd2, dd2);
	} else {
		tm_printf((UB*)"ADC open OK\n");
	}
	dd3 = tk_opn_dev((UB*)"adcc", TD_UPDATE);
	if(dd3 < E_OK) {
		tm_printf((UB*)"ADC open error %d(%x)\n", dd3, dd3);
	} else {
		tm_printf((UB*)"ADC open OK\n");
	}

	/* PE13:Arduino D3 & PE14:Arduino D4 */
	out_w(GPIO_MODER(E), ((in_w(GPIO_MODER(E)) & 0xC3FFFFFF) |(1<<26)|(1<<28)));
	pb = in_w(GPIO_ODR(B));
	pe = in_w(GPIO_ODR(E));

	while(1) {
		pb ^= ((1<<0)|(1<<14));
		pe ^= ((1<<1)|(1<<13)|(1<<14));
		out_w(GPIO_ODR(B), pb);
		out_w(GPIO_ODR(E), pe);

		err = tk_srea_dev(dd2, 15, &data, 1, &asz);			// A0:  PA3  ADC12_INP15
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A0 %d(%x)  ", data, data);
		}

		err = tk_srea_dev(dd2, 10, &data, 1, &asz);			// A1:  PC0  ADC123_INP10
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A1 %d(%x)  ", data, data);
		}

		err = tk_srea_dev(dd3, 1, &data, 1, &asz);			// A2:  PC3  ADC3_INP1
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A2 %d(%x)  ", data, data);
		}

		err = tk_srea_dev(dd2, 5, &data, 1, &asz);			// A3:  PB1  ADC12_INP5
		if(err < E_OK) {
			tm_printf((UB*)"read error %d(%x)\n", err, err);
		} else {
			tm_printf((UB*)"A3 %d(%x)\n", data, data);
		}
		tk_dly_tsk(1000);
	}
	tk_ext_tsk();
}

/* I2C通信タスクの生成情報と関連データ */
LOCAL void task_i2c(INT stacd, void *exinf);	// 実行関数
LOCAL ID	tskid_i2c;			// ID番号
LOCAL T_CTSK ctsk_i2c = {
	.itskpri	= 10,			// 初期優先度
	.stksz		= 1024,			// スタックサイズ
	.task		= task_i2c,		// 実行関数のポインタ
	.tskatr		= TA_HLNG | TA_RNG3,	// タスク属性
};

LOCAL void led_cnt(INT no)
{

}

/* I2C通信タスクの実行関数 */
LOCAL void task_i2c(INT stacd, void *exinf)
{
	UW	val;

	gesture_sensor_init(0);

	while(1) {
		gesture_sensor_get(&val);

		if(val & GES_RIGHT_FLAG ) {
			tm_printf((UB*)"Right\n");
			led_cnt(0);
		} else if(val & GES_LEFT_FLAG) {
			tm_printf((UB*)"Left\n");
			led_cnt(2);
		} else if(val & GES_UP_FLAG) {
			tm_printf((UB*)"Up\n");
			led_cnt(3);
		} else if(val & GES_DOWN_FLAG) {
			tm_printf((UB*)"Down\n");
			led_cnt(4);
		}

		if(val & GES_FORWARD_FLAG) {
			tm_printf((UB*)"Forward\n");
			led_cnt(-1);
		} else if(val & GES_BACKWARD_FLAG) {
			tm_printf((UB*)"Balckward\n");
			led_cnt(-2);
		}
		if(val & GES_CLOCKWISE_FLAG) {
			tm_printf((UB*)"Clockwise\n");
		} else if(val & GES_COUNT_CLOCKWISE_FLAG) {
			tm_printf((UB*)"Anti-Cockwise\n");
		}

		tk_dly_tsk(500);
	}
	tk_ext_tsk();
}

LOCAL void task_ser(INT stacd, void *exinf);	// 実行関数
LOCAL ID	tskid_ser;			// ID番号
LOCAL T_CTSK ctsk_ser = {
	.itskpri	= 10,			// 初期優先度
	.stksz		= 1024,			// スタックサイズ
	.task		= task_ser,		// 実行関数のポインタ
	.tskatr		= TA_HLNG | TA_RNG3,	// タスク属性
};

LOCAL void task_ser(INT stacd, void *exinf)
{
	UB	buf[]	= {'T','E','S','T','\n','\r'};
	UW	data;
	ER	err;
	ID	dd;

	err = dev_init_ser(2);
	if(err < E_OK) tm_printf((UB*)"Init error %d\n", err);

	dd = tk_opn_dev((UB*)"serc", TD_UPDATE);
	if(dd < E_OK) tm_printf((UB*)"Open error %d\n", dd);

#if 0
	data = DEV_SER_MODE_8BIT|DEV_SER_MODE_1STOP|DEV_SER_MODE_PNON;
	err = tk_swri_dev(dd, TDN_SER_MODE, &data, sizeof(data), NULL);
	if(err < E_OK) tm_printf((UB*)"error1 %d\n", err);

	data = 115200;
	err = tk_swri_dev(dd, TDN_SER_SPEED, &data, sizeof(data), NULL);
	if(err < E_OK) tm_printf((UB*)"error2 %d\n", err);
#endif

	err = tk_swri_dev(dd, 0, buf, sizeof(buf), NULL);
	if(err < E_OK) tm_printf((UB*)"error3 %d\n", err);

	tk_dly_tsk(500);
	tm_printf((UB*)"End task\n");
	tk_ext_tsk();
}

/* usermain関数 */
EXPORT INT usermain(void)
{

	tm_printf((UB*)"Start Program\n");

	tskid_ser = tk_cre_tsk(&ctsk_ser);
	tk_sta_tsk(tskid_ser, 0);

#if 0
	tskid_i2c = tk_cre_tsk(&ctsk_i2c);
	tk_sta_tsk(tskid_i2c, 0);
#endif

#if 0
	tskid1 = tk_cre_tsk(&ctsk1);
	tk_sta_tsk(tskid1, 0);
#endif
	tk_slp_tsk(TMO_FEVR);

	return 0;
}

#else

ID	tsk1_id;

UW	si = 0;
UW	sj = 0;

void ptmrhdr(void *exinf)
{

	if( (si++) % 100 == 99) {
		tk_wup_tsk(tsk1_id);
		
		if(sj++ > 10) {
			StopPhysicalTimer(1);
		}
	}
}

void tsk1(INT stacd, void *exinf)
{
	T_DPTMR	dptmr;
	SYSTIM	systim;
	UW	old;
	ER	ercd;

	tm_printf("Start Task-1\n");

	dptmr.ptmratr = TA_HLNG;
	dptmr.ptmrhdr = ptmrhdr;
	DefinePhysicalTimerHandler(1, &dptmr);
	ercd = StartPhysicalTimer( 1, 10000*18, TA_CYC_PTMR);
	if(ercd != E_OK) {
		tm_printf("Err %d\n", ercd);
	}

	tk_get_tim(&systim);
	old = systim.lo;
	while(1) {
		tk_slp_tsk(TMO_FEVR);
		tk_get_tim(&systim);
		tm_printf("%u i=%d j=%d \n", systim.lo - old, si, sj);
		old = systim.lo;
	}
}

/* ----------------------------------------------------------
 *
 * User-Main Definition (Run on initial task)
 *
 */
EXPORT INT usermain( void )
{
	T_RVER	rver;
	SYSTIM	systim;
	UW	old;
	T_CTSK	ctsk;

	tm_putstring("Start User-main program.\n");

	tk_ref_ver(&rver);		/* Get the OS Version. */

	tm_printf("Make Code: %04x  Product ID: %04x\n", rver.maker, rver.prid);
	tm_printf("Product Ver. %04x\nProduct Num. %04x %04x %04x %04x\n", 
			rver.prver, rver.prno[0],rver.prno[1],rver.prno[2],rver.prno[3]);

	tm_printf("*** System Timer\n");
	tk_get_tim(&systim);
	old = systim.lo;
	for( INT i= 0; i <10; i++) {
		tk_dly_tsk(100);
		tk_get_tim(&systim);
		tm_printf("%u\n", systim.lo - old);
		old = systim.lo;
	}

	tm_printf("*** Physical Timer\n");
	ctsk.tskatr	= TA_HLNG | TA_RNG3;
	ctsk.task	= tsk1;
	ctsk.itskpri	= 10;
	ctsk.stksz	= 1024;
	tsk1_id = tk_cre_tsk(&ctsk);
	if(tsk1_id > E_OK) {
		tk_sta_tsk(tsk1_id, 0);
	} else {
		tm_printf("Task Create Error %d\n", tsk1_id);
	}

	tk_slp_tsk(TMO_FEVR);
	return 0;
}

#endif