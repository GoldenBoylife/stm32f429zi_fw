#include "led.h"
#include "pwm.h" //pwm기능 써서 밝기 조절하고 싶으니까


#ifdef _USE_LED
typedef struct
{
	GPIO_TypeDef *port;
	uint16_t pin;
	GPIO_PinState on_state;
	GPIO_PinState off_state;
} led_tbl_t;

/*각 LED채널을 넣어주기. */
led_tbl_t led_tbl[LED_MAX_CH] = 
{
	// {GPIOA, GPIO_PIN_2,GPIO_PIN_RESET, GPIO_PIN_SET},
	// {GPIOA, GPIO_PIN_2,GPIO_PIN_RESET, GPIO_PIN_SET},
	{GPIOA, GPIO_PIN_2,GPIO_PIN_SET, GPIO_PIN_RESET},
	//PA2는 회로상 SET해야 켜진다. 
	{GPIOA, GPIO_PIN_3,GPIO_PIN_SET, GPIO_PIN_RESET}
	//PA3는 회로상 SET해야 켜진다. 

};


static uint16_t led_duty[1] = {10};
//초기값
//채널 갯수만큼 현재 1개

bool ledInit(void)
{   
	          GPIO_InitTypeDef GPIO_InitStruct = {0};

//	  __HAL_RCC_GPIOA_CLK_ENABLE();

	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);

	  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	for(int i=0; i<LED_MAX_CH;i++)
	{
    	ledOff(i);
		//애초에 _DEF_LED_CH0가 0이기때문에 이렇게 해도 되겠다.
	}
    return true;
}

void ledOn(uint8_t ch)
{
	/*1개 채널일때만 이렇게 쓴다. */
    // switch(ch)
    // {
    //     case _DEF_LED_CH0:
    //         HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin, GPIO_PIN_RESET);
    //         //pwmWrite(_DEF_LED_CH0,0);
    //         //pin RESET에서는 0이 되야 불이 켜진다.
    //         // pwmWrite(ch,100 - led_duty[ch]);
    //         //0이 되니까 불이 켜진다.

    //         break;

    // }
	/*여러 채널 쓸 땐*/
	if (ch>=LED_MAX_CH) return;
	HAL_GPIO_WritePin(led_tbl[ch].port,led_tbl[ch].pin, led_tbl[ch].on_state);

}

void ledOff(uint8_t ch)
{
//     switch(ch)
//     {
//         case _DEF_LED_CH0:
//             HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin, GPIO_PIN_SET);
// //            pwmWrite(_DEF_LED_CH0,100);
//             //100은 완전꺼짐
//             break;
//     }
	if(ch>=LED_MAX_CH) return;
	HAL_GPIO_WritePin(led_tbl[ch].port,led_tbl[ch].pin, led_tbl[ch].off_state);
}


void ledToggle(uint8_t ch)
{
//     switch(ch) 
//     {
//         case _DEF_LED_CH0:
//             HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
// //            if(pwmRead(_DEF_LED_CH0) == 100){
// //              //100이면 꺼진상태
// //              ledOn(_DEF_LED_CH0);
// //            }else{
// //              ledOff(_DEF_LED_CH0);
// //            }
//             break;

//     }
	if(ch>=LED_MAX_CH) return;
	HAL_GPIO_TogglePin(led_tbl[ch].port, led_tbl[ch].pin);
}
void ledDuty(uint8_t ch, uint16_t duty_data)
{
  switch(ch)
  {
    case _DEF_LED_CH0:
        led_duty[ch] = duty_data;
        break;
  }
}


#endif //_USE_LED
