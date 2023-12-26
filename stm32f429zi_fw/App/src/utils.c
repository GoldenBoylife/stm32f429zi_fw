/*
 * utils.c
 *
 *  Created on: Nov 25, 2023
 *      Author: GoldenBoy
 */


#include "utils.h"

void delay(uint32_t time_ms)
{
    HAL_Delay(time_ms);
}

uint32_t millis(void)
{
    return HAL_GetTick();
}

