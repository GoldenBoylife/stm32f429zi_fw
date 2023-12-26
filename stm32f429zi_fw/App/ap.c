#include "ap.h"
#include "lcd/st7735.h"
#include "clcd/CLCD.h"

#include "def.h"

float adc_vol =0;
float adc_vol_temperature =0;
float adc_vol_vref =0;

bool button_data[BUTTON_MAX_CH];

static void infoCli(uint8_t argc, const char **argv);




void apInit(void) 
{
  cliInit();
	ledInit();
	pwmInit();
	adcInit();
	buttonInit();
  btnGpioInit();
	uartInit();
  gpioInit();
  //flashInit();//추후
  spiInit();

	cliAdd("info", infoCli);
	//infoCli라는 함수추가

  // spiBegin(_DEF_SPI0);

  // uint8_t tx[2];
  // spiDmaTxTransfer(_DEF_SPI0,tx,2,100);
	  // cliPrintf("st7735Init() : %d",st7735Init());
  // gpioPinWrite(1, _DEF_HIGH);

//	 CLCD_GPIO_Init(); // LCD GPIO 초기화
	 CLCD_Init(); // Char LCD 내부 초기화
	 CLCD_Puts(0, 0, "Welcome to");
	 CLCD_Puts(0, 1, "CLCD World");
	 CLCD_Puts(6, 1, "Booting...");
	 CLCD_Clear();

//	LCD1602_Begin4BIT(
//                    RS_GPIO_Port, RS_Pin,
//                    EN_GPIO_Port , EN_Pin,
//                    D4_GPIO_Port, D4_Pin,
//                    D5_GPIO_Port, D5_Pin,
//                    D6_GPIO_Port, D6_Pin,
//                    D7_GPIO_Port, D7_Pin);
		

}

void apMain(void)
{
	uint32_t pre_time;
	 uint8_t a = 0;
	 uint8_t str[20];

  ledDuty(_DEF_LED_CH0,5); //5%

	uint16_t pwm_value = pwmRead(_DEF_PWM_CH0);
  while(1)
  {
		 HAL_Delay(100);
		 CLCD_Clear();
		 CLCD_Puts(0,0,"Hi! GoldenBoy");
		 sprintf(str, "%d", a++);
		 CLCD_Puts(0, 1, str);
//		 cliPrintf("clcd: %d " ,a);
    
    /* 16x2 LCD Test Begin */
//		LCD1602_print("HELLO World!!!");


  if(btnGpioGetPressed(_DEF_BTN_GPIO_CH0)){
    ledOn(_DEF_LED_CH0);
    ledOn(_DEF_LED_CH1);
  }else{
    ledOff(_DEF_LED_CH0);
    ledOff(_DEF_LED_CH1);
  }

    // cliPrintf("Pressed? %d\r\n",btnGpioGetPressed(_DEF_BTN_GPIO_CH0));
    pwmWrite(_DEF_PWM_CH0,pwm_value);
    if(millis() - pre_time >= 350)
    {
      pre_time = millis();
      // ledOn(_DEF_LED_CH0);
      // ledToggle(_DEF_LED_CH0);
      //uartWrite(_DEF_UART_CH3,(uint8_t *)"test\n",5);
//      uartPrintf(_DEF_UART_CH3,"test %d\r\n",millis());
    }
    adc_vol = (float)adcRead(_DEF_ADC_CH0)/100.f;
    adc_vol_temperature =(float)adcRead(_DEF_ADC_CH1)/100.f;
    adc_vol_vref = (float)adcRead(_DEF_ADC_CH2)/100.f;

#if 0 //def.h에 BUTTON_CH_SEL추가해서 없앰.
    for(int i =0; i< BUTTON_MAX_CH ; i++)
    {
      button_data[i] = buttonGetPressed(i);
    }
#elif 1
    /*이제 가독성이 편해졌다.*/
    button_data[_BUTTON_CH_SEL]   = buttonGetPressed(_BUTTON_CH_SEL);
    button_data[_BUTTON_CH_LEFT]  = buttonGetPressed(_BUTTON_CH_LEFT);
    button_data[_BUTTON_CH_DOWN]  = buttonGetPressed(_BUTTON_CH_DOWN);
    button_data[_BUTTON_CH_UP]    = buttonGetPressed(_BUTTON_CH_UP);
    button_data[_BUTTON_CH_RIGHT] = buttonGetPressed(_BUTTON_CH_RIGHT);

    cliMain();
#endif

  }
}


void infoCli(uint8_t argc, const char **argv)
{
//  uartPrintf(_DEF_UART_CH3, "infoCli run %d\r\n",argc);
  //cli전용printf만들어야 내부적으로 사용되는 채널로 보냄,
  //uartPrintf의 가변 인자 처리를 위해서 uartVPrintf()함수 씀
  //uartVPrintf함수 사용해서 cliPrintf()구현

  bool ret = false;
  
  /*clin# info test하면*/
  if(argc ==1 && cliIsStr(argv[0], "test"))
  {//인자가 1개이고, info 첫번째 인자가 test이면
  
    cliPrintf("infoCli run test\r\n");
    //이명령어 쓸수 있다고 말함.
    ret = true;
  }

  /*clin# info print 5하면*/
  if(argc ==2 &&cliIsStr(argv[0], "print"))
  {
    uint8_t count;

    count = (uint8_t)cliGetData(argv[1]);
    for(int i=0;i<count;i++)
    {
      cliPrintf("print %d/%d\r\n", i+1, count);
    }
    ret = true;
  }

   /*명령어 info button하면, 현재 눌린 버튼 정보가 뜸*/
  if(argc ==1 && cliIsStr(argv[0], "button"))
  {
    while(cliKeepLoop())
    {
      for(int i =0; i<BUTTON_MAX_CH; i++)
      {
        cliPrintf("%d", buttonGetPressed(i));
        //5개 반복
      }
      cliPrintf("\r\n");
      delay(100);
    }
    ret = true;
  }

  /*cli# info btn_gpio*/
  if(argc==1 &&cliIsStr(argv[0],"btn_gpio"))
  {
    while(cliKeepLoop())
    {
      for(int i =0 ;i < BTN_GPIO_MAX_CH;i++)
      {
        cliPrintf("%d",btnGpioGetPressed(i));
      }
      cliPrintf("\r\n");
      delay(100);
    }
    ret = true;
  }

  /*cli# info*/
  if(ret == false)
  {
    cliPrintf("info test\r\n");
    cliPrintf("info print 0~10\r\n");
    cliPrintf("info button\r\n");
    cliPrintf("info btn_gpio\r\n");
  }


}
