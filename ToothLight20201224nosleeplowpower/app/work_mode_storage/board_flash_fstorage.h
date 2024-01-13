#ifndef _BOARD_FLASH_H_
#define _BOARD_FLASH_H_
#include "stdint.h"
#include "stdbool.h"
/*********************************************************************
 * INCLUDES
 */
//#include "common.h"

/*********************************************************************
 * DEFINITIONS
 */
//定义文件ID和该文件包含的记录的KEY
#define DEVICE_FILE                       (0x1000)//文件ID
#define DEVICE_VERSION_KEY                (0x1001)//记录KEY,该记录存放的文件ID=0x1000
#define DEVICE_DESP_KEY                   (0x1002)//记录KEY,该记录存放的文件ID=0X1000

//FDS异步操作标志结构体
typedef struct
{
    bool version_record_update;    //version_record记录更新标志
	  bool desp_record_update;       //desp_record记录更新标志
	  bool read;       //读记录标志
	  bool gc;         //碎片收集标志
    bool busy;       //FDS忙标志
}my_fds_info_t;

void board_fstorage_initial(void);
void read_ledinfor_record(void);
void read_deviceid_record(void);
void write_ledinfor_fds(void);
void write_deviceid_fds(void);
#endif /* _BOARD_FLASH_H_ */

