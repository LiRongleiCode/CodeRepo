#ifndef __SYSTEM_H
#define __SYSTEM_H
#include "stm8s.h"
#include "HardwareCFG.h"

void ALL_GPIO_Init(void);
void key_process(void);
void key_task(void);
void system_control(void);
#endif


