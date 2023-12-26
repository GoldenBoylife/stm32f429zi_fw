/*
 * adc.c
 *
 *  Created on: Nov 26, 2023
 *      Author: GoldenBoy
 */

#include "adc.h"


extern ADC_HandleTypeDef hadc1;
uint16_t adc_data[3]; //3개채널씀
//uint32_t adc_time;

bool adcInit(void)
{
//  HAL_ADCEx_Calibration_Start(&hadc1);
  //강의에서는 있지만 나는 없다.
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_data, 3 );
  //위 함수는 stm32f4xx_hal_adc.c에서 옴
  //adc_data는 16 bit로 선언했지만, 함수는 32bit쓰므로 형변환해야 애러안남
  //데이터 갯수가 3이므로 3넣음.

  return true;
}

uint16_t adcRead(uint8_t ch)
{
  uint16_t ret =0;
#if 0 //switch(ch)안해도 됨
  switch(ch)
  {

    case _DEF_ADC_CH0:
      #if 0 /*Polling  방식*/
      /*아래 3줄은 stm32f4xx_hal_adc.c에서 옴*/
      //스타트->기다림->값출력이 계속 반복됨.
      HAL_ADC_Start(&hadc1);
      HAL_ADC_PollForConversion(&hadc1,10 );
      //10mm정도 줌
      adc_data = HAL_ADC_GetValue(&hadc1);
      //출력이 uint32_t다.
      ret = (330 * adc_data) /4096;
      //voltage구하기 위해서
      //계산편의 위해 3.3*100
      #elif 1 /*DMA방식*/

      ret = adc_data[ch];

      #endif
      break;
  }
#elif 1
  ret = adc_data[ch];
#endif
  return ret;
}

uint32_t adcReadVoltage(uint8_t ch)
{
  uint32_t ret;
  uint32_t adc_data =0;
  uint32_t adc_vol =0;

  adc_data = adcRead(ch);

  ret = (330* adc_data) /4096;
  return ret;

}

/*ADC DMA에서 더 이상 interrupt안씀. */
///*ADC conversion이 완료되었을때 실행된다.*/
////ADC_start함수쪽에서 타고 들어가면 이 콜백함수 나타난다.
////DMA쪽 conversion이니까 3개 채널이 완료되었을때 호출된다.
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{
//  static uint32_t adc_cnt =0;
//  //static으로 하면 전역변수처럼 메모리가 계속 기억된다. 다만 이 함수 내에서만 사용가능
//  static uint32_t pre_time;
//  adc_cnt++;
//  if( millis() -pre_time >= 1000)
//  { //1초가되면,
//    pre_time =millis();
//    adc_time = adc_cnt;
//    adc_cnt =0;
//    //1초동안 adc conversion된 갯수를 adc_time에 넣는다.
//  }
//}

