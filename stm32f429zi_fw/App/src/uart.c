/*
 * uart.c
 *
 *  Created on: Nov 28, 2023
 *      Author: GoldenBoy
 */

#include "uart.h"

#define UART_Q_BUF_MAX 256

extern UART_HandleTypeDef huart3;

static uint16_t q_in =0;
static uint16_t q_out =0;
static uint8_t q_buf[UART_Q_BUF_MAX];


bool uartInit(void)
{

  HAL_UART_Receive_DMA(&huart3, &q_buf[0],UART_Q_BUF_MAX);
  return true;
}
uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret =0;
  HAL_StatusTypeDef hal_ret;

  switch(ch)
  {
    case _DEF_UART_CH3:
      hal_ret = HAL_UART_Transmit(&huart3,p_data,length,100);
      //맨마지막 Timeout 인자는 100ms정도로 한다.
      if(hal_ret ==HAL_OK)
      {
        ret = length;
      }
      break;
  }

  return ret;
}
uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret =0;

  switch(ch)
  {
    case _DEF_UART_CH3:
      q_in = (UART_Q_BUF_MAX - huart3.hdmarx->Instance->NDTR) % UART_Q_BUF_MAX;
      ret = (UART_Q_BUF_MAX + q_in - q_out) % UART_Q_BUF_MAX;
      break;
  }
  return ret;
}
uint32_t uartRead(uint8_t ch) //한바이트를 읽어옴.
{
  uint8_t ret = 0;

  switch(ch)
  {
    case _DEF_UART_CH3:
      if(q_out !=q_in)
      {
        ret = q_buf[q_out];
        q_out = (q_out +1) %UART_Q_BUF_MAX;
      }
      break;
  }
  return ret;
}



uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  uint32_t ret =0;
  va_list arg;
  char print_buf[256];

  /*va_start와 va_end사이에 가변인자 쓰는 함수 넣어야 함*/
  va_start(arg,fmt);
  int len;
  len = vsnprintf(print_buf, 256, fmt,arg);
  va_end(arg);

  if(len >0)
  {
    ret = uartWrite(ch,(uint8_t *)print_buf,len);
  }

  return ret;
}



uint32_t uartVPrintf(uint8_t ch, const char *fmt, va_list arg)
{
  uint32_t ret =0;
  char print_buf[256];



  int len;
  len = vsnprintf(print_buf, 256, fmt,arg);
  va_end(arg);

  if(len >0)
  {
    ret = uartWrite(ch,(uint8_t *)print_buf,len);
  }

  return ret;
}
