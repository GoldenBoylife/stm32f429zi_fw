
#include "def.h"
#include "cli.h"

#ifdef _USE_GPIO
bool gpioInit();
bool gpioPinMode(uint8_t ch,uint8_t mode);
void gpioPinWrite(uint8_t ch, bool value);
bool gpioPinRead(uint8_t ch);
void gpioPinToggle(uint8_t ch); 

#endif //_USE_GPIO
