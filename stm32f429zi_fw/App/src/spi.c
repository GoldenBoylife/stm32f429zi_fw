#include "spi.h"

#ifdef _USE_SPI


typedef struct 
{
    bool is_open;
    bool is_tx_done;

    void (*func_tx)(void);
    /*spi 관리하기 위해서 handle들*/
    SPI_HandleTypeDef *h_spi;
    DMA_HandleTypeDef *h_dma;
} spi_tbl_t;

spi_tbl_t spi_tbl[SPI_MAX_CH];

/*초기화 핸들값들*/
extern SPI_HandleTypeDef hspi4; //main.c에서 선언된 것 여기서 쓰고 싶음.
extern DMA_HandleTypeDef hdma_spi4_tx;  //main.c에서 선언된 것 여기서 쓰고 싶음.



bool spiInit(void)
{
    bool ret = true;

    for(int i=0; i< SPI_MAX_CH;i++) 
    {
        spi_tbl[i].is_open = false;
        spi_tbl[i].is_tx_done = true;
        spi_tbl[i].func_tx = NULL;
    }

    return ret;
}
bool spiBegin(uint8_t ch)
{
    bool ret = false;
    spi_tbl_t *p_spi = &spi_tbl[ch];

    switch(ch) 
    {
        case _DEF_SPI0:
            p_spi->h_spi = &hspi4;
            p_spi->h_dma = &hdma_spi4_tx;

            /*여기는 이미 main.c에 선언되어 있어서 주석처리함*/
            hspi4.Instance              = SPI4;
            hspi4.Init.Mode             = SPI_MODE_MASTER;
            hspi4.Init.Direction        = SPI_DIRECTION_2LINES;
            hspi4.Init.DataSize         = SPI_DATASIZE_8BIT;
            hspi4.Init.CLKPolarity      = SPI_POLARITY_LOW;
            hspi4.Init.CLKPhase         = SPI_PHASE_1EDGE;
            hspi4.Init.NSS              = SPI_NSS_SOFT;
            hspi4.Init.BaudRatePrescaler= SPI_BAUDRATEPRESCALER_2;
            hspi4.Init.FirstBit         = SPI_FIRSTBIT_MSB;
            hspi4.Init.TIMode           = SPI_TIMODE_DISABLE;
            hspi4.Init.CRCCalculation   = SPI_CRCCALCULATION_DISABLE;
            hspi4.Init.CRCPolynomial    = 10;

            if(HAL_SPI_Init(&hspi4) == HAL_OK)
            {
                p_spi->is_open = true;
                ret = true;
            }
            break;
    }
    return ret;
}
void spiSetDataMode(uint8_t ch, uint8_t dataMode)
{
    spi_tbl_t *p_spi = &spi_tbl[ch];

    if (p_spi->is_open == false) return;

    switch(dataMode)
    {
        //CPOL =0, CPHA =0
        case SPI_MODE0:
            p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_LOW;
            p_spi->h_spi->Init.CLKPhase = SPI_PHASE_1EDGE;
            HAL_SPI_Init(p_spi->h_spi);
            break;
            
        //CPOL =0, CPHA =1
        case SPI_MODE1:
            p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_LOW;
            p_spi->h_spi->Init.CLKPhase = SPI_PHASE_2EDGE;
            HAL_SPI_Init(p_spi->h_spi);
            break;

        //CPOL =1, CPHA =0
        case SPI_MODE2:
            p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
            p_spi->h_spi->Init.CLKPhase = SPI_PHASE_1EDGE;
            HAL_SPI_Init(p_spi->h_spi);
            break;

        //CPOL =1, CPHA =1
        case SPI_MODE3:
            p_spi->h_spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
            p_spi->h_spi->Init.CLKPhase = SPI_PHASE_2EDGE;
            HAL_SPI_Init(p_spi->h_spi);
            break;

    }
}  
void spiSetBitWidth(uint8_t ch, uint8_t bit_width)
{
    spi_tbl_t *p_spi = &spi_tbl[ch];
    if(p_spi->is_open == false) 
        return ;

    p_spi->h_spi->Init.DataSize = SPI_DATASIZE_8BIT;
    //상하 비트의 순서를 맞추기 위해서 

    if(bit_width == 16)
    {
        p_spi->h_spi->Init.DataSize = SPI_DATASIZE_16BIT;
    }
    HAL_SPI_Init(p_spi->h_spi);
}

uint8_t spiTransfer8(uint8_t ch, uint8_t data)
{
    uint8_t ret;
    spi_tbl_t *p_spi = &spi_tbl[ch];

    if(p_spi->is_open == false) 
        return 0;

    HAL_SPI_TransmitReceive(p_spi->h_spi, &data, &ret, 1, 0xffff);
    //한바이트씩 송수신함
    return ret;
}

uint16_t spiTransfer16(uint8_t ch, uint16_t data)
{
    uint8_t tBuf[2];
    uint8_t rBuf[2];
    uint16_t ret;
    spi_tbl_t *p_spi = &spi_tbl[ch];

    if(p_spi->is_open == false)
        return 0;
    if(p_spi->h_spi->Init.DataSize == SPI_DATASIZE_8BIT)
    {
        tBuf[1] = (uint8_t) data;       //상위 byte가 뒤로 오고,
        tBuf[0] = (uint8_t)(data>>8);   //하위 byte가 앞으로 옴
        //비트 쉬프트해서 받음
        HAL_SPI_TransmitReceive(p_spi->h_spi, (uint8_t *)&tBuf, (uint8_t *)&rBuf, 2, 0xffff);
        //사이즈가 8비트면, 2바이트씩 송수신

        /*받은 상태에서는 반대로 풀어준다. */
        //왜냐하면 msb부터 먼저가기 때문
        ret  = rBuf[0];
        ret <<=8;
        ret += rBuf[1];
    }
    else 
    {
        HAL_SPI_TransmitReceive(p_spi->h_spi, (uint8_t *)&data, (uint8_t *)&ret, 1, 0xffff);   
        //애초에 16비트로 되어 있으면 한번만 송수신
    }
    return ret;
}

/*DMA 송신 시작*/
void spiDmaTxStart(uint8_t spi_ch, uint8_t *p_buf, uint32_t length)
{
    spi_tbl_t *p_spi = &spi_tbl[spi_ch];

    if(p_spi->is_open == false) 
        return ;
    
    p_spi->is_tx_done = false; //송신 시작할땐 false
    HAL_SPI_Transmit_DMA(p_spi->h_spi, p_buf, length);
    //송신 시작
}

void spiDmaTxTransfer(uint8_t ch, void *buf, uint32_t length, uint32_t timeout)
{
  uint32_t t_time;


  spiDmaTxStart(ch, (uint8_t *)buf, length);

  t_time = millis();

  if (timeout == 0) return;
  //무시하고 리턴

  while(1)
  {
    if(spiDmaTxIsDone(ch))
    {
      break;
    }
    if((millis()-t_time) > timeout)
    {
      break;
    }
  }
}

bool spiDmaTxIsDone(uint8_t ch)
{
  spi_tbl_t  *p_spi = &spi_tbl[ch];

  if (p_spi->is_open == false)     return true;

  return p_spi->is_tx_done;
}

void spiAttachTxInterrupt(uint8_t ch, void (*func)())
{
  spi_tbl_t  *p_spi = &spi_tbl[ch];


  if (p_spi->is_open == false)     return;

  p_spi->func_tx = func;
}


/*이미 있지만, "추가"라고해서 씀..*/
//void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
//spi전송이 완료되면 HAL라이브러리에 의해 자동으로 호출됨.
//사용자는 이를 재정의하여 원하는 동작을 "추가"할 수 있음. 
//이제보니 이미 있지만 추가로 써도 될듯. 
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    spi_tbl_t *p_spi;
    /* ?? 이하 _DEF_SPI0쓰는 것으로 보아, 채널 바뀔 시 직접 해야 할듯*/
    //아니면, spi_ch로 받아야 하는데, 아마도 콜백함수라 인자 못받나?
    if(hspi->Instance == spi_tbl[_DEF_SPI0].h_spi->Instance)
    {
        p_spi = &spi_tbl[_DEF_SPI0];
        p_spi->is_tx_done = true;
        //송신완료가 되면 true가 됨, 내가 보낸 것이 완료되었다는 뜻
        if(p_spi->func_tx != NULL)
        {
            (*p_spi->func_tx)();
            //콜백함수가 있으면 호출함
        }
    }
}

// /*이미 있어서 안씀.*/
// extern void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi);
// //주변 장치를 초기화 할 때 씀
// //사용자가 설정한 초기화를 할 때 씀. 

// extern void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi);
// //주변 장치를 초기화를 해제, spi쓰는 어플이 종료되거나 다시 설정해야 할때
// //핀 재구성, NVIC 설정, 리소스 해제


#endif //_USE_SPI
