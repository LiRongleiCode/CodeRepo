#ifndef LED_ALARM_H__
#define LED_ALARM_H__

#include <stdint.h>
#include <stdbool.h>
#include "boards.h"
#include "bsp_btn_ble.h"
#include "app_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

void led_alarm_timer_initial(void);
void led_alarm_start(void);
void led_alarm_stop(void);



#ifdef __cplusplus
}
#endif

#endif