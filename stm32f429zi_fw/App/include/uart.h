/*
 * uart.h
 *
 *  Created on: Nov 28, 2023
 *      Author: GoldenBoy
 */

#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

#include "def.h"

bool uartInit(void);
uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length);
uint32_t uartAvailable(uint8_t ch);
uint32_t uartRead(uint8_t ch); //한바이트를 읽어옴.
uint32_t uartPrintf(uint8_t ch, const char *fmt, ...);
// ...는 가변인자 뜻함. 인자 갯수 랜덤
uint32_t uartVPrintf(uint8_t ch, const char *fmt, va_list arg);

#endif /* INCLUDE_UART_H_ */
