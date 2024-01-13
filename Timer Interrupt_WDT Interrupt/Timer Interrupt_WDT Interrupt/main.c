/* =========================================================================
 *  Project:       Timer Interrupt & WDT Interrupt
 *  File:          main.c
 *  Description:   Set Timer0、Timer1、WDT Interrupt
 *                 1. FINST = 4MHz/4T(I_HRC) 
 *                 2. Timer0 overflow interrupt flag bit will be set every 2048us then toggle PB0 output state  (2048us = 1us*256*8)
 *                 3. Timer1 underflow interrupt flag bit will be set every 1024us then toggle PB1 output state  (1024us = 1us*256*4)
 *                 4. WDT timeout interrupt flag bit will be set every 3.5ms then toggle PB2 output state
 *                    4.1  Set project options : WDT Time Base = 3.5ms
 * 
 *  Author:        JasonLee
 *  Version:       V1.0
 *  Date:          2019/10/01
 =========================================================================*/
#include <ny8.h>
#include "ny8_constant.h"
#include "apptick.h"

#define UPDATE_REG(x)	__asm__("MOVR _" #x ",F")
unsigned char timer = 0;
#define C_PWM_DUTY_25  0x10//0x40
#define LED_TURNON_TIMEOUT      200                //2000ms
#define LED_TURNOFF_TIMEOUT     200                //2000ms
#define TURNON_TIMEOUT          175                //2400ms
#define key_state_0     0   //按键的初始状态 
#define key_state_1     1   //按键的按下状态
#define key_state_2     2   //按键释放状态/或长按状态
#define key_state_3     3   //按键长按后释放状态
#define KEY_PROCESS_TIMEOUT     2                   //20ms
#define CHARGE_CHECK_TIMEOUT    2                   //20ms
#define V0_STATE     1
#define V1_STATE     2
#define V2_STATE     3
#define V3_STATE     4
#define TURN_OFF     4
#define HALT_MODE		0x04
#define FHOSC_SEL		0x01
#define STANDBY_MODE	0x08

static unsigned char TrunOnDeviceFlag = 0;
static unsigned char TrunOffDeviceFlag = 0;
void key_process(void);
unsigned char get_key_status = 0;
static unsigned char buzzerOneTime = 0;
static unsigned char buzzerTwoTime = 0;
static unsigned char buzzerThreeTime = 0;
static unsigned char buzzerFourTime = 0;
static unsigned char deviceWorkStatus = 0;
static unsigned char recordDevice = 0;
static unsigned char wakeupFlag = 0;
static unsigned char turnOnOffBuzzer = 0;
unsigned char turnOnFlag = 0;
unsigned char key_press_flag = 0;
unsigned int key_press_timer = 0;
volatile unsigned char key_click = 0;
volatile unsigned char control_digital = 0;
volatile unsigned char key_long = 0;
unsigned char key_release_flag = 1;
unsigned char key_release_timer = 0;
volatile unsigned char key_long_flag = 0;
volatile unsigned char sleep_flag = 0;
unsigned char charge_remove_timer = 0;
unsigned char charge_plugin_timer = 0;
unsigned char charge_remove_flag = 0;             //初始状态是无充电器
unsigned char charge_plugin_flag = 1;
unsigned char charge_on_status = 0;
unsigned char charge_remove_status = 0;
unsigned char weekup_by_int0 = 0;
static unsigned char Couter = 0;

void turnOffDevice(void);
void delay(int count);
void charge_process();
//! interrupt service routine
void isr(void) __interrupt(0)
{
	unsigned int x = 0;
	if(INTFbits.INT0IF)
	{ 
		INTF = (char)~(C_INT_EXT0);			// Clear INT0IF(External interrupt 0 flag bit)		
		weekup_by_int0 = 1;
	}
	if(INTFbits.PABIF)
	{
    	INTFbits.PABIF = 0;					// Clear PABIF(PortB input change interrupt flag bit)
    	while((~PORTB)&0x40)
        {
        	x++;
        	PORTA &= 0xFB;
        	PORTB |= 0x20;
        	wakeupFlag = 1;
        	CLRWDT();
        	delay(10);
        	if((x>40)&&(!TrunOnDeviceFlag))   //长按按键开机
        	{   
        		PORTA |= 0x01;
        		PORTA |= 0x04;
				PORTB &= 0xDF;
    			BWUCON = 0;
    			OSCCR = C_Normal_Mode|C_FHOSC_Sel;
				TrunOnDeviceFlag = 1;
				turnOnFlag = 1;
				INTFbits.PABIF = 0;
				return;
        	}
        }
        if((x<=40)&&(!turnOnFlag))
        {	
        	sleep_flag = 1;
        }        
    }
	if(INTFbits.T0IF)
	{ 
		INTF= (char)~(C_INT_TMR0);			// Clear T0IF flag bit	
		timer++;
		if(timer>5)
		{
			tick_Event();                              //10 ms 一次。
			key_process();
			charge_process();
			timer = 0;
		}
	}
	if(INTFbits.WDTIF)
	{ 
		INTF= (char)~(C_INT_WDT);			// Clear WDTIF flag bit		
	}	
}

void key_process(void)
{
	unsigned char key_press;   //按键是否被按下
	key_press = (~PORTB)&0x40;
	if(key_press)                          //判断如果有按键按下
	{
		key_release_timer = 0;           //清空按键松开计时器
		if((++key_press_timer>=4)&&(key_release_flag))        //如果按下超过40ms,
		{
			key_press_flag = 1;
			key_long_flag = 0;
			if((key_press_timer > 150)&&(!key_long))    //如果时间超过3000ms
			{
				key_long = 1;
				key_long_flag = 1;
				key_release_flag = 0;
			}
		}
	}
	else                                   //按键松开
	{
		key_press_timer = 0;
		if((++key_release_timer>=4)&&(key_press_flag))
		{
			key_release_flag = 1;
			key_press_flag = 0;
			if((!key_long_flag)&&(!wakeupFlag))
			{
				key_click = 1;
			}
		}
	}
}

void key_initial(void)
{							// PORTB data buffer=0x00 
	//DISI();
    BPHCON &= (char)~C_PB6_PHB;
    BWUCON |= C_PB6_Wakeup;					// Enable PB6 input change wakeup function
    IOSTB  |= C_PB6_Input;					// Set PB6 to input mode,others set to output mode   置高为输入
    PORTB  |= 0x40;								// PORTB data buffer=0x00 
    
    //BPHCON |= C_PB0_PHB;                   //去掉上拉，
    BPHCON &= (char)~C_PB0_PHB;             //1,上拉
    //ABPLCON &= 0xEF;                         //2,下拉
    //BODCON |= 0x01;                         //3,开漏
    IOSTB  |= C_PB0_Input;					// Set PB4 to input mode,others set to output mode   置高为输入
    PORTB  |= 0x01;								// PORTB data buffer=0x00 	

	UPDATE_REG(PORTB);	
    // Initial Interrupt Setting
    INTEDG = C_INT0_En | C_INT0_RisingEdge;//C_INT0_FallingEdge;		// External interrupt 0 will be set while rising edge occurs on pin PB4
    INTE = C_INT_EXT0 | C_INT_PABKey;					// Enable PortB input change interrupt
    //INTE = C_INT_PABKey;					// Enable PortB input change interrupt
    INTF = 0;								// Clear all interrupt flags

    // Normal mode into Slow mode
    OSCCR = C_FLOSC_Sel;					// OSCCR[0]=0 , FOSC is Low-frequency oscillation (FLOSC)

    // Slow mode into Normal mode
    OSCCR = C_FHOSC_Sel;					// OSCCR[0]=1 , FOSC is high-frequency oscillation (FHOSC)
}
void volume_initial(void)
{
	IOSTA |= 0x80;
	UPDATE_REG(PORTA);	
	IOSTB |= 0x86;
	UPDATE_REG(PORTB);	
}
void digital_initial(void)
{
	IOSTA &= 0xFB;
	BPHCON &= (char)~C_PA2_PHB;
	PORTA &= 0xFB;
	UPDATE_REG(PORTA);	
}
void Open_digital(void)
{
	PORTA &= 0xFB;
}
void Close_digital(void)
{
	PORTA |= 0x04;
}
void digital_process_task(void)
{
	static unsigned long digitalProcessTimer = 0;
	if(control_digital)
	{
		if (digitalProcessTimer == 0)
    	{
    		digitalProcessTimer = tick_GetTickCount();
    		Open_digital();
    	}
    	if (tick_GetElapsedTime(digitalProcessTimer) >= 20)        //100ms
    	{
        	digitalProcessTimer = 0;
			control_digital = 0;
			Close_digital();
        }
    }
}
void key_scan_task(void)
{
	static unsigned long keyScanTimer = 0;
	if (keyScanTimer == 0)
    {
    	keyScanTimer = tick_GetTickCount();
    }
    if (tick_GetElapsedTime(keyScanTimer) >= 1)
    {
        keyScanTimer = tick_GetTickCount();
        key_process();
    }
}
void charger_scan_task(void)
{
	static unsigned long ChargerScanTimer = 0;
	if (ChargerScanTimer == 0)
    {
    	ChargerScanTimer = tick_GetTickCount();
    }
    if (tick_GetElapsedTime(ChargerScanTimer) >= 1)
    {
        ChargerScanTimer = tick_GetTickCount();
        charge_process();
    }
}
void v1_mode(void)
{
	//IOSTB |= 0x8B;                  //先设为输入
	IOSTA |= 0x80;            //PA7   V1
	IOSTA &= 0x7F;            //PA7   V1 PA7为输出
	PORTA &= 0x7F;                  //设置PA7输出低电平
	UPDATE_REG(PORTA);	
	IOSTB |= 0x86;
	UPDATE_REG(PORTB);	
}
void v2_mode(void)
{
	//IOSTB |= 0x8B;                  //先设为输入
	IOSTA |= 0x80;            //PA7   V1
	UPDATE_REG(PORTA);	
	IOSTB |= 0x86;
	IOSTB &= 0xFD;                  //设置PB1为输出
	PORTB &= 0xFD;                  //设置PB1输出低电平
	UPDATE_REG(PORTB);	
}
void v3_mode(void)
{
	IOSTA |= 0x80;            //PA7   V1
	UPDATE_REG(PORTA);
	IOSTB |= 0x86;
	IOSTB &= 0xFB;                  //设置PB2为输出
	PORTB &= 0xFB;                  //设置PB2输出低电平
	UPDATE_REG(PORTB);	
}
void v0_mode(void)
{
	IOSTA |= 0x80;            //PA7   V1
	UPDATE_REG(PORTA);	
	IOSTB |= 0x86;
	//IOSTB |= 0x8B;                  //先设为输入
	IOSTB &= 0x7F;                  //设置PB7为输出
	PORTB &= 0x7F;                  //设置PB7输出低电平
	UPDATE_REG(PORTB);	
}
void stop_buzzer(void)
{
	//BZ2CR =(char) ~(C_BZ2_En| C_BZ2_TMR2B2);
	TMR1 = 0;
	PWM1DUTY = 0;
	T1CR1 = 0;
	T1CR2 = 0;
	
	IOSTB |= 0x08;   //  PB3配置成输入。
}
void start_buzzer(void)
{
// ;initial Timer 1/2 & PWM1 control register
	TMRH = 0;								// 
	TMR1 = 0x40;//0xFF;							// Move FFH to TMR1 (Initial Timer1 register) 
	PWM1DUTY = C_PWM_DUTY_25;				// PWM1 Duty = 64/256 = 25%			
	T1CR1 = C_PWM1_En | C_TMR1_Reload | C_TMR1_En;	// PWM1 output will be present on PB3 , PWM1 output is active high, reloaded from TMR1, non-stop mode 
	T1CR2 = C_TMR1_ClkSrc_Inst | C_PS1_Div2;	// Enable Prescaler1, Prescaler1 dividing rate = 1:2, Timer1 clock source is instruction clock 
	
}

void turnOnDevice_buzzer(void)
{
// ;initial Timer 1/2 & PWM1 control register
	TMRH = 0;								// 
	TMR1 = 0x7F;//0xFF;							// Move FFH to TMR1 (Initial Timer1 register) 
	PWM1DUTY = 0x04;//C_PWM_DUTY_25;				// PWM1 Duty = 64/256 = 25%			
	T1CR1 = C_PWM1_En | C_TMR1_Reload | C_TMR1_En;	// PWM1 output will be present on PB3 , PWM1 output is active high, reloaded from TMR1, non-stop mode 
	T1CR2 = C_TMR1_ClkSrc_Inst | C_PS1_Div2;	// Enable Prescaler1, Prescaler1 dividing rate = 1:2, Timer1 clock source is instruction clock 
}
void buzzer_task(void)
{
	static unsigned char ucBuzzerOnTimer = 0;
	static unsigned char ucBuzzerOffTimer = 0;
	static unsigned char BuzzerTimes = 0;
	static unsigned char startBuzzerOffFlag = 0;
	if(buzzerOneTime)
	{
		if (ucBuzzerOnTimer == 0)
    	{
    		start_buzzer();
    		ucBuzzerOnTimer = tick_GetTickCount();
    	}
    	if (tick_GetElapsedTime(ucBuzzerOnTimer) >= 4)     //500ms
    	{
    		ucBuzzerOnTimer = 0;
    		buzzerOneTime = 0;
    		stop_buzzer();
		}
	}
	if(buzzerTwoTime)
	{
		if(!startBuzzerOffFlag)
		{
			if (ucBuzzerOnTimer == 0)
    		{
    			start_buzzer();
    			ucBuzzerOnTimer = tick_GetTickCount();
    		}
    		if (tick_GetElapsedTime(ucBuzzerOnTimer) >= 4)     //500ms
    		{
    			ucBuzzerOnTimer = 0;
    			ucBuzzerOffTimer = 0;
    			startBuzzerOffFlag = 1;
    			stop_buzzer();
			}
		}
		else
		{
			if(ucBuzzerOffTimer == 0)
			{
				ucBuzzerOffTimer = tick_GetTickCount();
				stop_buzzer();
			}
			if(tick_GetElapsedTime(ucBuzzerOffTimer) >= 8)
			{
				startBuzzerOffFlag = 0;
				ucBuzzerOffTimer = 0;
				BuzzerTimes++;
				if(BuzzerTimes>1)
				{
					BuzzerTimes = 0;
					buzzerTwoTime = 0;
				}
			}
		}
	}
	if(buzzerThreeTime)
	{
		if(!startBuzzerOffFlag)
		{
			if (ucBuzzerOnTimer == 0)
    		{
    			start_buzzer();
    			ucBuzzerOnTimer = tick_GetTickCount();
    		}
    		if (tick_GetElapsedTime(ucBuzzerOnTimer) >= 4)     //500ms
    		{
    			ucBuzzerOnTimer = 0;
    			ucBuzzerOffTimer = 0;
    			startBuzzerOffFlag = 1;
    			stop_buzzer();
			}
		}
		else
		{
			if(ucBuzzerOffTimer == 0)
			{
				ucBuzzerOffTimer = tick_GetTickCount();
				stop_buzzer();
			}
			if(tick_GetElapsedTime(ucBuzzerOffTimer) >= 8)
			{
				startBuzzerOffFlag = 0;
				ucBuzzerOffTimer = 0;
				BuzzerTimes++;
				if(BuzzerTimes>2)
				{
					BuzzerTimes = 0;
					buzzerThreeTime = 0;
				}
			}
		}
		
	}
	if(buzzerFourTime)
	{
		if(!startBuzzerOffFlag)
		{
			if (ucBuzzerOnTimer == 0)
    		{
    			ucBuzzerOnTimer = tick_GetTickCount();
    			start_buzzer();
    		}
    		if (tick_GetElapsedTime(ucBuzzerOnTimer) >= 4)     //500ms
    		{
    			ucBuzzerOnTimer = 0;
    			ucBuzzerOffTimer = 0;
    			startBuzzerOffFlag = 1;
    			stop_buzzer();
			}
		}
		else
		{
			if(ucBuzzerOffTimer == 0)
			{
				ucBuzzerOffTimer = tick_GetTickCount();
				stop_buzzer();
			}
			if(tick_GetElapsedTime(ucBuzzerOffTimer) >= 8)
			{
				startBuzzerOffFlag = 0;
				ucBuzzerOffTimer = 0;
				BuzzerTimes++;
				if(BuzzerTimes>3)
				{
					BuzzerTimes = 0;
					buzzerFourTime = 0;
				}
			}
		}
	}
	if(turnOnOffBuzzer)
	{
		if(!startBuzzerOffFlag)
		{
			if (ucBuzzerOnTimer == 0)
    		{
    			turnOnDevice_buzzer();
    			ucBuzzerOnTimer = tick_GetTickCount();
    		}
    		if (tick_GetElapsedTime(ucBuzzerOnTimer) >= 3)     //30ms
    		{
    			ucBuzzerOnTimer = 0;
    			ucBuzzerOffTimer = 0;
    			startBuzzerOffFlag = 1;
    			stop_buzzer();
			}
		}
		else
		{
			if(ucBuzzerOffTimer == 0)
			{
				ucBuzzerOffTimer = tick_GetTickCount();
				stop_buzzer();
			}
			if(tick_GetElapsedTime(ucBuzzerOffTimer) >= 4)    //60ms
			{
				startBuzzerOffFlag = 0;
				ucBuzzerOffTimer = 0;
				BuzzerTimes++;
				if(BuzzerTimes>1)
				{
					BuzzerTimes = 0;
					turnOnOffBuzzer = 0;
					wakeupFlag = 0;
					//deviceWorkStatus = recordDevice;    //打开记录
				}
			}
		}
	}
}
void key_process_task(void)
{
	static unsigned long keyProcessTimer = 0;
	static unsigned char FlashFlag = 0;
	if (keyProcessTimer == 0)
    {
    	keyProcessTimer = tick_GetTickCount();
    }
    if (tick_GetElapsedTime(keyProcessTimer) >= KEY_PROCESS_TIMEOUT)
    {
        keyProcessTimer = tick_GetTickCount();
        if(key_click)
        {
        	key_click = 0;
        	control_digital = 1;
        	FlashFlag = 1;
        	deviceWorkStatus++;
        	if(deviceWorkStatus > V3_STATE)
        	{
        		deviceWorkStatus = V0_STATE;
        	}   
        }
        if(FlashFlag)
        {
        	FlashFlag = 0;
        	if(deviceWorkStatus == V0_STATE)
        	{
        		buzzerOneTime = 1;
        		v0_mode();
        	}
        	else if(deviceWorkStatus == V1_STATE)
        	{
        		buzzerTwoTime = 1;
        		v1_mode();
        	}
        	else if(deviceWorkStatus == V2_STATE)
        	{
        		buzzerThreeTime = 1;
        		v2_mode();
        	}
        	else if(deviceWorkStatus == V3_STATE)
        	{
        		buzzerFourTime = 1;
        		v3_mode();
        	}     	
        }
        if(key_long)
        {
        	key_long = 0;
        	TrunOnDeviceFlag = 0;
        	buzzerOneTime = 0;
        	buzzerTwoTime = 0;
        	buzzerThreeTime = 0;
        	buzzerFourTime = 0;
        	TrunOffDeviceFlag = 1;
        }
    }
}
static unsigned long TrunOffDeviceTimer = 0;
void led_task(void)
{
	static unsigned long TrunOnDeviceTimer = 0;
	static unsigned long TrunOffDeviceTimer = 0;
	static unsigned char flag = 0;
	if(TrunOnDeviceFlag == 1)
	{
		if (TrunOnDeviceTimer == 0)
    	{
    		PORTB &= 0xDF;
    		TrunOnDeviceTimer = tick_GetTickCount();
    	}
    	if (tick_GetElapsedTime(TrunOnDeviceTimer) >= LED_TURNON_TIMEOUT)
    	{
        	TrunOnDeviceTimer = 0;
        	TrunOnDeviceFlag = 0;
        	PORTB |= 0x20;
        	turnOnOffBuzzer = 1;                             //启动开机蜂鸣器
		}
	}
	if(TrunOffDeviceFlag == 1)
	{
		if (TrunOffDeviceTimer == 0)
    	{
    		PORTB &= 0xDF;
    		TrunOffDeviceTimer = tick_GetTickCount();
    	}
    	if (tick_GetElapsedTime(TrunOffDeviceTimer) >= LED_TURNOFF_TIMEOUT)
    	{
        	TrunOffDeviceTimer = 0;
        	TrunOffDeviceFlag = 0;
        	PORTB |= 0x20;
        	//turnOffDevice();        	//长按3S之后，灯亮2S，亮完之后，开始进入休眠。
        	sleep_flag = 1;
		}
	}
}
void charge_process(void)
{
	unsigned char charge_detect;   //按键是否被按下
	charge_detect = (~PORTB)&0x01;              //获取PB0电平状态
	if(charge_detect)                          //判断如果是低电平，有充电器接入
	{
		charge_remove_timer = 0;           //清空充电器移开计时器
		//if(charge_remove_flag)
		{
			charge_plugin_flag = 1;
			charge_remove_flag = 0;
			charge_on_status = 1;
			
			PORTA &= 0xFE;      //PA0 = 0;
       		PORTB |= 0x20;      //PB5 = 1;
			sleep_flag = 1;
	//		Couter = 0;
	//		TrunOnDeviceFlag = 0;
	//		charge_remove_status = 0;
		}
	}
	else                                   //无充电器
	{
		charge_plugin_timer = 0;

		if(charge_plugin_flag)
		{
			charge_remove_flag = 1;
			charge_plugin_flag = 0;
			charge_remove_status = 1;
			

    		BWUCON = 0;
    		OSCCR = C_Normal_Mode|C_FHOSC_Sel;
		}
		
	}
}

void charge_check_Task()
{
	static unsigned long Waiter8sTimer = 0;
	
	if((charge_remove_status)&&(!turnOnFlag))               //电池移除非手动开机
	{
		if (Waiter8sTimer == 0)
    	{
    		Waiter8sTimer = tick_GetTickCount();
    	}
    	if (tick_GetElapsedTime(Waiter8sTimer) >= TURNON_TIMEOUT)
    	{
        	Waiter8sTimer = 0;
        	Couter++;
            if(Couter>=2)
            {
            	Couter = 0;
            	charge_remove_status = 0;
				PORTA |= 0x01;       //PA0
        		PORTA |= 0x04;       //PA2
				PORTB &= 0xDF;       //PB5 = 0
				TrunOnDeviceFlag = 1;
				//turnOnFlag = 1;
			}
		}
	}
}
void delay(int count)
{
	int i;
	for(i=1;i<=count;i++)
	;
}
void ModeChoode(void)
{
    if(deviceWorkStatus == V0_STATE)
    {
    	v0_mode();
    }
    else if(deviceWorkStatus == V1_STATE)
    {
        //buzzerTwoTime = 1;
        v1_mode();
    }
    else if(deviceWorkStatus == V2_STATE)
    {
        //buzzerThreeTime = 1;
        v2_mode();
    }
    else if(deviceWorkStatus == V3_STATE)
    {
        //buzzerFourTime = 1;
        v3_mode();
    }
}
void turnOffDevice(void)
{
	DISI();
	IOSTB =  0;								// Set PB to output mode
    PORTB = 0;                           	// PortB Data Register = 0x00
	recordDevice = deviceWorkStatus;               //记录当前设备状态。
	turnOnFlag = 0;
	Couter = 0;
	TrunOnDeviceFlag = 0;
	charge_remove_status = 0;
	charge_plugin_flag = 1;            //重新置1，如同是充电关机
	
	IOSTA = 0; 
	PORTA = 0;
	IOSTC = 0; 
	PORTC = 0;
	AWUCON = 0;								// Disable PA input change wakeup function
	UPDATE_REG(PORTA);
	UPDATE_REG(PORTB);
	PCON &= (char)~(C_WDT_En);
	key_initial();

    // A. Normal mode into Halt mode. While PB1 input change then wakeup and set PB2 outputs low
    OSCCR = HALT_MODE;
    delay(2);
    SLEEP();							// 1. Execute instruction to enters Halt mode (from Normal mode)
    delay(2);
	IOSTA = C_PA0_Output;
	ABPLCON &= (char)~(C_PA0_PHB);
	//PORTA |= 0x01;
	PORTA &= 0xFE;
	UPDATE_REG(PORTA);
	PORTB |= 0x20;
    IOSTB &= 0xDF;              //led
	//BODCON = C_PB5_OD;
	BPHCON &= (char)~(C_PB5_PHB);
	PORTB |= 0x20;
    
   	/*****************************************/
	IOSTB  |= 0x40;                         //0 output,1 input
	BPHCON &= (char)~(C_PB6_PHB);                  //上拉
    PORTB  |= 0x40;								// PORTB data buffer=0x00 
    
  	IOSTB  |= 0x01;                         //0 output,1 input
	BPHCON &= (char)~(C_PB0_PHB);                  //上拉
    PORTB  |= 0x01;								// PORTB data buffer=0x00   
    /*******************************************/
	tick_Init();
	volume_initial();
	digital_initial();
//	charge_check_initial();
	UPDATE_REG(PORTB);	
	//v0_mode();
//;Initial Timer0
	PCON1 = C_TMR0_Dis;						// Disable Timer0
	TMR0 = 0;								// Load 0x00 to TMR0 (Initial Timer0 register)
	T0MD = C_PS0_TMR0 | C_PS0_Div2;		// Prescaler0 is assigned to Timer0, Prescaler0 dividing rate = 1:8,clock source is instruction clock
//;Setting Interrupt Enable Register	
	INTE |= C_INT_TMR0;
//;Initial Power control register
	//PCON = C_WDT_En | C_LVR_En;				// Enable WDT ,  Enable LVR
	
//;Enable Timer0 & Global interrupt bit 
	PCON1 = C_TMR0_En;						// Enable Timer0
// ;initial Timer 1/2 & PWM1 control register
	TMRH = 0;								// 
	TMR1 = 0x40;//0xFF;							// Move FFH to TMR1 (Initial Timer1 register) 
	PWM1DUTY = C_PWM_DUTY_25;				// PWM1 Duty = 64/256 = 25%			
	T1CR1 = C_PWM1_En | C_TMR1_Reload | C_TMR1_En;	// PWM1 output will be present on PB3 , PWM1 output is active high, reloaded from TMR1, non-stop mode 
	T1CR2 = C_TMR1_ClkSrc_Inst | C_PS1_Div2;	// Enable Prescaler1, Prescaler1 dividing rate = 1:2, Timer1 clock source is instruction clock 
	delay(2);
	stop_buzzer();	
	ENI();	
	ModeChoode();
}
void main(void)
{
	IOSTB =  0;								// Set PB to output mode
    PORTB = 0;                           	// PortB Data Register = 0x00
//unsigned char R_shift_regl = 0xFF;
	IOSTA = C_PA0_Output;
	//IOSTA &= 0xFE;                       //0 output,1 input
	ABPLCON &= (char)~(C_PA0_PHB);
	UPDATE_REG(PORTA);	
	PORTA |= 0x01;
	
    IOSTB &= 0xDF;                       //0 output,1 input
	BODCON = C_PB5_OD;
	PORTB |= 0x20;
	/*****************************************/
	IOSTB  |= 0x40;                         //0 output,1 input 
	BPHCON &= (char)~(C_PB6_PHB);                  //上拉
    PORTB  |= 0x40;								// PORTB data buffer=0x00 
  
  	IOSTB  |= 0x01;                         //0 output,1 input 
	BPHCON &= (char)~(C_PB0_PHB);                  //上拉
    PORTB  |= 0x01;								// PORTB data buffer=0x00 
    
    UPDATE_REG(PORTB);	
    /*******************************************/
	tick_Init();
	volume_initial();
	digital_initial();
	PORTA |= 0x04;                             //开机后，高电平。
//	charge_check_initial();
	v3_mode();           //最大档位
	UPDATE_REG(PORTB);	
//;Initial Timer0
    
	PCON1 = C_TMR0_Dis;						// Disable Timer0
	TMR0 = 0;								// Load 0x00 to TMR0 (Initial Timer0 register)
	T0MD = C_PS0_TMR0 | C_PS0_Div2 ;		// Prescaler0 is assigned to Timer0, Prescaler0 dividing rate = 1:8,clock source is instruction clock
//;Setting Interrupt Enable Register	
	INTE |= C_INT_TMR0;
//;Initial Power control register
	
//;Enable Timer0 & Global interrupt bit 
	PCON1 = C_TMR0_En;						// Enable Timer0
// ;initial Timer 1/2 & PWM1 control register
	TMRH = 0;								// 
	TMR1 = 0x40;//0xFF;							// Move FFH to TMR1 (Initial Timer1 register) 
	
	PWM1DUTY = C_PWM_DUTY_25;				// PWM1 Duty = 64/256 = 25%			
	T1CR1 = C_PWM1_En | C_TMR1_Reload | C_TMR1_En;	// PWM1 output will be present on PB3 , PWM1 output is active high, reloaded from TMR1, non-stop mode 
	T1CR2 = C_TMR1_ClkSrc_Inst | C_PS1_Div2;	// Enable Prescaler1, Prescaler1 dividing rate = 1:2, Timer1 clock source is instruction clock 
	delay(2);
	stop_buzzer();	
	ENI();	
    while(1)
    {
    	if(sleep_flag)
    	{
    		sleep_flag = 0;
    		turnOffDevice();
    	}
    	led_task();
    	key_process_task();
		buzzer_task();
		charge_check_Task();
		digital_process_task();
    	CLRWDT();
    }
}
