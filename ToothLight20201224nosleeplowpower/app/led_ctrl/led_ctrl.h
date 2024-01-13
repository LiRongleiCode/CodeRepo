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
//PWM��������ʵ��ID,ID�������Ŷ�Ӧ��0:PWM0 1:PWM1 2:PWM2
#define PWM_INSTANCE  0 
//��������Ϊm_pwm0��PWM��������ʵ��������Ϊ0��ʾ��ʵ����Ӧ��PWM����ΪPWM0   
    

extern void Led_TimerCreate(); 
extern void Light_uninit(void);    
extern void light_work_choose(led_infor_t *p_led_infor); 
extern void beep_start_pwm(void);  
extern void stop_all_light_timer(void);   
#ifdef __cplusplus
}
#endif

#endif