#include "ProcRex.h"
#include "stddef.h"
#include "pca10040.h"
#include "led_ctrl.h"
#include "board_flash_fstorage.h"
 static void date_parser(uint8_t const *pbuffer,uint8_t const *plength,uint8_t *readybuff,uint8_t *readylength);
//将应答指令打包
static uint8_t Package_SendCmd(uint8_t cmdId, uint8_t len, const uint8_t *param, uint8_t *readybuff,uint8_t *readylength)  
{
    static uint16_t  crc_result;
    if (len > MAX_SEND_CNT)
    {
        return 0;
    }
    readybuff[0] = HEADER;
    readybuff[1] = RESP;
    readybuff[2] = cmdId;
    readybuff[3] = len;
    memcpy(&readybuff[4], param, len);
    crc_result = crc16_compute(&readybuff[2],len+1+1,NULL);
   // memset(&readybuff[4+len],0,sizeof(uint16_t));
    memcpy(&readybuff[4+len],(uint8_t*)&crc_result,sizeof(uint16_t));
    *readylength = 4+len+2;
    //return ble_app_data_send(buf,(uint16_t*)&length);
    return 0;
}
//打包电池电量
void Package_batterylevel(uint8_t const *batterylevel,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(SEND_BATTERY_LEVEL, sizeof(uint8_t), batterylevel,readybuff,readylength);
}
//打包头灯工作状态
void Package_LedWorkStatus(uint8_t const *ledstatus,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(SEND_LED_STATUS, sizeof(uint8_t)*2, ledstatus,readybuff,readylength);
}
//打包SN
static void Package_Deviceid(uint8_t const *id,uint8_t sizeid,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(GET_ID, sizeid, id,readybuff,readylength);
}
//打包停止反馈
static void Package_Startstop(uint8_t const *startflg,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(START_STOP_CMD, sizeof(uint8_t), startflg,readybuff,readylength);
}
//打包头灯工作模式
static void Package_Ledworkmode(uint8_t const *workmode,uint8_t sizemode,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(GET_LED_MODE, sizemode, workmode,readybuff,readylength);
}
//打包头灯设置反馈信息
static void Package_Ledsetfeedback(uint8_t const *feedback,uint8_t sizefeedback,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(SET_LED, sizefeedback, feedback,readybuff,readylength);
}
//解析app发送过来的指令
 void receive_date_parser(uint8_t const *pbuffer,uint8_t const *plength,uint8_t *readybuff,uint8_t *readylength)
 {
     uint8_t bufferlength = 0;
     uint8_t index = 0;
   //  bufferlength = *plength;
     if ((pbuffer == NULL)||(plength == NULL))
     {
         return;
     }
     if(*plength > 4)
     {
         for(uint8_t i =0; i<*plength;i++)
         {
             if((pbuffer[i] == HEADER)&&(pbuffer[i+1] == SEND))
             index = i;
             break;
         }
         bufferlength = pbuffer[index+3]+3;
        date_parser(&pbuffer[index+2],&bufferlength,readybuff,readylength);
     }
 }
 
 static void date_parser(uint8_t const *pbuffer,uint8_t const *plength,uint8_t *readybuff,uint8_t *readylength)
 {
     uint8_t length = *plength;
    // uint8_t success = SETSUCCESS;
     static uint8_t tempbuff[10] = {0};
     switch(pbuffer[0])
     {
         case GET_ID:
         { 
             if((pbuffer[1] == 0x01)&&(length>3))
             {
							 read_deviceid_record();
                Package_Deviceid(device_id.serialsnum,sizeof(uint8_t)*6,readybuff,readylength);               //打包SN号
             }
         }
         break;
         case SET_LED:                                           //设置头灯工作模式
         {
             if((pbuffer[1] == 0x06)&&(length>6)&&(!charge_flg))
             {
                led_infor.work_mode = (Led_work_mode_t)pbuffer[3];
                led_infor.blue_brightness = (Brightness_Adj_t)pbuffer[4];
                led_infor.red_brightness = (Brightness_Adj_t) pbuffer[5];
                led_infor.blue_timer  =  pbuffer[6];
                led_infor.red_timer = pbuffer[7];
                tempbuff[0] = SETSUCCESS;
                write_ledinfor_fds();
                Package_Ledsetfeedback(tempbuff,sizeof(uint8_t),readybuff,readylength);
                stop_all_light_timer();     //停止所有头灯定时器
                light_work_choose(&led_infor); //重新选择头灯工作模式
             }
                                            //TODO: store the command
                                            //re start the led
         }
         break;
        case GET_LED_MODE:
         {
             if((pbuffer[1] == 0x01)&&(length>2))
             {
                 tempbuff[0] = 0x00;
                 tempbuff[1] = led_infor.work_mode;
                 tempbuff[2] = led_infor.blue_brightness;
                 tempbuff[3] = led_infor.red_brightness;
                 tempbuff[4] = led_infor.blue_timer;
                 tempbuff[5] = led_infor.red_timer;
                 Package_Ledworkmode(tempbuff,sizeof(uint8_t)*6,readybuff,readylength);
             }
         }
         break;
        case START_STOP_CMD:                            //APP发送头灯停止工作指令  //to do add start stop.
         {
             /*
             if((pbuffer[1] == 0x01)&&(pbuffer[2] == 0x00))
             {
                 tempbuff[0] = SETSTOP;
                 Package_Startstop(&tempbuff[0],readybuff,readylength);  
                 gotosleep_rightnow = true;                                      //准备进入休眠
             }
             */
             if((pbuffer[1] == 0x01)&&(!charge_flg))
             {
                 if(pbuffer[2] == 0x01)
                 {
                     light_work_choose(&led_infor);
										 light_work_status.light_operater = APPSTART;
                 }
                 if(pbuffer[2] == 0x00)
                 {
                     Light_uninit();
											light_work_status.report_flg = true;
                     light_work_status.light_status = NONELIGHT;
										 light_work_status.light_operater = FINISHLIGHT;
                 }
                 tempbuff[0] = SETSTOP;
                 Package_Startstop(&tempbuff[0],readybuff,readylength);  
                 gotosleep_rightnow = true;                                      //准备进入休眠
             }
                                      //TODO: start command
         }
         break;
         default:
         break;
     }
 }
 /*
//字符串转十六进制
void StringToHex(char *str, uint8_t *strhex)
{
	uint8_t i,cnt=0;
	char *p = str;             //指针p初始化为指向str
	uint8_t len = strlen(str); //获取字符串中的字符个数
	while(*p != '\0') {        //结束符判断
		for (i = 0; i < len; i ++)  //循环判断当前字符是数字还是小写字符还是大写字母
		{
			if ((*p >= '0') && (*p <= '9')) //当前字符为0~9
				strhex[cnt] = *p - '0' + 0x30;//转为十六进制
			if ((*p >= 'A') && (*p <= 'Z')) //当前字符为大写字母A~Z
				strhex[cnt] = *p - 'A' + 0x41;//转为十六进制
			if ((*p >= 'a') && (*p <= 'z')) //当前字符为小写字母a~z时

				strhex[cnt] = *p - 'a' + 0x61;  //转为十六进制
			p ++;    //指向下一个字符
			cnt ++;  
		}
	}
}

sprintf(buf, "%02x%02x%02x%02x%02x", port[0], port[1], port[2], port[3], port[4]);  //将16进制转换位ASSII
*/
