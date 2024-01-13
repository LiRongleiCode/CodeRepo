#include "bat_volt_check.h"
#include "pca10040.h"
#include "led_alarm.h"
#include "nrf_delay.h"
#include "app_timer.h"
//����SAADC�������������С ���������������Ҫ����������SAADC�������ٸ����ݺ�����¼��ж�
#define SAMPLES_BUFFER_LEN 1 //5

APP_TIMER_DEF(m_battery_timer_id);                       //��ز���APP��ʱ��
APP_TIMER_DEF(s_battery_timer_id);                       //��ؽ�ֹ

static nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_BUFFER_LEN];

#define BATTERY_LEVEL_MEAS_INTERVAL     APP_TIMER_TICKS(15000)              // ��ص������Լ����5s����
#define BATTERY_MEAS_STOP_INTERVAL     APP_TIMER_TICKS(10)              // ��ش򿪺�ر�ʱ��

static void saadc_timeout_handler(void * p_context);
void bat_sample_start(void)
{
    ret_code_t         err_code;
        /*------------------������ص���ˢ��-END-----------------*/
    err_code = app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL);         //һ������������ص����ɼ���
    APP_ERROR_CHECK(err_code);  
}
static void bat_sample_stop(void)
{
    ret_code_t         err_code;
        /*------------------������ص���ˢ��-END-----------------*/
    err_code = app_timer_start(s_battery_timer_id, BATTERY_MEAS_STOP_INTERVAL, NULL);         //һ������������ص����ɼ���
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
           //������ز���APP��ʱ��
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
    //����һ��ADC������
    saadc_init();
	nrfx_saadc_sample();              //ADC read
    bat_sample_stop();
}
#include <math.h>
const float eps = 1e-8;
#define More(a, b) (((a)-(b)) > (eps))
//SAADC�¼��ص�������ֻ��һ������������Ż�����¼��ص�����
static void saadc_callback(nrfx_saadc_evt_t const * p_event)
{
    float  val = 0.0;  //����SAADC�������ݼ����ʵ�ʵ�ѹֵ
    float  sum = 0.0;
	if (p_event->type == NRFX_SAADC_EVT_DONE)
    {
        ret_code_t err_code;

        //���úû��棬Ϊ��һ�β���׼��
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
        /***********************�͵�ѹ����*****************************/
       if(charge_flg == false)                    //δ���ʱ
       {
            if(More(val,3.3))   //�رձ�����
            {
               // app_timer_stop(led_alarm_timer_id);
                led_alarm_stop();
                bsp_board_led_off(3);
            }
            else 
            {
                if(More(val,3.0))   //�����ƴ�
                {
                    //app_timer_start(led_alarm_timer_id, LEDALARM_INTERVAL, NULL);
                    led_alarm_start();
                }
                //�͵�ѹ�ػ�
                else    //����3.0V���� 
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
                    //���ù㲥ָʾ��Ϊ�㲥ֹͣ��D1ָʾ��Ϩ��
                    err_code = bsp_indication_set(BSP_INDICATE_IDLE);
                    APP_ERROR_CHECK(err_code);
                   // sleep_mode_enter();         //�͵�����������
                }
            }
        }
    }
}
//��ʼ��SAADC������ʹ�õ�SAADCͨ���Ĳ����ͻ���
void saadc_init(void)
{
    ret_code_t err_code;
	//����ADCͨ�����ýṹ�壬��ʹ�õ��˲������ú��ʼ��
	//��ΪҪ��������оƬ��VDD�����Ե��˲������ú��ͨ������Ϊ��NRF_SAADC_INPUT_VDD
    nrf_saadc_channel_config_t channel_config =
        NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);//(NRF_SAADC_INPUT_VDD);
    //��ʼ��SAADC��ע���¼��ص�������
	  err_code = nrf_drv_saadc_init(NULL, saadc_callback); 
    APP_ERROR_CHECK(err_code);
    //��ʼ��SAADCͨ��0
    err_code = nrfx_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);
	
	//���û���1��������1��ַ��ֵ��SAADC���������еĿ��ƿ�m_cb��һ������ָ��
	err_code = nrfx_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_BUFFER_LEN);
    APP_ERROR_CHECK(err_code);
    //���û���2��������1��ַ��ֵ��SAADC���������еĿ��ƿ�m_cb�Ķ�������ָ��
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