#ifndef BAT_VOLT_CHECK_H__
#define BAT_VOLT_CHECK_H__

#include <stdint.h>
#include <stdbool.h>
//#include "app_timer.h"
#include "nrf_drv_saadc.h"
#include "boards.h"

#ifdef __cplusplus
extern "C" {
#endif
void saadc_init(void);    
void bat_volt_timer_init(void);
void bat_volt_check_start(void);
void saadc_uinit(void);


#ifdef __cplusplus
}
#endif

#endif