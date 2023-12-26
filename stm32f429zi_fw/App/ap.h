/*
 * ap.h
 *
 *  Created on: Nov 25, 2023
 *      Author: GoldenBoy
 */


#ifndef _AP_H_
#define _AP_H_

#include "def.h"
#include "led.h"
#include "pwm.h"
#include "adc.h"
#include "btn_gpio.h"
#include "button.h"
#include "uart.h"
#include "cli.h"
#include "gpio.h"
#include "spi.h"
void apInit(void);
void apMain(void);

#endif /*_AP_H_ */
