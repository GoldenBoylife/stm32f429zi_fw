/*
 * btn_gpio.h
 *
 *  Created on: Dec 17, 2023
 *      Author: GoldenBoy
 */

#ifndef INCLUDE_BTN_GPIO_H_
#define INCLUDE_BTN_GPIO_H_

#include "def.h"

#ifdef _USE_BTN_GPIO




bool btnGpioInit(void);
bool btnGpioGetPressed(uint8_t ch);

#endif
#endif /* INCLUDE_BTN_GPIO_H_ */
