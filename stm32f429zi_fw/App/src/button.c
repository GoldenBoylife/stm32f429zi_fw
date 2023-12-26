/*
 * button.c
 *
 *  Created on: Nov 26, 2023
 *      Author: GoldenBoy
 */

#include "button.h"
#include "adc.h" //adc로도 버튼 움직이니까.



/*buttonGetPressed에서 중복된 코드가 많으므로 테이블 만든다.*/
typedef struct
{
  uint16_t adc_min;
  uint16_t adc_max;

} button_adc_t;

static const button_adc_t button_adc[BUTTON_MAX_CH] =
    {
        {0,   20},
        {40,  87},
        {122, 152},
        {180, 220},
        {245, 285},
    };
static const uint8_t adc_ch = _DEF_ADC_CH0;





bool buttonInit(void)
{
  return true;
}

bool buttonGetPressed(uint8_t ch)
{
  bool ret = false;
  uint32_t adc_voltage;

  adc_voltage = adcReadVoltage(adc_ch);

  if( ch>= BUTTON_MAX_CH)
    return false;
  //채널을 넘어가면 배열넘어간것이니까 false로 되돌려줌.
#if 0
/*너무 공통된것이 많아서 테이블로 만들어서 코드 줄임*/
  if(ch == _DEF_ADC_CH0)
  {
    if(adc_voltage <= 20)// 0 ~0.2V
    {
      ret = true;
    }

  }
  //? 얼레, 왜 버튼이 다르다고 채널이 다르지?
  if(ch == _DEF_ADC_CH1)
  {
    if(adc_voltage >= 40 && adc_voltage <=87)// 0.4 ~0.87V
    {
      ret = true;
    }

  }
  if(ch == _DEF_ADC_CH2)
  {
    if(adc_voltage >= 122 && adc_voltage <= 152)// ~ 1.52
    {
      ret = true;
    }

  }
  if(ch == _DEF_ADC_CH3)
  {
    if(adc_voltage >= 180 && adc_voltage <=220)//
    {
      ret = true;
    }

  }
  if(ch == _DEF_ADC_CH4)
  {
    if(adc_voltage >=245 && adc_voltage <= 285)//
    {
      ret = true;
    }
  }
#elif 1
  if(adc_voltage >= button_adc[ch].adc_min && adc_voltage <= button_adc[ch].adc_max)
  {
    ret = true;
  }
#endif
  return ret;
}
uint32_t buttonGetData(uint8_t ch)
{
  uint32_t ret =0;

  for(int i = 0; i< BUTTON_MAX_CH;i++)
  {
    ret |=(buttonGetPressed(i) <<i);
    //버튼 값을 i만큼 쉬프트 시킴
    //버튼의 각 비트 별로 값을 or연산 해서  return 함
  }
  return ret;
}
