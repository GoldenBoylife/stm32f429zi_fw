
#include "def.h"

#ifdef _USE_SPI

bool spiInit(void);
bool spiBegin(uint8_t ch);
void spiSetDataMode(uint8_t ch, uint8_t dataMode);  //모드 0~3
void spiSetBitWidth(uint8_t ch, uint8_t bit_width); //bit_width : 8 or 16

uint8_t spiTransfer8(uint8_t ch, uint8_t data);
uint16_t spiTransfer16(uint8_t ch, uint16_t data);

void spiDmaTxStart(uint8_t ch, uint8_t *p_buf, uint32_t length);
void spiDmaTxTransfer(uint8_t ch, void *buf, uint32_t length, uint32_t timeout);
bool spiDmaTxIsDone(uint8_t ch);
void spiAttachTxInterrupt(uint8_t ch, void (*func)());
//완료 되었을 때 인터럽트 콜백


#endif //_USE_SPI