/****************************************Copyright (c)************************************************
**                                      [����ķ�Ƽ�]
**                                        IIKMSIK 
**                            �ٷ����̣�https://acmemcu.taobao.com
**                            �ٷ���̳��http://www.e930bbs.com
**                                   
**--------------File Info-----------------------------------------------------------------------------
** File name         : main.c
** Last modified Date: 2020-8-26        
** Last Version      :		   
** Descriptions      : ʹ�õ�SDK�汾-SDK_16.0
**						
**----------------------------------------------------------------------------------------------------
** Created by        : [����ķ]
** Created date      : 2018-12-24
** Version           : 1.0
** Descriptions      : ������ [18-2��ʹ�ÿɽ���˽�е�ַ] �����ӵĻ������޸ģ����ӵĹ������£�
**                   : 1)	����ʹ�ÿɽ���˽�е�ַʵ����˽����ַ�ı���������Ϊ60�루����Ϊ������ʱ����۲��ַ�ı仯���������Ĺ淶�����ַ�ı�����Ϊ 15���ӣ���
**                   : 2)	���������˰��������������������״��������к�����������ɹ���԰󶨣�����¼�������ĵ�ַ��������������:
**                   : 3)	δ����״̬�°��¿������ϵ�S2������������ִ�н��ð������㲥����ʱ��������Ժ��µ��������Ӱ󶨣�����������������
**---------------------------------------------------------------------------------------------------*/
//���õ�C��ͷ�ļ�
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
//Log��Ҫ���õ�ͷ�ļ�
//#include "nrf_log.h"
//#include "nrf_log_ctrl.h"
//#include "nrf_log_default_backends.h"
//APP��ʱ����Ҫ���õ�ͷ�ļ�
#include "app_timer.h"

#include "bsp_btn_ble.h"
//�㲥��Ҫ���õ�ͷ�ļ�
#include "ble_advdata.h"
#include "ble_advertising.h"
//��Դ������Ҫ���õ�ͷ�ļ�
#include "nrf_pwr_mgmt.h"
//SoftDevice handler configuration��Ҫ���õ�ͷ�ļ�
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "bsp_btn_ble.h"
//����д��ģ����Ҫ���õ�ͷ�ļ�
#include "nrf_ble_qwr.h"
//GATT��Ҫ���õ�ͷ�ļ�
#include "nrf_ble_gatt.h"
//���Ӳ���Э����Ҫ���õ�ͷ�ļ�
#include "ble_conn_params.h"
//����͸����Ҫ���õ�ͷ�ļ�
#include "my_ble_uarts.h"
//����͸����Ҫ���õ�ͷ�ļ�
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

//����FDSͷ�ļ�
//#include "fds.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
//����ļ���ȡ
#include "board_flash_fstorage.h"
#include "ProcRex.h"
#include "led_ctrl.h"
#include "led_alarm.h"
#include "bat_volt_check.h"

#define SN                              	"D0004"                          //
#define BRAND                            "YNBY"
//#define DEVICE_NAME                     "YNBYD0002"                      // �豸�����ַ��� 

#define UARTS_SERVICE_UUID_TYPE         BLE_UUID_TYPE_VENDOR_BEGIN         // ����͸������UUID���ͣ������Զ���UUID
#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(100, UNIT_1_25_MS)   // ��С���Ӽ�� (0.1 ��) 
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(200, UNIT_1_25_MS)   // ������Ӽ�� (0.2 ��) 
#define SLAVE_LATENCY                   0                                  // �ӻ��ӳ� 
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)    // �ල��ʱ(4 ��) 



#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)              // �����״ε���sd_ble_gap_conn_param_update()�����������Ӳ����ӳ�ʱ�䣨5�룩
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)             // ����ÿ�ε���sd_ble_gap_conn_param_update()�����������Ӳ����ļ��ʱ�䣨30�룩
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                  // ����������Ӳ���Э��ǰ�������Ӳ���Э�̵���������3�Σ�

#define APP_ADV_INTERVAL                800//320                                // �㲥��� 500ms(200ms)����λ0.625 ms 
#define APP_ADV_DURATION                0//18000//0                                  // �㲥����ʱ�䣬��λ��10ms������Ϊ0��ʾ����ʱ 180s

#define APP_BLE_OBSERVER_PRIO           3               //Ӧ�ó���BLE�¼����������ȼ���Ӧ�ó������޸ĸ���ֵ
#define APP_BLE_CONN_CFG_TAG            1               //SoftDevice BLE���ñ�־

#define UART_TX_BUF_SIZE 256       //���ڷ��ͻ����С���ֽ�����
#define UART_RX_BUF_SIZE 256       //���ڽ��ջ����С���ֽ�����

#define BUTTON_DETECTION_DELAY          APP_TIMER_TICKS(50)
//����SAADC�������������С ���������������Ҫ����������SAADC�������ٸ����ݺ�����¼��ж�
//#define SAMPLES_BUFFER_LEN 5


//����stack dump�Ĵ�����룬��������ջ����ʱȷ����ջλ��
#define DEAD_BEEF                         0xDEADBEEF   
//PWM��������ʵ��ID,ID�������Ŷ�Ӧ��0:PWM0 1:PWM1 2:PWM2
//#define PWM_INSTANCE  0 
//��������Ϊm_pwm0��PWM��������ʵ��������Ϊ0��ʾ��ʵ����Ӧ��PWM����ΪPWM0
//��������������������P0.12
#define  BEEP_PIN   12

APP_TIMER_DEF(beep_timer_id);
//APP_TIMER_DEF(m_battery_timer_id);                       //��ز���APP��ʱ��
APP_TIMER_DEF(data_upload_timer_id);                     //���ݶ�ʱ�ϴ�
APP_TIMER_DEF(goto_sleep_timer_id);                     //���߶�ʱ
//APP_TIMER_DEF(led_alarm_timer_id);                     //alarm
APP_TIMER_DEF(BtCharge_status_timer_id);                     //���״̬���
//����SAADC�����������飬���ﶨ���2ά���飬Ҳ���Զ����2��һά���飬���Ǳ�������һ����
//static nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_BUFFER_LEN];
        
//#define BATTERY_LEVEL_MEAS_INTERVAL     APP_TIMER_TICKS(1000)              // ��ص������Լ����5��
#define BEEP_INTERVAL                   APP_TIMER_TICKS(200)  //500ms,0.5s
#define DATAUPLOAD_INTERVAL             APP_TIMER_TICKS(5000)              //���ݼ��1S�Ӹ���һ��
#define GOTOSLEEP_INTERVAL              APP_TIMER_TICKS(1000*60*3)   //3min
//#define LEDALARM_INTERVAL               APP_TIMER_TICKS(200)       //0.2S
#define BTCHARGESTATUS_INTERVAL         APP_TIMER_TICKS(100)       //0.1S

//APP��ʱ���¼����������ú���������SAADC����
//static void saadc_timeout_handler(void * p_context);
void sleep_mode_enter();
uint32_t ble_app_data_send(uint8_t   * p_data, uint16_t  * p_lengt);
static void beep_start(void);
uint16_t value = 0;
uint8_t sendbuff[10] = {0};

BLE_UARTS_DEF(m_uarts, NRF_SDH_BLE_TOTAL_LINK_COUNT);                 //��������Ϊm_uarts�Ĵ���͸������ʵ��
NRF_BLE_GATT_DEF(m_gatt);               //��������Ϊm_gatt��GATTģ��ʵ��
NRF_BLE_QWR_DEF(m_qwr);                 //����һ������Ϊm_qwr���Ŷ�д��ʵ��
BLE_ADVERTISING_DEF(m_advertising);     //��������Ϊm_advertising�Ĺ㲥ģ��ʵ��

//�ñ������ڱ������Ӿ������ʼֵ����Ϊ������
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID; 
//���͵�������ݳ���
static uint16_t   m_ble_uarts_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            
static bool uart_enabled = false;
static bool first_flag = false;
static bool adv_start_flg = false;
//���崮��͸������UUID�б�
static ble_uuid_t m_adv_uuids[]          =                                          
{
    {BLE_UUID_UARTS_SERVICE, UARTS_SERVICE_UUID_TYPE}
};


//�豸�������飺���� ��ŵ����
//const char device_name[12] = {0xE4,0xB8,0x89,0xE8,0xAF,0xBA,0xE7,0x89,0x99,0xE7,0x81,0xAF};	

//GAP������ʼ�����ú���������Ҫ��GAP�����������豸���ƣ������������ѡ���Ӳ���
static void gap_params_init(void)
{
    ret_code_t              err_code;
	  //�������Ӳ����ṹ�����
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;
    //����GAP�İ�ȫģʽ
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    //����GAP�豸���ƣ�ʹ��Ӣ���豸����
		char *devicename = malloc(strlen(SN) + strlen(BRAND) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(devicename, BRAND);
    strcat(devicename, SN);
    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                              (const uint8_t *)devicename,
                                              strlen(devicename));
	
	  //����GAP�豸���ƣ�ʹ���������豸����
 //   err_code = sd_ble_gap_device_name_set(&sec_mode,
  //                                        (const uint8_t *)device_name,
  //                                        sizeof(device_name));
																					
    //��麯�����صĴ������
		APP_ERROR_CHECK(err_code);
																				
    //������ѡ���Ӳ���������ǰ������gap_conn_params
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;//��С���Ӽ��
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;//��С���Ӽ��
    gap_conn_params.slave_latency     = SLAVE_LATENCY;    //�ӻ��ӳ�
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT; //�ල��ʱ
    //����Э��ջAPI sd_ble_gap_ppcp_set����GAP����
    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);												
}
//GATT�¼����������ú����д���MTU�����¼�
void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
    //�����MTU�����¼�
	if ((m_conn_handle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED))
    {
        //���ô���͸���������Ч���ݳ��ȣ�MTU-opcode-handle��
        m_ble_uarts_max_data_len = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
    }
}
//��ʼ��GATT����ģ��
static void gatt_init(void)
{
    //��ʼ��GATT����ģ��
	ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
	//��麯�����صĴ������
    APP_ERROR_CHECK(err_code);
	//����ATT MTU�Ĵ�С,�������õ�ֵΪ247
	err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);
}

//�Ŷ�д���¼������������ڴ����Ŷ�д��ģ��Ĵ���
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    //���������
	APP_ERROR_HANDLER(nrf_error);
}
//�����¼��ص����������ڳ�ʼ��ʱע�ᣬ�ú������ж��¼����Ͳ����д���
//�����յ����ݳ��ȴﵽ�趨�����ֵ���߽��յ����з�������Ϊһ�����ݽ�����ɣ�֮�󽫽��յ����ݷ��͸�����
void uart_event_handle(app_uart_evt_t * p_event)
{
    static uint8_t data_array[BLE_UARTS_MAX_DATA_LEN];
    static uint8_t index = 0;
    uint32_t       err_code;
    //�ж��¼�����
    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY://���ڽ����¼� ͨ�����ڽ����������ݶ�APP�ϴ��Ĵ���
            UNUSED_VARIABLE(app_uart_get(&data_array[index]));
            index++;
            //���մ������ݣ������յ����ݳ��ȴﵽm_ble_uarts_max_data_len���߽��յ����з�����Ϊһ�����ݽ������
            if ((data_array[index - 1] == '\n') ||
                (data_array[index - 1] == '\r') ||
                (index >= m_ble_uarts_max_data_len))
            {
                if (index > 1)
                {
                    //���ڽ��յ�����ʹ��notify���͸�BLE����
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
        //ͨѶ�����¼������������
        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);                     //���ε�����ֹδ�Ӵ����߸�λ
            break;
        //FIFO�����¼������������
        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;
        default:
            break;
    }
}
//��������
void uart_config(void)
{
    uint32_t err_code;
	
	//���崮��ͨѶ�������ýṹ�岢��ʼ��
    const app_uart_comm_params_t comm_params =
    {
        RX_PIN_NUMBER,//����uart��������
        TX_PIN_NUMBER,//����uart��������
        RTS_PIN_NUMBER,//����uart RTS���ţ����عرպ���Ȼ������RTS��CTS���ţ����������������ԣ������������������ţ����������Կ���ΪIOʹ��
        CTS_PIN_NUMBER,//����uart CTS����
        APP_UART_FLOW_CONTROL_DISABLED,//�ر�uartӲ������
        false,//��ֹ��ż����
        NRF_UART_BAUDRATE_115200//uart����������Ϊ115200bps
    };
    //��ʼ�����ڣ�ע�ᴮ���¼��ص�����
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
	if(uart_enabled == false)//��ʼ������
	{
		uart_config();
		uart_enabled = true;
	}
	else
	{
		app_uart_close();//����ʼ������
		uart_enabled = false;
	}
}
//����͸���¼��ص�����������͸�������ʼ��ʱע�ᣬ���ʹ�APP�����������ݡ�
static void uarts_data_handler(ble_uarts_evt_t * p_evt)
{
    uint32_t err_code;
    uint8_t length = 0;
    
    	  //֪ͨʹ�ܺ�ų�ʼ������
	//  if (p_evt->type == BLE_NUS_EVT_COMM_STARTED)
		{
	//		uart_reconfig();
		}
		//֪ͨ�رպ󣬹رմ���
	//	else if(p_evt->type == BLE_NUS_EVT_COMM_STOPPED)
		{
	//	  uart_reconfig();
		}
        uart_enabled = true;
	  //�ж��¼�����:���յ��������¼�
    if ((p_evt->type == BLE_UARTS_EVT_RX_DATA)&&(uart_enabled == true))
    {
        cmdfeedback_flg = true;                                  //һ��APP�����ݹ���������λ
        memset(sendbuff,0,sizeof(uint8_t)*10);
				receive_date_parser(p_evt->params.rx_data.p_data,(uint8_t*)&(p_evt->params.rx_data.length),sendbuff,&length);
        ble_app_data_send(sendbuff,(uint16_t*)&length);
				p_evt->params.rx_data.length = 0;
        cmdfeedback_flg = false;                          //һ����Ӧ�����
    }
}
//�����ʼ����������ʼ���Ŷ�д��ģ��ͳ�ʼ��Ӧ�ó���ʹ�õķ���
static void services_init(void)
{
    ret_code_t         err_code;
	uint8_t tempsn[5];
	//���崮��͸����ʼ���ṹ��
	ble_uarts_init_t     uarts_init;
	//�����Ŷ�д���ʼ���ṹ�����
    nrf_ble_qwr_init_t qwr_init = {0};

    //�Ŷ�д���¼�������
    qwr_init.error_handler = nrf_qwr_error_handler;
    //��ʼ���Ŷ�д��ģ��
    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
	//��麯������ֵ
    APP_ERROR_CHECK(err_code);
    
		
	/*------------------���´����ʼ������͸������-------------*/
	//���㴮��͸�������ʼ���ṹ��
	memset(&uarts_init, 0, sizeof(uarts_init));
	//���ô���͸���¼��ص�����
    uarts_init.data_handler = uarts_data_handler;
    //��ʼ������͸������
    err_code = ble_uarts_init(&m_uarts, &uarts_init);
    APP_ERROR_CHECK(err_code);
    /*------------------������ص���ˢ��-END-----------------*/
   // err_code = app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL);         //һ������������ص����ɼ���
   // APP_ERROR_CHECK(err_code); 
    /*------------------������س��״̬���-END-----------------*/
    err_code = app_timer_start(BtCharge_status_timer_id, BTCHARGESTATUS_INTERVAL, NULL);         //һ������������س��״̬��⡣
    APP_ERROR_CHECK(err_code); 
    read_ledinfor_record();
		//StringToHex(SN,tempsn);
		device_id.serialsnum[0] = 0;
		memcpy(device_id.serialsnum+1,(uint8_t *)SN,5);
		write_deviceid_fds();
    //read_deviceid_record();
}

//���Ӳ���Э��ģ���¼�������
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    ret_code_t err_code;
    //�ж��¼����ͣ������¼�����ִ�ж���
	//���Ӳ���Э��ʧ��
    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
	//���Ӳ���Э�̳ɹ�
	if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_SUCCEEDED)
    {
       //���ܴ���;
    }
}

//���Ӳ���Э��ģ��������¼�������nrf_error�����˴�����룬ͨ��nrf_error���Է���������Ϣ
static void conn_params_error_handler(uint32_t nrf_error)
{
    //���������
	  APP_ERROR_HANDLER(nrf_error);
}


//���Ӳ���Э��ģ���ʼ��
static void conn_params_init(void)
{
    ret_code_t             err_code;
	  //�������Ӳ���Э��ģ���ʼ���ṹ��
    ble_conn_params_init_t cp_init;
    //����֮ǰ������
    memset(&cp_init, 0, sizeof(cp_init));
    //����ΪNULL����������ȡ���Ӳ���
    cp_init.p_conn_params                  = NULL;
	  //���ӻ�����֪ͨ���״η������Ӳ�����������֮���ʱ������Ϊ5��
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
	  //ÿ�ε���sd_ble_gap_conn_param_update()�����������Ӳ������������֮��ļ��ʱ������Ϊ��30��
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
	  //�������Ӳ���Э��ǰ�������Ӳ���Э�̵�����������Ϊ��3��
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
	  //���Ӳ������´������¼���ʼ��ʱ
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
	  //���Ӳ�������ʧ�ܲ��Ͽ�����
    cp_init.disconnect_on_fail             = false;
	  //ע�����Ӳ��������¼����
    cp_init.evt_handler                    = on_conn_params_evt;
	  //ע�����Ӳ������´����¼����
    cp_init.error_handler                  = conn_params_error_handler;
    //���ÿ⺯���������Ӳ������³�ʼ���ṹ��Ϊ�����������ʼ�����Ӳ���Э��ģ��
    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}
//�㲥�¼�������
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    ret_code_t err_code;
    //�жϹ㲥�¼�����
    switch (ble_adv_evt)
    {
        //���ٹ㲥�����¼������ٹ㲥�������������¼�
        case BLE_ADV_EVT_FAST:
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);                          //���ڹ㲥״̬���رյ�20201107
            APP_ERROR_CHECK(err_code);
            break;
            //�㲥IDLE�¼����㲥��ʱ���������¼�
        case BLE_ADV_EVT_IDLE:
            //���ù㲥ָʾ��Ϊ�㲥ֹͣ��D1ָʾ��Ϩ��
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            APP_ERROR_CHECK(err_code);
            break;
        default:
            break;
    }
}

//�����㲥���ú������õ�ģʽ����͹㲥��ʼ�������õĹ㲥ģʽһ��
static void advertising_start()
{
    
    ret_code_t ret = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
	//��麯�����صĴ������
    APP_ERROR_CHECK(ret);
}
//�㲥��ʼ��
static void advertising_init(void)
{
    ret_code_t             err_code;
	  //����㲥��ʼ�����ýṹ�����
    ble_advertising_init_t init;
    //����֮ǰ������
    memset(&init, 0, sizeof(init));
    //�豸�������ͣ�ȫ��
    init.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
	  //�Ƿ������ۣ�����
    init.advdata.include_appearance      = false;
	  //Flag:һ��ɷ���ģʽ����֧��BR/EDR
    init.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
	  //UUID�ŵ�ɨ����Ӧ����
	  init.srdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.srdata.uuids_complete.p_uuids  = m_adv_uuids;
	
    //���ù㲥ģʽΪ���ٹ㲥
    init.config.ble_adv_fast_enabled  = true;
	  //���ù㲥����͹㲥����ʱ��
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;
    //�㲥�¼��ص�����
    init.evt_handler = on_adv_evt;
    //��ʼ���㲥
    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);
    //���ù㲥���ñ�ǡ�APP_BLE_CONN_CFG_TAG�����ڸ��ٹ㲥���õı�ǣ�����Ϊδ��Ԥ����һ���������ڽ�����SoftDevice�汾�У�
    //����ʹ��sd_ble_gap_adv_set_configure()�����µĹ㲥����
	//��ǰSoftDevice�汾��S132 V7.0.1�汾��֧�ֵ����㲥������Ϊ1�����APP_BLE_CONN_CFG_TAGֻ��д1��
    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
    sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_ADV, BLE_GAP_ADV_SET_HANDLE_NOT_SET, -40);
}

//BLE�¼�������
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    ret_code_t err_code = NRF_SUCCESS;
    //�ж�BLE�¼����ͣ������¼�����ִ����Ӧ����
    switch (p_ble_evt->header.evt_id)
    {
        //�Ͽ������¼�
		case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;			    
            err_code = app_timer_stop(data_upload_timer_id);    //ֹͣ���¡�
            APP_ERROR_CHECK(err_code);
            break;
				
        //�����¼�
        case BLE_GAP_EVT_CONNECTED:
            //����ָʾ��״̬Ϊ����״̬����ָʾ��D1����
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            err_code = app_timer_start(data_upload_timer_id, DATAUPLOAD_INTERVAL, NULL);         //���ӳɹ����������ݡ�
            APP_ERROR_CHECK(err_code);               
            //�������Ӿ��
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            //�����Ӿ��������Ŷ�д��ʵ����������Ŷ�д��ʵ���͸����ӹ��������������ж�����ӵ�ʱ��ͨ��������ͬ���Ŷ�д��ʵ�����ܷ��㵥�������������
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            break;
				
        //PHY�����¼�
        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
       //     NRF_LOG_DEBUG("PHY update request.");
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
			//��ӦPHY���¹��
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;
        	//��ȫ���������¼�
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            //��֧�����
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
                
			//ϵͳ���Է������ڵȴ���
		case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            //ϵͳ����û�д洢������ϵͳ����
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;
            //GATT�ͻ��˳�ʱ�¼�
        case BLE_GATTC_EVT_TIMEOUT:
            //�Ͽ���ǰ����
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;
				
            //GATT��������ʱ�¼�
        case BLE_GATTS_EVT_TIMEOUT:
			//�Ͽ���ǰ����
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            break;
    }
}

//��ʼ��BLEЭ��ջ
static void ble_stack_init(void)
{
    ret_code_t err_code;
    
    
    //����ʹ��SoftDevice���ú����л����sdk_config.h�ļ��е�Ƶʱ�ӵ����������õ�Ƶʱ��
    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);
    
    //���屣��Ӧ�ó���RAM��ʼ��ַ�ı���
    uint32_t ram_start = 0;
	  //ʹ��sdk_config.h�ļ���Ĭ�ϲ�������Э��ջ����ȡӦ�ó���RAM��ʼ��ַ�����浽����ram_start
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    //ʹ��BLEЭ��ջ
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    //ע��BLE�¼��ص�����
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}
//��ʼ����Դ����ģ��
static void power_management_init(void)
{
    ret_code_t err_code;
	//��ʼ����Դ����
    err_code = nrf_pwr_mgmt_init();
	//��麯�����صĴ������
    APP_ERROR_CHECK(err_code);
}

static void beep_init(void)
{
    //�����������������������ţ�������P0.12Ϊ���
	nrf_gpio_cfg_output(BEEP_PIN);
	//P0.12����͵�ƽ����������ֹͣ����
	nrf_gpio_pin_clear(BEEP_PIN);
}

//��ʼ��ָʾ��
static void leds_init(void)
{
    ret_code_t err_code;
    //��ʼ��BSPָʾ��
    err_code = bsp_init(BSP_INIT_LEDS, NULL);
    APP_ERROR_CHECK(err_code);

}
static void beep_timeout_handler(void * p_context)
{
    ret_code_t err_code = true;
    UNUSED_PARAMETER(p_context);
   nrf_gpio_pin_clear(BEEP_PIN);        //������������
   if(light_work_status.light_status == NONELIGHT)     //ͷ��δ������ͷ��
   {
       light_work_choose(&led_infor);
				light_work_status.light_operater = KEYSTART;
   }
   else                                    //ͷ�ƿ��ţ��ر�ͷ��
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
	//���ù㲥ָʾ��Ϊ�㲥ֹͣ��D1ָʾ��Ϩ��
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
            charge_flg = true;                     //����������־λ
            touch_button_disable();                //��ֹ��������
            {
                Light_uninit();
                nrf_gpio_pin_clear(LED_1);
                nrf_gpio_pin_clear(LED_2);
            }
        }
    }
    if((nrf_gpio_pin_read(CHG_TEST)==false)&&(remove_charge_flg))   //�Ƴ������
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
        touch_button_enable();                   //ʹ�ܰ���
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
        if(sleep_flg)                               //�͵�����������
        {
            //sleep_flg = false;
            beep_start();
            saadc_uinit();
            Light_uninit();
            app_timer_stop_all();
            sleep_mode_enter();         //�͵�����������
        }
    }
}
//��ʼ��APP��ʱ��ģ��
static void timers_init(void)
{
    //��ʼ��APP��ʱ��ģ��
    ret_code_t err_code = app_timer_init();
	  //��鷵��ֵ
    APP_ERROR_CHECK(err_code);

    //���봴���û���ʱ����Ĵ��룬�����û���ʱ���� 
    err_code = app_timer_create(&beep_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                beep_timeout_handler);
    APP_ERROR_CHECK(err_code);
       //������ز���APP��ʱ��
    
   // err_code = app_timer_create(&m_battery_timer_id,
    //                            APP_TIMER_MODE_REPEATED,
     //                           saadc_timeout_handler);
   // APP_ERROR_CHECK(err_code);  
    //���������ϱ�APP��ʱ��
    err_code = app_timer_create(&data_upload_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                DataUpload_timeout_handler);
    APP_ERROR_CHECK(err_code); 
    //��������ʱ��
   // err_code = app_timer_create(&led_alarm_timer_id,
    //                            APP_TIMER_MODE_REPEATED,
   //                            ledalarm_timeout_handler);
   // APP_ERROR_CHECK(err_code); 
    //�������״̬���鶨ʱ��ÿ��0.1s���һ�Ρ�
    err_code = app_timer_create(&BtCharge_status_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                BtChargeStatus_timeout_handler);
    APP_ERROR_CHECK(err_code); 
}

//����״̬�����������û�й������־��������˯��ֱ����һ���¼���������ϵͳ
static void idle_state_handle(void)
{
    //��������log
//	if (NRF_LOG_PROCESS() == false)
    {
        //���е�Դ�����ú�����Ҫ�ŵ���ѭ������ִ��
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
//��ʼ������S1�����S1״̬������ʱS1�������£���ɾ������Ϣ
//��ʼ������S2ΪAPP������������״̬�°���S2������ִ�н��ð������㲥

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
//APP�����¼��ص�����
void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
    ret_code_t err_code;
	switch (pin_no)//�жϼ�ֵ
    {
        case TOUCH_BUTTON://��BUTTON_2����
		if(button_action == APP_BUTTON_PUSH)//���������¼�
		{
            //����״̬��ȥ�Ͽ�����
			if (m_conn_handle == BLE_CONN_HANDLE_ALL)
            {
                //���ð������㲥
				err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
                if (err_code != NRF_ERROR_INVALID_STATE)
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            else                    //δ����״̬
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
    	  //�������Ӱ���S1��IOλ����
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
//������
int main(void)
{
	//��ʼ��log����ģ��
//	log_init();
	//��ʼ������
	//uart_config();
	//��ʼ��APP��ʱ��
	timers_init();
   // bat_volt_timer_init();
   // bat_volt_check_start();
    led_alarm_timer_initial();
    Led_TimerCreate();
    //��������ʼ��
    beep_init();
	//��ʼ��ָʾ��
	leds_init();
	//��ʼ������S1����ʼ�����ⰴ��S1�Ƿ��£����������ɾ��Flash�д洢�İ���Ϣ
	buttons_init();
    charger_detect_init();
    //��ʼ��flash�洢����
    board_fstorage_initial();
    saadc_init();
    bat_volt_check_start();
    saadc_uinit();                  //adc disable
    //��������Э��ջ��ʼ��
	//��ʼ����Դ����
	power_management_init();
	//��ʼ��Э��ջ
	ble_stack_init();
	//����GAP����
	gap_params_init();
	//��ʼ��GATT
	gatt_init();
	//��ʼ������
	services_init();
	//��ʼ���㲥
	advertising_init();	
	//���Ӳ���Э�̳�ʼ��
    conn_params_init(); 
	//�����㲥
    advertising_start();
    //��ѭ��

	while(true)
	{
		//��������LOG�����е�Դ����
		idle_state_handle();

	}
}

