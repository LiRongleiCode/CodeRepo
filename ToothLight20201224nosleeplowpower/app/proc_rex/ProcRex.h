#ifndef PROCREX_H_
#define PROCREX_H_


#include "string.h"
#include "crc16.h"

#define     MAX_SEND_DATA_CNT             50
#define     MAX_SEND_CNT                  50
#define     HEADER                        0XFC

#define     SEND                          0x00
#define     RESP                          0x01

#define     GET_ID                        0x00
#define     SET_LED                       0x01
#define     GET_LED_MODE                  0x02
#define     SEND_BATTERY_LEVEL            0x03
#define     START_STOP_CMD                0x04
#define     SEND_LED_STATUS               0x05

#define     SETSUCCESS                    0x00
#define     SETSTOP                       0x00

#define     ONLY_BLUE                     0X0001
#define     ONLY_RED                      0x0002
#define     BLUE_AND_RED                  0x0003
#define     BLUE_TO_RED                   0x0004
#define     RED_TO_BLUE                   0X0005


void receive_date_parser(uint8_t const *pbuffer,uint8_t const *plength,uint8_t *readybuff,uint8_t *readylength);
void Package_batterylevel(uint8_t const *batterylevel,uint8_t *readybuff,uint8_t *readylength);
void Package_LedWorkStatus(uint8_t const *ledstatus,uint8_t *readybuff,uint8_t *readylength);
#endif
