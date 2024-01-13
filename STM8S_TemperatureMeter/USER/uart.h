#ifndef __UART_H
#define __UART_H
#include "stm8s_uart1.h"

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define RxBufferSize 64

void Uart_Init(void);
void UART1_SendByte(uint8_t data);
void UART1_SendString(uint8_t* Data,uint16_t len);
uint8_t UART1_ReceiveByte(void);

#endif
