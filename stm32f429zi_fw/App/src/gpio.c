#include "gpio.h"




typedef struct 
{
    GPIO_TypeDef *port;
    uint32_t    pin;
    uint8_t mode;   //input mode or output mode
    GPIO_PinState on_state;
    GPIO_PinState off_state;
    bool init_value;    //gpio초기값을 low할지 high할지 
} gpio_tbl_t;

gpio_tbl_t gpio_tbl[GPIO_MAX_CH] = 
{
    /*1개: sd카드*/
    {GPIOB, GPIO_PIN_9, _DEF_INPUT_PULLUP, GPIO_PIN_RESET,GPIO_PIN_SET,1},   //0.SD : CD
    //init_value는 1로

    /*3개: tftLCD*/
    {GPIOA,GPIO_PIN_12,_DEF_OUTPUT ,GPIO_PIN_SET,GPIO_PIN_RESET,_DEF_LOW},      //1.LCD : BKL
    {GPIOA,GPIO_PIN_13,_DEF_OUTPUT ,GPIO_PIN_SET,GPIO_PIN_RESET,_DEF_HIGH},      //2.LCD : CS
    {GPIOA,GPIO_PIN_14,_DEF_OUTPUT ,GPIO_PIN_SET,GPIO_PIN_RESET,_DEF_HIGH},      //3.LCD : DC

    /*7개: CLCD:*/
    /*
    RS: A11  reset
    RW: B2  rest
    EN: B11  rest
    D4: B13 reset
    D5: B14 reset
    D6: B15 rest
    D7: C4 rest
    */
    {GPIO_RS,GPIO_PIN_RS,_DEF_OUTPUT ,GPIO_PIN_SET,GPIO_PIN_RESET,_DEF_LOW},      //3.LCD : DC
    //{GPIO_RW,GPIO_PIN_RW,_DEF_OUTPUT ,GPIO_PIN_SET,GPIO_PIN_RESET,_DEF_LOW},      //3.LCD : DC
    {GPIO_EN,GPIO_PIN_EN,_DEF_OUTPUT ,GPIO_PIN_SET,GPIO_PIN_RESET,_DEF_LOW},      //3.LCD : DC
    {GPIO_D4,GPIO_PIN_D4,_DEF_OUTPUT ,GPIO_PIN_SET,GPIO_PIN_RESET,_DEF_LOW},      //3.LCD : DC
    {GPIO_D5,GPIO_PIN_D5,_DEF_OUTPUT ,GPIO_PIN_SET,GPIO_PIN_RESET,_DEF_LOW},      //3.LCD : DC
    {GPIO_D6,GPIO_PIN_D6,_DEF_OUTPUT ,GPIO_PIN_SET,GPIO_PIN_RESET,_DEF_LOW},      //3.LCD : DC
    {GPIO_D7,GPIO_PIN_D7,_DEF_OUTPUT ,GPIO_PIN_SET,GPIO_PIN_RESET,_DEF_LOW},      //3.LCD : DC
	// {GPIOA, GPIO_PIN_3,GPIO_PIN_SET, GPIO_PIN_RESET}



};

static void cliGpio(uint8_t argc,const char **argv);

bool gpioInit()
{
    bool ret = true;



    for( int i=0; i <GPIO_MAX_CH ;i++) 
    {
        gpioPinMode(i,gpio_tbl[i].mode);
        gpioPinWrite(i,gpio_tbl[i].init_value);
    }

    cliAdd("gpio",cliGpio);
    return ret;

    
    // 참고 : 이미 main.c에서 선언되어 있는데;; 여기서 다시 해도 될가? 
    // 만약 안된다면, main.c에 추가해서 이리로 값을 가져와야 함. 
    // GPIO_InitTypeDef GPIO_InitStruct = {0}; //초기 구조체

    // __HAL_RCC_GPIOB_CLK_ENABLE();
    // /*Configure GPIO pin : PB9 */
    // GPIO_InitStruct.Pin = GPIO_PIN_9;
    // GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    // GPIO_InitStruct.Pull = GPIO_PULLUP;
    // HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
}

bool gpioPinMode(uint8_t ch,uint8_t mode)
{
    bool ret = true;
    GPIO_InitTypeDef GPIO_InitStruct = {0}; //초기 구조체
 //   if(ch >= GPIO_MAX_CH) 
 //       return false;

   /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

    switch(mode) 
    {
        case _DEF_INPUT:            //input
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;
    
        case _DEF_INPUT_PULLUP:     //input이면서 pullup
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            break;
        case _DEF_INPUT_PULLDOWN:     //input이면서 pulldown
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            break;
        case _DEF_OUTPUT:     //output
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //PP: PUSH PULL
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;
        case _DEF_OUTPUT_PULLUP:     //output이면서 pullup
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            break;
        case _DEF_OUTPUT_PULLDOWN:     //output이면서 pulldown
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            break;
    }

	

    GPIO_InitStruct.Pin = gpio_tbl[ch].pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(gpio_tbl[ch].port, &GPIO_InitStruct);

    return ret;

}
void gpioPinWrite(uint8_t ch, bool value)
{
    if(ch >= GPIO_MAX_CH) 
        return;
    if(value == true) 
    {
        HAL_GPIO_WritePin(gpio_tbl[ch].port,gpio_tbl[ch].pin, gpio_tbl[ch].on_state);
    }
    else 
    {
        HAL_GPIO_WritePin(gpio_tbl[ch].port,gpio_tbl[ch].pin, gpio_tbl[ch].off_state);
    }
}

bool gpioPinRead(uint8_t ch)
{
    bool ret = false;
    if (ch >= GPIO_MAX_CH)
        return false;
    
    if (HAL_GPIO_ReadPin(gpio_tbl[ch].port, gpio_tbl[ch].pin) ==  gpio_tbl[ch].on_state)
    {
        ret= true;
    }

    return ret;
}

void gpioPinToggle(uint8_t ch)
{
    if (ch >= GPIO_MAX_CH)
        return;

    HAL_GPIO_TogglePin(gpio_tbl[ch].port, gpio_tbl[ch].pin);
    
}


//왜 static으로 했지?
static void cliGpio(uint8_t argc,const char **argv)
{
    bool ret = false;


    if(argc ==1 && cliIsStr(argv[0],"show"))
    {
        while(cliKeepLoop())
        {
            for (int i=0; i< GPIO_MAX_CH;i++) 
            {
                cliPrintf("%d", gpioPinRead(i));
            }
            cliPrintf("\n\r");
            delay(100);
        }
        ret = true;
    }

    /*write 명령어*/
    if(argc ==3 ,cliIsStr(argv[0],"write"))
    {
        uint8_t ch;
        uint8_t data;

        ch = (uint8_t)cliGetData(argv[1]);
        data = (uint8_t)cliGetData(argv[2]);
        gpioPinWrite(ch,data);
        while(cliKeepLoop())
        {
            cliPrintf("argv[0]: %s, argv[1]: %d, argv[2]: %d \r\n",argv[0],ch,data);
            
            delay(100);
        }
        ret = true;
    }

    if(argc ==2 && cliIsStr(argv[0],"read"))
    {
        uint8_t ch;
        ch = (uint8_t)cliGetData(argv[1]);
       while(cliKeepLoop())
       {
        cliPrintf("gpio read [ch %d] : %d\r\n", ch, gpioPinRead(ch));
        delay(100);
       }
       ret = true;
    }

    if(ret  != true )
    {
        cliPrintf("gpio show\n\r");
        cliPrintf("gpio read ch[0~ %d]\n\r", GPIO_MAX_CH-1);
        cliPrintf("gpio write ch[0~ %d] 0:1\n\r", GPIO_MAX_CH-1);
    }
}
