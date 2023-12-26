/*
 * pwm.h
 *
 *  Created on: Nov 26, 2023
 *      Author: GoldenBoy
 */

#ifndef INCLUDE_PWM_H_
#define INCLUDE_PWM_H_

#include "def.h" //해야 bool uint16_t씀
bool pwmInit(void);
void pwmWrite(uint8_t ch,uint16_t duty);
uint16_t pwmRead(uint8_t ch);


#endif /* INCLUDE_PWM_H_ */

