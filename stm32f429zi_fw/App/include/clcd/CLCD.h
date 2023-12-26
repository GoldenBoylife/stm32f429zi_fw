#include "def.h"
#include "stm32f4xx_hal.h"


#ifdef _USE_HW_CLCD

/*/////   PIN CONNECTION   /////*/
/*		CLCD_RS		PE0			*/
/*		CLCD_RW 	PE1			*/
/*		CLCD_EN 	PE2			*/
/*		CLCD_D4 	PE4			*/
/*		CLCD_D5 	PE5			*/
/*		CLCD_D6 	PE6			*/
/*		CLCD_D7 	PE7			*/
////////////////////////////////*/
//내꺼 버전 PA11,PB2,PB11,PB13,PB14,PB15,PC4
/*/////   PIN CONNECTION   /////*/
/*		CLCD_RS		PA11			*/
/*		CLCD_RW 	GND         	*/
/*		CLCD_EN 	PB11			*/
/*		CLCD_D4 	PB13			*/
/*		CLCD_D5 	PB14			*/
/*		CLCD_D6 	PB15			*/
/*		CLCD_D7 	PC4			*/
////////////////////////////////*/



void CLCD_GPIO_Init(void);
void CLCD_Write_Instruction(unsigned char b);
void CLCD_Write_Display(unsigned char b);
void CLCD_Gotoxy(unsigned char x, unsigned char y);
void CLCD_Puts(unsigned char x, unsigned char y, unsigned char *str);
void CLCD_Init(void);
void CLCD_Clear(void);
static void Delay(__IO uint32_t nCount);


#endif  //_USE_HW_CLCD