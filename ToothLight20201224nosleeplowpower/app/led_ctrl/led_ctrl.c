#include "led_ctrl.h"
#include "pca10040.h"
#include "bat_volt_check.h"

//PWM驱动程序实例ID,ID和外设编号对应，0:PWM0 1:PWM1 2:PWM2
#define PWM_INSTANCE  0 
//定义名称为m_pwm0的PWM驱动程序实例，参数为0表示该实例对应的PWM外设为PWM0


APP_TIMER_DEF(OnlyRedLed_timer_id);
APP_TIMER_DEF(OnlyBlueLed_timer_id);
APP_TIMER_DEF(BlueAndRedLed_timer_id);
APP_TIMER_DEF(BlueToRedLed_ftimer_id);
APP_TIMER_DEF(RedToBlueLed_ftimer_id);
APP_TIMER_DEF(BlueToRedLed_stimer_id);
APP_TIMER_DEF(RedToBlueLed_stimer_id);
//#define BEEP_INTERVAL                APP_TIMER_TICKS(200)  //500ms,0.5s
#define LED_INTERVAL(x)              APP_TIMER_TICKS(1000*60*(x))

static nrfx_pwm_t m_pwm0 = NRFX_PWM_INSTANCE(PWM_INSTANCE);
static uint16_t const              m_top  = 46;//10000;
static uint16_t const              m_step = 23;//100;

//定义PWM序列（通用装载模式），该序列必须位于RAM中，因此要定义为static类型的
uint16_t step  = m_top / m_step;
static nrf_pwm_values_common_t seq0_values[m_step*2];
static void pwm_play(void);
static void pwm_common_init(bsp_led_t led,Brightness_Adj_t brightness);

nrf_pwm_sequence_t seq0 =
{
    .values.p_common = seq0_values,//指向PWM序列
    .length          = NRF_PWM_VALUES_LENGTH(seq0_values),//PWM序列中包含的周期个数
    .repeats         = 0, //序列中周期重复次数为0
    .end_delay       = 0  //序列后不插入延时
};
void beep_start_pwm(void)
{
    ret_code_t err_code;
    nrfx_pwm_uninit(&m_pwm0);
    pwm_common_init(BSP_BEEP,BRIGHTNESS50);
    pwm_play();
}
 //工作模式选择，第一次定时时间到
static void lightwork_firsttimeout_handler(void * p_context)    
{
    ret_code_t err_code;
    UNUSED_PARAMETER(p_context);
    if(led_infor.work_mode == DEFAULTLED)
    {
        nrfx_pwm_uninit(&m_pwm0);
        pwm_common_init(BSP_RED_LED,led_infor.red_brightness);
        pwm_play();
        err_code = app_timer_start(BlueToRedLed_stimer_id, LED_INTERVAL(led_infor.red_timer), NULL);   //blue to red, it is red turn.
        APP_ERROR_CHECK(err_code);
        light_work_status.report_flg = true;
        light_work_status.light_status = REDLIGHT;
				light_work_status.light_operater = TURNLIGHT;
    }
    else if((led_infor.work_mode == ONLYBLUELED)\
        ||(led_infor.work_mode == ONLYREDLED)\
        ||(led_infor.work_mode == BLUEANDREDLED))
    {
        nrfx_pwm_uninit(&m_pwm0);
     //   (void)nrfx_pwm_simple_playback(&m_pwm0, &seq0, 1,
     //                                  NRFX_PWM_FLAG_STOP); 
        gotosleep_flg = true;                                        //准备进入休眠
        light_work_status.report_flg = true;
        light_work_status.light_status = NONELIGHT;
				light_work_status.light_operater = FINISHLIGHT;
    }
 //   else if(p_device_infor->led_infor.work_mode == ONLYREDLED)
//    {
        
 //   }
  //  else if(p_device_infor->led_infor.work_mode == BLUEANDREDLED)
  //  {
 //       
 //   }
    else if(led_infor.work_mode == BLUETOREDLED)
    {
        nrfx_pwm_uninit(&m_pwm0);
        pwm_common_init(BSP_RED_LED,led_infor.red_brightness);
        pwm_play();
        err_code = app_timer_start(BlueToRedLed_stimer_id, LED_INTERVAL(led_infor.red_timer), NULL);
        APP_ERROR_CHECK(err_code);
        light_work_status.report_flg = true;
        light_work_status.light_status = REDLIGHT;
				light_work_status.light_operater = TURNLIGHT;
    }
    else if(led_infor.work_mode == REDTOBLUELED)
    {
        nrfx_pwm_uninit(&m_pwm0);
        pwm_common_init(BSP_BLUE_LED,led_infor.blue_brightness);
        pwm_play();
        err_code = app_timer_start(RedToBlueLed_stimer_id, LED_INTERVAL(led_infor.blue_timer), NULL);
        APP_ERROR_CHECK(err_code);
        light_work_status.report_flg = true;
        light_work_status.light_status = BLUELIGHT;
				light_work_status.light_operater = TURNLIGHT;
    }
}
static void lightwork_secondtimeout_handler(void * p_context)    
{
    ret_code_t err_code;
    UNUSED_PARAMETER(p_context);
    if((led_infor.work_mode == DEFAULTLED)\
        ||(led_infor.work_mode == BLUETOREDLED)\
        ||(led_infor.work_mode == REDTOBLUELED))
    {
        nrfx_pwm_uninit(&m_pwm0);
     //   (void)nrfx_pwm_simple_playback(&m_pwm0, &seq0, 1,
      //                                 NRFX_PWM_FLAG_STOP);
        gotosleep_flg = true;                                  //准备进入休眠
        light_work_status.report_flg = true;
        light_work_status.light_status = NONELIGHT;
				light_work_status.light_operater = FINISHLIGHT;
    }
 //   else if(p_device_infor->led_infor.work_mode == ONLYBLUELED)
 //   {
        
 //   }
 //   else if(p_device_infor->led_infor.work_mode == ONLYREDLED)
 //   {
        
 //   }
 //   else if(p_device_infor->led_infor.work_mode == BLUEANDREDLED)
 //   {
        
  //  }
  //  else if(p_device_infor->led_infor.work_mode == BLUETOREDLED)
  //  {

  //  }
  //  else if(p_device_infor->led_infor.work_mode == REDTOBLUELED)
  //  {

 //   }
}
void Led_TimerCreate(void)
{
    ret_code_t err_code = true;
    err_code = app_timer_create(&OnlyRedLed_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                lightwork_firsttimeout_handler);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_create(&OnlyBlueLed_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                lightwork_firsttimeout_handler);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_create(&BlueAndRedLed_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                lightwork_firsttimeout_handler);
    APP_ERROR_CHECK(err_code);
    
    err_code = app_timer_create(&BlueToRedLed_ftimer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                lightwork_firsttimeout_handler);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_create(&RedToBlueLed_ftimer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                lightwork_firsttimeout_handler);
    APP_ERROR_CHECK(err_code);
    
    err_code = app_timer_create(&BlueToRedLed_stimer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                lightwork_secondtimeout_handler);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_create(&RedToBlueLed_stimer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                lightwork_secondtimeout_handler);
    APP_ERROR_CHECK(err_code);
}
void Light_uninit(void)
{
    nrfx_pwm_uninit(&m_pwm0);
}
void stop_all_light_timer(void)
{
    app_timer_stop(OnlyRedLed_timer_id);
    app_timer_stop(OnlyBlueLed_timer_id);
    app_timer_stop(BlueAndRedLed_timer_id);
    app_timer_stop(BlueToRedLed_ftimer_id);
    app_timer_stop(RedToBlueLed_ftimer_id);
    app_timer_stop(BlueToRedLed_stimer_id);
    app_timer_stop(RedToBlueLed_stimer_id);
}
void light_work_choose(led_infor_t *p_led_infor)
{
    ret_code_t err_code;
    switch (p_led_infor->work_mode)
    {
        case DEFAULTLED:
        {
            Light_uninit(); 
            pwm_common_init(BSP_BLUE_LED,p_led_infor->blue_brightness);
            pwm_play();
            err_code = app_timer_start(BlueToRedLed_ftimer_id, LED_INTERVAL(p_led_infor->blue_timer), NULL);
            APP_ERROR_CHECK(err_code);
            light_work_status.report_flg = true;
            light_work_status.light_status = BLUELIGHT;
        }
        case ONLYBLUELED:
        {
           Light_uninit(); 
           pwm_common_init(BSP_BLUE_LED,p_led_infor->blue_brightness);
           pwm_play();
           err_code = app_timer_start(OnlyBlueLed_timer_id, LED_INTERVAL(p_led_infor->blue_timer), NULL);
           APP_ERROR_CHECK(err_code);
           light_work_status.report_flg = true;
           light_work_status.light_status = BLUELIGHT;
        }
        break;
        case ONLYREDLED:
        {
           Light_uninit(); 
           pwm_common_init(BSP_RED_LED,p_led_infor->red_brightness);
           pwm_play();
           err_code = app_timer_start(OnlyRedLed_timer_id, LED_INTERVAL(p_led_infor->red_timer), NULL);
           APP_ERROR_CHECK(err_code); 
           light_work_status.report_flg = true;
           light_work_status.light_status = REDLIGHT;
        }
        break;
        case BLUEANDREDLED:
        {
            Light_uninit(); 
            pwm_common_init(BSP_RED_BLUE_LED,p_led_infor->blueandred_brightness);
            pwm_play();
            err_code = app_timer_start(BlueAndRedLed_timer_id, LED_INTERVAL(p_led_infor->blueandred_timer), NULL);
            APP_ERROR_CHECK(err_code); 
        }
        break;
        case BLUETOREDLED:
        {
            Light_uninit(); 
            pwm_common_init(BSP_BLUE_LED,p_led_infor->blue_brightness);
            pwm_play();
            err_code = app_timer_start(BlueToRedLed_ftimer_id, LED_INTERVAL(p_led_infor->blue_timer), NULL);
            APP_ERROR_CHECK(err_code);
            light_work_status.report_flg = true;
            light_work_status.light_status = BLUELIGHT;
        }
        break;
        case REDTOBLUELED:
        {
            Light_uninit(); 
            pwm_common_init(BSP_RED_LED,p_led_infor->red_brightness);
            pwm_play();
            err_code = app_timer_start(RedToBlueLed_ftimer_id, LED_INTERVAL(p_led_infor->red_timer), NULL);
            APP_ERROR_CHECK(err_code);
            light_work_status.report_flg = true;
            light_work_status.light_status = REDLIGHT;
        }
        break;
        default:
        {
            Light_uninit(); 
            pwm_common_init(BSP_BLUE_LED,p_led_infor->blue_brightness);
            pwm_play();
            err_code = app_timer_start(BlueToRedLed_ftimer_id, LED_INTERVAL(p_led_infor->blue_timer), NULL);
            APP_ERROR_CHECK(err_code);
            light_work_status.report_flg = true;
            light_work_status.light_status = BLUELIGHT;
        }
        break;
    }
}
static void pwm_common_init(bsp_led_t led,Brightness_Adj_t brightness)
{
    //设置PWM序列中各个周期的值：逐步修改占空比，实现呼吸灯效果
	for (uint8_t i = 0; i < m_step; ++i)
	{
       // value += step;
		seq0_values[i] = 0X8000+(brightness*4);//brightness*1000;//value;
		seq0_values[m_step+i] = 0X8000+(46-brightness*4);//m_top - brightness*100;
	}
    //定义PWM初始化配置结构体并初始化参数
    if(led == BSP_BLUE_LED)
    {
        nrfx_pwm_config_t const config0 =
        {
            .output_pins =
            {
                 BSP_LED_0, //| NRFX_PWM_PIN_INVERTED, //通道0映射到P0.17（驱动开发板上的指示灯D1），空闲状态输出高电平
                //BSP_LED_1 | NRFX_PWM_PIN_INVERTED, //通道1映射到P0.18（驱动开发板上的指示灯D2），空闲状态输出高电平
                //BSP_LED_2 | NRFX_PWM_PIN_INVERTED, //通道2映射到P0.19（驱动开发板上的指示灯D3），空闲状态输出高电平
                NRFX_PWM_PIN_NOT_USED              //通道3不使用
            },
            .irq_priority = APP_IRQ_PRIORITY_LOWEST,//中断优先级
            .base_clock   = NRF_PWM_CLK_125kHz,//NRF_PWM_CLK_1MHz,       //PWM时钟频率设置为1MHz  
            .count_mode   = NRF_PWM_MODE_UP,        //向上计数模式
            .top_value    = m_top,                  //计数最大值为10000
            .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,//NRF_PWM_LOAD_COMMON,    //通用装载模式
            .step_mode    = NRF_PWM_STEP_AUTO       //序列中的周期自动推进
        };
        //初始化PWM
        APP_ERROR_CHECK(nrfx_pwm_init(&m_pwm0, &config0, NULL));
    }
    if(led == BSP_RED_LED)
    {
        nrfx_pwm_config_t const config0 =
        {
            .output_pins =
            {
                // BSP_LED_0 | NRFX_PWM_PIN_INVERTED, //通道0映射到P0.17（驱动开发板上的指示灯D1），空闲状态输出高电平
                BSP_LED_1, //| NRFX_PWM_PIN_INVERTED, //通道1映射到P0.18（驱动开发板上的指示灯D2），空闲状态输出高电平
               // BSP_LED_2 | NRFX_PWM_PIN_INVERTED, //通道2映射到P0.19（驱动开发板上的指示灯D3），空闲状态输出高电平
                NRFX_PWM_PIN_NOT_USED              //通道3不使用
            },
            .irq_priority = APP_IRQ_PRIORITY_LOWEST,//中断优先级
            .base_clock   = NRF_PWM_CLK_1MHz,       //PWM时钟频率设置为1MHz  
            .count_mode   = NRF_PWM_MODE_UP,        //向上计数模式
            .top_value    = m_top,                  //计数最大值为10000
            .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,//NRF_PWM_LOAD_COMMON,    //通用装载模式
            .step_mode    = NRF_PWM_STEP_AUTO       //序列中的周期自动推进
        };
        //初始化PWM
        APP_ERROR_CHECK(nrfx_pwm_init(&m_pwm0, &config0, NULL));
    }
    if(led == BSP_RED_BLUE_LED)
    {
        nrfx_pwm_config_t const config0 =
        {
            .output_pins =
            {
                BSP_LED_0, //| NRFX_PWM_PIN_INVERTED, //通道0映射到P0.17（驱动开发板上的指示灯D1），空闲状态输出高电平
                BSP_LED_1 //| NRFX_PWM_PIN_INVERTED, //通道1映射到P0.18（驱动开发板上的指示灯D2），空闲状态输出高电平
               // BSP_LED_2 | NRFX_PWM_PIN_INVERTED, //通道2映射到P0.19（驱动开发板上的指示灯D3），空闲状态输出高电平
               // NRFX_PWM_PIN_NOT_USED              //通道3不使用
            },
            .irq_priority = APP_IRQ_PRIORITY_LOWEST,//中断优先级
            .base_clock   = NRF_PWM_CLK_1MHz,       //PWM时钟频率设置为1MHz  
            .count_mode   = NRF_PWM_MODE_UP,        //向上计数模式
            .top_value    = m_top,                  //计数最大值为10000
            .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,//NRF_PWM_LOAD_COMMON,    //通用装载模式
            .step_mode    = NRF_PWM_STEP_AUTO       //序列中的周期自动推进
        };
        //初始化PWM
        APP_ERROR_CHECK(nrfx_pwm_init(&m_pwm0, &config0, NULL));
    }
    if(led == BSP_BEEP)
    {
        nrfx_pwm_config_t const config0 =
        {
            .output_pins =
            {
                 BEEP_PIN, //| NRFX_PWM_PIN_INVERTED, //通道0映射到P0.17（驱动开发板上的指示灯D1），空闲状态输出高电平
                //BSP_LED_1 | NRFX_PWM_PIN_INVERTED, //通道1映射到P0.18（驱动开发板上的指示灯D2），空闲状态输出高电平
                //BSP_LED_2 | NRFX_PWM_PIN_INVERTED, //通道2映射到P0.19（驱动开发板上的指示灯D3），空闲状态输出高电平
                NRFX_PWM_PIN_NOT_USED              //通道3不使用
            },
            .irq_priority = APP_IRQ_PRIORITY_LOWEST,//中断优先级
            .base_clock   = NRF_PWM_CLK_125kHz,//NRF_PWM_CLK_1MHz,       //PWM时钟频率设置为1MHz  
            .count_mode   = NRF_PWM_MODE_UP,        //向上计数模式
            .top_value    = m_top,                  //计数最大值为10000
            .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,//NRF_PWM_LOAD_COMMON,    //通用装载模式
            .step_mode    = NRF_PWM_STEP_AUTO       //序列中的周期自动推进
        };
        //初始化PWM
        APP_ERROR_CHECK(nrfx_pwm_init(&m_pwm0, &config0, NULL));
    }
}
//播放PWM
static void pwm_play(void)
{
    //定义PWM播放序列，播放序列包含了PWM序列的起始地址、大小和序列播放控制描述

    //启动PWM序列播放，flags设置为NRFX_PWM_FLAG_LOOP：序列播放完成后，自动触发任务重新播放
		//如改为NRFX_PWM_FLAG_STOP，则播放结束后，PWM停止
    (void)nrfx_pwm_simple_playback(&m_pwm0, &seq0, 1,
                                       NRFX_PWM_FLAG_LOOP);
}