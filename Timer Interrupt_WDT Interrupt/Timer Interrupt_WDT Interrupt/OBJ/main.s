;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.7.1 #0 (MSVC)
;--------------------------------------------------------
; NY8A port
;--------------------------------------------------------
	.file	"main.c"
	list	p=NY8BM72A,c=on
	#include "ny8bm72a.inc"
	.debuginfo language C89
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_multi_16b
	extern	_tick_Event
	extern	_tick_Init
	extern	_tick_GetTickCount
	extern	_tick_GetElapsedTime
	extern	_PORTAbits
	extern	_PORTBbits
	extern	_PORTCbits
	extern	_PCONbits
	extern	_BWUCONbits
	extern	_ABPLCONbits
	extern	_BPHCONbits
	extern	_CPHCONbits
	extern	_INTEbits
	extern	_INTFbits
	extern	_ADMDbits
	extern	_ADRbits
	extern	_ADVREFHbits
	extern	_ADCRbits
	extern	_AWUCONbits
	extern	_PACONbits
	extern	_INTEDGbits
	extern	_ANAENbits
	extern	_RFCbits
	extern	_INTE2bits
	extern	_INDF
	extern	_TMR0
	extern	_PCL
	extern	_STATUS
	extern	_FSR
	extern	_PORTA
	extern	_PORTB
	extern	_PORTC
	extern	_PCON
	extern	_BWUCON
	extern	_PCHBUF
	extern	_ABPLCON
	extern	_BPHCON
	extern	_CPHCON
	extern	_INTE
	extern	_INTF
	extern	_ADMD
	extern	_ADR
	extern	_ADD
	extern	_ADVREFH
	extern	_ADCR
	extern	_AWUCON
	extern	_PACON
	extern	_INTEDG
	extern	_TMRH
	extern	_ANAEN
	extern	_RFC
	extern	_TM3RH
	extern	_OSCCALH
	extern	_OSCCALL
	extern	_INTE2
	extern	_TMR1
	extern	_T1CR1
	extern	_T1CR2
	extern	_PWM1DUTY
	extern	_PS1CV
	extern	_BZ1CR
	extern	_IRCR
	extern	_TBHP
	extern	_TBHD
	extern	_TMR2
	extern	_T2CR1
	extern	_T2CR2
	extern	_PWM2DUTY
	extern	_PS2CV
	extern	_BZ2CR
	extern	_OSCCR
	extern	_TMR3
	extern	_T3CR1
	extern	_T3CR2
	extern	_PWM3DUTY
	extern	_PS3CV
	extern	_BZ3CR
	extern	_IOSTA
	extern	_IOSTB
	extern	_IOSTC
	extern	_APHCON
	extern	_PS0CV
	extern	_CPLCON
	extern	_BODCON
	extern	_CODCON
	extern	_CMPCR
	extern	_PCON1
	extern	_T0MD
	extern	___sdcc_saved_fsr
	extern	___sdcc_saved_stk00
	extern	___sdcc_saved_stk01
	extern	__nyc_ny8_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	extern	_turnOffDevice
	extern	_delay
	extern	_charge_process
	extern	_key_process
	extern	_isr
	extern	_key_initial
	extern	_volume_initial
	extern	_digital_initial
	extern	_Open_digital
	extern	_Close_digital
	extern	_digital_process_task
	extern	_key_scan_task
	extern	_charger_scan_task
	extern	_v1_mode
	extern	_v2_mode
	extern	_v3_mode
	extern	_v0_mode
	extern	_stop_buzzer
	extern	_start_buzzer
	extern	_turnOnDevice_buzzer
	extern	_buzzer_task
	extern	_key_process_task
	extern	_led_task
	extern	_charge_check_Task
	extern	_ModeChoode
	extern	_main
	extern	_timer
	extern	_get_key_status
	extern	_turnOnFlag
	extern	_key_press_flag
	extern	_key_press_timer
	extern	_key_click
	extern	_control_digital
	extern	_key_long
	extern	_key_release_flag
	extern	_key_release_timer
	extern	_key_long_flag
	extern	_sleep_flag
	extern	_charge_remove_timer
	extern	_charge_plugin_timer
	extern	_charge_remove_flag
	extern	_charge_plugin_flag
	extern	_charge_on_status
	extern	_charge_remove_status
	extern	_weekup_by_int0

	extern PSAVE
	extern SSAVE
	extern WSAVE
	extern STK04
	extern STK03
	extern STK02
	extern STK01
	extern STK00

.segment "uninit", 0x20000040
PSAVE:
	.res 1
SSAVE:
	.res 1
WSAVE:
	.res 1
STK04:
	.res 1
STK03:
	.res 1
STK02:
	.res 1
STK01:
	.res 1
STK00:
	.res 1

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
.segment "uninit"
r0x104D:
	.res	1
.segment "uninit"
r0x104E:
	.res	1
.segment "uninit"
r0x104F:
	.res	1
.segment "uninit"
r0x1050:
	.res	1
.segment "uninit"
r0x1051:
	.res	1
.segment "uninit"
r0x1052:
	.res	1
.segment "uninit"
r0x104B:
	.res	1
.segment "uninit"
r0x1042:
	.res	1
.segment "uninit"
r0x104C:
	.res	1
.segment "uninit"
r0x104A:
	.res	1
.segment "uninit"
r0x1043:
	.res	1
.segment "uninit"
r0x1049:
	.res	1
.segment "uninit"
r0x1044:
	.res	1
.segment "uninit"
r0x1045:
	.res	1
.segment "uninit"
r0x1046:
	.res	1
.segment "uninit"
r0x1047:
	.res	1
.segment "uninit"
r0x1048:
	.res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

.segment "idata"
_timer:
	dw	0x00
	.debuginfo gvariable name=_timer,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_TrunOnDeviceFlag:
	dw	0x00
	.debuginfo gvariable name=_TrunOnDeviceFlag,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_TrunOffDeviceFlag:
	dw	0x00
	.debuginfo gvariable name=_TrunOffDeviceFlag,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_get_key_status:
	dw	0x00
	.debuginfo gvariable name=_get_key_status,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_buzzerOneTime:
	dw	0x00
	.debuginfo gvariable name=_buzzerOneTime,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_buzzerTwoTime:
	dw	0x00
	.debuginfo gvariable name=_buzzerTwoTime,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_buzzerThreeTime:
	dw	0x00
	.debuginfo gvariable name=_buzzerThreeTime,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_buzzerFourTime:
	dw	0x00
	.debuginfo gvariable name=_buzzerFourTime,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_deviceWorkStatus:
	dw	0x00
	.debuginfo gvariable name=_deviceWorkStatus,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_recordDevice:
	dw	0x00
	.debuginfo gvariable name=_recordDevice,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_wakeupFlag:
	dw	0x00
	.debuginfo gvariable name=_wakeupFlag,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_turnOnOffBuzzer:
	dw	0x00
	.debuginfo gvariable name=_turnOnOffBuzzer,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_turnOnFlag:
	dw	0x00
	.debuginfo gvariable name=_turnOnFlag,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_key_press_flag:
	dw	0x00
	.debuginfo gvariable name=_key_press_flag,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_key_press_timer:
	dw	0x00, 0x00
	.debuginfo gvariable name=_key_press_timer,2byte,array=0,entsize=2,ext=1,enc=unsigned


.segment "idata"
_key_click:
	dw	0x00
	.debuginfo gvariable name=_key_click,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_control_digital:
	dw	0x00
	.debuginfo gvariable name=_control_digital,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_key_long:
	dw	0x00
	.debuginfo gvariable name=_key_long,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_key_release_flag:
	dw	0x01
	.debuginfo gvariable name=_key_release_flag,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_key_release_timer:
	dw	0x00
	.debuginfo gvariable name=_key_release_timer,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_key_long_flag:
	dw	0x00
	.debuginfo gvariable name=_key_long_flag,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_sleep_flag:
	dw	0x00
	.debuginfo gvariable name=_sleep_flag,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_charge_remove_timer:
	dw	0x00
	.debuginfo gvariable name=_charge_remove_timer,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_charge_plugin_timer:
	dw	0x00
	.debuginfo gvariable name=_charge_plugin_timer,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_charge_remove_flag:
	dw	0x00
	.debuginfo gvariable name=_charge_remove_flag,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_charge_plugin_flag:
	dw	0x01
	.debuginfo gvariable name=_charge_plugin_flag,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_charge_on_status:
	dw	0x00
	.debuginfo gvariable name=_charge_on_status,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_charge_remove_status:
	dw	0x00
	.debuginfo gvariable name=_charge_remove_status,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_weekup_by_int0:
	dw	0x00
	.debuginfo gvariable name=_weekup_by_int0,1byte,array=0,entsize=1,ext=1,enc=unsigned


.segment "idata"
_Couter:
	dw	0x00
	.debuginfo gvariable name=_Couter,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_digital_process_task_digitalProcessTimer_65536_38:
	dw	0x00, 0x00, 0x00, 0x00
	.debuginfo gvariable name=_digital_process_task_digitalProcessTimer_65536_38,4byte,array=0,entsize=4,ext=0,enc=unsigned


.segment "idata"
_key_scan_task_keyScanTimer_65536_43:
	dw	0x00, 0x00, 0x00, 0x00
	.debuginfo gvariable name=_key_scan_task_keyScanTimer_65536_43,4byte,array=0,entsize=4,ext=0,enc=unsigned


.segment "idata"
_charger_scan_task_ChargerScanTimer_65536_47:
	dw	0x00, 0x00, 0x00, 0x00
	.debuginfo gvariable name=_charger_scan_task_ChargerScanTimer_65536_47,4byte,array=0,entsize=4,ext=0,enc=unsigned


.segment "idata"
_buzzer_task_ucBuzzerOnTimer_65536_65:
	dw	0x00
	.debuginfo gvariable name=_buzzer_task_ucBuzzerOnTimer_65536_65,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_buzzer_task_ucBuzzerOffTimer_65536_65:
	dw	0x00
	.debuginfo gvariable name=_buzzer_task_ucBuzzerOffTimer_65536_65,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_buzzer_task_BuzzerTimes_65536_65:
	dw	0x00
	.debuginfo gvariable name=_buzzer_task_BuzzerTimes_65536_65,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_buzzer_task_startBuzzerOffFlag_65536_65:
	dw	0x00
	.debuginfo gvariable name=_buzzer_task_startBuzzerOffFlag_65536_65,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_key_process_task_keyProcessTimer_65536_102:
	dw	0x00, 0x00, 0x00, 0x00
	.debuginfo gvariable name=_key_process_task_keyProcessTimer_65536_102,4byte,array=0,entsize=4,ext=0,enc=unsigned


.segment "idata"
_key_process_task_FlashFlag_65536_102:
	dw	0x00
	.debuginfo gvariable name=_key_process_task_FlashFlag_65536_102,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_TrunOffDeviceTimer:
	dw	0x00, 0x00, 0x00, 0x00
	.debuginfo gvariable name=_TrunOffDeviceTimer,4byte,array=0,entsize=4,ext=0,enc=unsigned


.segment "idata"
_led_task_TrunOnDeviceTimer_65536_114:
	dw	0x00, 0x00, 0x00, 0x00
	.debuginfo gvariable name=_led_task_TrunOnDeviceTimer_65536_114,4byte,array=0,entsize=4,ext=0,enc=unsigned


.segment "idata"
_led_task_TrunOffDeviceTimer_65536_114:
	dw	0x00, 0x00, 0x00, 0x00
	.debuginfo gvariable name=_led_task_TrunOffDeviceTimer_65536_114,4byte,array=0,entsize=4,ext=0,enc=unsigned


.segment "idata"
_led_task_flag_65536_114:
	dw	0x00
	.debuginfo gvariable name=_led_task_flag_65536_114,1byte,array=0,entsize=1,ext=0,enc=unsigned


.segment "idata"
_charge_check_Task_Waiter8sTimer_65536_127:
	dw	0x00, 0x00, 0x00, 0x00
	.debuginfo gvariable name=_charge_check_Task_Waiter8sTimer_65536_127,4byte,array=0,entsize=4,ext=0,enc=unsigned

;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; reset vector 
;--------------------------------------------------------
ORG	0x0000
	LGOTO	__nyc_ny8_startup
;--------------------------------------------------------
; interrupt and initialization code
;--------------------------------------------------------
ORG 0x0008
	MGOTO	__sdcc_interrupt

.segment "code"
__sdcc_interrupt:
;***
;  pBlock Stats: dbName = I
;***
;functions called:
;   _delay
;   _tick_Event
;   _key_process
;   _charge_process
;   _delay
;   _tick_Event
;   _key_process
;   _charge_process
;8 compiler assigned registers:
;   STK00
;   STK01
;   r0x104D
;   r0x104E
;   r0x104F
;   r0x1050
;   r0x1051
;   r0x1052
;; Starting pCode block
_isr:
; 0 exit points
	.line	75, "main.c"; 	void isr(void) __interrupt(0)
	MOVAR	WSAVE
	SWAPR	STATUS,W
	CLRR	STATUS
	MOVAR	SSAVE
	MOVR	PCHBUF,W
	CLRR	PCHBUF
	MOVAR	PSAVE
	MOVR	FSR,W
	BANKSEL	___sdcc_saved_fsr
	MOVAR	___sdcc_saved_fsr
	MOVR	STK00,W
	BANKSEL	___sdcc_saved_stk00
	MOVAR	___sdcc_saved_stk00
	MOVR	STK01,W
	BANKSEL	___sdcc_saved_stk01
	MOVAR	___sdcc_saved_stk01
	.line	77, "main.c"; 	unsigned int x = 0;
	BANKSEL	r0x104D
	CLRR	r0x104D
	BANKSEL	r0x104E
	CLRR	r0x104E
	.line	78, "main.c"; 	if(INTFbits.INT0IF)
	BTRSS	_INTFbits,2
	MGOTO	_00106_DS_
	.line	80, "main.c"; 	INTF = (char)~(C_INT_EXT0);			// Clear INT0IF(External interrupt 0 flag bit)		
	MOVIA	0xfb
	MOVAR	_INTF
	.line	81, "main.c"; 	weekup_by_int0 = 1;
	MOVIA	0x01
	BANKSEL	_weekup_by_int0
	MOVAR	_weekup_by_int0
_00106_DS_:
	.line	83, "main.c"; 	if(INTFbits.PABIF)
	BTRSS	_INTFbits,1
	MGOTO	_00117_DS_
	.line	85, "main.c"; 	INTFbits.PABIF = 0;					// Clear PABIF(PortB input change interrupt flag bit)
	MOVIA	0xfd
	MOVAR	(_INTFbits + 0)
	.line	86, "main.c"; 	while((~PORTB)&0x40)
	BANKSEL	r0x104F
	CLRR	r0x104F
	BANKSEL	r0x1050
	CLRR	r0x1050
_00110_DS_:
	MOVR	_PORTB,W
	BANKSEL	r0x1051
	MOVAR	r0x1051
	BANKSEL	r0x1052
	CLRR	r0x1052
	BANKSEL	r0x1051
	COMR	r0x1051,W
	MOVAR	r0x1051
	BANKSEL	r0x1052
	COMR	r0x1052,W
	MOVAR	r0x1052
	BANKSEL	r0x1051
	BTRSS	r0x1051,6
	MGOTO	_00112_DS_
	.line	88, "main.c"; 	x++;
	BANKSEL	r0x104F
	INCR	r0x104F,F
	BTRSS	STATUS,2
	MGOTO	_00002_DS_
	BANKSEL	r0x1050
	INCR	r0x1050,F
_00002_DS_:
	BANKSEL	r0x104F
	MOVR	r0x104F,W
	BANKSEL	r0x104D
	MOVAR	r0x104D
	BANKSEL	r0x1050
	MOVR	r0x1050,W
	BANKSEL	r0x104E
	MOVAR	r0x104E
	.line	89, "main.c"; 	PORTA &= 0xFB;
	BANKSEL	_PORTA
	BCR	_PORTA,2
	.line	90, "main.c"; 	PORTB |= 0x20;
	BSR	_PORTB,5
	.line	91, "main.c"; 	wakeupFlag = 1;
	MOVIA	0x01
	BANKSEL	_wakeupFlag
	MOVAR	_wakeupFlag
	.line	92, "main.c"; 	CLRWDT();
	clrwdt
	.line	93, "main.c"; 	delay(10);
	MOVIA	0x0a
	MOVAR	STK00
	MOVIA	0x00
	MCALL	_delay
;;swapping arguments (AOP_TYPEs 1/2)
;;unsigned compare: left >= lit (0x29=41), size=2
	.line	94, "main.c"; 	if((x>40)&&(!TrunOnDeviceFlag))   //长按按键开机
	MOVIA	0x29
	BANKSEL	r0x104F
	SUBAR	r0x104F,W
	MOVIA	0x00
	BANKSEL	r0x1050
	SBCAR	r0x1050,W
	BTRSS	STATUS,0
	MGOTO	_00110_DS_
	BANKSEL	_TrunOnDeviceFlag
	MOVR	_TrunOnDeviceFlag,W
	BTRSS	STATUS,2
	MGOTO	_00110_DS_
	.line	96, "main.c"; 	PORTA |= 0x01;
	BANKSEL	_PORTA
	BSR	_PORTA,0
	.line	97, "main.c"; 	PORTA |= 0x04;
	BSR	_PORTA,2
	.line	98, "main.c"; 	PORTB &= 0xDF;
	BCR	_PORTB,5
	.line	99, "main.c"; 	BWUCON = 0;
	CLRR	_BWUCON
	.line	100, "main.c"; 	OSCCR = C_Normal_Mode|C_FHOSC_Sel;
	MOVIA	0x01
	.debuginfo linetag 1
	SFUN	_OSCCR
	NOP	
	.line	101, "main.c"; 	TrunOnDeviceFlag = 1;
	MOVIA	0x01
	BANKSEL	_TrunOnDeviceFlag
	MOVAR	_TrunOnDeviceFlag
	.line	102, "main.c"; 	turnOnFlag = 1;
	BANKSEL	_turnOnFlag
	MOVAR	_turnOnFlag
	.line	103, "main.c"; 	INTFbits.PABIF = 0;
	MOVIA	0xfd
	MOVAR	(_INTFbits + 0)
	.line	104, "main.c"; 	return;
	MGOTO	_00124_DS_
;;swapping arguments (AOP_TYPEs 1/2)
;;unsigned compare: left >= lit (0x29=41), size=2
_00112_DS_:
	.line	107, "main.c"; 	if((x<=40)&&(!turnOnFlag))
	MOVIA	0x29
	BANKSEL	r0x104D
	SUBAR	r0x104D,W
	MOVIA	0x00
	BANKSEL	r0x104E
	SBCAR	r0x104E,W
	BTRSC	STATUS,0
	MGOTO	_00117_DS_
	BANKSEL	_turnOnFlag
	MOVR	_turnOnFlag,W
	BTRSS	STATUS,2
	MGOTO	_00117_DS_
	.line	109, "main.c"; 	sleep_flag = 1;
	MOVIA	0x01
	BANKSEL	_sleep_flag
	MOVAR	_sleep_flag
_00117_DS_:
	.line	112, "main.c"; 	if(INTFbits.T0IF)
	BTRSS	_INTFbits,0
	MGOTO	_00121_DS_
	.line	114, "main.c"; 	INTF= (char)~(C_INT_TMR0);			// Clear T0IF flag bit	
	MOVIA	0xfe
	MOVAR	_INTF
	.line	115, "main.c"; 	timer++;
	BANKSEL	_timer
	INCR	_timer,F
;;swapping arguments (AOP_TYPEs 1/3)
;;unsigned compare: left >= lit (0x6=6), size=1
	.line	116, "main.c"; 	if(timer>5)
	MOVIA	0x06
	SUBAR	_timer,W
	BTRSS	STATUS,0
	MGOTO	_00121_DS_
	.line	118, "main.c"; 	tick_Event();                              //10 ms 一次。
	MCALL	_tick_Event
	.line	119, "main.c"; 	key_process();
	MCALL	_key_process
	.line	120, "main.c"; 	charge_process();
	MCALL	_charge_process
	.line	121, "main.c"; 	timer = 0;
	BANKSEL	_timer
	CLRR	_timer
_00121_DS_:
	.line	124, "main.c"; 	if(INTFbits.WDTIF)
	BTRSS	_INTFbits,6
	MGOTO	_00124_DS_
	.line	126, "main.c"; 	INTF= (char)~(C_INT_WDT);			// Clear WDTIF flag bit		
	MOVIA	0xbf
	MOVAR	_INTF
_00124_DS_:
	.line	128, "main.c"; 	}
	BANKSEL	___sdcc_saved_stk01
	MOVR	___sdcc_saved_stk01,W
	MOVAR	STK01
	BANKSEL	___sdcc_saved_stk00
	MOVR	___sdcc_saved_stk00,W
	MOVAR	STK00
	BANKSEL	___sdcc_saved_fsr
	MOVR	___sdcc_saved_fsr,W
	MOVAR	FSR
	MOVR	PSAVE,W
	MOVAR	PCHBUF
	CLRR	STATUS
	SWAPR	SSAVE,W
	MOVAR	STATUS
	SWAPR	WSAVE,F
	SWAPR	WSAVE,W
END_OF_INTERRUPT:
	RETIE	

;--------------------------------------------------------
; code
;--------------------------------------------------------
;***
;  pBlock Stats: dbName = M
;***
;has an exit
;functions called:
;   _tick_Init
;   _volume_initial
;   _digital_initial
;   _v3_mode
;   _delay
;   _stop_buzzer
;   _turnOffDevice
;   _led_task
;   _key_process_task
;   _buzzer_task
;   _charge_check_Task
;   _digital_process_task
;   _tick_Init
;   _volume_initial
;   _digital_initial
;   _v3_mode
;   _delay
;   _stop_buzzer
;   _turnOffDevice
;   _led_task
;   _key_process_task
;   _buzzer_task
;   _charge_check_Task
;   _digital_process_task
;2 compiler assigned registers:
;   r0x1048
;   STK00
;; Starting pCode block
.segment "code"; module=main, function=_main
	.debuginfo subprogram _main
_main:
; 2 exit points
	.line	766, "main.c"; 	IOSTB =  0;								// Set PB to output mode
	CLRA	
	IOST	_IOSTB
	.line	767, "main.c"; 	PORTB = 0;                           	// PortB Data Register = 0x00
	CLRR	_PORTB
	.line	769, "main.c"; 	IOSTA = C_PA0_Output;
	CLRA	
	IOST	_IOSTA
	.line	771, "main.c"; 	ABPLCON &= (char)~(C_PA0_PHB);
	BANKSEL	_ABPLCON
	BCR	_ABPLCON,0
	.line	772, "main.c"; 	UPDATE_REG(PORTA);	
	MOVR	_PORTA,F
	.line	773, "main.c"; 	PORTA |= 0x01;
	BANKSEL	_PORTA
	BSR	_PORTA,0
	.line	775, "main.c"; 	IOSTB &= 0xDF;                       //0 output,1 input
	IOSTR	_IOSTB
	BANKSEL	r0x1048
	MOVAR	r0x1048
	BCR	r0x1048,5
	MOVR	r0x1048,W
	IOST	_IOSTB
	.line	776, "main.c"; 	BODCON = C_PB5_OD;
	MOVIA	0x20
	IOST	_BODCON
	.line	777, "main.c"; 	PORTB |= 0x20;
	BSR	_PORTB,5
	.line	779, "main.c"; 	IOSTB  |= 0x40;                         //0 output,1 input 
	IOSTR	_IOSTB
	MOVAR	r0x1048
	BSR	r0x1048,6
	MOVR	r0x1048,W
	IOST	_IOSTB
	.line	780, "main.c"; 	BPHCON &= (char)~(C_PB6_PHB);                  //上拉
	BCR	_BPHCON,6
	.line	781, "main.c"; 	PORTB  |= 0x40;								// PORTB data buffer=0x00 
	BSR	_PORTB,6
	.line	783, "main.c"; 	IOSTB  |= 0x01;                         //0 output,1 input 
	IOSTR	_IOSTB
	MOVAR	r0x1048
	BSR	r0x1048,0
	MOVR	r0x1048,W
	IOST	_IOSTB
	.line	784, "main.c"; 	BPHCON &= (char)~(C_PB0_PHB);                  //上拉
	BCR	_BPHCON,0
	.line	785, "main.c"; 	PORTB  |= 0x01;								// PORTB data buffer=0x00 
	BSR	_PORTB,0
	.line	787, "main.c"; 	UPDATE_REG(PORTB);	
	MOVR	_PORTB,F
	.line	789, "main.c"; 	tick_Init();
	MCALL	_tick_Init
	.line	790, "main.c"; 	volume_initial();
	MCALL	_volume_initial
	.line	791, "main.c"; 	digital_initial();
	MCALL	_digital_initial
	.line	792, "main.c"; 	PORTA |= 0x04;                             //开机后，高电平。
	BANKSEL	_PORTA
	BSR	_PORTA,2
	.line	794, "main.c"; 	v3_mode();           //最大档位
	MCALL	_v3_mode
	.line	795, "main.c"; 	UPDATE_REG(PORTB);	
	MOVR	_PORTB,F
	.line	798, "main.c"; 	PCON1 = C_TMR0_Dis;						// Disable Timer0
	CLRA	
	IOST	_PCON1
	.line	799, "main.c"; 	TMR0 = 0;								// Load 0x00 to TMR0 (Initial Timer0 register)
	CLRR	_TMR0
	.line	800, "main.c"; 	T0MD = C_PS0_TMR0 | C_PS0_Div2 ;		// Prescaler0 is assigned to Timer0, Prescaler0 dividing rate = 1:8,clock source is instruction clock
	CLRA	
	T0MD	
	.line	802, "main.c"; 	INTE |= C_INT_TMR0;
	BSR	_INTE,0
	.line	806, "main.c"; 	PCON1 = C_TMR0_En;						// Enable Timer0
	MOVIA	0x01
	IOST	_PCON1
	.line	808, "main.c"; 	TMRH = 0;								// 
	CLRR	_TMRH
	.line	809, "main.c"; 	TMR1 = 0x40;//0xFF;							// Move FFH to TMR1 (Initial Timer1 register) 
	MOVIA	0x40
	SFUN	_TMR1
	.line	811, "main.c"; 	PWM1DUTY = C_PWM_DUTY_25;				// PWM1 Duty = 64/256 = 25%			
	MOVIA	0x10
	SFUN	_PWM1DUTY
	.line	812, "main.c"; 	T1CR1 = C_PWM1_En | C_TMR1_Reload | C_TMR1_En;	// PWM1 output will be present on PB3 , PWM1 output is active high, reloaded from TMR1, non-stop mode 
	MOVIA	0x83
	SFUN	_T1CR1
	.line	813, "main.c"; 	T1CR2 = C_TMR1_ClkSrc_Inst | C_PS1_Div2;	// Enable Prescaler1, Prescaler1 dividing rate = 1:2, Timer1 clock source is instruction clock 
	CLRA	
	SFUN	_T1CR2
	.line	814, "main.c"; 	delay(2);
	MOVIA	0x02
	MOVAR	STK00
	MOVIA	0x00
	MCALL	_delay
	.line	815, "main.c"; 	stop_buzzer();	
	MCALL	_stop_buzzer
	.line	816, "main.c"; 	ENI();	
	ENI
_00407_DS_:
	.line	819, "main.c"; 	if(sleep_flag)
	BANKSEL	_sleep_flag
	MOVR	_sleep_flag,W
	BTRSC	STATUS,2
	MGOTO	_00405_DS_
	.line	821, "main.c"; 	sleep_flag = 0;
	CLRR	_sleep_flag
	.line	822, "main.c"; 	turnOffDevice();
	MCALL	_turnOffDevice
_00405_DS_:
	.line	824, "main.c"; 	led_task();
	MCALL	_led_task
	.line	825, "main.c"; 	key_process_task();
	MCALL	_key_process_task
	.line	826, "main.c"; 	buzzer_task();
	MCALL	_buzzer_task
	.line	827, "main.c"; 	charge_check_Task();
	MCALL	_charge_check_Task
	.line	828, "main.c"; 	digital_process_task();
	MCALL	_digital_process_task
	.line	829, "main.c"; 	CLRWDT();
	clrwdt
	MGOTO	_00407_DS_
	.line	831, "main.c"; 	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _key_initial
;   _delay
;   _delay
;   _tick_Init
;   _volume_initial
;   _digital_initial
;   _delay
;   _stop_buzzer
;   _ModeChoode
;   _key_initial
;   _delay
;   _delay
;   _tick_Init
;   _volume_initial
;   _digital_initial
;   _delay
;   _stop_buzzer
;   _ModeChoode
;2 compiler assigned registers:
;   STK00
;   r0x1047
;; Starting pCode block
.segment "code"; module=main, function=_turnOffDevice
	.debuginfo subprogram _turnOffDevice
_turnOffDevice:
; 2 exit points
	.line	691, "main.c"; 	DISI();
	DISI
	.line	692, "main.c"; 	IOSTB =  0;								// Set PB to output mode
	CLRA	
	IOST	_IOSTB
	.line	693, "main.c"; 	PORTB = 0;                           	// PortB Data Register = 0x00
	CLRR	_PORTB
	.line	694, "main.c"; 	recordDevice = deviceWorkStatus;               //记录当前设备状态。
	BANKSEL	_deviceWorkStatus
	MOVR	_deviceWorkStatus,W
;;2	MOVAR	_recordDevice
	.line	695, "main.c"; 	turnOnFlag = 0;
	BANKSEL	_turnOnFlag
	CLRR	_turnOnFlag
	.line	696, "main.c"; 	Couter = 0;
	BANKSEL	_Couter
	CLRR	_Couter
	.line	697, "main.c"; 	TrunOnDeviceFlag = 0;
	BANKSEL	_TrunOnDeviceFlag
	CLRR	_TrunOnDeviceFlag
	.line	698, "main.c"; 	charge_remove_status = 0;
	BANKSEL	_charge_remove_status
	CLRR	_charge_remove_status
	.line	699, "main.c"; 	charge_plugin_flag = 1;            //重新置1，如同是充电关机
	MOVIA	0x01
	BANKSEL	_charge_plugin_flag
	MOVAR	_charge_plugin_flag
	.line	701, "main.c"; 	IOSTA = 0; 
	CLRA	
	IOST	_IOSTA
	.line	702, "main.c"; 	PORTA = 0;
	BANKSEL	_PORTA
	CLRR	_PORTA
	.line	703, "main.c"; 	IOSTC = 0; 
	CLRA	
	IOST	_IOSTC
	.line	704, "main.c"; 	PORTC = 0;
	CLRR	_PORTC
	.line	705, "main.c"; 	AWUCON = 0;								// Disable PA input change wakeup function
	CLRR	_AWUCON
	.line	706, "main.c"; 	UPDATE_REG(PORTA);
	MOVR	_PORTA,F
	.line	707, "main.c"; 	UPDATE_REG(PORTB);
	MOVR	_PORTB,F
	.line	708, "main.c"; 	PCON &= (char)~(C_WDT_En);
	BCR	_PCON,7
	.line	709, "main.c"; 	key_initial();
	MCALL	_key_initial
	.line	712, "main.c"; 	OSCCR = HALT_MODE;
	MOVIA	0x04
	.debuginfo linetag 1
	SFUN	_OSCCR
	NOP	
	.line	713, "main.c"; 	delay(2);
	MOVIA	0x02
	MOVAR	STK00
	MOVIA	0x00
	MCALL	_delay
	.line	714, "main.c"; 	SLEEP();							// 1. Execute instruction to enters Halt mode (from Normal mode)
	sleep
	.line	715, "main.c"; 	delay(2);
	MOVIA	0x02
	MOVAR	STK00
	MOVIA	0x00
	MCALL	_delay
	.line	716, "main.c"; 	IOSTA = C_PA0_Output;
	CLRA	
	IOST	_IOSTA
	.line	717, "main.c"; 	ABPLCON &= (char)~(C_PA0_PHB);
	BANKSEL	_ABPLCON
	BCR	_ABPLCON,0
	.line	719, "main.c"; 	PORTA &= 0xFE;
	BANKSEL	_PORTA
	BCR	_PORTA,0
	.line	720, "main.c"; 	UPDATE_REG(PORTA);
	MOVR	_PORTA,F
	.line	721, "main.c"; 	PORTB |= 0x20;
	BSR	_PORTB,5
	.line	722, "main.c"; 	IOSTB &= 0xDF;              //led
	IOSTR	_IOSTB
	BANKSEL	r0x1047
	MOVAR	r0x1047
	BCR	r0x1047,5
	MOVR	r0x1047,W
	IOST	_IOSTB
	.line	724, "main.c"; 	BPHCON &= (char)~(C_PB5_PHB);
	BCR	_BPHCON,5
	.line	725, "main.c"; 	PORTB |= 0x20;
	BSR	_PORTB,5
	.line	728, "main.c"; 	IOSTB  |= 0x40;                         //0 output,1 input
	IOSTR	_IOSTB
	MOVAR	r0x1047
	BSR	r0x1047,6
	MOVR	r0x1047,W
	IOST	_IOSTB
	.line	729, "main.c"; 	BPHCON &= (char)~(C_PB6_PHB);                  //上拉
	BCR	_BPHCON,6
	.line	730, "main.c"; 	PORTB  |= 0x40;								// PORTB data buffer=0x00 
	BSR	_PORTB,6
	.line	732, "main.c"; 	IOSTB  |= 0x01;                         //0 output,1 input
	IOSTR	_IOSTB
	MOVAR	r0x1047
	BSR	r0x1047,0
	MOVR	r0x1047,W
	IOST	_IOSTB
	.line	733, "main.c"; 	BPHCON &= (char)~(C_PB0_PHB);                  //上拉
	BCR	_BPHCON,0
	.line	734, "main.c"; 	PORTB  |= 0x01;								// PORTB data buffer=0x00   
	BSR	_PORTB,0
	.line	736, "main.c"; 	tick_Init();
	MCALL	_tick_Init
	.line	737, "main.c"; 	volume_initial();
	MCALL	_volume_initial
	.line	738, "main.c"; 	digital_initial();
	MCALL	_digital_initial
	.line	740, "main.c"; 	UPDATE_REG(PORTB);	
	MOVR	_PORTB,F
	.line	743, "main.c"; 	PCON1 = C_TMR0_Dis;						// Disable Timer0
	CLRA	
	IOST	_PCON1
	.line	744, "main.c"; 	TMR0 = 0;								// Load 0x00 to TMR0 (Initial Timer0 register)
	CLRR	_TMR0
	.line	745, "main.c"; 	T0MD = C_PS0_TMR0 | C_PS0_Div2;		// Prescaler0 is assigned to Timer0, Prescaler0 dividing rate = 1:8,clock source is instruction clock
	CLRA	
	T0MD	
	.line	747, "main.c"; 	INTE |= C_INT_TMR0;
	BSR	_INTE,0
	.line	752, "main.c"; 	PCON1 = C_TMR0_En;						// Enable Timer0
	MOVIA	0x01
	IOST	_PCON1
	.line	754, "main.c"; 	TMRH = 0;								// 
	CLRR	_TMRH
	.line	755, "main.c"; 	TMR1 = 0x40;//0xFF;							// Move FFH to TMR1 (Initial Timer1 register) 
	MOVIA	0x40
	SFUN	_TMR1
	.line	756, "main.c"; 	PWM1DUTY = C_PWM_DUTY_25;				// PWM1 Duty = 64/256 = 25%			
	MOVIA	0x10
	SFUN	_PWM1DUTY
	.line	757, "main.c"; 	T1CR1 = C_PWM1_En | C_TMR1_Reload | C_TMR1_En;	// PWM1 output will be present on PB3 , PWM1 output is active high, reloaded from TMR1, non-stop mode 
	MOVIA	0x83
	SFUN	_T1CR1
	.line	758, "main.c"; 	T1CR2 = C_TMR1_ClkSrc_Inst | C_PS1_Div2;	// Enable Prescaler1, Prescaler1 dividing rate = 1:2, Timer1 clock source is instruction clock 
	CLRA	
	SFUN	_T1CR2
	.line	759, "main.c"; 	delay(2);
	MOVIA	0x02
	MOVAR	STK00
	MOVIA	0x00
	MCALL	_delay
	.line	760, "main.c"; 	stop_buzzer();	
	MCALL	_stop_buzzer
	.line	761, "main.c"; 	ENI();	
	ENI
	.line	762, "main.c"; 	ModeChoode();
	MCALL	_ModeChoode
	.line	763, "main.c"; 	}
	RETURN	
; exit point of _turnOffDevice

;***
;  pBlock Stats: dbName = C
;***
;functions called:
;   _v0_mode
;   _v1_mode
;   _v2_mode
;   _v3_mode
;   _v0_mode
;   _v1_mode
;   _v2_mode
;   _v3_mode
;; Starting pCode block
.segment "code"; module=main, function=_ModeChoode
	.debuginfo subprogram _ModeChoode
_ModeChoode:
; 0 exit points
	.line	669, "main.c"; 	if(deviceWorkStatus == V0_STATE)
	BANKSEL	_deviceWorkStatus
	MOVR	_deviceWorkStatus,W
	XORIA	0x01
	BTRSS	STATUS,2
	MGOTO	_00393_DS_
	.line	671, "main.c"; 	v0_mode();
	MCALL	_v0_mode
	MGOTO	_00395_DS_
_00393_DS_:
	.line	673, "main.c"; 	else if(deviceWorkStatus == V1_STATE)
	BANKSEL	_deviceWorkStatus
	MOVR	_deviceWorkStatus,W
	XORIA	0x02
	BTRSS	STATUS,2
	MGOTO	_00390_DS_
	.line	676, "main.c"; 	v1_mode();
	MCALL	_v1_mode
	MGOTO	_00395_DS_
_00390_DS_:
	.line	678, "main.c"; 	else if(deviceWorkStatus == V2_STATE)
	BANKSEL	_deviceWorkStatus
	MOVR	_deviceWorkStatus,W
	XORIA	0x03
	BTRSS	STATUS,2
	MGOTO	_00387_DS_
	.line	681, "main.c"; 	v2_mode();
	MCALL	_v2_mode
	MGOTO	_00395_DS_
_00387_DS_:
	.line	683, "main.c"; 	else if(deviceWorkStatus == V3_STATE)
	BANKSEL	_deviceWorkStatus
	MOVR	_deviceWorkStatus,W
	XORIA	0x04
	BTRSC	STATUS,2
	.line	686, "main.c"; 	v3_mode();
	MCALL	_v3_mode
_00395_DS_:
	.line	688, "main.c"; 	}
	RETURN	

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;6 compiler assigned registers:
;   r0x1042
;   STK00
;   r0x1043
;   r0x1044
;   r0x1045
;   r0x1046
;; Starting pCode block
.segment "code"; module=main, function=_delay
	.debuginfo subprogram _delay
;local variable name mapping:
	.debuginfo variable _count[0]=r0x1043,enc=signed
	.debuginfo variable _count[1]=r0x1042,enc=signed
	.debuginfo variable _i[0]=r0x1044,enc=signed
	.debuginfo variable _i[1]=r0x1045,enc=signed
_delay:
; 2 exit points
	.line	661, "main.c"; 	void delay(int count)
	BANKSEL	r0x1042
	MOVAR	r0x1042
	MOVR	STK00,W
	BANKSEL	r0x1043
	MOVAR	r0x1043
	.line	664, "main.c"; 	for(i=1;i<=count;i++)
	MOVIA	0x01
	BANKSEL	r0x1044
	MOVAR	r0x1044
	BANKSEL	r0x1045
	CLRR	r0x1045
_00368_DS_:
	BANKSEL	r0x1042
	MOVR	r0x1042,W
	ADDIA	0x80
	BANKSEL	r0x1046
	MOVAR	r0x1046
	BANKSEL	r0x1045
	MOVR	r0x1045,W
	ADDIA	0x80
	BANKSEL	r0x1046
	SUBAR	r0x1046,W
	BTRSS	STATUS,2
	MGOTO	_00379_DS_
	BANKSEL	r0x1044
	MOVR	r0x1044,W
	BANKSEL	r0x1043
	SUBAR	r0x1043,W
_00379_DS_:
	BTRSS	STATUS,0
	MGOTO	_00370_DS_
	BANKSEL	r0x1044
	INCR	r0x1044,F
	BTRSS	STATUS,2
	MGOTO	_00001_DS_
	BANKSEL	r0x1045
	INCR	r0x1045,F
_00001_DS_:
	MGOTO	_00368_DS_
_00370_DS_:
	.line	666, "main.c"; 	}
	RETURN	
; exit point of _delay

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_charge_check_Task
	.debuginfo subprogram _charge_check_Task
;local variable name mapping:
	.debuginfo variable _Waiter8sTimer=_charge_check_Task_Waiter8sTimer_65536_127,enc=unsigned
_charge_check_Task:
; 2 exit points
	.line	638, "main.c"; 	if((charge_remove_status)&&(!turnOnFlag))               //电池移除非手动开机
	BANKSEL	_charge_remove_status
	MOVR	_charge_remove_status,W
	BTRSC	STATUS,2
	MGOTO	_00361_DS_
	BANKSEL	_turnOnFlag
	MOVR	_turnOnFlag,W
	BTRSS	STATUS,2
	MGOTO	_00361_DS_
	.line	640, "main.c"; 	if (Waiter8sTimer == 0)
	BANKSEL	_charge_check_Task_Waiter8sTimer_65536_127
	MOVR	_charge_check_Task_Waiter8sTimer_65536_127,W
	IORAR	(_charge_check_Task_Waiter8sTimer_65536_127 + 1),W
	IORAR	(_charge_check_Task_Waiter8sTimer_65536_127 + 2),W
	IORAR	(_charge_check_Task_Waiter8sTimer_65536_127 + 3),W
	BTRSS	STATUS,2
	MGOTO	_00353_DS_
	.line	642, "main.c"; 	Waiter8sTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BANKSEL	_charge_check_Task_Waiter8sTimer_65536_127
	MOVAR	_charge_check_Task_Waiter8sTimer_65536_127
	CLRR	(_charge_check_Task_Waiter8sTimer_65536_127 + 1)
	CLRR	(_charge_check_Task_Waiter8sTimer_65536_127 + 2)
	CLRR	(_charge_check_Task_Waiter8sTimer_65536_127 + 3)
_00353_DS_:
	.line	644, "main.c"; 	if (tick_GetElapsedTime(Waiter8sTimer) >= TURNON_TIMEOUT)
	BANKSEL	_charge_check_Task_Waiter8sTimer_65536_127
	MOVR	_charge_check_Task_Waiter8sTimer_65536_127,W
	BANKSEL	r0x1042
	MOVAR	r0x1042
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1042
	MOVAR	r0x1042
;;unsigned compare: left < lit (0xAF=175), size=1
	MOVIA	0xaf
	SUBAR	r0x1042,W
	BTRSS	STATUS,0
	MGOTO	_00361_DS_
	.line	646, "main.c"; 	Waiter8sTimer = 0;
	BANKSEL	_charge_check_Task_Waiter8sTimer_65536_127
	CLRR	_charge_check_Task_Waiter8sTimer_65536_127
	CLRR	(_charge_check_Task_Waiter8sTimer_65536_127 + 1)
	CLRR	(_charge_check_Task_Waiter8sTimer_65536_127 + 2)
	CLRR	(_charge_check_Task_Waiter8sTimer_65536_127 + 3)
	.line	647, "main.c"; 	Couter++;
	BANKSEL	_Couter
	INCR	_Couter,F
;;unsigned compare: left < lit (0x2=2), size=1
	.line	648, "main.c"; 	if(Couter>=2)
	MOVIA	0x02
	SUBAR	_Couter,W
	BTRSS	STATUS,0
	MGOTO	_00361_DS_
	.line	650, "main.c"; 	Couter = 0;
	CLRR	_Couter
	.line	651, "main.c"; 	charge_remove_status = 0;
	BANKSEL	_charge_remove_status
	CLRR	_charge_remove_status
	.line	652, "main.c"; 	PORTA |= 0x01;       //PA0
	BANKSEL	_PORTA
	BSR	_PORTA,0
	.line	653, "main.c"; 	PORTA |= 0x04;       //PA2
	BSR	_PORTA,2
	.line	654, "main.c"; 	PORTB &= 0xDF;       //PB5 = 0
	BCR	_PORTB,5
	.line	655, "main.c"; 	TrunOnDeviceFlag = 1;
	MOVIA	0x01
	BANKSEL	_TrunOnDeviceFlag
	MOVAR	_TrunOnDeviceFlag
_00361_DS_:
	.line	660, "main.c"; 	}
	RETURN	
; exit point of _charge_check_Task

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1049
;; Starting pCode block
.segment "code"; module=main, function=_charge_process
	.debuginfo subprogram _charge_process
_charge_process:
; 2 exit points
	.line	598, "main.c"; 	charge_detect = (~PORTB)&0x01;              //获取PB0电平状态
	COMR	_PORTB,W
	BANKSEL	r0x1049
	MOVAR	r0x1049
	BTRSS	r0x1049,0
	MGOTO	_00345_DS_
	.line	601, "main.c"; 	charge_remove_timer = 0;           //清空充电器移开计时器
	BANKSEL	_charge_remove_timer
	CLRR	_charge_remove_timer
	.line	604, "main.c"; 	charge_plugin_flag = 1;
	MOVIA	0x01
	BANKSEL	_charge_plugin_flag
	MOVAR	_charge_plugin_flag
	.line	605, "main.c"; 	charge_remove_flag = 0;
	BANKSEL	_charge_remove_flag
	CLRR	_charge_remove_flag
	.line	606, "main.c"; 	charge_on_status = 1;
	MOVIA	0x01
	BANKSEL	_charge_on_status
	MOVAR	_charge_on_status
	.line	608, "main.c"; 	PORTA &= 0xFE;      //PA0 = 0;
	BANKSEL	_PORTA
	BCR	_PORTA,0
	.line	609, "main.c"; 	PORTB |= 0x20;      //PB5 = 1;
	BSR	_PORTB,5
	.line	610, "main.c"; 	sleep_flag = 1;
	MOVIA	0x01
	BANKSEL	_sleep_flag
	MOVAR	_sleep_flag
	MGOTO	_00347_DS_
_00345_DS_:
	.line	618, "main.c"; 	charge_plugin_timer = 0;
	BANKSEL	_charge_plugin_timer
	CLRR	_charge_plugin_timer
	.line	620, "main.c"; 	if(charge_plugin_flag)
	BANKSEL	_charge_plugin_flag
	MOVR	_charge_plugin_flag,W
	BTRSC	STATUS,2
	MGOTO	_00347_DS_
	.line	622, "main.c"; 	charge_remove_flag = 1;
	MOVIA	0x01
	BANKSEL	_charge_remove_flag
	MOVAR	_charge_remove_flag
	.line	623, "main.c"; 	charge_plugin_flag = 0;
	BANKSEL	_charge_plugin_flag
	CLRR	_charge_plugin_flag
	.line	624, "main.c"; 	charge_remove_status = 1;
	MOVIA	0x01
	BANKSEL	_charge_remove_status
	MOVAR	_charge_remove_status
	.line	627, "main.c"; 	BWUCON = 0;
	CLRR	_BWUCON
	.line	628, "main.c"; 	OSCCR = C_Normal_Mode|C_FHOSC_Sel;
	MOVIA	0x01
	.debuginfo linetag 1
	SFUN	_OSCCR
	NOP	
_00347_DS_:
	.line	632, "main.c"; 	}
	RETURN	
; exit point of _charge_process

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_led_task
	.debuginfo subprogram _led_task
;local variable name mapping:
	.debuginfo variable _TrunOnDeviceTimer=_led_task_TrunOnDeviceTimer_65536_114,enc=unsigned
	.debuginfo variable _TrunOffDeviceTimer=_led_task_TrunOffDeviceTimer_65536_114,enc=unsigned
_led_task:
; 2 exit points
	.line	563, "main.c"; 	if(TrunOnDeviceFlag == 1)
	BANKSEL	_TrunOnDeviceFlag
	MOVR	_TrunOnDeviceFlag,W
	XORIA	0x01
	BTRSS	STATUS,2
	MGOTO	_00330_DS_
	.line	565, "main.c"; 	if (TrunOnDeviceTimer == 0)
	BANKSEL	_led_task_TrunOnDeviceTimer_65536_114
	MOVR	_led_task_TrunOnDeviceTimer_65536_114,W
	IORAR	(_led_task_TrunOnDeviceTimer_65536_114 + 1),W
	IORAR	(_led_task_TrunOnDeviceTimer_65536_114 + 2),W
	IORAR	(_led_task_TrunOnDeviceTimer_65536_114 + 3),W
	BTRSS	STATUS,2
	MGOTO	_00326_DS_
	.line	567, "main.c"; 	PORTB &= 0xDF;
	BCR	_PORTB,5
	.line	568, "main.c"; 	TrunOnDeviceTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BANKSEL	_led_task_TrunOnDeviceTimer_65536_114
	MOVAR	_led_task_TrunOnDeviceTimer_65536_114
	CLRR	(_led_task_TrunOnDeviceTimer_65536_114 + 1)
	CLRR	(_led_task_TrunOnDeviceTimer_65536_114 + 2)
	CLRR	(_led_task_TrunOnDeviceTimer_65536_114 + 3)
_00326_DS_:
	.line	570, "main.c"; 	if (tick_GetElapsedTime(TrunOnDeviceTimer) >= LED_TURNON_TIMEOUT)
	BANKSEL	_led_task_TrunOnDeviceTimer_65536_114
	MOVR	_led_task_TrunOnDeviceTimer_65536_114,W
	BANKSEL	r0x1042
	MOVAR	r0x1042
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1042
	MOVAR	r0x1042
;;unsigned compare: left < lit (0xC8=200), size=1
	MOVIA	0xc8
	SUBAR	r0x1042,W
	BTRSS	STATUS,0
	MGOTO	_00330_DS_
	.line	572, "main.c"; 	TrunOnDeviceTimer = 0;
	BANKSEL	_led_task_TrunOnDeviceTimer_65536_114
	CLRR	_led_task_TrunOnDeviceTimer_65536_114
	CLRR	(_led_task_TrunOnDeviceTimer_65536_114 + 1)
	CLRR	(_led_task_TrunOnDeviceTimer_65536_114 + 2)
	CLRR	(_led_task_TrunOnDeviceTimer_65536_114 + 3)
	.line	573, "main.c"; 	TrunOnDeviceFlag = 0;
	BANKSEL	_TrunOnDeviceFlag
	CLRR	_TrunOnDeviceFlag
	.line	574, "main.c"; 	PORTB |= 0x20;
	BSR	_PORTB,5
	.line	575, "main.c"; 	turnOnOffBuzzer = 1;                             //启动开机蜂鸣器
	MOVIA	0x01
	BANKSEL	_turnOnOffBuzzer
	MOVAR	_turnOnOffBuzzer
_00330_DS_:
	.line	578, "main.c"; 	if(TrunOffDeviceFlag == 1)
	BANKSEL	_TrunOffDeviceFlag
	MOVR	_TrunOffDeviceFlag,W
	XORIA	0x01
	BTRSS	STATUS,2
	MGOTO	_00337_DS_
	.line	580, "main.c"; 	if (TrunOffDeviceTimer == 0)
	BANKSEL	_led_task_TrunOffDeviceTimer_65536_114
	MOVR	_led_task_TrunOffDeviceTimer_65536_114,W
	IORAR	(_led_task_TrunOffDeviceTimer_65536_114 + 1),W
	IORAR	(_led_task_TrunOffDeviceTimer_65536_114 + 2),W
	IORAR	(_led_task_TrunOffDeviceTimer_65536_114 + 3),W
	BTRSS	STATUS,2
	MGOTO	_00332_DS_
	.line	582, "main.c"; 	PORTB &= 0xDF;
	BCR	_PORTB,5
	.line	583, "main.c"; 	TrunOffDeviceTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BANKSEL	_led_task_TrunOffDeviceTimer_65536_114
	MOVAR	_led_task_TrunOffDeviceTimer_65536_114
	CLRR	(_led_task_TrunOffDeviceTimer_65536_114 + 1)
	CLRR	(_led_task_TrunOffDeviceTimer_65536_114 + 2)
	CLRR	(_led_task_TrunOffDeviceTimer_65536_114 + 3)
_00332_DS_:
	.line	585, "main.c"; 	if (tick_GetElapsedTime(TrunOffDeviceTimer) >= LED_TURNOFF_TIMEOUT)
	BANKSEL	_led_task_TrunOffDeviceTimer_65536_114
	MOVR	_led_task_TrunOffDeviceTimer_65536_114,W
	BANKSEL	r0x1042
	MOVAR	r0x1042
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1042
	MOVAR	r0x1042
;;unsigned compare: left < lit (0xC8=200), size=1
	MOVIA	0xc8
	SUBAR	r0x1042,W
	BTRSS	STATUS,0
	MGOTO	_00337_DS_
	.line	587, "main.c"; 	TrunOffDeviceTimer = 0;
	BANKSEL	_led_task_TrunOffDeviceTimer_65536_114
	CLRR	_led_task_TrunOffDeviceTimer_65536_114
	CLRR	(_led_task_TrunOffDeviceTimer_65536_114 + 1)
	CLRR	(_led_task_TrunOffDeviceTimer_65536_114 + 2)
	CLRR	(_led_task_TrunOffDeviceTimer_65536_114 + 3)
	.line	588, "main.c"; 	TrunOffDeviceFlag = 0;
	BANKSEL	_TrunOffDeviceFlag
	CLRR	_TrunOffDeviceFlag
	.line	589, "main.c"; 	PORTB |= 0x20;
	BSR	_PORTB,5
	.line	591, "main.c"; 	sleep_flag = 1;
	MOVIA	0x01
	BANKSEL	_sleep_flag
	MOVAR	_sleep_flag
_00337_DS_:
	.line	594, "main.c"; 	}
	RETURN	
; exit point of _led_task

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _v0_mode
;   _v1_mode
;   _v2_mode
;   _v3_mode
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _v0_mode
;   _v1_mode
;   _v2_mode
;   _v3_mode
;1 compiler assigned register :
;   r0x1043
;; Starting pCode block
.segment "code"; module=main, function=_key_process_task
	.debuginfo subprogram _key_process_task
;local variable name mapping:
	.debuginfo variable _keyProcessTimer=_key_process_task_keyProcessTimer_65536_102,enc=unsigned
	.debuginfo variable _FlashFlag=_key_process_task_FlashFlag_65536_102,enc=unsigned
_key_process_task:
; 2 exit points
	.line	503, "main.c"; 	if (keyProcessTimer == 0)
	BANKSEL	_key_process_task_keyProcessTimer_65536_102
	MOVR	_key_process_task_keyProcessTimer_65536_102,W
	IORAR	(_key_process_task_keyProcessTimer_65536_102 + 1),W
	IORAR	(_key_process_task_keyProcessTimer_65536_102 + 2),W
	IORAR	(_key_process_task_keyProcessTimer_65536_102 + 3),W
	BTRSS	STATUS,2
	MGOTO	_00298_DS_
	.line	505, "main.c"; 	keyProcessTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x1043
	MOVAR	r0x1043
	BANKSEL	_key_process_task_keyProcessTimer_65536_102
	MOVAR	_key_process_task_keyProcessTimer_65536_102
	CLRR	(_key_process_task_keyProcessTimer_65536_102 + 1)
	CLRR	(_key_process_task_keyProcessTimer_65536_102 + 2)
	CLRR	(_key_process_task_keyProcessTimer_65536_102 + 3)
_00298_DS_:
	.line	507, "main.c"; 	if (tick_GetElapsedTime(keyProcessTimer) >= KEY_PROCESS_TIMEOUT)
	BANKSEL	_key_process_task_keyProcessTimer_65536_102
	MOVR	_key_process_task_keyProcessTimer_65536_102,W
	BANKSEL	r0x1043
	MOVAR	r0x1043
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1043
	MOVAR	r0x1043
;;unsigned compare: left < lit (0x2=2), size=1
	MOVIA	0x02
	SUBAR	r0x1043,W
	BTRSS	STATUS,0
	MGOTO	_00320_DS_
	.line	509, "main.c"; 	keyProcessTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x1043
	MOVAR	r0x1043
	BANKSEL	_key_process_task_keyProcessTimer_65536_102
	MOVAR	_key_process_task_keyProcessTimer_65536_102
	CLRR	(_key_process_task_keyProcessTimer_65536_102 + 1)
	CLRR	(_key_process_task_keyProcessTimer_65536_102 + 2)
	CLRR	(_key_process_task_keyProcessTimer_65536_102 + 3)
	.line	510, "main.c"; 	if(key_click)
	BANKSEL	_key_click
	MOVR	_key_click,W
	BTRSC	STATUS,2
	MGOTO	_00302_DS_
	.line	512, "main.c"; 	key_click = 0;
	CLRR	_key_click
	.line	513, "main.c"; 	control_digital = 1;
	MOVIA	0x01
	BANKSEL	_control_digital
	MOVAR	_control_digital
	.line	514, "main.c"; 	FlashFlag = 1;
	BANKSEL	_key_process_task_FlashFlag_65536_102
	MOVAR	_key_process_task_FlashFlag_65536_102
	.line	515, "main.c"; 	deviceWorkStatus++;
	BANKSEL	_deviceWorkStatus
	INCR	_deviceWorkStatus,F
;;swapping arguments (AOP_TYPEs 1/3)
;;unsigned compare: left >= lit (0x5=5), size=1
	.line	516, "main.c"; 	if(deviceWorkStatus > V3_STATE)
	MOVIA	0x05
	SUBAR	_deviceWorkStatus,W
	BTRSS	STATUS,0
	MGOTO	_00302_DS_
	.line	518, "main.c"; 	deviceWorkStatus = V0_STATE;
	MOVIA	0x01
	MOVAR	_deviceWorkStatus
_00302_DS_:
	.line	521, "main.c"; 	if(FlashFlag)
	BANKSEL	_key_process_task_FlashFlag_65536_102
	MOVR	_key_process_task_FlashFlag_65536_102,W
	BTRSC	STATUS,2
	MGOTO	_00315_DS_
	.line	523, "main.c"; 	FlashFlag = 0;
	CLRR	_key_process_task_FlashFlag_65536_102
	.line	524, "main.c"; 	if(deviceWorkStatus == V0_STATE)
	BANKSEL	_deviceWorkStatus
	MOVR	_deviceWorkStatus,W
	XORIA	0x01
	BTRSS	STATUS,2
	MGOTO	_00312_DS_
	.line	526, "main.c"; 	buzzerOneTime = 1;
	MOVIA	0x01
	BANKSEL	_buzzerOneTime
	MOVAR	_buzzerOneTime
	.line	527, "main.c"; 	v0_mode();
	MCALL	_v0_mode
	MGOTO	_00315_DS_
_00312_DS_:
	.line	529, "main.c"; 	else if(deviceWorkStatus == V1_STATE)
	BANKSEL	_deviceWorkStatus
	MOVR	_deviceWorkStatus,W
	XORIA	0x02
	BTRSS	STATUS,2
	MGOTO	_00309_DS_
	.line	531, "main.c"; 	buzzerTwoTime = 1;
	MOVIA	0x01
	BANKSEL	_buzzerTwoTime
	MOVAR	_buzzerTwoTime
	.line	532, "main.c"; 	v1_mode();
	MCALL	_v1_mode
	MGOTO	_00315_DS_
_00309_DS_:
	.line	534, "main.c"; 	else if(deviceWorkStatus == V2_STATE)
	BANKSEL	_deviceWorkStatus
	MOVR	_deviceWorkStatus,W
	XORIA	0x03
	BTRSS	STATUS,2
	MGOTO	_00306_DS_
	.line	536, "main.c"; 	buzzerThreeTime = 1;
	MOVIA	0x01
	BANKSEL	_buzzerThreeTime
	MOVAR	_buzzerThreeTime
	.line	537, "main.c"; 	v2_mode();
	MCALL	_v2_mode
	MGOTO	_00315_DS_
_00306_DS_:
	.line	539, "main.c"; 	else if(deviceWorkStatus == V3_STATE)
	BANKSEL	_deviceWorkStatus
	MOVR	_deviceWorkStatus,W
	XORIA	0x04
	BTRSS	STATUS,2
	MGOTO	_00315_DS_
	.line	541, "main.c"; 	buzzerFourTime = 1;
	MOVIA	0x01
	BANKSEL	_buzzerFourTime
	MOVAR	_buzzerFourTime
	.line	542, "main.c"; 	v3_mode();
	MCALL	_v3_mode
_00315_DS_:
	.line	545, "main.c"; 	if(key_long)
	BANKSEL	_key_long
	MOVR	_key_long,W
	BTRSC	STATUS,2
	MGOTO	_00320_DS_
	.line	547, "main.c"; 	key_long = 0;
	CLRR	_key_long
	.line	548, "main.c"; 	TrunOnDeviceFlag = 0;
	BANKSEL	_TrunOnDeviceFlag
	CLRR	_TrunOnDeviceFlag
	.line	549, "main.c"; 	buzzerOneTime = 0;
	BANKSEL	_buzzerOneTime
	CLRR	_buzzerOneTime
	.line	550, "main.c"; 	buzzerTwoTime = 0;
	BANKSEL	_buzzerTwoTime
	CLRR	_buzzerTwoTime
	.line	551, "main.c"; 	buzzerThreeTime = 0;
	BANKSEL	_buzzerThreeTime
	CLRR	_buzzerThreeTime
	.line	552, "main.c"; 	buzzerFourTime = 0;
	BANKSEL	_buzzerFourTime
	CLRR	_buzzerFourTime
	.line	553, "main.c"; 	TrunOffDeviceFlag = 1;
	MOVIA	0x01
	BANKSEL	_TrunOffDeviceFlag
	MOVAR	_TrunOffDeviceFlag
_00320_DS_:
	.line	556, "main.c"; 	}
	RETURN	
; exit point of _key_process_task

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _start_buzzer
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _stop_buzzer
;   _start_buzzer
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _stop_buzzer
;   _tick_GetTickCount
;   _stop_buzzer
;   _tick_GetElapsedTime
;   _start_buzzer
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _stop_buzzer
;   _tick_GetTickCount
;   _stop_buzzer
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _start_buzzer
;   _tick_GetElapsedTime
;   _stop_buzzer
;   _tick_GetTickCount
;   _stop_buzzer
;   _tick_GetElapsedTime
;   _turnOnDevice_buzzer
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _stop_buzzer
;   _tick_GetTickCount
;   _stop_buzzer
;   _tick_GetElapsedTime
;   _start_buzzer
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _stop_buzzer
;   _start_buzzer
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _stop_buzzer
;   _tick_GetTickCount
;   _stop_buzzer
;   _tick_GetElapsedTime
;   _start_buzzer
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _stop_buzzer
;   _tick_GetTickCount
;   _stop_buzzer
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _start_buzzer
;   _tick_GetElapsedTime
;   _stop_buzzer
;   _tick_GetTickCount
;   _stop_buzzer
;   _tick_GetElapsedTime
;   _turnOnDevice_buzzer
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _stop_buzzer
;   _tick_GetTickCount
;   _stop_buzzer
;   _tick_GetElapsedTime
;1 compiler assigned register :
;   r0x1043
;; Starting pCode block
.segment "code"; module=main, function=_buzzer_task
	.debuginfo subprogram _buzzer_task
;local variable name mapping:
	.debuginfo variable _ucBuzzerOnTimer=_buzzer_task_ucBuzzerOnTimer_65536_65,enc=unsigned
	.debuginfo variable _startBuzzerOffFlag=_buzzer_task_startBuzzerOffFlag_65536_65,enc=unsigned
	.debuginfo variable _ucBuzzerOffTimer=_buzzer_task_ucBuzzerOffTimer_65536_65,enc=unsigned
	.debuginfo variable _BuzzerTimes=_buzzer_task_BuzzerTimes_65536_65,enc=unsigned
_buzzer_task:
; 2 exit points
	.line	333, "main.c"; 	if(buzzerOneTime)
	BANKSEL	_buzzerOneTime
	MOVR	_buzzerOneTime,W
	BTRSC	STATUS,2
	MGOTO	_00231_DS_
	.line	335, "main.c"; 	if (ucBuzzerOnTimer == 0)
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOnTimer_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00227_DS_
	.line	337, "main.c"; 	start_buzzer();
	MCALL	_start_buzzer
	.line	338, "main.c"; 	ucBuzzerOnTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVAR	_buzzer_task_ucBuzzerOnTimer_65536_65
_00227_DS_:
	.line	340, "main.c"; 	if (tick_GetElapsedTime(ucBuzzerOnTimer) >= 4)     //500ms
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOnTimer_65536_65,W
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1043
	MOVAR	r0x1043
;;unsigned compare: left < lit (0x4=4), size=1
	MOVIA	0x04
	SUBAR	r0x1043,W
	BTRSS	STATUS,0
	MGOTO	_00231_DS_
	.line	342, "main.c"; 	ucBuzzerOnTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOnTimer_65536_65
	.line	343, "main.c"; 	buzzerOneTime = 0;
	BANKSEL	_buzzerOneTime
	CLRR	_buzzerOneTime
	.line	344, "main.c"; 	stop_buzzer();
	MCALL	_stop_buzzer
_00231_DS_:
	.line	347, "main.c"; 	if(buzzerTwoTime)
	BANKSEL	_buzzerTwoTime
	MOVR	_buzzerTwoTime,W
	BTRSC	STATUS,2
	MGOTO	_00246_DS_
	.line	349, "main.c"; 	if(!startBuzzerOffFlag)
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	MOVR	_buzzer_task_startBuzzerOffFlag_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00243_DS_
	.line	351, "main.c"; 	if (ucBuzzerOnTimer == 0)
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOnTimer_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00233_DS_
	.line	353, "main.c"; 	start_buzzer();
	MCALL	_start_buzzer
	.line	354, "main.c"; 	ucBuzzerOnTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVAR	_buzzer_task_ucBuzzerOnTimer_65536_65
_00233_DS_:
	.line	356, "main.c"; 	if (tick_GetElapsedTime(ucBuzzerOnTimer) >= 4)     //500ms
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOnTimer_65536_65,W
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1043
	MOVAR	r0x1043
;;unsigned compare: left < lit (0x4=4), size=1
	MOVIA	0x04
	SUBAR	r0x1043,W
	BTRSS	STATUS,0
	MGOTO	_00246_DS_
	.line	358, "main.c"; 	ucBuzzerOnTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOnTimer_65536_65
	.line	359, "main.c"; 	ucBuzzerOffTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	360, "main.c"; 	startBuzzerOffFlag = 1;
	MOVIA	0x01
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	MOVAR	_buzzer_task_startBuzzerOffFlag_65536_65
	.line	361, "main.c"; 	stop_buzzer();
	MCALL	_stop_buzzer
	MGOTO	_00246_DS_
_00243_DS_:
	.line	366, "main.c"; 	if(ucBuzzerOffTimer == 0)
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOffTimer_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00237_DS_
	.line	368, "main.c"; 	ucBuzzerOffTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVAR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	369, "main.c"; 	stop_buzzer();
	MCALL	_stop_buzzer
_00237_DS_:
	.line	371, "main.c"; 	if(tick_GetElapsedTime(ucBuzzerOffTimer) >= 8)
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOffTimer_65536_65,W
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1043
	MOVAR	r0x1043
;;unsigned compare: left < lit (0x8=8), size=1
	MOVIA	0x08
	SUBAR	r0x1043,W
	BTRSS	STATUS,0
	MGOTO	_00246_DS_
	.line	373, "main.c"; 	startBuzzerOffFlag = 0;
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	CLRR	_buzzer_task_startBuzzerOffFlag_65536_65
	.line	374, "main.c"; 	ucBuzzerOffTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	375, "main.c"; 	BuzzerTimes++;
	BANKSEL	_buzzer_task_BuzzerTimes_65536_65
	INCR	_buzzer_task_BuzzerTimes_65536_65,F
;;swapping arguments (AOP_TYPEs 1/3)
;;unsigned compare: left >= lit (0x2=2), size=1
	.line	376, "main.c"; 	if(BuzzerTimes>1)
	MOVIA	0x02
	SUBAR	_buzzer_task_BuzzerTimes_65536_65,W
	BTRSS	STATUS,0
	MGOTO	_00246_DS_
	.line	378, "main.c"; 	BuzzerTimes = 0;
	CLRR	_buzzer_task_BuzzerTimes_65536_65
	.line	379, "main.c"; 	buzzerTwoTime = 0;
	BANKSEL	_buzzerTwoTime
	CLRR	_buzzerTwoTime
_00246_DS_:
	.line	384, "main.c"; 	if(buzzerThreeTime)
	BANKSEL	_buzzerThreeTime
	MOVR	_buzzerThreeTime,W
	BTRSC	STATUS,2
	MGOTO	_00261_DS_
	.line	386, "main.c"; 	if(!startBuzzerOffFlag)
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	MOVR	_buzzer_task_startBuzzerOffFlag_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00258_DS_
	.line	388, "main.c"; 	if (ucBuzzerOnTimer == 0)
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOnTimer_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00248_DS_
	.line	390, "main.c"; 	start_buzzer();
	MCALL	_start_buzzer
	.line	391, "main.c"; 	ucBuzzerOnTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVAR	_buzzer_task_ucBuzzerOnTimer_65536_65
_00248_DS_:
	.line	393, "main.c"; 	if (tick_GetElapsedTime(ucBuzzerOnTimer) >= 4)     //500ms
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOnTimer_65536_65,W
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1043
	MOVAR	r0x1043
;;unsigned compare: left < lit (0x4=4), size=1
	MOVIA	0x04
	SUBAR	r0x1043,W
	BTRSS	STATUS,0
	MGOTO	_00261_DS_
	.line	395, "main.c"; 	ucBuzzerOnTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOnTimer_65536_65
	.line	396, "main.c"; 	ucBuzzerOffTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	397, "main.c"; 	startBuzzerOffFlag = 1;
	MOVIA	0x01
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	MOVAR	_buzzer_task_startBuzzerOffFlag_65536_65
	.line	398, "main.c"; 	stop_buzzer();
	MCALL	_stop_buzzer
	MGOTO	_00261_DS_
_00258_DS_:
	.line	403, "main.c"; 	if(ucBuzzerOffTimer == 0)
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOffTimer_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00252_DS_
	.line	405, "main.c"; 	ucBuzzerOffTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVAR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	406, "main.c"; 	stop_buzzer();
	MCALL	_stop_buzzer
_00252_DS_:
	.line	408, "main.c"; 	if(tick_GetElapsedTime(ucBuzzerOffTimer) >= 8)
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOffTimer_65536_65,W
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1043
	MOVAR	r0x1043
;;unsigned compare: left < lit (0x8=8), size=1
	MOVIA	0x08
	SUBAR	r0x1043,W
	BTRSS	STATUS,0
	MGOTO	_00261_DS_
	.line	410, "main.c"; 	startBuzzerOffFlag = 0;
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	CLRR	_buzzer_task_startBuzzerOffFlag_65536_65
	.line	411, "main.c"; 	ucBuzzerOffTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	412, "main.c"; 	BuzzerTimes++;
	BANKSEL	_buzzer_task_BuzzerTimes_65536_65
	INCR	_buzzer_task_BuzzerTimes_65536_65,F
;;swapping arguments (AOP_TYPEs 1/3)
;;unsigned compare: left >= lit (0x3=3), size=1
	.line	413, "main.c"; 	if(BuzzerTimes>2)
	MOVIA	0x03
	SUBAR	_buzzer_task_BuzzerTimes_65536_65,W
	BTRSS	STATUS,0
	MGOTO	_00261_DS_
	.line	415, "main.c"; 	BuzzerTimes = 0;
	CLRR	_buzzer_task_BuzzerTimes_65536_65
	.line	416, "main.c"; 	buzzerThreeTime = 0;
	BANKSEL	_buzzerThreeTime
	CLRR	_buzzerThreeTime
_00261_DS_:
	.line	422, "main.c"; 	if(buzzerFourTime)
	BANKSEL	_buzzerFourTime
	MOVR	_buzzerFourTime,W
	BTRSC	STATUS,2
	MGOTO	_00276_DS_
	.line	424, "main.c"; 	if(!startBuzzerOffFlag)
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	MOVR	_buzzer_task_startBuzzerOffFlag_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00273_DS_
	.line	426, "main.c"; 	if (ucBuzzerOnTimer == 0)
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOnTimer_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00263_DS_
	.line	428, "main.c"; 	ucBuzzerOnTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVAR	_buzzer_task_ucBuzzerOnTimer_65536_65
	.line	429, "main.c"; 	start_buzzer();
	MCALL	_start_buzzer
_00263_DS_:
	.line	431, "main.c"; 	if (tick_GetElapsedTime(ucBuzzerOnTimer) >= 4)     //500ms
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOnTimer_65536_65,W
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1043
	MOVAR	r0x1043
;;unsigned compare: left < lit (0x4=4), size=1
	MOVIA	0x04
	SUBAR	r0x1043,W
	BTRSS	STATUS,0
	MGOTO	_00276_DS_
	.line	433, "main.c"; 	ucBuzzerOnTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOnTimer_65536_65
	.line	434, "main.c"; 	ucBuzzerOffTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	435, "main.c"; 	startBuzzerOffFlag = 1;
	MOVIA	0x01
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	MOVAR	_buzzer_task_startBuzzerOffFlag_65536_65
	.line	436, "main.c"; 	stop_buzzer();
	MCALL	_stop_buzzer
	MGOTO	_00276_DS_
_00273_DS_:
	.line	441, "main.c"; 	if(ucBuzzerOffTimer == 0)
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOffTimer_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00267_DS_
	.line	443, "main.c"; 	ucBuzzerOffTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVAR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	444, "main.c"; 	stop_buzzer();
	MCALL	_stop_buzzer
_00267_DS_:
	.line	446, "main.c"; 	if(tick_GetElapsedTime(ucBuzzerOffTimer) >= 8)
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOffTimer_65536_65,W
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1043
	MOVAR	r0x1043
;;unsigned compare: left < lit (0x8=8), size=1
	MOVIA	0x08
	SUBAR	r0x1043,W
	BTRSS	STATUS,0
	MGOTO	_00276_DS_
	.line	448, "main.c"; 	startBuzzerOffFlag = 0;
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	CLRR	_buzzer_task_startBuzzerOffFlag_65536_65
	.line	449, "main.c"; 	ucBuzzerOffTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	450, "main.c"; 	BuzzerTimes++;
	BANKSEL	_buzzer_task_BuzzerTimes_65536_65
	INCR	_buzzer_task_BuzzerTimes_65536_65,F
;;swapping arguments (AOP_TYPEs 1/3)
;;unsigned compare: left >= lit (0x4=4), size=1
	.line	451, "main.c"; 	if(BuzzerTimes>3)
	MOVIA	0x04
	SUBAR	_buzzer_task_BuzzerTimes_65536_65,W
	BTRSS	STATUS,0
	MGOTO	_00276_DS_
	.line	453, "main.c"; 	BuzzerTimes = 0;
	CLRR	_buzzer_task_BuzzerTimes_65536_65
	.line	454, "main.c"; 	buzzerFourTime = 0;
	BANKSEL	_buzzerFourTime
	CLRR	_buzzerFourTime
_00276_DS_:
	.line	459, "main.c"; 	if(turnOnOffBuzzer)
	BANKSEL	_turnOnOffBuzzer
	MOVR	_turnOnOffBuzzer,W
	BTRSC	STATUS,2
	MGOTO	_00292_DS_
	.line	461, "main.c"; 	if(!startBuzzerOffFlag)
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	MOVR	_buzzer_task_startBuzzerOffFlag_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00288_DS_
	.line	463, "main.c"; 	if (ucBuzzerOnTimer == 0)
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOnTimer_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00278_DS_
	.line	465, "main.c"; 	turnOnDevice_buzzer();
	MCALL	_turnOnDevice_buzzer
	.line	466, "main.c"; 	ucBuzzerOnTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVAR	_buzzer_task_ucBuzzerOnTimer_65536_65
_00278_DS_:
	.line	468, "main.c"; 	if (tick_GetElapsedTime(ucBuzzerOnTimer) >= 3)     //30ms
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOnTimer_65536_65,W
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1043
	MOVAR	r0x1043
;;unsigned compare: left < lit (0x3=3), size=1
	MOVIA	0x03
	SUBAR	r0x1043,W
	BTRSS	STATUS,0
	MGOTO	_00292_DS_
	.line	470, "main.c"; 	ucBuzzerOnTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOnTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOnTimer_65536_65
	.line	471, "main.c"; 	ucBuzzerOffTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	472, "main.c"; 	startBuzzerOffFlag = 1;
	MOVIA	0x01
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	MOVAR	_buzzer_task_startBuzzerOffFlag_65536_65
	.line	473, "main.c"; 	stop_buzzer();
	MCALL	_stop_buzzer
	MGOTO	_00292_DS_
_00288_DS_:
	.line	478, "main.c"; 	if(ucBuzzerOffTimer == 0)
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOffTimer_65536_65,W
	BTRSS	STATUS,2
	MGOTO	_00282_DS_
	.line	480, "main.c"; 	ucBuzzerOffTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVAR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	481, "main.c"; 	stop_buzzer();
	MCALL	_stop_buzzer
_00282_DS_:
	.line	483, "main.c"; 	if(tick_GetElapsedTime(ucBuzzerOffTimer) >= 4)    //60ms
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	MOVR	_buzzer_task_ucBuzzerOffTimer_65536_65,W
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1043
	MOVAR	r0x1043
;;unsigned compare: left < lit (0x4=4), size=1
	MOVIA	0x04
	SUBAR	r0x1043,W
	BTRSS	STATUS,0
	MGOTO	_00292_DS_
	.line	485, "main.c"; 	startBuzzerOffFlag = 0;
	BANKSEL	_buzzer_task_startBuzzerOffFlag_65536_65
	CLRR	_buzzer_task_startBuzzerOffFlag_65536_65
	.line	486, "main.c"; 	ucBuzzerOffTimer = 0;
	BANKSEL	_buzzer_task_ucBuzzerOffTimer_65536_65
	CLRR	_buzzer_task_ucBuzzerOffTimer_65536_65
	.line	487, "main.c"; 	BuzzerTimes++;
	BANKSEL	_buzzer_task_BuzzerTimes_65536_65
	INCR	_buzzer_task_BuzzerTimes_65536_65,F
;;swapping arguments (AOP_TYPEs 1/3)
;;unsigned compare: left >= lit (0x2=2), size=1
	.line	488, "main.c"; 	if(BuzzerTimes>1)
	MOVIA	0x02
	SUBAR	_buzzer_task_BuzzerTimes_65536_65,W
	BTRSS	STATUS,0
	MGOTO	_00292_DS_
	.line	490, "main.c"; 	BuzzerTimes = 0;
	CLRR	_buzzer_task_BuzzerTimes_65536_65
	.line	491, "main.c"; 	turnOnOffBuzzer = 0;
	BANKSEL	_turnOnOffBuzzer
	CLRR	_turnOnOffBuzzer
	.line	492, "main.c"; 	wakeupFlag = 0;
	BANKSEL	_wakeupFlag
	CLRR	_wakeupFlag
_00292_DS_:
	.line	498, "main.c"; 	}
	RETURN	
; exit point of _buzzer_task

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
.segment "code"; module=main, function=_turnOnDevice_buzzer
	.debuginfo subprogram _turnOnDevice_buzzer
_turnOnDevice_buzzer:
; 2 exit points
	.line	321, "main.c"; 	TMRH = 0;								// 
	CLRR	_TMRH
	.line	322, "main.c"; 	TMR1 = 0x7F;//0xFF;							// Move FFH to TMR1 (Initial Timer1 register) 
	MOVIA	0x7f
	SFUN	_TMR1
	.line	323, "main.c"; 	PWM1DUTY = 0x04;//C_PWM_DUTY_25;				// PWM1 Duty = 64/256 = 25%			
	MOVIA	0x04
	SFUN	_PWM1DUTY
	.line	324, "main.c"; 	T1CR1 = C_PWM1_En | C_TMR1_Reload | C_TMR1_En;	// PWM1 output will be present on PB3 , PWM1 output is active high, reloaded from TMR1, non-stop mode 
	MOVIA	0x83
	SFUN	_T1CR1
	.line	325, "main.c"; 	T1CR2 = C_TMR1_ClkSrc_Inst | C_PS1_Div2;	// Enable Prescaler1, Prescaler1 dividing rate = 1:2, Timer1 clock source is instruction clock 
	CLRA	
	SFUN	_T1CR2
	.line	326, "main.c"; 	}
	RETURN	
; exit point of _turnOnDevice_buzzer

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
.segment "code"; module=main, function=_start_buzzer
	.debuginfo subprogram _start_buzzer
_start_buzzer:
; 2 exit points
	.line	310, "main.c"; 	TMRH = 0;								// 
	CLRR	_TMRH
	.line	311, "main.c"; 	TMR1 = 0x40;//0xFF;							// Move FFH to TMR1 (Initial Timer1 register) 
	MOVIA	0x40
	SFUN	_TMR1
	.line	312, "main.c"; 	PWM1DUTY = C_PWM_DUTY_25;				// PWM1 Duty = 64/256 = 25%			
	MOVIA	0x10
	SFUN	_PWM1DUTY
	.line	313, "main.c"; 	T1CR1 = C_PWM1_En | C_TMR1_Reload | C_TMR1_En;	// PWM1 output will be present on PB3 , PWM1 output is active high, reloaded from TMR1, non-stop mode 
	MOVIA	0x83
	SFUN	_T1CR1
	.line	314, "main.c"; 	T1CR2 = C_TMR1_ClkSrc_Inst | C_PS1_Div2;	// Enable Prescaler1, Prescaler1 dividing rate = 1:2, Timer1 clock source is instruction clock 
	CLRA	
	SFUN	_T1CR2
	.line	316, "main.c"; 	}
	RETURN	
; exit point of _start_buzzer

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_stop_buzzer
	.debuginfo subprogram _stop_buzzer
_stop_buzzer:
; 2 exit points
	.line	300, "main.c"; 	TMR1 = 0;
	CLRA	
	SFUN	_TMR1
	.line	301, "main.c"; 	PWM1DUTY = 0;
	SFUN	_PWM1DUTY
	.line	302, "main.c"; 	T1CR1 = 0;
	SFUN	_T1CR1
	.line	303, "main.c"; 	T1CR2 = 0;
	CLRA	
	SFUN	_T1CR2
	.line	305, "main.c"; 	IOSTB |= 0x08;   //  PB3配置成输入。
	IOSTR	_IOSTB
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BSR	r0x1042,3
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	306, "main.c"; 	}
	RETURN	
; exit point of _stop_buzzer

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_v0_mode
	.debuginfo subprogram _v0_mode
_v0_mode:
; 2 exit points
	.line	289, "main.c"; 	IOSTA |= 0x80;            //PA7   V1
	IOSTR	_IOSTA
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BSR	r0x1042,7
	MOVR	r0x1042,W
	IOST	_IOSTA
	.line	290, "main.c"; 	UPDATE_REG(PORTA);	
	MOVR	_PORTA,F
	.line	291, "main.c"; 	IOSTB |= 0x86;
	IOSTR	_IOSTB
	BANKSEL	r0x1042
	MOVAR	r0x1042
	MOVIA	0x86
	IORAR	r0x1042,F
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	293, "main.c"; 	IOSTB &= 0x7F;                  //设置PB7为输出
	IOSTR	_IOSTB
	MOVAR	r0x1042
	BCR	r0x1042,7
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	294, "main.c"; 	PORTB &= 0x7F;                  //设置PB7输出低电平
	BCR	_PORTB,7
	.line	295, "main.c"; 	UPDATE_REG(PORTB);	
	MOVR	_PORTB,F
	.line	296, "main.c"; 	}
	RETURN	
; exit point of _v0_mode

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_v3_mode
	.debuginfo subprogram _v3_mode
_v3_mode:
; 2 exit points
	.line	280, "main.c"; 	IOSTA |= 0x80;            //PA7   V1
	IOSTR	_IOSTA
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BSR	r0x1042,7
	MOVR	r0x1042,W
	IOST	_IOSTA
	.line	281, "main.c"; 	UPDATE_REG(PORTA);
	MOVR	_PORTA,F
	.line	282, "main.c"; 	IOSTB |= 0x86;
	IOSTR	_IOSTB
	BANKSEL	r0x1042
	MOVAR	r0x1042
	MOVIA	0x86
	IORAR	r0x1042,F
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	283, "main.c"; 	IOSTB &= 0xFB;                  //设置PB2为输出
	IOSTR	_IOSTB
	MOVAR	r0x1042
	BCR	r0x1042,2
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	284, "main.c"; 	PORTB &= 0xFB;                  //设置PB2输出低电平
	BCR	_PORTB,2
	.line	285, "main.c"; 	UPDATE_REG(PORTB);	
	MOVR	_PORTB,F
	.line	286, "main.c"; 	}
	RETURN	
; exit point of _v3_mode

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_v2_mode
	.debuginfo subprogram _v2_mode
_v2_mode:
; 2 exit points
	.line	271, "main.c"; 	IOSTA |= 0x80;            //PA7   V1
	IOSTR	_IOSTA
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BSR	r0x1042,7
	MOVR	r0x1042,W
	IOST	_IOSTA
	.line	272, "main.c"; 	UPDATE_REG(PORTA);	
	MOVR	_PORTA,F
	.line	273, "main.c"; 	IOSTB |= 0x86;
	IOSTR	_IOSTB
	BANKSEL	r0x1042
	MOVAR	r0x1042
	MOVIA	0x86
	IORAR	r0x1042,F
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	274, "main.c"; 	IOSTB &= 0xFD;                  //设置PB1为输出
	IOSTR	_IOSTB
	MOVAR	r0x1042
	BCR	r0x1042,1
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	275, "main.c"; 	PORTB &= 0xFD;                  //设置PB1输出低电平
	BCR	_PORTB,1
	.line	276, "main.c"; 	UPDATE_REG(PORTB);	
	MOVR	_PORTB,F
	.line	277, "main.c"; 	}
	RETURN	
; exit point of _v2_mode

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_v1_mode
	.debuginfo subprogram _v1_mode
_v1_mode:
; 2 exit points
	.line	261, "main.c"; 	IOSTA |= 0x80;            //PA7   V1
	IOSTR	_IOSTA
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BSR	r0x1042,7
	MOVR	r0x1042,W
	IOST	_IOSTA
	.line	262, "main.c"; 	IOSTA &= 0x7F;            //PA7   V1 PA7为输出
	IOSTR	_IOSTA
	MOVAR	r0x1042
	BCR	r0x1042,7
	MOVR	r0x1042,W
	IOST	_IOSTA
	.line	263, "main.c"; 	PORTA &= 0x7F;                  //设置PA7输出低电平
	BANKSEL	_PORTA
	BCR	_PORTA,7
	.line	264, "main.c"; 	UPDATE_REG(PORTA);	
	MOVR	_PORTA,F
	.line	265, "main.c"; 	IOSTB |= 0x86;
	IOSTR	_IOSTB
	BANKSEL	r0x1042
	MOVAR	r0x1042
	MOVIA	0x86
	IORAR	r0x1042,F
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	266, "main.c"; 	UPDATE_REG(PORTB);	
	MOVR	_PORTB,F
	.line	267, "main.c"; 	}
	RETURN	
; exit point of _v1_mode

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _charge_process
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _charge_process
;1 compiler assigned register :
;   r0x104A
;; Starting pCode block
.segment "code"; module=main, function=_charger_scan_task
	.debuginfo subprogram _charger_scan_task
;local variable name mapping:
	.debuginfo variable _ChargerScanTimer=_charger_scan_task_ChargerScanTimer_65536_47,enc=unsigned
_charger_scan_task:
; 2 exit points
	.line	248, "main.c"; 	if (ChargerScanTimer == 0)
	BANKSEL	_charger_scan_task_ChargerScanTimer_65536_47
	MOVR	_charger_scan_task_ChargerScanTimer_65536_47,W
	IORAR	(_charger_scan_task_ChargerScanTimer_65536_47 + 1),W
	IORAR	(_charger_scan_task_ChargerScanTimer_65536_47 + 2),W
	IORAR	(_charger_scan_task_ChargerScanTimer_65536_47 + 3),W
	BTRSS	STATUS,2
	MGOTO	_00190_DS_
	.line	250, "main.c"; 	ChargerScanTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x104A
	MOVAR	r0x104A
	BANKSEL	_charger_scan_task_ChargerScanTimer_65536_47
	MOVAR	_charger_scan_task_ChargerScanTimer_65536_47
	CLRR	(_charger_scan_task_ChargerScanTimer_65536_47 + 1)
	CLRR	(_charger_scan_task_ChargerScanTimer_65536_47 + 2)
	CLRR	(_charger_scan_task_ChargerScanTimer_65536_47 + 3)
_00190_DS_:
	.line	252, "main.c"; 	if (tick_GetElapsedTime(ChargerScanTimer) >= 1)
	BANKSEL	_charger_scan_task_ChargerScanTimer_65536_47
	MOVR	_charger_scan_task_ChargerScanTimer_65536_47,W
	BANKSEL	r0x104A
	MOVAR	r0x104A
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x104A
	MOVAR	r0x104A
;;unsigned compare: left < lit (0x1=1), size=1
	MOVIA	0x01
	SUBAR	r0x104A,W
	BTRSS	STATUS,0
	MGOTO	_00193_DS_
	.line	254, "main.c"; 	ChargerScanTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x104A
	MOVAR	r0x104A
	BANKSEL	_charger_scan_task_ChargerScanTimer_65536_47
	MOVAR	_charger_scan_task_ChargerScanTimer_65536_47
	CLRR	(_charger_scan_task_ChargerScanTimer_65536_47 + 1)
	CLRR	(_charger_scan_task_ChargerScanTimer_65536_47 + 2)
	CLRR	(_charger_scan_task_ChargerScanTimer_65536_47 + 3)
	.line	255, "main.c"; 	charge_process();
	MCALL	_charge_process
_00193_DS_:
	.line	257, "main.c"; 	}
	RETURN	
; exit point of _charger_scan_task

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _key_process
;   _tick_GetTickCount
;   _tick_GetElapsedTime
;   _tick_GetTickCount
;   _key_process
;1 compiler assigned register :
;   r0x104C
;; Starting pCode block
.segment "code"; module=main, function=_key_scan_task
	.debuginfo subprogram _key_scan_task
;local variable name mapping:
	.debuginfo variable _keyScanTimer=_key_scan_task_keyScanTimer_65536_43,enc=unsigned
_key_scan_task:
; 2 exit points
	.line	235, "main.c"; 	if (keyScanTimer == 0)
	BANKSEL	_key_scan_task_keyScanTimer_65536_43
	MOVR	_key_scan_task_keyScanTimer_65536_43,W
	IORAR	(_key_scan_task_keyScanTimer_65536_43 + 1),W
	IORAR	(_key_scan_task_keyScanTimer_65536_43 + 2),W
	IORAR	(_key_scan_task_keyScanTimer_65536_43 + 3),W
	BTRSS	STATUS,2
	MGOTO	_00181_DS_
	.line	237, "main.c"; 	keyScanTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x104C
	MOVAR	r0x104C
	BANKSEL	_key_scan_task_keyScanTimer_65536_43
	MOVAR	_key_scan_task_keyScanTimer_65536_43
	CLRR	(_key_scan_task_keyScanTimer_65536_43 + 1)
	CLRR	(_key_scan_task_keyScanTimer_65536_43 + 2)
	CLRR	(_key_scan_task_keyScanTimer_65536_43 + 3)
_00181_DS_:
	.line	239, "main.c"; 	if (tick_GetElapsedTime(keyScanTimer) >= 1)
	BANKSEL	_key_scan_task_keyScanTimer_65536_43
	MOVR	_key_scan_task_keyScanTimer_65536_43,W
	BANKSEL	r0x104C
	MOVAR	r0x104C
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x104C
	MOVAR	r0x104C
;;unsigned compare: left < lit (0x1=1), size=1
	MOVIA	0x01
	SUBAR	r0x104C,W
	BTRSS	STATUS,0
	MGOTO	_00184_DS_
	.line	241, "main.c"; 	keyScanTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x104C
	MOVAR	r0x104C
	BANKSEL	_key_scan_task_keyScanTimer_65536_43
	MOVAR	_key_scan_task_keyScanTimer_65536_43
	CLRR	(_key_scan_task_keyScanTimer_65536_43 + 1)
	CLRR	(_key_scan_task_keyScanTimer_65536_43 + 2)
	CLRR	(_key_scan_task_keyScanTimer_65536_43 + 3)
	.line	242, "main.c"; 	key_process();
	MCALL	_key_process
_00184_DS_:
	.line	244, "main.c"; 	}
	RETURN	
; exit point of _key_scan_task

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _tick_GetTickCount
;   _Open_digital
;   _tick_GetElapsedTime
;   _Close_digital
;   _tick_GetTickCount
;   _Open_digital
;   _tick_GetElapsedTime
;   _Close_digital
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_digital_process_task
	.debuginfo subprogram _digital_process_task
;local variable name mapping:
	.debuginfo variable _digitalProcessTimer=_digital_process_task_digitalProcessTimer_65536_38,enc=unsigned
_digital_process_task:
; 2 exit points
	.line	217, "main.c"; 	if(control_digital)
	BANKSEL	_control_digital
	MOVR	_control_digital,W
	BTRSC	STATUS,2
	MGOTO	_00175_DS_
	.line	219, "main.c"; 	if (digitalProcessTimer == 0)
	BANKSEL	_digital_process_task_digitalProcessTimer_65536_38
	MOVR	_digital_process_task_digitalProcessTimer_65536_38,W
	IORAR	(_digital_process_task_digitalProcessTimer_65536_38 + 1),W
	IORAR	(_digital_process_task_digitalProcessTimer_65536_38 + 2),W
	IORAR	(_digital_process_task_digitalProcessTimer_65536_38 + 3),W
	BTRSS	STATUS,2
	MGOTO	_00170_DS_
	.line	221, "main.c"; 	digitalProcessTimer = tick_GetTickCount();
	MCALL	_tick_GetTickCount
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BANKSEL	_digital_process_task_digitalProcessTimer_65536_38
	MOVAR	_digital_process_task_digitalProcessTimer_65536_38
	CLRR	(_digital_process_task_digitalProcessTimer_65536_38 + 1)
	CLRR	(_digital_process_task_digitalProcessTimer_65536_38 + 2)
	CLRR	(_digital_process_task_digitalProcessTimer_65536_38 + 3)
	.line	222, "main.c"; 	Open_digital();
	MCALL	_Open_digital
_00170_DS_:
	.line	224, "main.c"; 	if (tick_GetElapsedTime(digitalProcessTimer) >= 20)        //100ms
	BANKSEL	_digital_process_task_digitalProcessTimer_65536_38
	MOVR	_digital_process_task_digitalProcessTimer_65536_38,W
	BANKSEL	r0x1042
	MOVAR	r0x1042
	MCALL	_tick_GetElapsedTime
	BANKSEL	r0x1042
	MOVAR	r0x1042
;;unsigned compare: left < lit (0x14=20), size=1
	MOVIA	0x14
	SUBAR	r0x1042,W
	BTRSS	STATUS,0
	MGOTO	_00175_DS_
	.line	226, "main.c"; 	digitalProcessTimer = 0;
	BANKSEL	_digital_process_task_digitalProcessTimer_65536_38
	CLRR	_digital_process_task_digitalProcessTimer_65536_38
	CLRR	(_digital_process_task_digitalProcessTimer_65536_38 + 1)
	CLRR	(_digital_process_task_digitalProcessTimer_65536_38 + 2)
	CLRR	(_digital_process_task_digitalProcessTimer_65536_38 + 3)
	.line	227, "main.c"; 	control_digital = 0;
	BANKSEL	_control_digital
	CLRR	_control_digital
	.line	228, "main.c"; 	Close_digital();
	MCALL	_Close_digital
_00175_DS_:
	.line	231, "main.c"; 	}
	RETURN	
; exit point of _digital_process_task

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
.segment "code"; module=main, function=_Close_digital
	.debuginfo subprogram _Close_digital
_Close_digital:
; 2 exit points
	.line	212, "main.c"; 	PORTA |= 0x04;
	BANKSEL	_PORTA
	BSR	_PORTA,2
	.line	213, "main.c"; 	}
	RETURN	
; exit point of _Close_digital

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
.segment "code"; module=main, function=_Open_digital
	.debuginfo subprogram _Open_digital
_Open_digital:
; 2 exit points
	.line	208, "main.c"; 	PORTA &= 0xFB;
	BANKSEL	_PORTA
	BCR	_PORTA,2
	.line	209, "main.c"; 	}
	RETURN	
; exit point of _Open_digital

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_digital_initial
	.debuginfo subprogram _digital_initial
_digital_initial:
; 2 exit points
	.line	201, "main.c"; 	IOSTA &= 0xFB;
	IOSTR	_IOSTA
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BCR	r0x1042,2
	MOVR	r0x1042,W
	IOST	_IOSTA
	.line	202, "main.c"; 	BPHCON &= (char)~C_PA2_PHB;
	BCR	_BPHCON,2
	.line	203, "main.c"; 	PORTA &= 0xFB;
	BANKSEL	_PORTA
	BCR	_PORTA,2
	.line	204, "main.c"; 	UPDATE_REG(PORTA);	
	MOVR	_PORTA,F
	.line	205, "main.c"; 	}
	RETURN	
; exit point of _digital_initial

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_volume_initial
	.debuginfo subprogram _volume_initial
_volume_initial:
; 2 exit points
	.line	194, "main.c"; 	IOSTA |= 0x80;
	IOSTR	_IOSTA
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BSR	r0x1042,7
	MOVR	r0x1042,W
	IOST	_IOSTA
	.line	195, "main.c"; 	UPDATE_REG(PORTA);	
	MOVR	_PORTA,F
	.line	196, "main.c"; 	IOSTB |= 0x86;
	IOSTR	_IOSTB
	BANKSEL	r0x1042
	MOVAR	r0x1042
	MOVIA	0x86
	IORAR	r0x1042,F
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	197, "main.c"; 	UPDATE_REG(PORTB);	
	MOVR	_PORTB,F
	.line	198, "main.c"; 	}
	RETURN	
; exit point of _volume_initial

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1042
;; Starting pCode block
.segment "code"; module=main, function=_key_initial
	.debuginfo subprogram _key_initial
_key_initial:
; 2 exit points
	.line	167, "main.c"; 	BPHCON &= (char)~C_PB6_PHB;
	BCR	_BPHCON,6
	.line	168, "main.c"; 	BWUCON |= C_PB6_Wakeup;					// Enable PB6 input change wakeup function
	BSR	_BWUCON,6
	.line	169, "main.c"; 	IOSTB  |= C_PB6_Input;					// Set PB6 to input mode,others set to output mode   置高为输入
	IOSTR	_IOSTB
	BANKSEL	r0x1042
	MOVAR	r0x1042
	BSR	r0x1042,6
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	170, "main.c"; 	PORTB  |= 0x40;								// PORTB data buffer=0x00 
	BSR	_PORTB,6
	.line	173, "main.c"; 	BPHCON &= (char)~C_PB0_PHB;             //1,上拉
	BCR	_BPHCON,0
	.line	176, "main.c"; 	IOSTB  |= C_PB0_Input;					// Set PB4 to input mode,others set to output mode   置高为输入
	IOSTR	_IOSTB
	MOVAR	r0x1042
	BSR	r0x1042,0
	MOVR	r0x1042,W
	IOST	_IOSTB
	.line	177, "main.c"; 	PORTB  |= 0x01;								// PORTB data buffer=0x00 	
	BSR	_PORTB,0
	.line	179, "main.c"; 	UPDATE_REG(PORTB);	
	MOVR	_PORTB,F
	.line	181, "main.c"; 	INTEDG = C_INT0_En | C_INT0_RisingEdge;//C_INT0_FallingEdge;		// External interrupt 0 will be set while rising edge occurs on pin PB4
	MOVIA	0x11
	MOVAR	_INTEDG
	.line	182, "main.c"; 	INTE = C_INT_EXT0 | C_INT_PABKey;					// Enable PortB input change interrupt
	MOVIA	0x06
	MOVAR	_INTE
	.line	184, "main.c"; 	INTF = 0;								// Clear all interrupt flags
	CLRR	_INTF
	.line	187, "main.c"; 	OSCCR = C_FLOSC_Sel;					// OSCCR[0]=0 , FOSC is Low-frequency oscillation (FLOSC)
	CLRA	
	.debuginfo linetag 1
	SFUN	_OSCCR
	NOP	
	.line	190, "main.c"; 	OSCCR = C_FHOSC_Sel;					// OSCCR[0]=1 , FOSC is high-frequency oscillation (FHOSC)
	MOVIA	0x01
	.debuginfo linetag 1
	SFUN	_OSCCR
	NOP	
	.line	191, "main.c"; 	}
	RETURN	
; exit point of _key_initial

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x104B
;; Starting pCode block
.segment "code"; module=main, function=_key_process
	.debuginfo subprogram _key_process
_key_process:
; 2 exit points
	.line	133, "main.c"; 	key_press = (~PORTB)&0x40;
	COMR	_PORTB,W
	BANKSEL	r0x104B
	MOVAR	r0x104B
	BTRSS	r0x104B,6
	MGOTO	_00142_DS_
	.line	136, "main.c"; 	key_release_timer = 0;           //清空按键松开计时器
	BANKSEL	_key_release_timer
	CLRR	_key_release_timer
	.line	137, "main.c"; 	if((++key_press_timer>=4)&&(key_release_flag))        //如果按下超过40ms,
	BANKSEL	_key_press_timer
	INCR	_key_press_timer,F
	BTRSC	STATUS,2
	INCR	(_key_press_timer + 1),F
;;unsigned compare: left < lit (0x4=4), size=2
	MOVIA	0x04
	SUBAR	_key_press_timer,W
	MOVIA	0x00
	SBCAR	(_key_press_timer + 1),W
	BTRSS	STATUS,0
	MGOTO	_00144_DS_
	BANKSEL	_key_release_flag
	MOVR	_key_release_flag,W
	BTRSC	STATUS,2
	MGOTO	_00144_DS_
	.line	139, "main.c"; 	key_press_flag = 1;
	MOVIA	0x01
	BANKSEL	_key_press_flag
	MOVAR	_key_press_flag
	.line	140, "main.c"; 	key_long_flag = 0;
	BANKSEL	_key_long_flag
	CLRR	_key_long_flag
;;swapping arguments (AOP_TYPEs 1/3)
;;unsigned compare: left >= lit (0x97=151), size=2
	.line	141, "main.c"; 	if((key_press_timer > 150)&&(!key_long))    //如果时间超过3000ms
	MOVIA	0x97
	BANKSEL	_key_press_timer
	SUBAR	_key_press_timer,W
	MOVIA	0x00
	SBCAR	(_key_press_timer + 1),W
	BTRSS	STATUS,0
	MGOTO	_00144_DS_
	BANKSEL	_key_long
	MOVR	_key_long,W
	BTRSS	STATUS,2
	MGOTO	_00144_DS_
	.line	143, "main.c"; 	key_long = 1;
	MOVIA	0x01
	MOVAR	_key_long
	.line	144, "main.c"; 	key_long_flag = 1;
	BANKSEL	_key_long_flag
	MOVAR	_key_long_flag
	.line	145, "main.c"; 	key_release_flag = 0;
	BANKSEL	_key_release_flag
	CLRR	_key_release_flag
	MGOTO	_00144_DS_
_00142_DS_:
	.line	151, "main.c"; 	key_press_timer = 0;
	BANKSEL	_key_press_timer
	CLRR	_key_press_timer
	CLRR	(_key_press_timer + 1)
	.line	152, "main.c"; 	if((++key_release_timer>=4)&&(key_press_flag))
	BANKSEL	_key_release_timer
	INCR	_key_release_timer,F
;;unsigned compare: left < lit (0x4=4), size=1
	MOVIA	0x04
	SUBAR	_key_release_timer,W
	BTRSS	STATUS,0
	MGOTO	_00144_DS_
	BANKSEL	_key_press_flag
	MOVR	_key_press_flag,W
	BTRSC	STATUS,2
	MGOTO	_00144_DS_
	.line	154, "main.c"; 	key_release_flag = 1;
	MOVIA	0x01
	BANKSEL	_key_release_flag
	MOVAR	_key_release_flag
	.line	155, "main.c"; 	key_press_flag = 0;
	BANKSEL	_key_press_flag
	CLRR	_key_press_flag
	.line	156, "main.c"; 	if((!key_long_flag)&&(!wakeupFlag))
	BANKSEL	_key_long_flag
	MOVR	_key_long_flag,W
	BTRSS	STATUS,2
	MGOTO	_00144_DS_
	BANKSEL	_wakeupFlag
	MOVR	_wakeupFlag,W
	BTRSS	STATUS,2
	MGOTO	_00144_DS_
	.line	158, "main.c"; 	key_click = 1;
	MOVIA	0x01
	BANKSEL	_key_click
	MOVAR	_key_click
_00144_DS_:
	.line	162, "main.c"; 	}
	RETURN	
; exit point of _key_process


;	code size estimation:
;	 1047+  276 =  1323 instructions ( 3198 byte)

	end
