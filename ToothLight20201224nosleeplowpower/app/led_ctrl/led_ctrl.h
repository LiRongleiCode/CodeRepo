#ifndef LED_CTRL_H__
#define LED_CTRL_H__

#include <stdint.h>
#include <stdbool.h>
#include "nrfx_pwm.h"
#include "boards.h"
#include "bsp_btn_ble.h"
#include "app_timer.h"

#ifdef __cplusplus
extern "C" {
#endif
//PWM驱动程序实例ID,ID和外设编号对应，0:PWM0 1:PWM1 2:PWM2
#define PWM_INSTANCE  0 
//定义名称为m_pwm0的PWM驱动程序实例，参数为0表示该实例对应的PWM外设为PWM0   
    

extern void Led_TimerCreate(); 
extern void Light_uninit(void);    
extern void light_work_choose(led_infor_t *p_led_infor); 
extern void beep_start_pwm(void);  
extern void stop_all_light_timer(void);   
#ifdef __cplusplus
}
#endif

#endif