#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H
#include "stm8s.h"
#include "HardwareCFG.h"

extern float Temperature;
extern uint8_t BatVoltLevel;

void MeasurementTask(void);
#endif
