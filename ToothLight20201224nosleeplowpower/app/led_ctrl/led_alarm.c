#include "led_alarm.h"
#include "pca10040.h"

APP_TIMER_DEF(led_alarm_timer_id);                     //alarm

#define LEDALARM_INTERVAL               APP_TIMER_TICKS(200)       //0.2S

static uint8_t ledcnt = 0;
static void ledalarm_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    ledcnt++;
    if(ledcnt%4 == 0)
    {
        bsp_board_led_off(3);
    }
    else
    {
        bsp_board_led_on(3);
    }
}

void led_alarm_timer_initial(void)
{
    ret_code_t err_code;
        //创建闪定时器
    err_code = app_timer_create(&led_alarm_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                ledalarm_timeout_handler);
    APP_ERROR_CHECK(err_code); 
}

void led_alarm_start(void)
{
    app_timer_start(led_alarm_timer_id, LEDALARM_INTERVAL, NULL);
}

void led_alarm_stop(void)
{
     app_timer_stop(led_alarm_timer_id);
}
