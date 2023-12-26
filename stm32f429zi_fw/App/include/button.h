/*
 * button.h
 *
 *  Created on: Nov 26, 2023
 *      Author: GoldenBoy
 */

#ifndef INCLUDE_BUTTON_H_
#define INCLUDE_BUTTON_H_

#include "def.h"





bool buttonInit(void);
bool buttonGetPressed(uint8_t ch);
uint32_t buttonGetData(uint8_t ch);


#endif /* INCLUDE_BUTTON_H_ */
