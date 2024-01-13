#include "bat_volt_check.h"
#include "pca10040.h"
#include "led_alarm.h"
#include "nrf_delay.h"
#include "app_timer.h"
//定义SAADC采样缓存数组大小 ，这个参数极其重要，它决定了SAADC采样多少个数据后产生事件中断
#define SAMPLES_BUFFER_LEN 1 //5

APP_TIMER_DEF(m_battery_timer_id);                       //电池测量APP定时器
APP_TIMER_DEF(s_battery_timer_id);                       //电池禁止

static nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_BUFFER_LEN];

#define BATTERY_LEVEL_MEAS_INTERVAL     APP_TIMER_TICKS(15000)              // 电池电量测试间隔：5s采样
#define BATTERY_MEAS_STOP_INTERVAL     APP_TIMER_TICKS(10)              // 电池打开后关闭时间

static void saadc_timeout_handler(void * p_context);
void bat_sample_start(void)
{
    ret_code_t         err_code;
        /*------------------启动电池电量刷新-END-----------------*/
    err_code = app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL);         //一开机，启动电池电量采集。
    APP_ERROR_CHECK(err_code);  
}
static void bat_sample_stop(void)
{
    ret_code_t         err_code;
        /*------------------启动电池电量刷新-END-----------------*/
    err_code = app_timer_start(s_battery_timer_id, BATTERY_MEAS_STOP_INTERVAL, NULL);         //一开机，启动电池电量采集。
    APP_ERROR_CHECK(err_code);  
}
static void adc_stop_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    saadc_uinit();
}
void bat_volt_timer_init(void)
{
    ret_code_t         err_code;
           //创建电池测量APP定时器
    err_code = app_timer_create(&m_battery_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                saadc_timeout_handler);
    APP_ERROR_CHECK(err_code); 
   err_code = app_timer_create(&s_battery_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                adc_stop_timeout_handler);
    APP_ERROR_CHECK(err_code); 
}
static void saadc_timeout_handler(void * p_context)
{	
	UNUSED_PARAMETER(p_context);
    //启动一次ADC采样。
    saadc_init();
	nrfx_saadc_sample();              //ADC read
    bat_sample_stop();
}
#include <math.h>
const float eps = 1e-8;
#define More(a, b) (((a)-(b)) > (eps))
//SAADC事件回调函数，只有一个缓存填满后才会进入事件回调函数
static void saadc_callback(nrfx_saadc_evt_t const * p_event)
{
    float  val = 0.0;  //保存SAADC采样数据计算的实际电压值
    float  sum = 0.0;
	if (p_event->type == NRFX_SAADC_EVT_DONE)
    {
        ret_code_t err_code;

        //设置好缓存，为下一次采样准备
		err_code = nrfx_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_BUFFER_LEN);
        APP_ERROR_CHECK(err_code);
        int i;

        for (i = 0; i < SAMPLES_BUFFER_LEN; i++)
        {
            sum += (p_event->data.done.p_buffer[i] * 2.4 /1024);	

        }
        val = (sum/SAMPLES_BUFFER_LEN)*2;
        /*
        if(More(val,4.15))
        {
            batlevel = 100;
        }
        else
        {
            if(More(val,3.2))
            {
                batlevel = (val-3.2)*1000;
            }
            else
            {
                batlevel = 0;
            }
        }
        */
        if(More(val,4.15))                 //4.15 full voltage
        {
            batlevel = 100;  
        }
        else if(More(val,4.1))
        {
            batlevel = 90; 
        }
        else if(More(val,4.0))
        {
            batlevel = 80; 
        }
        else if(More(val, 3.9))
        {
            batlevel = 70; 
        }
        else if(More(val, 3.8))
        {
            batlevel = 60;
        }
        else if(More(val,3.7))
        {
            batlevel = 50;
        }
        else if(More(val, 3.6))
        {
            batlevel = 40;
        }
        else if(More(val,3.5))
        {
            batlevel = 30;
        }
        else if(More(val, 3.4))
        {
            batlevel = 20;
        }
        else if(More(val, 3.3))
        {
            batlevel = 10;
        }
        else if(More(val, 3.2))
        {
            batlevel = 0;
        }
        /***********************低电压报警*****************************/
       if(charge_flg == false)                    //未充电时
       {
            if(More(val,3.3))   //关闭报警灯
            {
               // app_timer_stop(led_alarm_timer_id);
                led_alarm_stop();
                bsp_board_led_off(3);
            }
            else 
            {
                if(More(val,3.0))   //报警灯打开
                {
                    //app_timer_start(led_alarm_timer_id, LEDALARM_INTERVAL, NULL);
                    led_alarm_start();
                }
                //低电压关机
                else    //低于3.0V休眠 
                {
                    //app_timer_stop(led_alarm_timer_id);
                    led_alarm_stop();
                    bsp_board_led_off(3);               
                    ret_code_t err_code = true;
                    //beep_start();
                    if(!sleep_flg)
                    {
                        sleep_flg = true;
                    }
                    //设置广播指示灯为广播停止（D1指示灯熄灭）
                    err_code = bsp_indication_set(BSP_INDICATE_IDLE);
                    APP_ERROR_CHECK(err_code);
                   // sleep_mode_enter();         //低电量进入休眠
                }
            }
        }
    }
}
//初始化SAADC，配置使用的SAADC通道的参数和缓存
void saadc_init(void)
{
    ret_code_t err_code;
	//定义ADC通道配置结构体，并使用单端采样配置宏初始化
	//因为要采样的是芯片的VDD，所以单端采样配置宏的通道正极为：NRF_SAADC_INPUT_VDD
    nrf_saadc_channel_config_t channel_config =
        NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);//(NRF_SAADC_INPUT_VDD);
    //初始化SAADC，注册事件回调函数。
	  err_code = nrf_drv_saadc_init(NULL, saadc_callback); 
    APP_ERROR_CHECK(err_code);
    //初始化SAADC通道0
    err_code = nrfx_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);
	
	//配置缓存1，将缓存1地址赋值给SAADC驱动程序中的控制块m_cb的一级缓存指针
	err_code = nrfx_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_BUFFER_LEN);
    APP_ERROR_CHECK(err_code);
    //配置缓存2，将缓存1地址赋值给SAADC驱动程序中的控制块m_cb的二级缓存指针
  //  err_code = nrfx_saadc_buffer_convert(m_buffer_pool[1], SAMPLES_BUFFER_LEN);
  //  APP_ERROR_CHECK(err_code);
}
void bat_volt_check_start(void)
{
    bat_volt_timer_init();
    bat_sample_start();
}
void saadc_uinit(void)
{
    nrfx_saadc_uninit();
}