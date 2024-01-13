
/******************** (C) COPYRIGHT  风驰电子嵌入式开发工作室 ***************************

 * 文件名  ：uart.c
 * 描述    ：串口通信配置函数库     
 * 实验平台：风驰电子STM8开发板
 * 库版本  ：V2.0.0
 * 作者    ：ling_guansheng  QQ：779814207
 * 博客    ：
 *修改时间 ：2011-12-20
  风驰电子STM8开发板硬件连接
    |--------------------|
    |  USART1_RX-PA4     |
    |  USART1_TX-PA5     |
    |--------------------|

****************************************************************************************/

#include "uart.h"
#include "stm8s_uart1.h"
#include "stm8s_clk.h"

/* ********************************************
UART1  configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Receive and transmit enabled
 -  Receive interrupt
  - UART1 Clock disabled
*********************************************/
void Uart_Init(void)
{
    UART1_DeInit();
    UART1_Init((u32)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, \
    UART1_PARITY_NO , UART1_SYNCMODE_CLOCK_DISABLE , UART1_MODE_TXRX_ENABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE  );
    UART1_Cmd(ENABLE );
  
}

void UART1_SendByte(uint8_t data)
{
    UART1_SendData8((unsigned char)data);
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
}

void UART1_SendString(uint8_t* Data,uint16_t len)
{
  uint16_t i=0;
  for(;i<len;i++)
    UART1_SendByte(Data[i]);
  
}

uint8_t UART1_ReceiveByte(void)
{
     uint8_t USART1_RX_BUF; 
     while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
     USART1_RX_BUF=UART1_ReceiveData8();
     return  USART1_RX_BUF;
    
}

