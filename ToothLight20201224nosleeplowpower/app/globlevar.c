#include "pca10040.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "board_flash_fstorage.h"

bool sleep_flg = false;         //休眠
bool gotosleep_flg = false;
bool gotosleep_rightnow = false;
bool cmdfeedback_flg = false;
bool charge_flg = false;
//bool low_power = false;
uint8_t batlevel = 0;
//device_infor_t  device_infor;   //申明设备信息全局变量
light_work_status_t  light_work_status;
//初始化LED灯信息，并赋默认工作模式
led_infor_t led_infor =
{
    DEFAULTLED,
    BRIGHTNESS0,
    BRIGHTNESS0,
    BRIGHTNESS0,
    0x07,                      //7分钟
    0x03,                      //3分钟
    0x00,
    
};

//初始化设备SN号，并赋初始值
device_id_t device_id =
{
	0,
    {0x00,0x44,0x30,0x30,0x30,0x31},
};       
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

//sprintf(buf, "%02x%02x%02x%02x%02x", port[0], port[1], port[2], port[3], port[4]);  //将16进制转换位ASSII