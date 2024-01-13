/****************************************Copyright (c)************************************************
**                                      [艾克姆科技]
**                                        IIKMSIK 
**                            官方店铺：https://acmemcu.taobao.com
**                            官方论坛：http://www.e930bbs.com
**                                   
**--------------File Info-----------------------------------------------------------------------------
** File name         : main.c
** Last modified Date: 2020-8-26        
** Last Version      :		   
** Descriptions      : 使用的SDK版本-SDK_16.0
**						
**----------------------------------------------------------------------------------------------------
** Created by        : [艾克姆]
** Created date      : 2018-12-24
** Version           : 1.0
** Descriptions      : 本例在 [18-2：使用可解析私有地址] 的例子的基础上修改，增加的功能如下：
**                   : 1)	本机使用可解析私有地址实现隐私，地址改变周期设置为60秒（这是为了试验时方便观察地址的变化，蓝牙核心规范建议地址改变周期为 15分钟）。
**                   : 2)	本机启用了白名单过滤主机，当其首次启动运行后，如果和主机成功配对绑定，则会记录该主机的地址，将其加入白名单:
**                   : 3)	未连接状态下按下开发板上的S2按键，开发板执行禁用白名单广播，这时开发板可以和新的主机连接绑定，并将其加入白名单。
**---------------------------------------------------------------------------------------------------*/
//引用的C库头文件
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
//Log需要引用的头文件
//#include "nrf_log.h"
//#include "nrf_log_ctrl.h"
//#include "nrf_log_default_backends.h"
//APP定时器需要引用的头文件
#include "app_timer.h"

#include "bsp_btn_ble.h"
//广播需要引用的头文件
#include "ble_advdata.h"
#include "ble_advertising.h"
//电源管理需要引用的头文件
#include "nrf_pwr_mgmt.h"
//SoftDevice handler configuration需要引用的头文件
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "bsp_btn_ble.h"
//排序写入模块需要引用的头文件
#include "nrf_ble_qwr.h"
//GATT需要引用的头文件
#include "nrf_ble_gatt.h"
//连接参数协商需要引用的头文件
#include "ble_conn_params.h"
//串口透传需要引用的头文件
#include "my_ble_uarts.h"
//蓝牙透传需要引用的头文件
//
#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif
#include "app_uart.h"

#include "boards.h"
//#include "nrfx_pwm.h"
//#include "nrf_drv_saadc.h"

//引用FDS头文件
//#include "fds.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
//添加文件读取
#include "board_flash_fstorage.h"
#include "ProcRex.h"
#include "led_ctrl.h"
#include "led_alarm.h"
#include "bat_volt_check.h"

#define SN                              	"D0004"                          //
#define BRAND                            "YNBY"
//#define DEVICE_NAME                     "YNBYD0002"                      // 设备名称字符串 

#define UARTS_SERVICE_UUID_TYPE         BLE_UUID_TYPE_VENDOR_BEGIN         // 串口透传服务UUID类型：厂商自定义UUID
#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(100, UNIT_1_25_MS)   // 最小连接间隔 (0.1 秒) 
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(200, UNIT_1_25_MS)   // 最大连接间隔 (0.2 秒) 
#define SLAVE_LATENCY                   0                                  // 从机延迟 
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)    // 监督超时(4 秒) 



#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)              // 定义首次调用sd_ble_gap_conn_param_update()函数更新连接参数延迟时间（5秒）
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)             // 定义每次调用sd_ble_gap_conn_param_update()函数更新连接参数的间隔时间（30秒）
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                  // 定义放弃连接参数协商前尝试连接参数协商的最大次数（3次）

#define APP_ADV_INTERVAL                800//320                                // 广播间隔 500ms(200ms)，单位0.625 ms 
#define APP_ADV_DURATION                0//18000//0                                  // 广播持续时间，单位：10ms。设置为0表示不超时 180s

#define APP_BLE_OBSERVER_PRIO           3               //应用程序BLE事件监视者优先级，应用程序不能修改该数值
#define APP_BLE_CONN_CFG_TAG            1               //SoftDevice BLE配置标志

#define UART_TX_BUF_SIZE 256       //串口发送缓存大小（字节数）
#define UART_RX_BUF_SIZE 256       //串口接收缓存大小（字节数）

#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50)
//定义SAADC采样缓存数组大小 ，这个参数极其重要，它决定了SAADC采样多少个数据后产生事件中断
//#define SAMPLES_BUFFER_LEN 5


//用于stack dump的错误代码，可以用于栈回退时确定堆栈位置
#define DEAD_BEEF                         0xDEADBEEF   
//PWM驱动程序实例ID,ID和外设编号对应，0:PWM0 1:PWM1 2:PWM2
//#define PWM_INSTANCE  0 
//定义名称为m_pwm0的PWM驱动程序实例，参数为0表示该实例对应的PWM外设为PWM0
//定义驱动蜂鸣器的引脚P0.12
#define  BEEP_PIN   12

APP_TIMER_DEF(beep_timer_id);
//APP_TIMER_DEF(m_battery_timer_id);                       //电池测量APP定时器
APP_TIMER_DEF(data_upload_timer_id);                     //数据定时上传
APP_TIMER_DEF(goto_sleep_timer_id);                     //休眠定时
//APP_TIMER_DEF(led_alarm_timer_id);                     //alarm
APP_TIMER_DEF(BtCharge_status_timer_id);                     //充电状态检测
//定义SAADC采样缓存数组，这里定义成2维数组，也可以定义成2个一维数组，它们本质上是一样的
//static nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_BUFFER_LEN];
        
//#define BATTERY_LEVEL_MEAS_INTERVAL     APP_TIMER_TICKS(1000)              // 电池电量测试间隔：5秒
#define BEEP_INTERVAL                   APP_TIMER_TICKS(200)  //500ms,0.5s
#define DATAUPLOAD_INTERVAL             APP_TIMER_TICKS(5000)              //数据间隔1S钟更新一次
#define GOTOSLEEP_INTERVAL              APP_TIMER_TICKS(1000*60*3)   //3min
//#define LEDALARM_INTERVAL               APP_TIMER_TICKS(200)       //0.2S
#define BTCHARGESTATUS_INTERVAL         APP_TIMER_TICKS(100)       //0.1S

//APP定时器事件处理函数，该函数中启动SAADC采样
//static void saadc_timeout_handler(void * p_context);
void sleep_mode_enter();
uint32_t ble_app_data_send(uint8_t   * p_data, uint16_t  * p_lengt);
static void beep_start(void);
uint16_t value = 0;
uint8_t sendbuff[10] = {0};

BLE_UARTS_DEF(m_uarts, NRF_SDH_BLE_TOTAL_LINK_COUNT);                 //定义名称为m_uarts的串口透传服务实例
NRF_BLE_GATT_DEF(m_gatt);               //定义名称为m_gatt的GATT模块实例
NRF_BLE_QWR_DEF(m_qwr);                 //定义一个名称为m_qwr的排队写入实例
BLE_ADVERTISING_DEF(m_advertising);     //定义名称为m_advertising的广播模块实例

//该变量用于保存连接句柄，初始值设置为无连接
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID; 
//发送的最大数据长度
static uint16_t   m_ble_uarts_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            
static bool uart_enabled = false;
static bool first_flag = false;
static bool adv_start_flg = false;
//定义串口透传服务UUID列表
static ble_uuid_t m_adv_uuids[]          =                                          
{
    {BLE_UUID_UARTS_SERVICE, UARTS_SERVICE_UUID_TYPE}
};


//设备名称数组：中文 三诺牙灯
//const char device_name[12] = {0xE4,0xB8,0x89,0xE8,0xAF,0xBA,0xE7,0x89,0x99,0xE7,0x81,0xAF};	

//GAP参数初始化，该函数配置需要的GAP参数，包括设备名称，外观特征、首选连接参数
static void gap_params_init(void)
{
    ret_code_t              err_code;
	  //定义连接参数结构体变量
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;
    //设置GAP的安全模式
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    //设置GAP设备名称，使用英文设备名称
		char *devicename = malloc(strlen(SN) + strlen(BRAND) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(devicename, BRAND);
    strcat(devicename, SN);
    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                              (const uint8_t *)devicename,
                                              strlen(devicename));
	
	  //设置GAP设备名称，使用了中文设备名称
 //   err_code = sd_ble_gap_device_name_set(&sec_mode,
  //                                        (const uint8_t *)device_name,
  //                                        sizeof(device_name));
																					
    //检查函数返回的错误代码
		APP_ERROR_CHECK(err_code);
																				
    //设置首选连接参数，设置前先清零gap_conn_params
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;//最小连接间隔
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;//最小连接间隔
    gap_conn_params.slave_latency     = SLAVE_LATENCY;    //从机延迟
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT; //监督超时
    //调用协议栈API sd_ble_gap_ppcp_set配置GAP参数
    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);												
}
//GATT事件处理函数，该函数中处理MTU交换事件
void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
    //如果是MTU交换事件
	if ((m_conn_handle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED))
    {
        //设置串口透传服务的有效数据长度（MTU-opcode-handle）
        m_ble_uarts_max_data_len = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
    }
}
//初始化GATT程序模块
static void gatt_init(void)
{
    //初始化GATT程序模块
	ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
	//检查函数返回的错误代码
    APP_ERROR_CHECK(err_code);
	//设置ATT MTU的大小,这里设置的值为247
	err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);
}

//排队写入事件处理函数，用于处理排队写入模块的错误
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    //检查错误代码
	APP_ERROR_HANDLER(nrf_error);
}
//串口事件回调函数，串口初始化时注册，该函数中判断事件类型并进行处理
//当接收的数据长度达到设定的最大值或者接收到换行符后，则认为一包数据接收完成，之后将接收的数据发送给主机
void uart_event_handle(app_uart_evt_t * p_event)
{
    static uint8_t data_array[BLE_UARTS_MAX_DATA_LEN];
    static uint8_t index = 0;
    uint32_t       err_code;
    //判断事件类型
    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY://串口接收事件 通过串口接收来的数据对APP上传的处理。
            UNUSED_VARIABLE(app_uart_get(&data_array[index]));
            index++;
            //接收串口数据，当接收的数据长度达到m_ble_uarts_max_data_len或者接收到换行符后认为一包数据接收完成
            if ((data_array[index - 1] == '\n') ||
                (data_array[index - 1] == '\r') ||
                (index >= m_ble_uarts_max_data_len))
            {
                if (index > 1)
                {
                    //串口接收的数据使用notify发送给BLE主机
                    do
                    {
                        uint16_t length = (uint16_t)index;
                        err_code = ble_uarts_data_send(&m_uarts, data_array, &length, m_conn_handle);
                        if ((err_code != NRF_ERROR_INVALID_STATE) &&
                            (err_code != NRF_ERROR_RESOURCES) &&
                            (err_code != NRF_ERROR_NOT_FOUND))
                        {
                            APP_ERROR_CHECK(err_code);
                        }
                    } while (err_code == NRF_ERROR_RESOURCES);
                }

                index = 0;
            }
            break;
        //通讯错误事件，进入错误处理
        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);                     //屏蔽掉，防止未接串口线复位
            break;
        //FIFO错误事件，进入错误处理
        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;
        default:
            break;
    }
}
//串口配置
void uart_config(void)
{
    uint32_t err_code;
	
	//定义串口通讯参数配置结构体并初始化
    const app_uart_comm_params_t comm_params =
    {
        RX_PIN_NUMBER,//定义uart接收引脚
        TX_PIN_NUMBER,//定义uart发送引脚
        RTS_PIN_NUMBER,//定义uart RTS引脚，流控关闭后虽然定义了RTS和CTS引脚，但是驱动程序会忽略，不会配置这两个引脚，两个引脚仍可作为IO使用
        CTS_PIN_NUMBER,//定义uart CTS引脚
        APP_UART_FLOW_CONTROL_DISABLED,//关闭uart硬件流控
        false,//禁止奇偶检验
        NRF_UART_BAUDRATE_115200//uart波特率设置为115200bps
    };
    //初始化串口，注册串口事件回调函数
    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_event_handle,
                         APP_IRQ_PRIORITY_LOWEST,
                         err_code);

    APP_ERROR_CHECK(err_code);
	
}
static void uart_reconfig(void)
{
	if(uart_enabled == false)//初始化串口
	{
		uart_config();
		uart_enabled = true;
	}
	else
	{
		app_uart_close();//反初始化串口
		uart_enabled = false;
	}
}
//串口透传事件回调函数，串口透出服务初始化时注册，发送从APP接收来的数据。
static void uarts_data_handler(ble_uarts_evt_t * p_evt)
{
    uint32_t err_code;
    uint8_t length = 0;
    
    	  //通知使能后才初始化串口
	//  if (p_evt->type == BLE_NUS_EVT_COMM_STARTED)
		{
	//		uart_reconfig();
		}
		//通知关闭后，关闭串口
	//	else if(p_evt->type == BLE_NUS_EVT_COMM_STOPPED)
		{
	//	  uart_reconfig();
		}
        uart_enabled = true;
	  //判断事件类型:接收到新数据事件
    if ((p_evt->type == BLE_UARTS_EVT_RX_DATA)&&(uart_enabled == true))
    {
        cmdfeedback_flg = true;                                  //一旦APP有数据过来，就置位
        memset(sendbuff,0,sizeof(uint8_t)*10);
				receive_date_parser(p_evt->params.rx_data.p_data,(uint8_t*)&(p_evt->params.rx_data.length),sendbuff,&length);
        ble_app_data_send(sendbuff,(uint16_t*)&length);
				p_evt->params.rx_data.length = 0;
        cmdfeedback_flg = false;                          //一旦响应就清除
    }
}
//服务初始化，包含初始化排队写入模块和初始化应用程序使用的服务
static void services_init(void)
{
    ret_code_t         err_code;
	uint8_t tempsn[5];
	//定义串口透传初始化结构体
	ble_uarts_init_t     uarts_init;
	//定义排队写入初始化结构体变量
    nrf_ble_qwr_init_t qwr_init = {0};

    //排队写入事件处理函数
    qwr_init.error_handler = nrf_qwr_error_handler;
    //初始化排队写入模块
    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
	//检查函数返回值
    APP_ERROR_CHECK(err_code);
    
		
	/*------------------以下代码初始化串口透传服务-------------*/
	//清零串口透传服务初始化结构体
	memset(&uarts_init, 0, sizeof(uarts_init));
	//设置串口透传事件回调函数
    uarts_init.data_handler = uarts_data_handler;
    //初始化串口透传服务
    err_code = ble_uarts_init(&m_uarts, &uarts_init);
    APP_ERROR_CHECK(err_code);
    /*------------------启动电池电量刷新-END-----------------*/
   // err_code = app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL);         //一开机，启动电池电量采集。
   // APP_ERROR_CHECK(err_code); 
    /*------------------启动电池充电状态检测-END-----------------*/
    err_code = app_timer_start(BtCharge_status_timer_id, BTCHARGESTATUS_INTERVAL, NULL);         //一开机，启动电池充电状态检测。
    APP_ERROR_CHECK(err_code); 
    read_ledinfor_record();
		//StringToHex(SN,tempsn);
		device_id.serialsnum[0] = 0;
		memcpy(device_id.serialsnum+1,(uint8_t *)SN,5);
		write_deviceid_fds();
    //read_deviceid_record();
}

//连接参数协商模块事件处理函数
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    ret_code_t err_code;
    //判断事件类型，根据事件类型执行动作
	//连接参数协商失败
    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
	//连接参数协商成功
	if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_SUCCEEDED)
    {
       //功能代码;
    }
}

//连接参数协商模块错误处理事件，参数nrf_error包含了错误代码，通过nrf_error可以分析错误信息
static void conn_params_error_handler(uint32_t nrf_error)
{
    //检查错误代码
	  APP_ERROR_HANDLER(nrf_error);
}


//连接参数协商模块初始化
static void conn_params_init(void)
{
    ret_code_t             err_code;
	  //定义连接参数协商模块初始化结构体
    ble_conn_params_init_t cp_init;
    //配置之前先清零
    memset(&cp_init, 0, sizeof(cp_init));
    //设置为NULL，从主机获取连接参数
    cp_init.p_conn_params                  = NULL;
	  //连接或启动通知到首次发起连接参数更新请求之间的时间设置为5秒
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
	  //每次调用sd_ble_gap_conn_param_update()函数发起连接参数更新请求的之间的间隔时间设置为：30秒
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
	  //放弃连接参数协商前尝试连接参数协商的最大次数设置为：3次
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
	  //连接参数更新从连接事件开始计时
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
	  //连接参数更新失败不断开连接
    cp_init.disconnect_on_fail             = false;
	  //注册连接参数更新事件句柄
    cp_init.evt_handler                    = on_conn_params_evt;
	  //注册连接参数更新错误事件句柄
    cp_init.error_handler                  = conn_params_error_handler;
    //调用库函数（以连接参数更新初始化结构体为输入参数）初始化连接参数协商模块
    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}
//广播事件处理函数
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    ret_code_t err_code;
    //判断广播事件类型
    switch (ble_adv_evt)
    {
        //快速广播启动事件：快速广播启动后会产生该事件
        case BLE_ADV_EVT_FAST:
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);                          //处于广播状态，关闭灯20201107
            APP_ERROR_CHECK(err_code);
            break;
            //广播IDLE事件：广播超时后会产生该事件
        case BLE_ADV_EVT_IDLE:
            //设置广播指示灯为广播停止（D1指示灯熄灭）
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            APP_ERROR_CHECK(err_code);
            break;
        default:
            break;
    }
}

//启动广播，该函数所用的模式必须和广播初始化中设置的广播模式一样
static void advertising_start()
{
    
    ret_code_t ret = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
	//检查函数返回的错误代码
    APP_ERROR_CHECK(ret);
}
//广播初始化
static void advertising_init(void)
{
    ret_code_t             err_code;
	  //定义广播初始化配置结构体变量
    ble_advertising_init_t init;
    //配置之前先清零
    memset(&init, 0, sizeof(init));
    //设备名称类型：全称
    init.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
	  //是否包含外观：包含
    init.advdata.include_appearance      = false;
	  //Flag:一般可发现模式，不支持BR/EDR
    init.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
	  //UUID放到扫描响应里面
	  init.srdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.srdata.uuids_complete.p_uuids  = m_adv_uuids;
	
    //设置广播模式为快速广播
    init.config.ble_adv_fast_enabled  = true;
	  //设置广播间隔和广播持续时间
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;
    //广播事件回调函数
    init.evt_handler = on_adv_evt;
    //初始化广播
    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);
    //设置广播配置标记。APP_BLE_CONN_CFG_TAG是用于跟踪广播配置的标记，这是为未来预留的一个参数，在将来的SoftDevice版本中，
    //可以使用sd_ble_gap_adv_set_configure()配置新的广播配置
	//当前SoftDevice版本（S132 V7.0.1版本）支持的最大广播集数量为1，因此APP_BLE_CONN_CFG_TAG只能写1。
    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
    sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_ADV, BLE_GAP_ADV_SET_HANDLE_NOT_SET, -40);
}

//BLE事件处理函数
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    ret_code_t err_code = NRF_SUCCESS;
    //判断BLE事件类型，根据事件类型执行相应操作
    switch (p_ble_evt->header.evt_id)
    {
        //断开连接事件
		case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;			    
            err_code = app_timer_stop(data_upload_timer_id);    //停止更新。
            APP_ERROR_CHECK(err_code);
            break;
				
        //连接事件
        case BLE_GAP_EVT_CONNECTED:
            //设置指示灯状态为连接状态，即指示灯D1常亮
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            err_code = app_timer_start(data_upload_timer_id, DATAUPLOAD_INTERVAL, NULL);         //连接成功，更新数据。
            APP_ERROR_CHECK(err_code);               
            //保存连接句柄
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            //将连接句柄分配给排队写入实例，分配后排队写入实例和该连接关联，这样，当有多个连接的时候，通过关联不同的排队写入实例，很方便单独处理各个连接
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            break;
				
        //PHY更新事件
        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
       //     NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
			//响应PHY更新规程
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;
        	//安全参数请求事件
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            //不支持配对
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
                
			//系统属性访问正在等待中
		case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            //系统属性没有存储，更新系统属性
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;
            //GATT客户端超时事件
        case BLE_GATTC_EVT_TIMEOUT:
            //断开当前连接
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;
				
            //GATT服务器超时事件
        case BLE_GATTS_EVT_TIMEOUT:
			//断开当前连接
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            break;
    }
}

//初始化BLE协议栈
static void ble_stack_init(void)
{
    ret_code_t err_code;
    
    
    //请求使能SoftDevice，该函数中会根据sdk_config.h文件中低频时钟的设置来配置低频时钟
    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);
    
    //定义保存应用程序RAM起始地址的变量
    uint32_t ram_start = 0;
	  //使用sdk_config.h文件的默认参数配置协议栈，获取应用程序RAM起始地址，保存到变量ram_start
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    //使能BLE协议栈
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    //注册BLE事件回调函数
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}
//初始化电源管理模块
static void power_management_init(void)
{
    ret_code_t err_code;
	//初始化电源管理
    err_code = nrf_pwr_mgmt_init();
	//检查函数返回的错误代码
    APP_ERROR_CHECK(err_code);
}

static void beep_init(void)
{
    //配置用于驱动蜂鸣器的引脚，即配置P0.12为输出
	nrf_gpio_cfg_output(BEEP_PIN);
	//P0.12输出低电平，即蜂鸣器停止鸣响
	nrf_gpio_pin_clear(BEEP_PIN);
}

//初始化指示灯
static void leds_init(void)
{
    ret_code_t err_code;
    //初始化BSP指示灯
    err_code = bsp_init(BSP_INIT_LEDS, NULL);
    APP_ERROR_CHECK(err_code);

}
static void beep_timeout_handler(void * p_context)
{
    ret_code_t err_code = true;
    UNUSED_PARAMETER(p_context);
   nrf_gpio_pin_clear(BEEP_PIN);        //蜂鸣器不响了
   if(light_work_status.light_status == NONELIGHT)     //头灯未开，开头灯
   {
       light_work_choose(&led_infor);
				light_work_status.light_operater = KEYSTART;
   }
   else                                    //头灯开着，关闭头灯
   {
       Light_uninit();
		 	light_work_status.report_flg = true;
       light_work_status.light_status = NONELIGHT;
			 light_work_status.light_operater = FINISHLIGHT;
   }
}

static uint32_t ble_app_data_send(uint8_t   * p_data, uint16_t  * p_lengt)
{
    ret_code_t err_code = true;
    VERIFY_PARAM_NOT_NULL(p_data);
        do
        {
            err_code = ble_uarts_data_send(&m_uarts, p_data, p_lengt, m_conn_handle);
            if ((err_code != NRF_ERROR_INVALID_STATE) &&
                (err_code != NRF_ERROR_RESOURCES) &&
                 (err_code != NRF_ERROR_NOT_FOUND))
            {
                APP_ERROR_CHECK(err_code);
            }
        } while (err_code == NRF_ERROR_RESOURCES);
}
static void DataUpload_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    uint8_t length = 0;
    memset(sendbuff,0,sizeof(uint8_t)*10);
    Package_batterylevel(&batlevel,sendbuff,&length);
    if((!cmdfeedback_flg)&&(!light_work_status.report_flg))
    {
        ble_app_data_send(sendbuff,(uint16_t*)&length);
    }
}
static void GotoSleep_timeout_handler(void * p_context)
{
    ret_code_t err_code = true;
    UNUSED_PARAMETER(p_context);
    beep_start();
   // sleep_flg = true;
	//设置广播指示灯为广播停止（D1指示灯熄灭）
    err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);
    
}

static bool remove_charge_flg = false;
static bool mode_frm_hitolo_flg = false;
static bool charging_flg = false;
static bool full_flg = false;
static void BtChargeStatus_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    uint8_t temlength = 0;
    if((nrf_gpio_pin_read(CHG_TEST)== true)&&(!remove_charge_flg))
    {
        {
            remove_charge_flg = true;
            charge_flg = true;                     //充电器接入标志位
            touch_button_disable();                //禁止触摸按键
            {
                Light_uninit();
                nrf_gpio_pin_clear(LED_1);
                nrf_gpio_pin_clear(LED_2);
            }
        }
    }
    if((nrf_gpio_pin_read(CHG_TEST)==false)&&(remove_charge_flg))   //移除充电器
    {
        remove_charge_flg = false;
        full_flg = false;
        charging_flg = false;
        charge_flg = false;
        led_alarm_stop();
        bsp_board_led_off(3);
        Light_uninit();
        nrf_gpio_pin_clear(LED_1);
        nrf_gpio_pin_clear(LED_2);
        nrf_gpio_pin_clear(BEEP_PIN);
        touch_button_enable();                   //使能按键
    }
    if((nrf_gpio_pin_read(CHG_MODE)==false)&&(!mode_frm_hitolo_flg))
    {
        {
            charging_flg = true;
            mode_frm_hitolo_flg = true;
            full_flg = false;
        }
    }
    if((nrf_gpio_pin_read(CHG_MODE)==true)&&(mode_frm_hitolo_flg))
    {
        mode_frm_hitolo_flg = false;
        full_flg = true;
    }
    if(charge_flg)
    {
        if(charging_flg)
        {
            charging_flg = false;
           // app_timer_start(led_alarm_timer_id, LEDALARM_INTERVAL, NULL);
            led_alarm_start();
        }
        if(full_flg)
        {
            full_flg = false;
            //app_timer_stop(led_alarm_timer_id);
            led_alarm_stop();
            bsp_board_led_on(3);
        }
    }
    else
    {
        if((light_work_status.report_flg)&&(!cmdfeedback_flg))
        { 
						static uint8_t tempbuff[2] = {0};
						tempbuff[0] = light_work_status.light_status;
						tempbuff[1] = light_work_status.light_operater;
            memset(sendbuff,0,sizeof(uint8_t)*10);
            //Package_LedWorkStatus(&(light_work_status.light_status),sendbuff,&temlength);
						Package_LedWorkStatus(tempbuff,sendbuff,&temlength);
            ble_app_data_send(sendbuff,(uint16_t*)&temlength);
            light_work_status.report_flg = false;
        }
        if(sleep_flg)                               //低电量进入休眠
        {
            //sleep_flg = false;
            beep_start();
            saadc_uinit();
            Light_uninit();
            app_timer_stop_all();
            sleep_mode_enter();         //低电量进入休眠
        }
    }
}
//初始化APP定时器模块
static void timers_init(void)
{
    //初始化APP定时器模块
    ret_code_t err_code = app_timer_init();
	  //检查返回值
    APP_ERROR_CHECK(err_code);

    //加入创建用户定时任务的代码，创建用户定时任务。 
    err_code = app_timer_create(&beep_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                beep_timeout_handler);
    APP_ERROR_CHECK(err_code);
       //创建电池测量APP定时器
    
   // err_code = app_timer_create(&m_battery_timer_id,
    //                            APP_TIMER_MODE_REPEATED,
     //                           saadc_timeout_handler);
   // APP_ERROR_CHECK(err_code);  
    //创建数据上报APP定时器
    err_code = app_timer_create(&data_upload_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                DataUpload_timeout_handler);
    APP_ERROR_CHECK(err_code); 
    //创建闪定时器
   // err_code = app_timer_create(&led_alarm_timer_id,
    //                            APP_TIMER_MODE_REPEATED,
   //                            ledalarm_timeout_handler);
   // APP_ERROR_CHECK(err_code); 
    //创建电池状态检验定时，每隔0.1s检测一次。
    err_code = app_timer_create(&BtCharge_status_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                BtChargeStatus_timeout_handler);
    APP_ERROR_CHECK(err_code); 
}

//空闲状态处理函数。如果没有挂起的日志操作，则睡眠直到下一个事件发生后唤醒系统
static void idle_state_handle(void)
{
    //处理挂起的log
//	if (NRF_LOG_PROCESS() == false)
    {
        //运行电源管理，该函数需要放到主循环里面执行
			  nrf_pwr_mgmt_run();
    }
}
static void beep_start(void)
{
    ret_code_t err_code;
    beep_start_pwm();
    err_code = app_timer_start(beep_timer_id, BEEP_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);
}
//初始化按键S1并检测S1状态：启动时S1按键按下，会删除绑定信息
//初始化按键S2为APP按键：无连接状态下按下S2按键，执行禁用白名单广播

/**@brief Function for handling events from the BSP module.
 *
 * @param[in]   event   Event generated by button press.
 */
void bsp_event_handler(bsp_event_t event)
{
    uint32_t err_code;
    switch (event)
    {
        
        case BSP_EVENT_SLEEP:
            break;
        case BSP_EVENT_KEY_0:
            if (m_conn_handle ==BLE_CONN_HANDLE_INVALID)
            {
                beep_start();
            }
            break;
        case BSP_EVENT_DISCONNECT:
            err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BSP_EVENT_WHITELIST_OFF:
            if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
            {
                err_code = ble_advertising_restart_without_whitelist(&m_advertising);
                if (err_code != NRF_ERROR_INVALID_STATE)
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            break;
        default:
            break;
    }
}
#define TOUCH_BUTTON   3
//APP按键事件回调函数
void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
    ret_code_t err_code;
	switch (pin_no)//判断键值
    {
        case TOUCH_BUTTON://是BUTTON_2按键
		if(button_action == APP_BUTTON_PUSH)//按键按下事件
		{
            //连接状态才去断开连接
			if (m_conn_handle == BLE_CONN_HANDLE_ALL)
            {
                //禁用白名单广播
				err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
                if (err_code != NRF_ERROR_INVALID_STATE)
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            else                    //未连接状态
            {
                beep_start();
            }
		}
           break;
        default:
            return; 
    }
}

static void buttons_init()
{
    bsp_event_t startup_event;

    uint32_t err_code = bsp_init(BSP_INIT_LEDS|BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);
    err_code = bsp_btn_ble_init(NULL, &startup_event);
    APP_ERROR_CHECK(err_code);
}
static void charger_detect_init()
{
    	  //配置连接按键S1的IO位输入
	  nrf_gpio_cfg_input(CHG_TEST, NRF_GPIO_PIN_NOPULL);
      nrf_gpio_cfg_input(CHG_MODE, NRF_GPIO_PIN_PULLUP);
}

static void sleep_mode_enter(void)
{
    uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);
    err_code = sd_power_system_off();  
    APP_ERROR_CHECK(err_code);
}
//主函数
int main(void)
{
	//初始化log程序模块
//	log_init();
	//初始化串口
	//uart_config();
	//初始化APP定时器
	timers_init();
   // bat_volt_timer_init();
   // bat_volt_check_start();
    led_alarm_timer_initial();
    Led_TimerCreate();
    //蜂鸣器初始化
    beep_init();
	//初始化指示灯
	leds_init();
	//初始化按键S1，初始化后检测按键S1是否按下，如果按下则删除Flash中存储的绑定信息
	buttons_init();
    charger_detect_init();
    //初始化flash存储外设
    board_fstorage_initial();
    saadc_init();
    bat_volt_check_start();
    saadc_uinit();                  //adc disable
    //以下蓝牙协议栈初始化
	//初始化电源管理
	power_management_init();
	//初始化协议栈
	ble_stack_init();
	//配置GAP参数
	gap_params_init();
	//初始化GATT
	gatt_init();
	//初始化服务
	services_init();
	//初始化广播
	advertising_init();	
	//连接参数协商初始化
    conn_params_init(); 
	//启动广播
    advertising_start();
    //主循环

	while(true)
	{
		//处理挂起的LOG和运行电源管理
		idle_state_handle();

	}
}

