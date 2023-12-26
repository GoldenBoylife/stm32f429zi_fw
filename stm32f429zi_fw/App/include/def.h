/*
 * def.h
 *
 *  Created on: Nov 25, 2023
 *      Author: GoldenBoy
 */

#ifndef INCLUDE_DEF_H_
#define INCLUDE_DEF_H_

#include <stdbool.h>    //bool사용
#include <stdint.h>     //uint8_t같은 것 쓰기위해

#include <stdarg.h>   //uartPrintf쓸때 필요
#include <stdio.h>

#include "string.h"
#include "stdlib.h"

#include <main.h>


#define _USE_LED
#define       LED_MAX_CH 2
#define _DEF_LED_CH0 0  //PA2
#define _DEF_LED_CH1 1  //PA3, LED0
#define _DEF_LED_CH2 2
#define _DEF_LED_CH3 3
#define _DEF_LED_CH4 4


#define _DEF_PWM_CH0 0
#define _DEF_PWM_CH1 1
#define _DEF_PWM_CH2 2
#define _DEF_PWM_CH3 3
#define _DEF_PWM_CH4 4


#define _DEF_ADC_CH0 0  //rotation sensor
#define _DEF_ADC_CH1 1  //온도 or Vref
#define _DEF_ADC_CH2 2  //온도 or Vref
#define _DEF_ADC_CH3 3
#define _DEF_ADC_CH4 4

/*버튼의 기능을 기록하고 싶을 때*/
#define         BUTTON_MAX_CH 5
#define _BUTTON_CH_SEL    _DEF_ADC_CH0
#define _BUTTON_CH_LEFT   _DEF_ADC_CH1
#define _BUTTON_CH_DOWN   _DEF_ADC_CH2
#define _BUTTON_CH_UP     _DEF_ADC_CH3
#define _BUTTON_CH_RIGHT  _DEF_ADC_CH4





#define _USE_BTN_GPIO
#define         BTN_GPIO_MAX_CH 1
#define _DEF_BTN_GPIO_CH0 0
#define _DEF_BTN_GPIO_CH1 1
#define _DEF_BTN_GPIO_CH2 2
#define _DEF_BTN_GPIO_CH3 3

#define         UART_MAX_CH 1
#define _DEF_UART_CH0 0
#define _DEF_UART_CH1 1
#define _DEF_UART_CH2 2
#define _DEF_UART_CH3 3   //3번 UART가 USB쪽
#define _DEF_UART_CH4 4


#define _USE_CLI

#define _USE_GPIO
#define         GPIO_MAX_CH 1+3+7
                //1 : sd카드
                //3 : tftLCD
                //7 : CLCD
#define _DEF_INPUT              0
#define _DEF_INPUT_PULLUP       1
#define _DEF_INPUT_PULLDOWN     2   
#define _DEF_OUTPUT             3
#define _DEF_OUTPUT_PULLUP      4
#define _DEF_OUTPUT_PULLDOWN    5

#define _USE_SPI
#define         SPI_MAX_CH 1
#define _DEF_SPI0 0
#define _DEF_SPI1 1
#define _DEF_SPI2 2
#define _DEF_SPI3 3
#define SPI_MODE0   0
#define SPI_MODE1   1
#define SPI_MODE2   2
#define SPI_MODE3   3





#define _USE_HW_LCD
#define _USE_HW_ST7735
#define HW_LCD_WIDTH         160
#define HW_LCD_HEIGHT        80

#define _USE_HW_CLCD 
#define GPIO_RS GPIOA
// #define GPIO_RW GPIOB    //보통은 CLCD에서 RW는 GND
#define GPIO_EN GPIOB
#define GPIO_D4	GPIOB
#define GPIO_D5	GPIOB
#define GPIO_D6	GPIOB
#define GPIO_D7	GPIOC

#define GPIO_PIN_RS	GPIO_PIN_11
// #define GPIO_PIN_RW	GPIO_PIN_2  //보통은 CLCD에서 RW는 GND
#define GPIO_PIN_EN	GPIO_PIN_11
#define GPIO_PIN_D4	GPIO_PIN_13
#define GPIO_PIN_D5	GPIO_PIN_14
#define GPIO_PIN_D6	GPIO_PIN_15
#define GPIO_PIN_D7	GPIO_PIN_4

/*
RS: A11
RW: GND  //보통은 CLCD에서 RW는 GND
EN: B11
D4: B13
D5: B14
D6: B15
D7: C4
*/






#define _DEF_LOW       0
#define _DEF_HIGH      1



/*공통으로 쓰느 함수들*/
void delay(uint32_t time_ms);
uint32_t millis(void);


#endif /* INCLUDE_DEF_H_ */
