#include "pca10040.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "board_flash_fstorage.h"

bool sleep_flg = false;         //����
bool gotosleep_flg = false;
bool gotosleep_rightnow = false;
bool cmdfeedback_flg = false;
bool charge_flg = false;
//bool low_power = false;
uint8_t batlevel = 0;
//device_infor_t  device_infor;   //�����豸��Ϣȫ�ֱ���
light_work_status_t  light_work_status;
//��ʼ��LED����Ϣ������Ĭ�Ϲ���ģʽ
led_infor_t led_infor =
{
    DEFAULTLED,
    BRIGHTNESS0,
    BRIGHTNESS0,
    BRIGHTNESS0,
    0x07,                      //7����
    0x03,                      //3����
    0x00,
    
};

//��ʼ���豸SN�ţ�������ʼֵ
device_id_t device_id =
{
	0,
    {0x00,0x44,0x30,0x30,0x30,0x31},
};       
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

//sprintf(buf, "%02x%02x%02x%02x%02x", port[0], port[1], port[2], port[3], port[4]);  //��16����ת��λASSII