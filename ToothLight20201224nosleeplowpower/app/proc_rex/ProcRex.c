#include "ProcRex.h"
#include "stddef.h"
#include "pca10040.h"
#include "led_ctrl.h"
#include "board_flash_fstorage.h"
 static void date_parser(uint8_t const *pbuffer,uint8_t const *plength,uint8_t *readybuff,uint8_t *readylength);
//��Ӧ��ָ����
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
//�����ص���
void Package_batterylevel(uint8_t const *batterylevel,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(SEND_BATTERY_LEVEL, sizeof(uint8_t), batterylevel,readybuff,readylength);
}
//���ͷ�ƹ���״̬
void Package_LedWorkStatus(uint8_t const *ledstatus,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(SEND_LED_STATUS, sizeof(uint8_t)*2, ledstatus,readybuff,readylength);
}
//���SN
static void Package_Deviceid(uint8_t const *id,uint8_t sizeid,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(GET_ID, sizeid, id,readybuff,readylength);
}
//���ֹͣ����
static void Package_Startstop(uint8_t const *startflg,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(START_STOP_CMD, sizeof(uint8_t), startflg,readybuff,readylength);
}
//���ͷ�ƹ���ģʽ
static void Package_Ledworkmode(uint8_t const *workmode,uint8_t sizemode,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(GET_LED_MODE, sizemode, workmode,readybuff,readylength);
}
//���ͷ�����÷�����Ϣ
static void Package_Ledsetfeedback(uint8_t const *feedback,uint8_t sizefeedback,uint8_t *readybuff,uint8_t *readylength)
{
    Package_SendCmd(SET_LED, sizefeedback, feedback,readybuff,readylength);
}
//����app���͹�����ָ��
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
                Package_Deviceid(device_id.serialsnum,sizeof(uint8_t)*6,readybuff,readylength);               //���SN��
             }
         }
         break;
         case SET_LED:                                           //����ͷ�ƹ���ģʽ
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
                stop_all_light_timer();     //ֹͣ����ͷ�ƶ�ʱ��
                light_work_choose(&led_infor); //����ѡ��ͷ�ƹ���ģʽ
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
        case START_STOP_CMD:                            //APP����ͷ��ֹͣ����ָ��  //to do add start stop.
         {
             /*
             if((pbuffer[1] == 0x01)&&(pbuffer[2] == 0x00))
             {
                 tempbuff[0] = SETSTOP;
                 Package_Startstop(&tempbuff[0],readybuff,readylength);  
                 gotosleep_rightnow = true;                                      //׼����������
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
                 gotosleep_rightnow = true;                                      //׼����������
             }
                                      //TODO: start command
         }
         break;
         default:
         break;
     }
 }
 /*
//�ַ���תʮ������
void StringToHex(char *str, uint8_t *strhex)
{
	uint8_t i,cnt=0;
	char *p = str;             //ָ��p��ʼ��Ϊָ��str
	uint8_t len = strlen(str); //��ȡ�ַ����е��ַ�����
	while(*p != '\0') {        //�������ж�
		for (i = 0; i < len; i ++)  //ѭ���жϵ�ǰ�ַ������ֻ���Сд�ַ����Ǵ�д��ĸ
		{
			if ((*p >= '0') && (*p <= '9')) //��ǰ�ַ�Ϊ0~9
				strhex[cnt] = *p - '0' + 0x30;//תΪʮ������
			if ((*p >= 'A') && (*p <= 'Z')) //��ǰ�ַ�Ϊ��д��ĸA~Z
				strhex[cnt] = *p - 'A' + 0x41;//תΪʮ������
			if ((*p >= 'a') && (*p <= 'z')) //��ǰ�ַ�ΪСд��ĸa~zʱ

				strhex[cnt] = *p - 'a' + 0x61;  //תΪʮ������
			p ++;    //ָ����һ���ַ�
			cnt ++;  
		}
	}
}

sprintf(buf, "%02x%02x%02x%02x%02x", port[0], port[1], port[2], port[3], port[4]);  //��16����ת��λASSII
*/
