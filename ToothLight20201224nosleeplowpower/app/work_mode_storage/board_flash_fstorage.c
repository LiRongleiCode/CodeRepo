/*********************************************************************
 * INCLUDES
 */
#include "nrf_fstorage.h"
#include "nrf_fstorage_sd.h"
#include "nrf_soc.h"
#include "nrf_log.h"
#include "app_error.h"
//引用FDS头文件
#include "fds.h"
#include "board_flash_fstorage.h"
#include "pca10040.h"
//包含硬件/软件版本信息的记录
static fds_record_t const m_ledinfor_record =
{
    .file_id           = DEVICE_FILE,
    .key               = DEVICE_VERSION_KEY,
    .data.p_data       = &led_infor,
    //记录的长度必须以4字节为单位
    .data.length_words = (sizeof(led_infor) + 3) / sizeof(uint32_t),
};
//包含设备描述信息的记录
static fds_record_t const m_deviceid_record =
{
    .file_id           = DEVICE_FILE,
    .key               = DEVICE_DESP_KEY,
    .data.p_data       = &device_id,
    //记录的长度必须以4字节为单位
    .data.length_words = (sizeof(device_id) + 3) / sizeof(uint32_t),
};
//定义FDS异步操作标志结构体
static my_fds_info_t my_fds_info;
//定义并初始化记录描述符结构体变量
fds_record_desc_t desc = {0};
//定义并初始化记录查找令牌结构体变量
fds_find_token_t tok = {0};
//FDS事件处理函数
static void fds_evt_handler(fds_evt_t const * p_evt)
{
    //判断事件类型
    switch (p_evt->id)
    {
        case FDS_EVT_INIT://FDS初始化事件
			//初始化成功
            if (p_evt->result == NRF_SUCCESS)
            {
                my_fds_info.busy = false;
            }
            break;
        //FDS写记录事件
        case FDS_EVT_WRITE:
        {
            //写记录成功
			if (p_evt->result == NRF_SUCCESS)
            {
                my_fds_info.busy = false;
            }
        } break;
		//FDS更新记录事件
        case FDS_EVT_UPDATE:
        {
            //写记录成功
			if (p_evt->result == NRF_SUCCESS)
            {
                my_fds_info.busy = false;
            }
        } break;
		//FDS碎片整理事件
        case FDS_EVT_GC:
        {
            //碎片整理成功
			if (p_evt->result == NRF_SUCCESS)
            {
                my_fds_info.busy = false;
            }
        } break;
        default:
            break;
    }
}

//等待FDS初始化完成
static void wait_for_fds_ready(void)
{
    while (my_fds_info.busy)
    {
        (void) sd_app_evt_wait();
    }
}
void read_ledinfor_record(void)
{
    ret_code_t rc;	
	//清零tok,从头查找
    memset(&tok, 0x00, sizeof(fds_find_token_t));
	//在DEVICE_FILE文件中查找记录m_version_record
	rc = fds_record_find(DEVICE_FILE, DEVICE_VERSION_KEY, &desc, &tok);
	//查找到记录后，读取记录内容
	if (rc == NRF_SUCCESS)
	{
        fds_flash_record_t temp = {0};
		//打开记录读取记录内容
		rc = fds_record_open(&desc, &temp);
		APP_ERROR_CHECK(rc);
		//static device_version_t temp_version;
		//拷贝记录内容
		memcpy(&led_infor, temp.p_data, sizeof(led_infor_t));
		//读取后，关闭记录
		rc = fds_record_close(&desc);
		APP_ERROR_CHECK(rc);
	}
}
void read_deviceid_record(void)
{
    ret_code_t rc;
	//清零tok,从头查找
    memset(&tok, 0x00, sizeof(fds_find_token_t));
	//在DEVICE_FILE文件中查找记录m_version_record
	rc = fds_record_find(DEVICE_FILE, DEVICE_DESP_KEY, &desc, &tok);
	//查找到记录后，读取记录内容
	if (rc == NRF_SUCCESS)
	{
    fds_flash_record_t temp = {0};
		//打开记录读取记录内容
		rc = fds_record_open(&desc, &temp);
		APP_ERROR_CHECK(rc);
		//static deivce_desp_t temp_desp;
		//拷贝记录内容
		memcpy(&device_id, temp.p_data, sizeof(device_id_t));
		//读取后，关闭记录
		rc = fds_record_close(&desc);
		APP_ERROR_CHECK(rc);
	}
}
void board_fstorage_initial(void)
{
    ret_code_t rc;
    //注册FDS事件回调函数接收FS事件
	(void)fds_register(fds_evt_handler);
	my_fds_info.busy = true;
	rc = fds_init();//初始化FDS
    APP_ERROR_CHECK(rc);	
	//FDS初始化是异步的，因此要等待FDS初始化完成
    wait_for_fds_ready();
	//清零tok,从头查找
	memset(&tok, 0x00, sizeof(fds_find_token_t));
	//在DEVICE_FILE文件中查找记录m_version_record
	rc = fds_record_find(DEVICE_FILE, DEVICE_VERSION_KEY, &desc, &tok);								
	//没有查找到m_version_record记录，写入记录
	if (rc != NRF_SUCCESS)
	{
        my_fds_info.busy = true;
        rc = fds_record_write(&desc, &m_ledinfor_record);
        APP_ERROR_CHECK(rc);
        wait_for_fds_ready();
	}
	
	//清零tok，从头查找
	memset(&tok, 0x00, sizeof(fds_find_token_t));
	//在DEVICE_FILE文件中查找记录m_desp_record
	rc = fds_record_find(DEVICE_FILE, DEVICE_DESP_KEY, &desc, &tok);								
	//没有查找到m_desp_record记录，写入记录
	if (rc != NRF_SUCCESS)
	{
        my_fds_info.busy = true;
        rc = fds_record_write(&desc, &m_deviceid_record);
        APP_ERROR_CHECK(rc);
        wait_for_fds_ready();
	}
}
void write_ledinfor_fds(void)
{
    ret_code_t rc;
	//更新记录m_version_record
	//if((my_fds_info.version_record_update == true) && (my_fds_info.busy == false))
	//{
        //清零tok,从头查找
		memset(&tok, 0x00, sizeof(fds_find_token_t));
	//	my_fds_info.busy = true;
		//在DEVICE_FILE文件中查找记录m_version_record
		rc = fds_record_find(DEVICE_FILE, DEVICE_VERSION_KEY, &desc, &tok);	
		if (rc == NRF_SUCCESS)
		{
            my_fds_info.version_record_update = false;
			//更新记录m_version_record
			rc = fds_record_update(&desc, &m_ledinfor_record);
			APP_ERROR_CHECK(rc);
			wait_for_fds_ready();
		}	
	//}
    /*
    //收集碎片
    if((my_fds_info.gc == true) && (my_fds_info.busy == false))
	{
        my_fds_info.gc = false;
		my_fds_info.busy = true;
		rc = fds_gc();//FDS碎片回收
		APP_ERROR_CHECK(rc);//用错误处理检查函数返回值
		wait_for_fds_ready();
		NRF_LOG_INFO("FDS gc completed");
	}
    */
}
void write_deviceid_fds(void)
{
    ret_code_t rc;
        //清零tok,从头查找
		memset(&tok, 0x00, sizeof(fds_find_token_t));
		//在DEVICE_FILE文件中查找记录m_version_record
		rc = fds_record_find(DEVICE_FILE, DEVICE_DESP_KEY, &desc, &tok);	
		if (rc == NRF_SUCCESS)
		{
            my_fds_info.version_record_update = false;
			//更新记录m_version_record
			rc = fds_record_update(&desc, &m_deviceid_record);
			APP_ERROR_CHECK(rc);
			wait_for_fds_ready();
		}	
}











