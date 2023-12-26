/*
 * btn_gpio.c
 *
 *  Created on: Dec 17, 2023
 *      Author: GoldenBoy
 */


#include "btn_gpio.h"
#include "cli.h"

#ifdef _USE_BTN_GPIO 

typedef struct {
  GPIO_TypeDef *port;
  uint32_t pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
  
} btn_gpio_tbl_t;


btn_gpio_tbl_t btn_gpio_tbl[BTN_GPIO_MAX_CH] =
{
    {GPIOC, GPIO_PIN_13,GPIO_PIN_SET,GPIO_PIN_RESET},
    //회로상 SET해야 버튼 안누른 상태임.
};




bool btnGpioInit(void)
{
  bool ret = true;

  return ret;
}
bool btnGpioGetPressed(uint8_t ch)
{
  bool ret = false;
   if (ch>= BTN_GPIO_MAX_CH)
     return false;
  
  if(HAL_GPIO_ReadPin(btn_gpio_tbl[ch].port, btn_gpio_tbl[ch].pin) == btn_gpio_tbl[ch].on_state)
    ret = true;



  return ret;
}

#endif //_USE_BTN_GPIO
