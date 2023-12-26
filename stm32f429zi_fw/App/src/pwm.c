/*
 * pwm.c
 *
 *  Created on: Nov 26, 2023
 *      Author: GoldenBoy
 */


#include "pwm.h"
extern TIM_HandleTypeDef htim1;
//main.c에 있지만 임시로 가져옴.


bool pwmInit(void)
{
  bool ret = true;
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  //PWM 출력이 나감.
  return ret;
}

void pwmWrite(uint8_t ch, uint16_t duty)
{
  switch(ch)
  {
    case _DEF_PWM_CH0:
      htim1.Instance->CCR1= 20;
      //pulse값은 레지스터를 직접 바꿔서 컨트롤한다.
      //CCR1은  stm32fxx_hal_tim.c에 있다.
      //100은 100%다. 10은 10%다. 10% write된다.
      //결과: led가 출력의 10%만 켜진다.
      //pin회로가 SET모드라면 반대로 10%아닌 90%만큼 켜진다.
      break;
  }
}


uint16_t pwmRead(uint8_t ch)
{
    uint16_t pwm_data = 0;
    /*+ : 애러처리*/
    switch(ch)
    {
      case _DEF_PWM_CH1:
        pwm_data = htim1.Instance->CCR1;
        break;
    }

    return pwm_data;
}
